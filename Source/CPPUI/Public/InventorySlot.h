// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

class UImage;
class UButton;
/**
 * 
 */
UCLASS(Abstract)
class CPPUI_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly,Category="custom ui",meta=(BindWidget))
	TObjectPtr<UButton> btn = nullptr;

	UPROPERTY(BlueprintReadOnly,Category="custom ui",meta=(BindWidget))
	TObjectPtr<UImage> img = nullptr;
};
