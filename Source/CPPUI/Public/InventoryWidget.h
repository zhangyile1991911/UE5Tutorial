// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPPUI/Public/InventoryStruct.h"
#include "InventoryWidget.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(ClickSlot,const FInventoryStruct&,int);
class UInventorySlot;
class APickupBase;
/**
 * 
 */
UCLASS(Abstract)
class CPPUI_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

	void RefreshInventory();
	TArray<FInventoryStruct> container_data;
	TArray<TObjectPtr<UInventorySlot>> container_ui;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void ClickSlot1();
	
	UFUNCTION()
	void ClickSlot2();

	UFUNCTION()
	void ClickSlot3();

	UFUNCTION()
	void ClickSlot4();

	UFUNCTION()
	void ClickSlot5();


public:
	ClickSlot ClickSlotFunc;
	UPROPERTY(BlueprintReadOnly,Category="custom ui",meta=(BindWidget))
	TObjectPtr<UInventorySlot> slot1;

	UPROPERTY(BlueprintReadOnly,Category="custom ui",meta=(BindWidget))
	TObjectPtr<UInventorySlot> slot2;

	UPROPERTY(BlueprintReadOnly,Category="custom ui",meta=(BindWidget))
    TObjectPtr<UInventorySlot> slot3;

	UPROPERTY(BlueprintReadOnly,Category="custom ui",meta=(BindWidget))
	TObjectPtr<UInventorySlot> slot4;

	UPROPERTY(BlueprintReadOnly,Category="custom ui",meta=(BindWidget))
	TObjectPtr<UInventorySlot> slot5;

	void AddItem(const FInventoryStruct& item);
	void RemoveItem(const FInventoryStruct& item);
};