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
#include "MediaSoundComponent.h"
#include "MediaPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMaterialLibrary.h"

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

	MediaPlayer = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("MediaPlayer"));
	MediaPlayer->SetupAttachment(RootComponent);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);

	DirtDelcalL = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalL"));
	DirtDelcalL->SetupAttachment(GetMesh());

	DirtDelcalR = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalR"));
	DirtDelcalR->SetupAttachment(GetMesh());

	DirtDelcalCenter = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalCenter"));
	DirtDelcalCenter->SetupAttachment(GetMesh());

	/*BoxLeft = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxL"));
	BoxLeft->SetupAttachment(GetMesh());
	BoxLeft->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	BoxRight = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxR"));
	BoxRight->SetupAttachment(GetMesh());
	BoxRight->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);*/
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


//void AMyWheeledVehiclePawn::BoxCollisionOverlap_L(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	UE_LOG(LogTemp, Warning, TEXT("BOX L"));
//}
//
//void AMyWheeledVehiclePawn::BoxCollisionOverlap_R(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	UE_LOG(LogTemp, Warning, TEXT("BOX R"));
//}

void AMyWheeledVehiclePawn::Shoot()
{
	UE_LOG(LogTemp, Warning, TEXT("ROCKET LAUNCHED"));
	if (RocketClass)
		GetWorld()->SpawnActor<ARocketLauncher>(RocketClass, Arrow->GetComponentLocation(), Arrow->GetComponentRotation(), FActorSpawnParameters());
	else
		UE_LOG(LogTemp, Warning, TEXT("NOT ROCKET"));
}

void AMyWheeledVehiclePawn::TargetShoot()
{
	UE_LOG(LogTemp, Warning, TEXT("GuideMissile"));
	TArray<AActor*> Cars;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyWheeledVehiclePawn::StaticClass(), Cars);
	AActor* ChoosenTarget = nullptr;
	for (AActor* Element : Cars) 
	{
		if(Element != this)
		{
			if (this->GetHorizontalDotProductTo(Element) > 0.8) 
			{
				ChoosenTarget = Element;
				break;
			}
		}
	}


	ARocketLauncher* TargetMisil = GetWorld()->SpawnActorDeferred<ARocketLauncher>(RocketClass, Arrow->GetComponentTransform());
	if (ChoosenTarget) 
	{
		TargetMisil->SetHomingMisime(ChoosenTarget->GetRootComponent());
	}
	TargetMisil->FinishSpawning(Arrow->GetComponentTransform());
}

void AMyWheeledVehiclePawn::OnJump() 
{
	GetMesh()->AddImpulse(FVector(0,0,700), FName("Root"), true);
}

void AMyWheeledVehiclePawn::OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("RECIEVING DAMAGE"));

	Health = UKismetMathLibrary::Abs(Health - Damage);
	if (!DecalSides || !DecalCenter) 
	{
		DecalSides = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), DirtDelcalL->GetDecalMaterial());
		DecalCenter = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), DirtDelcalCenter->GetDecalMaterial());
	}

	float DefaultOpacity = DecalSides->K2_GetScalarParameterValue(FName("Opacity"));
	DecalSides->SetScalarParameterValue(FName("Dirt"), 2 + DefaultOpacity);
	DecalCenter->SetScalarParameterValue(FName("Dirt"), 2 + DefaultOpacity);

	DirtDelcalL->SetDecalMaterial(DecalSides);
	DirtDelcalR->SetDecalMaterial(DecalSides);
	DirtDelcalCenter->SetDecalMaterial(DecalCenter);
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
	PlayerInputComponent->BindAction(FName("Nitrous"), EInputEvent::IE_Released, this, &AMyWheeledVehiclePawn::DisableNitrous);
	PlayerInputComponent->BindAction(FName("Nitrous"), EInputEvent::IE_Pressed, this, &AMyWheeledVehiclePawn::EnabledNitrous);

	/////RadioInputs
	PlayerInputComponent->BindAction(FName("RadioB"), EInputEvent::IE_Pressed, this, &AMyWheeledVehiclePawn::RadioPressed);
	PlayerInputComponent->BindAction(FName("Shoot"), EInputEvent::IE_Pressed, this, &AMyWheeledVehiclePawn::Shoot);
	PlayerInputComponent->BindAction(FName("TargetShoot"), EInputEvent::IE_Pressed, this, &AMyWheeledVehiclePawn::TargetShoot);
	//PlayerInputComponent->BindAction(FName("Jump"), EInputEvent::IE_Pressed, this, &AMyWheeledVehiclePawn::OnJump);

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


}

void AMyWheeledVehiclePawn::BeginPlay() 
{
	Super::BeginPlay();
	Health = 100;

	this->OnTakeAnyDamage.AddDynamic(this, &AMyWheeledVehiclePawn::OnTakeDamage);

	//BoxLeft->OnComponentBeginOverlap.AddDynamic(this, &AMyWheeledVehiclePawn::BoxCollisionOverlap_L);
	//BoxRight->OnComponentBeginOverlap.AddDynamic(this, &AMyWheeledVehiclePawn::BoxCollisionOverlap_R);
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

void AMyWheeledVehiclePawn::RadioPressed()
{
	if (!(RadioPlaylists.Num() > 0 && MediaPlayer)) return;
	UE_LOG(LogTemp, Warning, TEXT("RADIO BUTTON"));

	if (CurrentPlaylist == RadioPlaylists.Num() - 1)
	{
		CurrentPlaylist = -1;
		MediaPlayer->GetMediaPlayer()->Pause();
	}
	else 
	{
		CurrentPlaylist++;
		MediaPlayer->GetMediaPlayer()->OpenPlaylist(RadioPlaylists[0]);
	}
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


