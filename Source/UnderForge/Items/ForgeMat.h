// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "PickUpItem.h"
#include "Utlities.h"
#include "ForgeMat.generated.h"


class AForgeStation;

UCLASS()
class UNDERFORGE_API AForgeMat : public APickUpItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AForgeMat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		EResource ResourceType;

	UFUNCTION(BlueprintCallable, Category = "ForgeStation")
		void DropOnStation();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AForgeStation* CurrentTouchingStation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* MatMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsHeld;
};
