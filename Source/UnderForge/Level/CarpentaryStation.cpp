// Fill out your copyright notice in the Description page of Project Settings.

#include "CarpentaryStation.h"
#include "Items/ForgeMat.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Utlities.h"

ACarpentaryStation::ACarpentaryStation()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionBox->bGenerateOverlapEvents = true;
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACarpentaryStation::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACarpentaryStation::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ACarpentaryStation::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACarpentaryStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACarpentaryStation::ProcessItem(AForgeMat* material)
{
	switch (material->ResourceType)
	{
	case(EResource::R_WOOD):
		material->Destroy();
		MakeResource();
		break;
	}
}

void ACarpentaryStation::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Begin"));
	if (AForgeMat* mat = Cast<AForgeMat>(OtherActor))
	{
		if (mat->ResourceType == EResource::R_IRON || mat->ResourceType == EResource::R_BRONZE)
		{
			UE_LOG(LogTemp, Warning, TEXT("Begin"));
			mat->CurrentTouchingStation = this;
		}
	}
}


void ACarpentaryStation::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("End"));
	if (AForgeMat* mat = Cast<AForgeMat>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("End"));
		mat->CurrentTouchingStation = nullptr;
	}
}

AForgeMat * ACarpentaryStation::MakeResource()
{
	AForgeMat* ResourceRef = GetWorld()->SpawnActor<AForgeMat>(ForgeMat, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
	return ResourceRef;
}