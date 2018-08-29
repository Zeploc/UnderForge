// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/ForgeStation.h"
#include "Utlities.h"
#include "Smeltery.generated.h"

UCLASS()
class UNDERFORGE_API ASmeltery : public AForgeStation
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmeltery();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ProcessItem(AForgeMat* material) override;
	virtual void ItemDectection(class AActor*, bool entering) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> ForgedPart;

	UFUNCTION(BlueprintCallable)
		class AForgePart* MakeResource(EResource type);
};
