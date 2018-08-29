// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourcePile.h"
#include "Items/ForgeMat.h"
#include "Engine/World.h"

AForgeMat * AResourcePile::GetResource()
{
	AForgeMat* ResourceRef = GetWorld()->SpawnActor<AForgeMat>(ForgeMat, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
	return ResourceRef;
}
