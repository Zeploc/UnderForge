// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/ForgeStation.h"
#include "CarpentaryStation.generated.h"

UCLASS()
class UNDERFORGE_API ACarpentaryStation : public AForgeStation
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACarpentaryStation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ProcessItem(AForgeMat* material) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgeMat> ForgeMat;

	UFUNCTION(BlueprintCallable)
		class AForgeMat* MakeResource();
};
