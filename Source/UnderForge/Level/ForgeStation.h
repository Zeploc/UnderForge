// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ForgeStation.generated.h"

class AForgeMat;
class AForgePart;

UCLASS()
class UNDERFORGE_API AForgeStation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AForgeStation();

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


	void ProcessItem(class APickUpItem* Item);

	virtual void ProcessMatItem(AForgeMat* material);
	virtual void ProcessPartItem(AForgePart* Part);
	virtual void ItemDectection(class AActor*, bool entering);
	UFUNCTION(BlueprintCallable)
		void ThrowAway(AActor* Actor);


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* StationMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USceneComponent* ObjectPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UArrowComponent* RefuseThrowDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool PotentiallyInteracting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float InteractTimer;


	UPROPERTY(EditDefaultsOnly, Category = Sounds)
		class USoundBase* SuccessInteractSound;
	UPROPERTY(EditDefaultsOnly, Category = Sounds)
		class USoundBase* FailInteractSound;
};
