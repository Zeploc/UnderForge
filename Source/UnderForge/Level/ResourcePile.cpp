// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourcePile.h"
#include "Items/ForgeMat.h"
#include "Engine/World.h"

AForgeMat * AResourcePile::GetResource()
{
	if (!HasAuthority())
	{
		//SERVER_GetResource();
		return nullptr;
	}
	AForgeMat* ResourceRef = GetWorld()->SpawnActor<AForgeMat>(ForgeMat, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
	return ResourceRef;
}

void AResourcePile::SERVER_GetResource_Implementation()
{
	GetResource();
}
bool AResourcePile::SERVER_GetResource_Validate()
{
	return true;
}
