// Fill out your copyright notice in the Description page of Project Settings.


#include "MyLayout.h"

#include "ActionWidget.h"
#include "InventoryWidget.h"
#include "CPPUI/CPPUICharacter.h"
#include "CPPUI/Public/MyStatusInfo.h"
#include "UMG.h"
#include "CPPUI/Public/PickupBase.h"
bool UMyLayout::Initialize()
{
	
	auto result = Super::Initialize();
	UE_LOG(LogTemp, Warning,TEXT("UMyLayout::Initialize result %d"),result);
	return result;

// 	//
// 	// AcppuiCharacter->HealthUpdate.AddDynamic(this,&UMyHUD::updateHealthPB);
// 	// AcppuiCharacter->EnergyUpdate.AddDynamic(this,&UMyHUD::updateEnergyPB);
// 	// AcppuiCharacter->MoodUpdate.AddDynamic(this,&UMyHUD::updateMoodPB);
// 	//
// 	// AcppuiCharacter->HealthUpdate.RemoveDynamic(this,&UMyHUD::updateHealthPB);
//
// 	return true;
}

void UMyLayout::NativeConstruct()
{
	UE_LOG(LogTemp, Warning,TEXT("UMyLayout::NativeConstruct"));
	Super::NativeConstruct();

	TObjectPtr<APlayerController> pc = GetWorld()->GetFirstPlayerController();
	AcppuiCharacter = Cast<ACPPUICharacter>(pc->GetPawn());
	checkf(AcppuiCharacter, TEXT("Unable to get a reference to the player character"));
	BIsInventoryShow = false;

	InventoryMenu->ClickSlotFunc.AddLambda([this](const FInventoryStruct& info,int index){
		ActionMenu->SetVisibility(ESlateVisibility::Visible);
		ActionMenu->UseText->SetText(FText::FromName(info.ActionText));
		CurSelect = &info;
		
	});
	
	ActionMenu->UseBtn->OnClicked.AddDynamic(this,&UMyLayout::ActionBtnFunc);
	ActionMenu->DropBtn->OnClicked.AddDynamic(this,&UMyLayout::DropBtnFunc);
	ActionMenu->CancelBtn->OnClicked.AddDynamic(this,&UMyLayout::CancelBtnFunc);
}

void UMyLayout::ActionBtnFunc()
{
	auto cha = GetWorld()->GetFirstPlayerController()->GetCharacter();
	auto cppcha = Cast<ACPPUICharacter>(cha);
	checkf(cppcha,TEXT("Cast<ACPPUICharacter>(cha) is invalid"))
	cppcha->AddEnergy(CurSelect->AddEnergy);
	cppcha->AddMood(CurSelect->AddMood);
	cppcha->AddHealth(CurSelect->AddHealth);

	InventoryMenu->RemoveItem(*CurSelect);
	CurSelect = nullptr;

	ActionMenu->SetVisibility(ESlateVisibility::Hidden);
}

void UMyLayout::DropBtnFunc()
{
	auto cha = GetWorld()->GetFirstPlayerController()->GetCharacter();
	auto cppcha = Cast<ACPPUICharacter>(cha);
	checkf(cppcha,TEXT("Cast<ACPPUICharacter>(cha) is invalid"))
	
	// auto worldPosition = cppcha->GetDropPosition();
	// auto actorPosition = cppcha->GetActorLocation();
	// UE_LOG(LogTemp,Log,TEXT("DropPosition = %f %f %f | actorPosition = %f %f %f"),
	// 	worldPosition.X,worldPosition.Y,worldPosition.Z,
	// 	actorPosition.X,actorPosition.Y,actorPosition.Z);
	// auto resultPosition = worldPosition + actorPosition;
	
	auto worldPosition = cppcha->GetDropPosition();
	auto world = GetWorld();
	FActorSpawnParameters param;
	param.Name = FName("drop position");
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	// param.Owner = world;
	// param.bDeferConstruction = true;
	APickupBase* pb = world->SpawnActor<APickupBase>(CurSelect->InstanceClass, worldPosition, FRotator::ZeroRotator,param);
	pb->SetInventoryStruct(*CurSelect);
	
	// pb->FinishSpawning();
	UE_LOG(LogTemp,Log,TEXT(" attach parent actor = %s"),pb->GetAttachParentActor() == nullptr ? TEXT("no parent") : TEXT("have parent"))
	
	InventoryMenu->RemoveItem(*CurSelect);
	CurSelect = nullptr;
	ActionMenu->SetVisibility(ESlateVisibility::Hidden);
	
}

