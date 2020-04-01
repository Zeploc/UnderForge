// Fill out your copyright notice in the Description page of Project Settings.

#include "Smeltery.h"
#include "Items/ForgeMat.h"
#include "Items/ForgePart.h"
#include "Player/ForgePlayer.h"
#include "Player/ForgeController.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Utlities.h"
#include "Engine.h"
#include "UnderForgeSingleton.h"

// Sets default values
ASmeltery::ASmeltery()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SmeltingTimePassed = 0.0f;
	bSmeltingMinigamePlaying = false;
	SmeltingTimeKABOOM = 10.0f;
	CurrentRecipeSmeltingTimeNeeded = 5.0f;
	SmeltingTimeMax = 8.0f;

	Rotator = CreateDefaultSubobject<USceneComponent>(TEXT("Rotating"));
	Rotator->SetupAttachment(StationMesh);
	Rotator->SetRelativeLocation(StationMesh->RelativeLocation);
	Rotator->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));

	CurrentProducingItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Station Mesh2"));
	CurrentProducingItem->SetupAttachment(Rotator);
	CurrentProducingItem->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	CurrentProducingItem->SetVisibility(true, false);
	CurrentState = EResource::R_IRONINGOT;
	OutputName = FString("Iron Ingot");

	PotentiallyInteracting = false;
}

// Called when the game starts or when spawned
void ASmeltery::BeginPlay()
{
	Super::BeginPlay();
	CurrentProducingItem->SetStaticMesh(IronIngot);
	InteractTimer = 0;
}

// Called every frame
void ASmeltery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ProcessSmelting(DeltaTime);
	FRotator rotate(0, 1, 0);
	Rotator->AddWorldRotation(rotate);
}

bool ASmeltery::TryInteract(AForgePlayer * _Player)
{
	//UE_LOG(LogTemp, Warning, TEXT("Minigame Start"));
	if (bSmeltingMinigamePlaying)
	{
		UE_LOG(LogTemp, Warning, TEXT("Minigame Interact"));
		return Super::TryInteract(_Player);
		//smelty->MiniGameComplete();
	}
	return false;
}

void ASmeltery::Interacted(AForgePlayer * _Player)
{
	EResource ResourceToMake = EResource::R_NONE;
	if (CurrentRecipe)
	{
		ResourceToMake = GetResourceFromRecipe(*CurrentRecipe);
	}
	
	if (SmeltingTimePassed < CurrentRecipeSmeltingTimeNeeded)
	{
		for (EResource Resource : CurrentRecipe->Resources)
		{
			if (CurrentlyProcessing.Contains(Resource))
			{
				MakeMat(Resource);
				CurrentlyProcessing.Remove(Resource);
			}
		}
	}
	else if (SmeltingTimePassed < SmeltingTimeKABOOM)
	{
		MakeResource(ResourceToMake);		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("KABOOM"));
		//KABOOM
	}
	// If no coal left
	if (!CurrentlyProcessing.Contains(EResource::R_COAL))
	{
	}
	SmeltingTimePassed = 0.0f;
	bSmeltingMinigamePlaying = false;
	CurrentRemainingTime = 0.0f;

	if (CurrentRecipe)
	{
		for (EResource Resource : CurrentRecipe->Resources)
		{
			if (CurrentlyProcessing.Contains(Resource))
			{
				CurrentlyProcessing.RemoveSingle(Resource);
				BI_OnRemoveResource(Resource);
			}
		}
		for (int i = 0; i < CurrentRecipe->iCoalCount; i++)
		{
			if (CurrentlyProcessing.Contains(EResource::R_COAL))
			{
				CurrentlyProcessing.RemoveSingle(EResource::R_COAL);
				BI_OnRemoveResource(EResource::R_COAL);
			}
		}
		CurrentRecipe = nullptr;
		CurrentResourceCreating = EResource::R_NONE;
	}
}

EResource ASmeltery::GetResourceFromRecipe(FIngotRecipe _IngotRecipe)
{
	TArray<EResource> Recipies;
	IngotRecipies.GenerateKeyArray(Recipies);
	for (EResource RecipeResource : Recipies)
	{
		if (IngotRecipies[RecipeResource] == _IngotRecipe)
		{
			return RecipeResource;
		}
	}
	return EResource::R_NONE;
}

void ASmeltery::ProcessMatItem(AForgeMat* material)
{
	bool bCanHaveResource = false;
	if (material->ResourceType == EResource::R_COAL)
		bCanHaveResource = true;

	if (!bCanHaveResource)
	{
		TArray<FIngotRecipe> Recipes;
		IngotRecipies.GenerateValueArray(Recipes);
		for (FIngotRecipe Recipie : Recipes)
		{
			// Create required for this recipe
			TArray<EResource> RecipeRequired = Recipie.Resources;
			int RequiredCoal = Recipie.iCoalCount;
			for (EResource CurrentResource : CurrentlyProcessing)
			{
				if (CurrentResource == EResource::R_COAL)
					RequiredCoal--;
				else
					// Removes current resource from required
					RecipeRequired.RemoveSingle(CurrentResource);
			}
			// If required resources still contains given resource
			if (RecipeRequired.Contains(material->ResourceType))
			{
				// Is need for recipe
				RecipeRequired.RemoveSingle(material->ResourceType);
				// If recipe is complete with this new item
				if (RecipeRequired.Num() <= 0 && RequiredCoal <= 0)
				{
					bCanHaveResource = true;
					CurrentRecipe = new FIngotRecipe(Recipie);
					CurrentResourceCreating = GetResourceFromRecipe(Recipie);
					CurrentRecipeSmeltingTimeNeeded = CurrentRecipe->fSmeltTime;
					SmeltingTimeKABOOM = CurrentRecipeSmeltingTimeNeeded + 5.0f;
					break;
				}
			}
		}
	}

	if (!bCanHaveResource)
	{
		ThrowAway(material);
		return;
	}
	UGameplayStatics::PlaySound2D(GetWorld(), SuccessInteractSound);
	CurrentlyProcessing.Add(material->ResourceType);
	BI_OnNewResource(material->ResourceType);
	material->Destroy();
	bool HasNonCoal = false;
	for (EResource Resource : CurrentlyProcessing)
	{
		if (Resource != EResource::R_COAL)
		{
			HasNonCoal = true;
			break;
		}
	}

	if (HasNonCoal && CurrentlyProcessing.Contains(EResource::R_COAL))
		bSmeltingMinigamePlaying = true;
}

