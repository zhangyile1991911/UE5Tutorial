// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionWidget.generated.h"

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS(Abstract)
class CPPUI_API UActionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly,Category="custom ui",meta=(BindWidget))
	TObjectPtr<UButton> UseBtn;
	
	UPROPERTY(BlueprintReadOnly,Category="custom ui",meta=(BindWidget))
	TObjectPtr<UTextBlock> UseText;

	UPROPERTY(BlueprintReadOnly,Category="custom ui",meta=(BindWidget))
	TObjectPtr<UButton> DropBtn;

	UPROPERTY(BlueprintReadOnly,Category="custom ui",meta=(BindWidget))
	TObjectPtr<UButton> CancelBtn;
};
