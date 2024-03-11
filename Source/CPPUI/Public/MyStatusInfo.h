// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyStatusInfo.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS(Abstract)
class CPPUI_API UMyStatusInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly,Category="custom ui",meta=(BindWidget))
	TObjectPtr<UTextBlock> HealthText;
	UPROPERTY(BlueprintReadOnly,Category="custom ui",meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthPb;
	
	UPROPERTY(BlueprintReadOnly,Category="custom ui",meta=(BindWidget))
	TObjectPtr<UTextBlock> EnergyText;
	UPROPERTY(BlueprintReadOnly,Category="custom ui",meta=(BindWidget))
	TObjectPtr<UProgressBar> EnergyPb;
	
	UPROPERTY(BlueprintReadOnly,Category="custom ui",meta=(BindWidget))
	TObjectPtr<UTextBlock> MoodText;
	UPROPERTY(BlueprintReadOnly,Category="custom ui",meta=(BindWidget))
	TObjectPtr<UProgressBar> MoodPb;
	
};
