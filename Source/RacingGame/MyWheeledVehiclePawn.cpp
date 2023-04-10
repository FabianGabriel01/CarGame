// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWheeledVehiclePawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/cameraComponent.h"
#include "ChaosVehicleMovementComponent.h"
#include "Components/AudioComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Components/SpotLightComponent.h"

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

}


void AMyWheeledVehiclePawn::MoveForward(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("MOVE FORWARD"));
	GetVehicleMovementComponent()->SetThrottleInput(Value);

	OnLight = false;
}


void AMyWheeledVehiclePawn::MoveBackward(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("MOVE BACKWARD"));

	OnLight = true;
	GetVehicleMovementComponent()->SetBrakeInput(Value);
	
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
}

void AMyWheeledVehiclePawn::BreakReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("Break Released"));
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
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


	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, FString::Printf(TEXT("Bool: %s"), OnLight ? TEXT("true") : TEXT("false")));
	}*/

}


