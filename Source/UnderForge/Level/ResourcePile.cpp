// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourcePile.h"
#include "Items/ForgeMat.h"
#include "Player/ForgePlayer.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "UnderForgeSingleton.h"

#include "Components/StaticMeshComponent.h"

// Called when the game starts or when spawned
void AResourcePile::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
		RecreateResource();
}

void AResourcePile::RecreateResource()
{
	if (!HasAuthority())
		return; 

	APickUpItem* NewResource = GetResource();
	if (NewResource)
	{
		NewResource->ItemMesh->SetSimulatePhysics(false);
		NewResource->SetActorLocation(ObjectPosition->GetComponentLocation());
		NewResource->SetActorRotation(ObjectPosition->GetComponentRotation());
		NewResource->CurrentStation = this;
	}
}

APickUpItem * AResourcePile::GetResource()
{
	UUnderForgeSingleton* GameSingleton = Cast<UUnderForgeSingleton>(GEngine->GameSingleton);
	if (!GameSingleton)
		return nullptr;

	FResource* FoundResource = GameSingleton->Resources.Find(Resource);
	if (!FoundResource)
		return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	APickUpItem* ResourceRef = GetWorld()->SpawnActor<APickUpItem>(FoundResource->ResourceClass, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation(), SpawnParams);
	//if (!ResourceRef)
		return ResourceRef;
	//AForgeMat* ResourceMatRef = Cast<AForgeMat>(ResourceRef);

	/*FTransform SpawnTransform = FTransform::Identity;
	SpawnTransform.SetLocation(ObjectPosition->GetComponentLocation());
	SpawnTransform.SetRotation(ObjectPosition->GetComponentRotation().Quaternion());
	AForgeMat* ResourceRef = GetWorld()->SpawnActorDeferred<AForgeMat>(ForgeMat, SpawnTransform);
	ResourceRef->HeldPlayer = _OwningPlayer;
	UGameplayStatics::FinishSpawningActor(ResourceRef, SpawnTransform);*/

	//return ResourceMatRef;
}

bool AResourcePile::CanTakeItem(APickUpItem * Item)
{
	return false;
}

void AResourcePile::ItemPickedUp(APickUpItem * Item)
{
	Super::ItemPickedUp(Item);
	RecreateResource();
}

void AResourcePile::Interacted(AForgePlayer * _Player)
{

	//if (HasAuthority())
	//{
	// Will spawn on all, client spawns temporary one
		//AForgeMat* CreatedResource = GetResource();
		//if (CreatedResource)
		//{
		//	CreatedResource->PickUp(_Player);
		//	_Player->SetHoldItem(CreatedResource);
		//}
		//_Player->CLIENT_PickUp(CreatedResource);
	//}
}
//
//void AResourcePile::SERVER_GetResource_Implementation()
//{
//	GetResource(nullptr);
//}
//bool AResourcePile::SERVER_GetResource_Validate()
//{
//	return true;
//}
