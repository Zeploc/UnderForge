// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgeStation.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Items/ForgeMat.h"
#include "Items/ForgePart.h"
#include "Player/ForgePlayer.h"

// TEMP
#include "Engine.h"

// Sets default values
AForgeStation::AForgeStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StationMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Station Mesh"));
	RootComponent = StationMesh;
	ObjectPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Part Position"));
	ObjectPosition->SetupAttachment(StationMesh);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(StationMesh);

	CollisionBox->SetGenerateOverlapEvents(true);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AForgeStation::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AForgeStation::OnOverlapEnd);

	RefuseThrowDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("Refuse Throw Direction"));
	RefuseThrowDirection->SetupAttachment(StationMesh);

	PotentiallyInteracting = false;
	bReplicates = true;
}

// Called when the game starts or when spawned
void AForgeStation::BeginPlay()
{
	Super::BeginPlay();
	InteractTimer = 0;
}

// Called every frame
void AForgeStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AForgeStation::TryInteract(AForgePlayer * _Player)
{
	if (_Player->GetHoldItem())
	{
		if (CanTakeItem(_Player->GetHoldItem()))
		{
			if (HasAuthority())
			{
				MULTI_ProcessItem(_Player->GetHoldItem());
				//_Player->ClearHoldItem();				
			}
			else
			{
				_Player->SERVER_InteractWith(this);
			}
			return true;
		}

		ThrowAway(_Player->GetHoldItem());
		return false;
	}

	if (!HasAuthority())
		_Player->SERVER_InteractWith(this);

	Interacted(_Player);
	// Called from derived if successful
	return true;
}

void AForgeStation::Interacted(AForgePlayer * _Player)
{
}

bool AForgeStation::CanTakeItem(APickUpItem * Item)
{
	if (AForgeMat* mat = Cast<AForgeMat>(Item))
	{
		return CanTakeMatItem(mat);
	}
	else if (AForgePart* part = Cast<AForgePart>(Item))
	{
		return CanTakePartItem(part);
	}
	return false;
}

bool AForgeStation::CanTakeMatItem(AForgeMat * material)
{
	return false;
}

bool AForgeStation::CanTakePartItem(AForgePart * Part)
{
	return false;
}

void AForgeStation::MorphStates(bool Next)
{
}

void AForgeStation::MULTI_ProcessItem_Implementation(APickUpItem * Item)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, FString(HasAuthority() ? "Server: " : "Client: ") + FString("Multi process item"));
	ProcessItem(Item);
}

void AForgeStation::ProcessItem(APickUpItem * Item)
{
	if (AForgeMat* mat = Cast<AForgeMat>(Item))
	{
		BI_OnProcessMatItem(mat);
		if (ProcessMatItem(mat))
		{
			Item->CurrentStation = this;
			return;
		}
	}
	else if (AForgePart* part = Cast<AForgePart>(Item))
	{
		BI_OnProcessPartItem(part);
		if (ProcessPartItem(part))
		{
			Item->CurrentStation = this;
			return;
		}
	}
}

void AForgeStation::ItemPickedUp(class APickUpItem* Item)
{
	BI_OnItemPickedUp(Item);
}

bool AForgeStation::ProcessMatItem(AForgeMat * material)
{
	ThrowAway(material);	
	return false;
}

bool AForgeStation::ProcessPartItem(AForgePart * Part)
{
	ThrowAway(Part);
	return false;
}

void AForgeStation::ItemDectection(AActor * actor, bool entering)
{
	if (APickUpItem* PickUp = Cast<APickUpItem>(actor))
	{
		if (entering)
		{
			//PickUp->CurrentStation = this;
			if (!PickUp->HeldPlayer && HasAuthority())
			{
				if (CanTakeItem(PickUp))
					MULTI_ProcessItem(PickUp);
			}
		}
	}	
}

void AForgeStation::ThrowAway(APickUpItem* PickUpItem)
{
	UGameplayStatics::PlaySound2D(GetWorld(), FailInteractSound);
	// YEET THAT BOI
	if (!PickUpItem) return;
	
	PickUpItem->ItemMesh->SetSimulatePhysics(true);
	PickUpItem->ItemMesh->AddImpulse(RefuseThrowDirection->GetForwardVector() * 1000.0f);
	PickUpItem->CurrentStation = nullptr;
	if (PickUpItem->HeldPlayer)
	{
		// NOT NETWORKED
		if (PickUpItem->HeldPlayer)
			PickUpItem->HeldPlayer->ClearHoldItem();
		PickUpItem->Drop();
	}
}

void AForgeStation::SetPotentiallyInteracting(bool _PotentiallyInteracting)
{
	PotentiallyInteracting = _PotentiallyInteracting;
	BI_OnSetPotentiallyInteracting();
}

void AForgeStation::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ItemDectection(OtherActor, true);
}


void AForgeStation::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ItemDectection(OtherActor,false);
}



