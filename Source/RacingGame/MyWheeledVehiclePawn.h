// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "MyWheeledVehiclePawn.generated.h"


class UCameraComponent;
class USpringArmComponent;
class UAudioComponent;
class USpotLightComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UPhysicsThrusterComponent;
class USoundBase;
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
	USoundBase* NitrousSound;

	UPROPERTY(EditAnywhere)
	UAudioComponent* AC_Nitrous;

	/// <summary>
	/// /////////////////////
	/// </summary>

	UPROPERTY(EditAnywhere)
	USpotLightComponent* Spotlight;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* ExhaustR;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* ExhaustL;

	/////////////////NITROUS
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* Nitrous;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* NitrousL;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* NitrousR;

	///////////////
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* WheelFR;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* WheelFL;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* WheelBR;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* WheelBL;

	UPROPERTY(EditAnywhere)
	UPhysicsThrusterComponent* NitrousBoost;

	UPROPERTY(EditAnywhere)
		float LocationImpulseX;

	UPROPERTY(EditAnywhere)
		float LocationImpulseY;

	UPROPERTY(EditAnywhere)
		float LocationImpulseZ;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;

	void SetIncreaseSmokeExhaust();
	void SetDecreaseSmokeExhaust();
	void EnabledNitrous();
	void DisableNitrous();

	void ActivateTrials(bool IsHandBrake);
	void DeactivateTrials();

	UNiagaraComponent* InItNiagaraNitrous(FName SocketName);
};
