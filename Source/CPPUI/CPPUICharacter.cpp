// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPPUICharacter.h"
#include "CPPUIProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "CPPUI/Public/MainHUD.h"
#include "MyLayout.h"
#include "Components/ArrowComponent.h"
#include "Engine/LocalPlayer.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ACPPUICharacter

ACPPUICharacter::ACPPUICharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));


	DropPosition = CreateDefaultSubobject<USceneComponent>(TEXT("DropPositionComponent"));
	DropPosition->SetupAttachment(GetCapsuleComponent());
	UE_LOG(LogTemp,Log,TEXT("ACPPUICharacter::ACPPUICharacter DropPosition Create"));
	
	UE_LOG(LogTemp, Warning,TEXT("ACPPUICharacter::ACPPUICharacter() Initial value"));
	HealthValue = 100;
	EnergyValue = 1000;
	MoodValue = 100;
}

void ACPPUICharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	HealthUpdate.Broadcast(HealthValue,HealthValue,100);
	EnergyUpdate.Broadcast(EnergyValue,EnergyValue,1000);
	MoodUpdate.Broadcast(MoodValue,MoodValue,100);

	UE_LOG(LogTemp,Log,TEXT("ACPPUICharacter::BeginPlay DropPosition %s"),DropPosition != nullptr ? TEXT("not null") : TEXT("null"));
	UE_LOG(LogTemp,Log,TEXT("ACPPUICharacter::BeginPlay DropPosition %s"),DropPosition->GetAttachmentRoot() != nullptr ? TEXT("not null") : TEXT("null"));
	UE_LOG(LogTemp,Log,TEXT("ACPPUICharacter::BeginPlay DropPosition %s"),*DropPosition->GetComponentLocation().ToString());
	UE_LOG(LogTemp,Log,TEXT("ACPPUICharacter::BeginPlay DropPosition %s"),*DropPosition->GetRelativeLocation().ToString());
	UE_LOG(LogTemp,Log,TEXT("ACPPUICharacter::BeginPlay DropPosition %s"),*DropPosition->GetComponentTransform().TransformPosition(DropPosition->GetComponentLocation()).ToString());
	UE_LOG(LogTemp,Log,TEXT("ACPPUICharacter::BeginPlay DropPosition %s"),*DropPosition->GetComponentToWorld().GetLocation().ToString());
	UE_LOG(LogTemp,Log,TEXT("ACPPUICharacter::BeginPlay DropPosition %s"),*DropPosition->GetAttachmentRoot()->GetComponentTransform().TransformPosition(DropPosition->GetComponentLocation()).ToString());
	UE_LOG(LogTemp,Log,TEXT("ACPPUICharacter::BeginPlay DropPosition %s"),*this->ActorToWorld().GetLocation().ToString());
	UE_LOG(LogTemp,Log,TEXT("ACPPUICharacter::BeginPlay DropPosition %s"),*this->GetActorTransform().TransformPosition(DropPosition->GetComponentLocation()).ToString());
	
}

void ACPPUICharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	totalDeltaSeconds +=DeltaSeconds;
	if(totalDeltaSeconds < 1.0f)return;

	totalDeltaSeconds = 0.0f;
	int oldValue =  EnergyValue;
	EnergyValue += 10;
	EnergyValue = FMathf::Clamp(EnergyValue,0,1000);
	EnergyUpdate.Broadcast(oldValue,EnergyValue,1000);
}

//////////////////////////////////////////////////////////////////////////// Input

void ACPPUICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPPUICharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPPUICharacter::Look);

		EnhancedInputComponent->BindAction(ToggleInventoryAction,ETriggerEvent::Triggered,this,&ACPPUICharacter::ToggleInventory);

		EnhancedInputComponent->BindAction(PickupAction,ETriggerEvent::Triggered,this,&ACPPUICharacter::PickupItem);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACPPUICharacter::AddEnergy(int val)
{
	int oldVal = EnergyValue;
	EnergyValue += val;
	EnergyValue = FMathf::Clamp(EnergyValue,0,1000);
	EnergyUpdate.Broadcast(oldVal,EnergyValue,1000);
}

void ACPPUICharacter::AddHealth(int val)
{
	int oldVal = HealthValue;
	HealthValue += val;
	HealthValue = FMathf::Clamp(HealthValue,0,100);
	HealthUpdate.Broadcast(oldVal,HealthValue,100);
}

void ACPPUICharacter::AddMood(int val)
{
	int oldVal = MoodValue;
	MoodValue += val;
	MoodValue = FMathf::Clamp(MoodValue,0,100);
	MoodUpdate.Broadcast(oldVal,MoodValue,100);
}

void ACPPUICharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller == nullptr)
	{
		return;
	}
	if(EnergyValue <= 1)return;

	int oldValue = EnergyValue;
	EnergyValue -= 1;
	EnergyValue = FMathf::Clamp(EnergyValue,0,1000);
	EnergyUpdate.Broadcast(oldValue,EnergyValue,1000);
	// add movement 
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);

	
}

void ACPPUICharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACPPUICharacter::ToggleInventory(const FInputActionValue& Value)
{
	GetMainHUD()->ToggleInventory();
}

void ACPPUICharacter::PickupItem(const FInputActionValue& Value)
{
	if(PickupItemList.IsEmpty())return;
	
	UE_LOG(LogTemp,Log,TEXT("ACPPUICharacter::PickupItem"));
	auto first = PickupItemList[0];
	
	GetMainHUD()->AddInventoryPickupItem(first);
	
	first->BePickedup();
	PickupItemList.RemoveAt(0);
}

void ACPPUICharacter::AddPickupItem(TObjectPtr<APickupBase> item)
{
	if(item == nullptr)return;
	PickupItemList.Add(item);
}

void ACPPUICharacter::RemovePickupItem(TObjectPtr<APickupBase> item)
{
	if(item == nullptr)return;
	PickupItemList.Remove(item);
}

FVector ACPPUICharacter::GetDropPosition() const
{
	return this->GetActorTransform().TransformPosition(DropPosition->GetComponentLocation());
	// FVector compLocal = DropPosition->GetComponentLocation();
	// if(DropPosition->GetAttachParentActor() != nullptr)
	// {
	// 	FVector world = DropPosition->GetAttachParentActor()->GetTransform().TransformPosition(compLocal);
	// 	// FVector world = DropPosition->GetAttachParent()->GetComponentTransform().TransformPosition(compLocal);
	// 	return world;	
	// }
	// return compLocal;
}

TObjectPtr<AMainHUD> ACPPUICharacter::GetMainHUD()
{
	if(MainHUD == nullptr)
	{
		auto pc= GetWorld()->GetFirstPlayerController();
		MainHUD = Cast<AMainHUD>(pc->GetHUD());
	}
	return MainHUD;
}

void ACPPUICharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool ACPPUICharacter::GetHasRifle()
{
	return bHasRifle;
}

