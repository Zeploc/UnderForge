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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* StationMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USceneComponent* ObjectPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* CollisionBox;

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

	UFUNCTION(BlueprintCallable)
	virtual bool TryInteract(class AForgePlayer* _Player);
	virtual void Interacted(class AForgePlayer* _Player);

	virtual bool CanTakeItem(class APickUpItem* Item);
	virtual bool CanTakeMatItem(class AForgeMat* material);
	virtual bool CanTakePartItem(class AForgePart* Part);

	UFUNCTION(BlueprintCallable)
		virtual void MorphStates(bool Next);

	UFUNCTION(NetMulticast, Reliable)
	void MULTI_ProcessItem(class APickUpItem* Item);
	virtual void ProcessItem(class APickUpItem* Item);

	virtual void ItemPickedUp(class APickUpItem* Item);
	UFUNCTION(BlueprintImplementableEvent)
		void BI_OnItemPickedUp(class APickUpItem* Item);

	virtual bool ProcessMatItem(AForgeMat* material);
	UFUNCTION(BlueprintImplementableEvent)
	void BI_OnProcessMatItem(AForgeMat* material);
	virtual bool ProcessPartItem(AForgePart* Part);
	UFUNCTION(BlueprintImplementableEvent)
	void BI_OnProcessPartItem(AForgePart* Part);
	virtual void ItemDectection(class AActor*, bool entering);
	UFUNCTION(BlueprintCallable)
		void ThrowAway(APickUpItem* PickUpItem);

	UFUNCTION(BlueprintCallable)
		virtual void SetPotentiallyInteracting(bool _PotentiallyInteracting);
	UFUNCTION(BlueprintImplementableEvent)
		void BI_OnSetPotentiallyInteracting();



	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UArrowComponent* RefuseThrowDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float InteractTimer;


	UPROPERTY(EditDefaultsOnly, Category = Sounds)
		class USoundBase* SuccessInteractSound;
	UPROPERTY(EditDefaultsOnly, Category = Sounds)
		class USoundBase* FailInteractSound;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool PotentiallyInteracting = false;
};
