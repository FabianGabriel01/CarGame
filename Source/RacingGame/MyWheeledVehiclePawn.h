// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "MyWheeledVehiclePawn.generated.h"


class UCameraComponent;
class USpringArmComponent;
class UAudioComponent;
class USpotLightComponent;
/**
 * 
 */
UCLASS()
class RACINGGAME_API AMyWheeledVehiclePawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()
	
private:
/// <summary>
/// Movement FUNCTION
/// </summary>
/// 
	void MoveForward(float Value);
	void MoveBackward(float Value);
	void MoveRightLeft(float Value);
	void BreakPresed();
	void BreakReleased();

/// <summary>
/// LOOK
/// </summary>
/// 
	void LookUpDown(float Value);
	void LookRightLeft(float Value);

	void Lights(bool Value);


public:
	AMyWheeledVehiclePawn();

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	/*UPROPERTY(EditAnywhere)
	TArray<class UChaosVehicleWheel*> Wheels;*/

	UPROPERTY(EditAnywhere)
	UAudioComponent* EngineSound;

	UPROPERTY(EditAnywhere)
	USpotLightComponent* Spotlight;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;
};
