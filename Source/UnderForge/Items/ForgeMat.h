// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utlities.h"
#include "ForgeMat.generated.h"


class AForgeStation;

UCLASS()
class UNDERFORGE_API AForgeMat : public AActor
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

<<<<<<< .merge_file_a12168
	AForgeStation* CurrentTouchingStation;
=======
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* MatMesh;

	ASmeltery* CurrentTouchingStation;
>>>>>>> .merge_file_a12156
	
};
