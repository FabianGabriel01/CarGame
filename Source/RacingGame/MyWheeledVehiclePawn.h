// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "RocketLauncher.h"
#include "MyWheeledVehiclePawn.generated.h"


class UCameraComponent;
class USpringArmComponent;
class UAudioComponent;
class USpotLightComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UPhysicsThrusterComponent;
class USoundBase;
class USceneCaptureComponent2D;
class UMaterialParameterCollection;
class UMediaSoundComponent;
class UMediaPlaylist;
class UBoxComponent;
class UArrowComponent;
class UDecalComponent;
class UMaterialInstanceDynamic;
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

	///////HUD
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHUD(float Speed, float RPMPercentage, int CurrentGear, float NitroPercentage, bool NitroPressed);

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bNitroPressed = false;

	float CurrentNitroValue = 100;

	//////////////////
	UPROPERTY(EditAnywhere)
		USceneCaptureComponent2D* SceneCapture;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UMaterialParameterCollection* MaterialsCollection;

	/////RadioSystem
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMediaSoundComponent* MediaPlayer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CurrentPlaylist = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UMediaPlaylist*>  RadioPlaylists;

	/// <summary>
	/// ///////////////////////////////////
	/// </summary>
	/*UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBoxComponent* BoxLeft;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBoxComponent* BoxRight;

	UFUNCTION(BlueprintCallable)
	void BoxCollisionOverlap_L(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void BoxCollisionOverlap_R(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/

	////for attack
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UArrowComponent* Arrow;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<class ARocketLauncher> RocketClass;

	UFUNCTION()
		void Shoot();

	UFUNCTION()
		void TargetShoot();

	UFUNCTION(BlueprintCallable)
	void OnJump();




	////////////Dirt Decal
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UDecalComponent* DirtDelcalL;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UDecalComponent* DirtDelcalR;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UDecalComponent* DirtDelcalCenter;


	//////////Health
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Health;

	/////ForDamage
	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UMaterialInstanceDynamic* DecalSides;
	UMaterialInstanceDynamic* DecalCenter;


protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	void SetIncreaseSmokeExhaust();
	void SetDecreaseSmokeExhaust();
	void EnabledNitrous();
	void DisableNitrous();

	void ActivateTrials(bool IsHandBrake);
	void DeactivateTrials();

	///Radio
	void RadioPressed();

	UNiagaraComponent* InItNiagaraNitrous(FName SocketName);
};
