// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWheeledVehiclePawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/cameraComponent.h"
#include "ChaosVehicleMovementComponent.h"
#include "Components/AudioComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Components/SpotLightComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Materials/MaterialParameterCollection.h"

AMyWheeledVehiclePawn::AMyWheeledVehiclePawn() 
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 600.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->SetRelativeLocation(FVector(0,0,140.f));
	SpringArm->SetRelativeRotation(FRotator(-20.0,0.f, 0));
	SpringArm->bUsePawnControlRotation = true;

	EngineSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioEngine"));
	EngineSound->SetupAttachment(GetMesh());

	Spotlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	Spotlight->SetupAttachment(GetMesh());
	Spotlight->SetRelativeLocation(FVector(-170.f, 80.f, 80.f));
	Spotlight->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	Spotlight->Intensity = 0.f;

	ExhaustR = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraR"));
	ExhaustR->SetupAttachment(GetMesh(), FName("LeftExhaust"));

	ExhaustL = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraL"));
	ExhaustL->SetupAttachment(GetMesh(), FName("RightExhaust"));

	WheelFR = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrialFR"));
	WheelFR->SetupAttachment(GetMesh(), FName("FR"));

	WheelFL = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrialFL"));
	WheelFL->SetupAttachment(GetMesh(), FName("FL"));

	WheelBR = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrialBR"));
	WheelBR->SetupAttachment(GetMesh(), FName("BR"));

	WheelBL = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrialBL"));
	WheelBL->SetupAttachment(GetMesh(), FName("BL"));

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceenCapture"));
	SceneCapture->SetupAttachment(GetMesh());
	SceneCapture->SetRelativeLocation(FVector(-230, 0, 130));
	SceneCapture->SetRelativeRotation(FRotator(0, 180,0));




}


void AMyWheeledVehiclePawn::MoveForward(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("MOVE FORWARD"));
	GetVehicleMovementComponent()->SetThrottleInput(Value);

}


void AMyWheeledVehiclePawn::MoveBackward(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("MOVE BACKWARD"));

	GetVehicleMovementComponent()->SetBrakeInput(Value);
	if (Value > 0 && !WheelFR->IsActive())
	{
		ActivateTrials(false);
	}
	if (Value == 0 && WheelFR->IsActive())
	{
		DeactivateTrials();
	}
	
}

void AMyWheeledVehiclePawn::MoveRightLeft(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("MOVE RIGHLEFT"));
	GetVehicleMovementComponent()->SetSteeringInput(Value);
}

void AMyWheeledVehiclePawn::BreakPresed()
{
	UE_LOG(LogTemp, Warning, TEXT("break pressed"));
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
	ActivateTrials(true);
}

void AMyWheeledVehiclePawn::BreakReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("Break Released"));
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
	DeactivateTrials();
}

void AMyWheeledVehiclePawn::LookUpDown(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("LOOK UP DOWN"));
	AddControllerPitchInput(Value *-1);
}

void AMyWheeledVehiclePawn::LookRightLeft(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("LOOK RIGHT LEFT"));
	AddControllerYawInput(Value);
}

void AMyWheeledVehiclePawn::Lights(bool Value)
{

	////USE CHANGE MATERIAL PAREMETER COLLECTION to turn on Light

}




void AMyWheeledVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AMyWheeledVehiclePawn::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveBackward"), this, &AMyWheeledVehiclePawn::MoveBackward);
	PlayerInputComponent->BindAxis(FName("MoveLeftRight"), this, &AMyWheeledVehiclePawn::MoveRightLeft);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AMyWheeledVehiclePawn::LookUpDown);
	PlayerInputComponent->BindAxis(FName("LookLeftRight"), this, &AMyWheeledVehiclePawn::LookRightLeft);

	PlayerInputComponent->BindAction(FName("Brake"), EInputEvent::IE_Pressed, this, &AMyWheeledVehiclePawn::BreakPresed);
	PlayerInputComponent->BindAction(FName("Brake"), EInputEvent::IE_Released, this, &AMyWheeledVehiclePawn::BreakReleased);
	PlayerInputComponent->BindAction(FName("Nitrous"), EInputEvent::IE_Pressed, this, &AMyWheeledVehiclePawn::EnabledNitrous);
	PlayerInputComponent->BindAction(FName("Nitrous"), EInputEvent::IE_Released, this, &AMyWheeledVehiclePawn::DisableNitrous);
}

