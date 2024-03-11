// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CPPUICharacter.generated.h"

class APickupBase;
class AMainHUD;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FIntStatUpdate,
	int32,OldVal,
	int32,newVal,
	int32,maxVal);


UCLASS(config=Game)
class ACPPUICharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ToggleInventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* PickupAction;

	
	
	UPROPERTY()
	int HealthValue;
	
	UPROPERTY()
	int EnergyValue;
	
	UPROPERTY()
	int MoodValue;

	

public:
	ACPPUICharacter();

	UPROPERTY()
	FIntStatUpdate HealthUpdate;

	UPROPERTY()
	FIntStatUpdate EnergyUpdate;
	
	UPROPERTY()
	FIntStatUpdate MoodUpdate;
protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;
	float totalDeltaSeconds = 0;

	UPROPERTY()
	TObjectPtr<AMainHUD> MainHUD;

	TArray<TObjectPtr<APickupBase>> PickupItemList;

	TObjectPtr<AMainHUD> GetMainHUD();

	
public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void ToggleInventory(const FInputActionValue& Value);

	void PickupItem(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="setup")
	TObjectPtr<USceneComponent> DropPosition;
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface
public:
	void AddEnergy(int val);
	void AddHealth(int val);
	void AddMood(int val);
public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	void AddPickupItem(TObjectPtr<APickupBase> item);
	void RemovePickupItem(TObjectPtr<APickupBase> item);
	FVector GetDropPosition() const;

};

