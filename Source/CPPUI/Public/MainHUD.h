// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "MainHUD.generated.h"

class APickupBase;
class UMyLayout;
class UWorld;
/**
 * 
 */
UCLASS(Abstract)
class CPPUI_API AMainHUD : public AHUD
{
private:
	GENERATED_BODY()

	

	UPROPERTY()
	TObjectPtr<UWorld> CurWorld = nullptr;
protected:
		virtual void BeginPlay() override;
		virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMyLayout> MainLayoutClass = nullptr;

	AMainHUD();
	
	UPROPERTY()
	TObjectPtr<UMyLayout> MainLayout = nullptr;
	TObjectPtr<UMyLayout> GetMainLayout()const{return MainLayout;}

	void ToggleInventory() const;
	void AddInventoryPickupItem(TObjectPtr<APickupBase>);
};
