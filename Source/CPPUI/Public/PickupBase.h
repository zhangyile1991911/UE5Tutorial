// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPPUI/Public/InventoryStruct.h"
#include "PickupBase.generated.h"


class UBoxComponent;

UCLASS(Abstract)
class CPPUI_API APickupBase : public AActor
{
	GENERATED_BODY()

protected:
	// Sets default values for this actor's properties
	APickupBase();

	UPROPERTY(EditAnywhere)
	FInventoryStruct structInfo;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UStaticMesh> CustomStaticMesh;

	UPROPERTY(BlueprintReadOnly,Category="components")
	TObjectPtr<UStaticMeshComponent> LooksStaticMesh;

	UPROPERTY(BlueprintReadOnly,Category="components")
	TObjectPtr<UBoxComponent> BoxComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION()
	void OnAActorIn(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnAActorOut(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FName& PickupText(){return structInfo.PickupText;}
	void BePickedup();
	const FInventoryStruct& GetInventoryStruct()const{return structInfo;}
	void SetInventoryStruct(const FInventoryStruct& Fs);
};
