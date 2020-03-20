// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "PickUpItem.h"
#include "ForgeTool.generated.h"

UCLASS()
class UNDERFORGE_API AForgeTool : public APickUpItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AForgeTool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* ToolMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AForgePlayer* CurrentPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AForgeStation* ToolStation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsHeld = false;

	UFUNCTION(BlueprintCallable)
		void PickUp(class AForgePlayer* NewPlayer);

	UFUNCTION(BlueprintCallable)
		void Drop();
};
