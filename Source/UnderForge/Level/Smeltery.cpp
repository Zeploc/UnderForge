// Fill out your copyright notice in the Description page of Project Settings.

#include "Smeltery.h"
#include "Items/ForgeMat.h"
#include "Components/BoxComponent.h"
// Sets default values
ASmeltery::ASmeltery()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionBox->bGenerateOverlapEvents = true;
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASmeltery::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ASmeltery::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ASmeltery::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASmeltery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASmeltery::ProcessItem(AForgeMat* material)
{
	switch (material->ResourceType)
	{
	case(EResource::R_WOOD):
		material->Destroy();
		break;

	case(EResource::R_IRON):
		material->Destroy();
		break;

	case(EResource::R_BRONZE):
		material->Destroy();
		break;
	}
}

void ASmeltery::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Begin"));
	if (AForgeMat* mat = Cast<AForgeMat>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Begin"));
		mat->CurrentTouchingStation = this;
	}
}


void ASmeltery::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("End"));
	if (AForgeMat* mat = Cast<AForgeMat>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("End"));
		mat->CurrentTouchingStation = nullptr;
	}
}
