// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"

#include "Blueprint/UserWidget.h"
#include "CPPUI/Public/MyLayout.h"

AMainHUD::AMainHUD()
{
	UE_LOG(LogTemp, Warning, TEXT("Hello World"));
}

void AMainHUD::ToggleInventory() const
{
	if(MainLayout->GetBIsInventoryShow())
	{
		MainLayout->InventorySliderOut();
	}
	else
	{
		MainLayout->InventorySliderIn();
	}
}



void AMainHUD::BeginPlay()
{
	UE_LOG(LogTemp,Warning,TEXT("AMainHUD::BeginPlay()"))
	Super::BeginPlay();
	
	CurWorld = GetWorld();
	checkf(CurWorld, TEXT("Failed to reference world."));
	//
	//
	MainLayout = CreateWidget<UMyLayout>(CurWorld,MainLayoutClass);
	checkf(MainLayout,TEXT("Invalid UILayout reference"));
	MainLayout->AddToViewport();
	MainLayout->SetVisibility(ESlateVisibility::Visible);

	MainLayout->BindAllHandler();
	
	
}

void AMainHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	MainLayout->ClearAllHandler();
	Super::EndPlay(EndPlayReason);
}

void AMainHUD::AddInventoryPickupItem(TObjectPtr<APickupBase> item)
{
	UE_LOG(LogTemp,Log,TEXT("AMainHUD::AddInventoryPickupItem"));
	MainLayout->AddInventoryItem(item);
}