void UMyLayout::CancelBtnFunc()
{
	ActionMenu->SetVisibility(ESlateVisibility::Hidden);
}

void UMyLayout::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// UE_LOG(LogTemp, Warning,TEXT("UMyHUD::NativeTick"));
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(pickupList.IsEmpty())
	{
		tips->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	refreshTip();
}

void UMyLayout::NativeDestruct()
{
	UE_LOG(LogTemp, Warning,TEXT("UMyLayout::NativeDestruct"));
	Super::NativeDestruct();
}

void UMyLayout::updateHealthPB(int oldVal, int newVal, int maxVal)
{
	int curVal = FMathf::Clamp(newVal,0,maxVal);
	StatusInfo->HealthPb->SetPercent(static_cast<float>(curVal)/ static_cast<float>(maxVal));
}

void UMyLayout::updateEnergyPB(int oldVal, int newVal, int maxVal)
{
	int curVal = FMathf::Clamp(newVal,0,maxVal);
	StatusInfo->EnergyPb->SetPercent(static_cast<float>(curVal)/ static_cast<float>(maxVal));
}

void UMyLayout::updateMoodPB(int oldVal, int newVal, int maxVal)
{
	int curVal = FMathf::Clamp(newVal,0,maxVal);
	StatusInfo->MoodPb->SetPercent(static_cast<float>(curVal)/ static_cast<float>(maxVal));
}

void UMyLayout::refreshTip()
{
	for(int i = 0;i < pickupList.Num();i++)
	{
		TObjectPtr<APickupBase> pickupActor = pickupList[i];	
		TObjectPtr<APlayerController> pc = GetWorld()->GetFirstPlayerController();
		FVector location = pickupActor->GetActorLocation();
		FVector2D ScreenLocation;
		if(UGameplayStatics::ProjectWorldToScreen(pc,location,ScreenLocation))
		{
			// UE_LOG(LogTemp,Warning,TEXT(" refresh tip %s"),pickupActor->PickupText())
			tips->SetVisibility(ESlateVisibility::Visible);
			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(tips->Slot);
			if (CanvasSlot)
			{
				CanvasSlot->SetPosition(ScreenLocation);
			}
			tips->SetText(FText::FromName(pickupActor->PickupText()));
			break;
		}
	}
}

void UMyLayout::ClearAllHandler()
{
	AcppuiCharacter->HealthUpdate.Clear();
	AcppuiCharacter->EnergyUpdate.Clear();
	AcppuiCharacter->MoodUpdate.Clear();
}

void UMyLayout::BindAllHandler()
{
	AcppuiCharacter->HealthUpdate.AddDynamic(this,&UMyLayout::updateHealthPB);
	AcppuiCharacter->EnergyUpdate.AddDynamic(this,&UMyLayout::updateEnergyPB);
	AcppuiCharacter->MoodUpdate.AddDynamic(this,&UMyLayout::updateMoodPB);
}

void UMyLayout::AddPickupBaseInfo(TObjectPtr<APickupBase> one)
{
	pickupList.Add(one);
}

void UMyLayout::RemovePickupBaseInfo(TObjectPtr<APickupBase> one)
{
	pickupList.Remove(one);
}

void UMyLayout::InventorySliderIn()
{
	if(bIsMoving)return;
	PlayAnimation(SliderIn);
	BIsInventoryShow = true;
}

void UMyLayout::InventorySliderOut()
{
	if(bIsMoving)return;
	PlayAnimation(SliderOut);
	BIsInventoryShow = false;
}

void UMyLayout::AddInventoryItem(TObjectPtr<APickupBase> item) const
{
	UE_LOG(LogTemp,Log,TEXT("UMyLayout::AddInventoryItem"));
	InventoryMenu->AddItem(item->GetInventoryStruct());
}

