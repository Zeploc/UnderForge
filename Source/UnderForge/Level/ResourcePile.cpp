// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourcePile.h"
#include "Items/ForgeMat.h"
#include "Player/ForgePlayer.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

AForgeMat * AResourcePile::GetResource(AForgePlayer * _OwningPlayer)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = _OwningPlayer;
	AForgeMat* ResourceRef = GetWorld()->SpawnActor<AForgeMat>(ForgeMat, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation(), SpawnParams);
	/*FTransform SpawnTransform = FTransform::Identity;
	SpawnTransform.SetLocation(ObjectPosition->GetComponentLocation());
	SpawnTransform.SetRotation(ObjectPosition->GetComponentRotation().Quaternion());
	AForgeMat* ResourceRef = GetWorld()->SpawnActorDeferred<AForgeMat>(ForgeMat, SpawnTransform);
	ResourceRef->HeldPlayer = _OwningPlayer;
	UGameplayStatics::FinishSpawningActor(ResourceRef, SpawnTransform);*/
	return ResourceRef;
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
