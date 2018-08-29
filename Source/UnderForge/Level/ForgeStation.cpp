// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgeStation.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Items/ForgeMat.h"

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

	CollisionBox->bGenerateOverlapEvents = true;
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AForgeStation::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AForgeStation::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AForgeStation::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AForgeStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AForgeStation::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AForgeMat* mat = Cast<AForgeMat>(OtherActor))
	{
		mat->CurrentTouchingStation = this;
	}
}


void AForgeStation::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AForgeMat* mat = Cast<AForgeMat>(OtherActor))
	{
		mat->CurrentTouchingStation = nullptr;
	}
}

void AForgeStation::ProcessItem(AForgeMat* material)
{
	switch (material->ResourceType)
	{
	case(EResource::R_WOOD):
		Destroy(material);
		break;

	case(EResource::R_IRON):
		Destroy(material);
		break;

	case(EResource::R_BRONZE):
		Destroy(material);
		break;
	}
}
