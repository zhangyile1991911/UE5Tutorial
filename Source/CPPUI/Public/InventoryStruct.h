// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryStruct.generated.h"

USTRUCT(BlueprintType)
struct CPPUI_API FInventoryStruct
{
	GENERATED_BODY()

public:
	// UPROPERTY(EditAnywhere)
	// TObjectPtr<AActor> Item = nullptr;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> ItemImage = nullptr;

	UPROPERTY(EditAnywhere)
	FName PickupText;

	UPROPERTY(EditAnywhere)
	FName ActionText;

	UPROPERTY(EditAnywhere)
	int AddHealth;
	
	UPROPERTY(EditAnywhere)
	int AddEnergy;
	
	UPROPERTY(EditAnywhere)
	int AddMood;

	UClass* InstanceClass;

	TObjectPtr<UStaticMesh> MeshComponent;
};