void AMyWheeledVehiclePawn::Tick(float DeltaSeconds)
{
	//Super::Tick(float DeltaSeconds);

	UChaosWheeledVehicleMovementComponent* VehicleComponent = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent());
	if (!VehicleComponent) 
	{
		UE_LOG(LogTemp, Warning, TEXT("NO ChAOSWHELEED MOVEMENT"));
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("RPM %f"), VehicleComponent->GetEngineRotationSpeed());
	EngineSound->SetFloatParameter(FName("RPM"), VehicleComponent->GetEngineRotationSpeed());

	const float RPM = VehicleComponent->GetEngineRotationSpeed();
	const float RPMPercentage = 100 * RPM / 7000;
	const float Speed = (float)UKismetMathLibrary::FCeil(VehicleComponent->GetForwardSpeedMPH());
	const int CurrentGear = VehicleComponent->GetCurrentGear();

	if (bNitroPressed) 
	{
		CurrentNitroValue = UKismetMathLibrary::FInterpTo(CurrentNitroValue, 0, DeltaSeconds, 1);
		if (CurrentNitroValue < 10) 
		{
			DisableNitrous();
		}
	}
	else 
	{
		CurrentNitroValue = UKismetMathLibrary::FInterpTo(CurrentNitroValue, 100, DeltaSeconds, 0.1f);
	}

	UpdateHUD(Speed, RPMPercentage,CurrentGear, CurrentNitroValue, bNitroPressed);


	if (RPM >= 1300 && RPM < 7000) 
	{
		SetIncreaseSmokeExhaust();
	}
	else 
	{
		SetDecreaseSmokeExhaust();
	}

	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, FString::Printf(TEXT("Bool: %s"), OnLight ? TEXT("true") : TEXT("false")));
	}*/

}

void AMyWheeledVehiclePawn::SetIncreaseSmokeExhaust()
{
	ExhaustL->SetFloatParameter(FName("SpawnRateRPM"), 600);
	ExhaustR->SetFloatParameter(FName("SpawnRateRPM"), 600);
}

void AMyWheeledVehiclePawn::SetDecreaseSmokeExhaust()
{
	ExhaustL->SetFloatParameter(FName("SpawnRateRPM"), 100);
	ExhaustR->SetFloatParameter(FName("SpawnRateRPM"), 100);
}

void AMyWheeledVehiclePawn::EnabledNitrous()
{
	if (!Nitrous) 
	{
		UE_LOG(LogTemp, Warning, TEXT("havent nigara system set"));
		return;
	}
	NitrousL = InItNiagaraNitrous(FName("LeftExhaust"));
	NitrousR = InItNiagaraNitrous(FName("RightExhaust"));

	NitrousBoost = NewObject<UPhysicsThrusterComponent>(this, UPhysicsThrusterComponent::StaticClass());
	NitrousBoost->SetupAttachment(GetMesh());
	NitrousBoost->SetRelativeLocation(FVector(LocationImpulseX, LocationImpulseY, LocationImpulseZ));
	//NitrousBoost->SetRelativeLocation(FVector(142.f, 0.f, 50.f));
	NitrousBoost->SetRelativeRotation(FRotator(180.f, 0, 0));
	NitrousBoost->ThrustStrength = 4000000;
	NitrousBoost->RegisterComponent();

	if (!NitrousBoost)
		UE_LOG(LogTemp, Warning, TEXT("NITRO NOT INITIALIZE"));
	NitrousBoost->Activate();

	if (NitrousSound) 
	{
		AC_Nitrous = NewObject<UAudioComponent>(this, UAudioComponent::StaticClass());
		AC_Nitrous->SetupAttachment(GetMesh());
		AC_Nitrous->SetSound(NitrousSound);
		AC_Nitrous->SetVolumeMultiplier(0.5f);
		AC_Nitrous->Activate();

	}

	bNitroPressed = true;
}

void AMyWheeledVehiclePawn::DisableNitrous()
{
	if (NitrousL && NitrousR) 
	{
		NitrousL->Deactivate();
		NitrousR->Deactivate();

		NitrousL->DestroyComponent();
		NitrousR->DestroyComponent();
	}

	if (NitrousBoost)
	{
		NitrousBoost->Deactivate();
		NitrousBoost->DestroyComponent();
	}

	if (AC_Nitrous) 
	{
		AC_Nitrous->Deactivate();
		AC_Nitrous->DestroyComponent();
	}

	bNitroPressed = false;
}

UNiagaraComponent* AMyWheeledVehiclePawn::InItNiagaraNitrous(FName SocketName)
{
	UNiagaraComponent* NC = NewObject<UNiagaraComponent>(this, UNiagaraComponent::StaticClass());
	NC->SetupAttachment(GetMesh(), SocketName);
	NC->SetAsset(Nitrous);
	NC->RegisterComponent();
	return NC;
}

void AMyWheeledVehiclePawn::ActivateTrials(bool IsHandBrake)
{
	WheelBR->Activate();
	WheelBL->Activate();

	if (IsHandBrake)
		return;
	WheelFR->Activate();
	WheelFL->Activate();

}

void AMyWheeledVehiclePawn::DeactivateTrials()
{
	WheelFR->Deactivate();

	WheelFL->Deactivate();

	WheelBR->Deactivate();

	WheelBL->Deactivate();
}


