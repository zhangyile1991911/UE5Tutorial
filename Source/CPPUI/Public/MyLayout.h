// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "Blueprint/UserWidget.h"
#include "MyLayout.generated.h"

class UTextBlock;
class APickupBase;
class UActionWidget;
class UMyStatusInfo;
class UInventoryWidget;
class ACPPUICharacter;
/**
 * 
 */
UCLASS(Abstract)
class CPPUI_API UMyLayout : public UUserWidget
{
private:
	GENERATED_BODY()

	bool BIsInventoryShow;
protected:
	UPROPERTY(BlueprintReadOnly,Category="custom ui",meta=(BindWidget))
	TObjectPtr<UInventoryWidget> InventoryMenu = nullptr;

	UPROPERTY(BlueprintReadOnly,Category="custom ui",meta=(BindWidget))
	TObjectPtr<UMyStatusInfo> StatusInfo = nullptr;

	UPROPERTY(BlueprintReadOnly,Category="custom ui",meta=(BindWidget))
	TObjectPtr<UActionWidget> ActionMenu = nullptr;

	UPROPERTY(BlueprintReadOnly,Category="custom ui",meta=(BindWidget))
	TObjectPtr<UTextBlock>  tips = nullptr;

	UPROPERTY(Transient,meta = (BindWidgetAnim))
    TObjectPtr<UWidgetAnimation> SliderIn;

	UPROPERTY(Transient,meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SliderOut;
	
	UPROPERTY(Interp,Category="Animations")
	bool bIsMoving = false;

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void updateHealthPB(int oldVal,int newVal,int maxVal);
	
	UFUNCTION()
	void updateEnergyPB(int oldVal,int newVal,int maxVal);
	
	UFUNCTION()
	void updateMoodPB(int oldVal,int newVal,int maxVal);

	TArray<TObjectPtr<APickupBase>> pickupList;

	void refreshTip();
	
	UFUNCTION()
	void CancelBtnFunc();

	UFUNCTION()
	void DropBtnFunc();

	UFUNCTION()
	void ActionBtnFunc();

	const FInventoryStruct* CurSelect;
public:
	void ClearAllHandler();
	void BindAllHandler();
	void AddPickupBaseInfo(TObjectPtr<APickupBase>);
	void RemovePickupBaseInfo(TObjectPtr<APickupBase>);
	void InventorySliderIn();
	void InventorySliderOut();
	TObjectPtr<ACPPUICharacter> AcppuiCharacter;
	bool GetBIsInventoryShow()const{return BIsInventoryShow;}
	void AddInventoryItem(TObjectPtr<APickupBase> item) const;
};
