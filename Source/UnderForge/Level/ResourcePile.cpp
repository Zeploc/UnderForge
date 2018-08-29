// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourcePile.h"
#include "Items/ForgeMat.h"
#include "Engine/World.h"

AForgeMat * AResourcePile::GetResource()
{
	FActorSpawnParameters SpawnParams;
	AForgeMat* ResourceRef = GetWorld()->SpawnActor<AForgeMat>(ForgeMat->GetClass(), ObjectPosition->GetComponentTransform(), SpawnParams);
	return ResourceRef;
}
