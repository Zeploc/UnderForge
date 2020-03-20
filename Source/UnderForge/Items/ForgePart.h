// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "PickUpItem.h"
#include "Utlities.h"
#include "ForgePart.generated.h"

UCLASS()
class UNDERFORGE_API AForgePart : public APickUpItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AForgePart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "ForgeStation")
		void DropOnStation();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* PartMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AForgeStation* CurrentTouchingStation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		ESwordPart SwordPart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		EResource ResourceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsHeld;
};
