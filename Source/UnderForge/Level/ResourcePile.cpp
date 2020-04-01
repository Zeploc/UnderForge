// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourcePile.h"
#include "Items/ForgeMat.h"
#include "Player/ForgePlayer.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "UnderForgeSingleton.h"

AForgeMat * AResourcePile::GetResource(AForgePlayer * _OwningPlayer)
{
	UUnderForgeSingleton* GameSingleton = Cast<UUnderForgeSingleton>(GEngine->GameSingleton);
	if (!GameSingleton)
		return nullptr;

	FResource* FoundResource = GameSingleton->Resources.Find(Resource);
	if (!FoundResource)
		return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = _OwningPlayer;
	APickUpItem* ResourceRef = GetWorld()->SpawnActor<APickUpItem>(FoundResource->ResourceClass, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation(), SpawnParams);
	if (!ResourceRef)
		return nullptr;
	AForgeMat* ResourceMatRef = Cast<AForgeMat>(ResourceRef);
	/*FTransform SpawnTransform = FTransform::Identity;
	SpawnTransform.SetLocation(ObjectPosition->GetComponentLocation());
	SpawnTransform.SetRotation(ObjectPosition->GetComponentRotation().Quaternion());
	AForgeMat* ResourceRef = GetWorld()->SpawnActorDeferred<AForgeMat>(ForgeMat, SpawnTransform);
	ResourceRef->HeldPlayer = _OwningPlayer;
	UGameplayStatics::FinishSpawningActor(ResourceRef, SpawnTransform);*/
	return ResourceMatRef;
}

void AResourcePile::Interacted(AForgePlayer * _Player)
{
	//if (HasAuthority())
	//{
	// Will spawn on all, client spawns temporary one
		AForgeMat* CreatedResource = GetResource(_Player);
		if (CreatedResource)
		{
			CreatedResource->PickUp(_Player);
			_Player->HoldItem = CreatedResource;
		}
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
