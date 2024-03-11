// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBase.h"

#include "MyLayout.h"
#include "Components/BoxComponent.h"
#include "CPPUI/CPPUICharacter.h"
#include "CPPUI/Public/MainHUD.h"

// Sets default values
APickupBase::APickupBase()
{
	UE_LOG(LogTemp,Log,TEXT("APickupBase::APickupBase()"))
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	UE_LOG(LogTemp,Log,TEXT("APickupBase::APickupBase() AddHealth = %d AddEnergy = %d AddMood = %d"),structInfo.AddHealth,structInfo.AddEnergy,structInfo.AddMood);
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	UE_LOG(LogTemp,Log,TEXT("APickupBase::BeginPlay()"))
	Super::BeginPlay();
	UStaticMeshComponent* MyMeshComponent = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if (MyMeshComponent)
	{
		// 成功获取到组件，可以在这里使用MyMeshComponent
		MyMeshComponent->SetStaticMesh(CustomStaticMesh);
	}
	BoxComponent = FindComponentByClass<UBoxComponent>();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&APickupBase::OnAActorIn);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this,&APickupBase::OnAActorOut);
	structInfo.InstanceClass = this->GetClass();
	structInfo.MeshComponent = FindComponentByClass<UStaticMeshComponent>()->GetStaticMesh();
	UE_LOG(LogTemp,Log,TEXT("APickupBase::BeginPlay() AddHealth = %d AddEnergy = %d AddMood = %d"),structInfo.AddHealth,structInfo.AddEnergy,structInfo.AddMood);
}

void APickupBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(LogTemp,Log,TEXT("APickupBase::EndPlay %s"),*UEnum::GetValueAsString(EndPlayReason));
	BoxComponent->OnComponentBeginOverlap.Clear();
	BoxComponent->OnComponentEndOverlap.Clear();
	structInfo.InstanceClass = nullptr;
	Super::EndPlay(EndPlayReason);
}


// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickupBase::BePickedup()
{
	UE_LOG(LogTemp,Warning,TEXT("APickupBase::BePickedup() %s"),*structInfo.PickupText.ToString())
	auto curHUd = GetWorld()->GetFirstPlayerController()->GetHUD();
	auto curMain = Cast<AMainHUD>(curHUd);
	curMain->GetMainLayout()->RemovePickupBaseInfo(this);
	this->Destroy();
}

void APickupBase::SetInventoryStruct(const FInventoryStruct& Fs)
{
	structInfo = Fs;
	FindComponentByClass<UStaticMeshComponent>()->SetStaticMesh(Fs.MeshComponent);
}

void APickupBase::OnAActorIn(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp,Log,TEXT("APickupBase::OnAActorIn OtherActor = %s"),*OtherActor->GetName());
	auto curHUd = GetWorld()->GetFirstPlayerController()->GetHUD();
	auto curMain = Cast<AMainHUD>(curHUd);
	curMain->GetMainLayout()->AddPickupBaseInfo(this);

	auto ch = Cast<ACPPUICharacter>(OtherActor);
	if(ch == nullptr)return;
	ch->AddPickupItem(this);
}

void APickupBase::OnAActorOut(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex)
{
	UE_LOG(LogTemp,Log,TEXT("APickupBase::OnAActorOut OtherActor = %s"),*OtherActor->GetName());
	auto curHUd = GetWorld()->GetFirstPlayerController()->GetHUD();
    auto curMain = Cast<AMainHUD>(curHUd);
	curMain->GetMainLayout()->RemovePickupBaseInfo(this);

	auto ch = Cast<ACPPUICharacter>(OtherActor);
	ch->RemovePickupItem(this);
}