void ASmeltery::ProcessSmelting(float DeltaTime)
{
	if (!bSmeltingMinigamePlaying)
		return;

	SmeltingTimePassed += DeltaTime;
	CurrentRemainingTime = CurrentRecipeSmeltingTimeNeeded - SmeltingTimePassed;
	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, "Delta" + FString::SanitizeFloat(SmeltingTimePassed));
	if (SmeltingTimePassed > SmeltingTimeKABOOM)
	{
		bSmeltingMinigamePlaying = false;
		SmeltingTimePassed = 0.0f;
		CurrentRemainingTime = 0.0f;
		CurrentlyProcessing.Empty();
		UE_LOG(LogTemp, Warning, TEXT("KABOOM"));
		//KABOOM
	}
}

AForgePart * ASmeltery::MakeResource(EResource type)
{
	if (!HasAuthority())
	{
		//SERVER_MakeResource(type);
		return nullptr;
	}
	
	UUnderForgeSingleton* GameSingleton = Cast<UUnderForgeSingleton>(GEngine->GameSingleton);
	if (!GameSingleton)
		return nullptr;

	FResource* FoundResource = GameSingleton->Resources.Find(type);

	if (FoundResource)
	{		
		APickUpItem * ResourceRef = GetWorld()->SpawnActor<APickUpItem>(FoundResource->ResourceClass, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
		AForgePart* ForgeResouce = Cast<AForgePart>(ResourceRef);
		return ForgeResouce;
	}
	
	return nullptr;
}

AForgeMat * ASmeltery::MakeMat(EResource type)
{
	if (!HasAuthority())
	{
		//SERVER_MakeMat(type);
		return nullptr;
	}

	UUnderForgeSingleton* GameSingleton = Cast<UUnderForgeSingleton>(GEngine->GameSingleton);
	if (!GameSingleton)
		return nullptr;

	FResource* FoundResource = GameSingleton->Resources.Find(type);

	if (FoundResource)
	{
		APickUpItem * ResourceRef = GetWorld()->SpawnActor<APickUpItem>(FoundResource->ResourceClass, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
		AForgeMat* ForgeMat = Cast<AForgeMat>(ResourceRef);
		return ForgeMat;
	}

	return nullptr;
}

void ASmeltery::SERVER_MakeResource_Implementation(EResource type)
{
	MakeResource(type);
}
bool ASmeltery::SERVER_MakeResource_Validate(EResource type)
{
	return true;
}

void ASmeltery::SERVER_MakeMat_Implementation(EResource type)
{
	MakeMat(type);
}
bool ASmeltery::SERVER_MakeMat_Validate(EResource type)
{
	return true;
}

void ASmeltery::MorphStates(bool Next)
{
	return; // REMOVING CHANGE MECHANIC
	if (bSmeltingMinigamePlaying) return;
	switch (CurrentState)
	{
	case EResource::R_IRONINGOT:
	{
		if (Next)
			CurrentState = EResource::R_STEELINGOT;
		else
			CurrentState = EResource::R_STEELINGOT;
		break;
	}
	case EResource::R_STEELINGOT:
	{
		if (Next)
			CurrentState = EResource::R_IRONINGOT;
		else
			CurrentState = EResource::R_IRONINGOT;
		break;
	}
	}

	switch (CurrentState)
	{
	case EResource::R_IRONINGOT:
		OutputName = FString("Iron Ingot");
		CurrentProducingItem->SetStaticMesh(IronIngot);
		break;
	case EResource::R_STEELINGOT:
		OutputName = FString("Steel Ingot");
		CurrentProducingItem->SetStaticMesh(SteelIngot);
		break;
	default:
		break;
	}
}

void ASmeltery::DamageForge(float Damage)
{
	UGameplayStatics::PlaySound2D(GetWorld(), SmelteryTakeDamageSound);
	ForgeHealth -= Damage;
	MULTI_UpdateForgeHealth(ForgeHealth);
}

void ASmeltery::MULTI_UpdateForgeHealth_Implementation(float _NewHealth)
{
	if (_NewHealth <= 0.0f)
	{
		if (GetWorld())
		{
			if (AForgeController* ForgeController = Cast<AForgeController>(GetWorld()->GetFirstPlayerController()))
			{
				ForgeController->EndGame();
			}
		}
	}

	if (HasAuthority())
		return;

	if (ForgeHealth > _NewHealth)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SmelteryTakeDamageSound);
	}
	ForgeHealth = _NewHealth;
}

void ASmeltery::RestartHealth()
{
	ForgeHealth = 1.0f;
}
