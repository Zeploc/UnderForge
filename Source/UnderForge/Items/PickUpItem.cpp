// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpItem.h"

#include "UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Player/ForgePlayer.h"
#include "Level/ForgeStation.h"

// Sets default values
APickUpItem::APickUpItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	RootComponent = ItemMesh;
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetCollisionProfileName("Pickup");
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	ItemMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	ItemMesh->SetRenderCustomDepth(true);

	bReplicates = true;
	bReplicateMovement = true;
}

// Called when the game starts or when spawned
void APickUpItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickUpItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickUpItem::PickUp(AForgePlayer * NewPlayer)
{
	/*if (HeldPlayer)
	{
		HeldPlayer->HoldItem = nullptr;
	}*/
	HeldPlayer = NewPlayer;
	ItemMesh->SetSimulatePhysics(false);
	AttachToComponent(NewPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketName);
	SetActorRelativeTransform(AttachOffset);
	//ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	ItemMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	if (CurrentStation)
	{
		CurrentStation->ItemPickedUp(this);
		CurrentStation = nullptr;
	}
	BI_OnPickUp();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Picked Up");
}

void APickUpItem::Drop()
{
	ItemMesh->SetSimulatePhysics(true);
	//ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	ItemMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);

	BI_OnDrop();

	/*if (CurrentStation && HasAuthority())
	{
		CurrentStation->MULTI_ProcessItem(this);
	}*/
	HeldPlayer = nullptr;
}

void APickUpItem::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APickUpItem, CanBePickedUp);
	DOREPLIFETIME(APickUpItem, CurrentStation);
}
