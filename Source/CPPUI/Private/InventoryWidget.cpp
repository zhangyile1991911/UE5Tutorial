// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPUI/Public/InventoryWidget.h"

#include "InventorySlot.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "CPPUI/Public/PickupBase.h"
#include "Runtime/Experimental/Voronoi/Private/voro++/src/container.hh"


void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp,Log,TEXT("UInventoryWidget::NativeConstruct()"));
	checkf(slot1,TEXT("slot1 is invalid"));

	container_ui.Add(slot1);
	container_ui.Add(slot2);
	container_ui.Add(slot3);
	container_ui.Add(slot4);
	container_ui.Add(slot5);

	container_data.Reserve(container_ui.Num());
	
	slot1->btn->OnClicked.AddDynamic(this,&UInventoryWidget::ClickSlot1);
	slot2->btn->OnClicked.AddDynamic(this,&UInventoryWidget::ClickSlot2);
	slot3->btn->OnClicked.AddDynamic(this,&UInventoryWidget::ClickSlot3);
	slot4->btn->OnClicked.AddDynamic(this,&UInventoryWidget::ClickSlot4);
	slot5->btn->OnClicked.AddDynamic(this,&UInventoryWidget::ClickSlot5);
}

void UInventoryWidget::NativeDestruct()
{
	UE_LOG(LogTemp,Log,TEXT("UInventoryWidget::NativeDestruct()"));
	
	container_ui.Empty();
	container_data.Empty();
	Super::NativeDestruct();
}

void UInventoryWidget::ClickSlot1()
{
	if(container_data.Num() > 0)
		ClickSlotFunc.Broadcast(container_data[0],0);
}

void UInventoryWidget::ClickSlot2()
{
	if(container_data.Num() > 1)
		ClickSlotFunc.Broadcast(container_data[1],1);
}

void UInventoryWidget::ClickSlot3()
{
	if(container_data.Num() > 2)
		ClickSlotFunc.Broadcast(container_data[2],2);
}

void UInventoryWidget::ClickSlot4()
{
	if(container_data.Num() > 3)
		ClickSlotFunc.Broadcast(container_data[3],3);
}

void UInventoryWidget::ClickSlot5()
{
	if(container_data.Num() > 4)
		ClickSlotFunc.Broadcast(container_data[4],4);
}


void UInventoryWidget::AddItem(const FInventoryStruct& item)
{
	UE_LOG(LogTemp,Log,TEXT("UInventoryWidget::AddItem %s"),*item.PickupText.ToString());
	// bool bIsChange = false;
	if(container_data.Num() >= container_ui.Num())return;

	UE_LOG(LogTemp,Log,TEXT("UInventoryWidget::AddItem container_data before %d"),container_data.Num());
	container_data.Add(item);
	UE_LOG(LogTemp,Log,TEXT("UInventoryWidget::AddItem container_data after %d"),container_data.Num());

	RefreshInventory();
}

void UInventoryWidget::RemoveItem(const FInventoryStruct& item)
{
	// bool bIsChange = false;
	for(int i = 0;i < container_data.Num();i++)
	{
		if(container_data[i].ItemImage == item.ItemImage)
		{
			container_data.RemoveAt(i);
			// container_data[i].ItemImage = nullptr;
			// container_data[i].ActionText = "";
			// container_data[i].PickupText = "";
			// container_data[i].AddEnergy = 0;
			// container_data[i].AddHealth = 0;
			// container_data[i].AddMood = 0;
			// bIsChange = true;
			break;
		}
	}
	// if(!bIsChange)return;

	RefreshInventory();
}


void UInventoryWidget::RefreshInventory()
{
	// UE_LOG(LogTemp,Log,TEXT("UInventoryWidget::RefreshInventory() container_data %d"),container_ui.Num());
	for(int i = 0;i < container_ui.Num();++i)
	{
		if(i >= container_data.Num())
		{
			container_ui[i]->img->SetBrushFromTexture(nullptr);	
		}
		else
		{
			container_ui[i]->img->SetBrushFromTexture(container_data[i].ItemImage);
		}
		// UE_LOG(LogTemp,Log,TEXT("UInventoryWidget::RefreshInventory() container_data %s"),*container_ui[i]->GetName());
	}

	// UE_LOG(LogTemp,Log,TEXT("UInventoryWidget::RefreshInventory() container_data %d"),container_data.Num());
	
	// for(int i = 0;i < container_data.Num();++i)
	// {
	// 	UE_LOG(LogTemp,Log,TEXT("UInventoryWidget::RefreshInventory() xxxxx index = %d container_data %s"),i,*container_data[i].PickupText.ToString());
	// 	// UE_LOG(LogTemp,Log,TEXT("UInventoryWidget::RefreshInventory() index = %d"),i);
	// }
}
