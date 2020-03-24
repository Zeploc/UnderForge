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
	if (!HasAuthority())
		_Player->SERVER_InteractWith(this);

	Interacted(_Player);
	// Called from derived if successful
	return true;
}

void AForgeStation::Interacted(AForgePlayer * _Player)
{
}

void AForgeStation::ProcessItem(APickUpItem * Item)
{
	if (AForgeMat* mat = Cast<AForgeMat>(Item))
	{
		ProcessMatItem(mat);
	}
	else if (AForgePart* part = Cast<AForgePart>(Item))
	{
		ProcessPartItem(part);
	}
}

void AForgeStation::ProcessMatItem(AForgeMat * material)
{
	ThrowAway(material);	
}

void AForgeStation::ProcessPartItem(AForgePart * Part)
{
	ThrowAway(Part);
}

void AForgeStation::ItemDectection(AActor * actor, bool entering)
{
	if (AForgeMat* mat = Cast<AForgeMat>(actor))
	{
		if (entering)
			mat->CurrentTouchingStation = this;
		else
			mat->CurrentTouchingStation = nullptr;
	}
	else if (AForgePart* part = Cast<AForgePart>(actor))
	{
		if (entering)
			part->CurrentTouchingStation = this;
		else
			part->CurrentTouchingStation = nullptr;
	}
}

void AForgeStation::ThrowAway(AActor * Actor)
{
	UGameplayStatics::PlaySound2D(GetWorld(), FailInteractSound);
	// YEET THAT BOI
	if (!Actor) return;
	if (UStaticMeshComponent* StaticMeshComp = Cast<UStaticMeshComponent>(Actor->GetComponentByClass(UStaticMeshComponent::StaticClass())))
	{
		StaticMeshComp->SetSimulatePhysics(true);
		StaticMeshComp->AddImpulse(RefuseThrowDirection->GetForwardVector() * 1000.0f);
	}
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



