// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "PickUpItem.h"
#include "ForgeItem.generated.h"



UCLASS()
class UNDERFORGE_API AForgeItem : public APickUpItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AForgeItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UBoxComponent* InteractBox;


};
