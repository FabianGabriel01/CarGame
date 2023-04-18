// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketLauncher.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RacingGame/MyDamageType.h"
#include "Components/SceneComponent.h"
#include "UObject/WeakObjectPtrTemplates.h"

// Sets default values
ARocketLauncher::ARocketLauncher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComp->SetRelativeScale3D(FVector(0.7f));
	SphereComp->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SphereComp);

	ProjectileMomvement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileMomvement->InitialSpeed = 5000;
	ProjectileMomvement->MaxSpeed = 5000;
	ProjectileMomvement->ProjectileGravityScale = 0;
	ProjectileMomvement->SetAutoActivate(false);

	NiagaraSmoke = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	NiagaraSmoke->SetupAttachment(SphereComp);

}

void ARocketLauncher::SetHomingMisime(TWeakObjectPtr<USceneComponent> SceneComp)
{
	ProjectileMomvement->bIsHomingProjectile = true;
	ProjectileMomvement->HomingAccelerationMagnitude = 3000;
	ProjectileMomvement->HomingTargetComponent = SceneComp;
}

// Called when the game starts or when spawned
void ARocketLauncher::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ARocketLauncher::SphereCollisionOverlap);
	ProjectileMomvement->SetAutoActivate(true);
	if (SoundExplode) 
	{
		UGameplayStatics::SpawnSound2D(GetWorld(), SoundExplode);
	}
	
}

// Called every frame
void ARocketLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ARocketLauncher::SphereCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	UE_LOG(LogTemp, Warning, TEXT("COLLIDING WITH SOMETHING"));
	TArray<AActor*> IgnoredActor;
	IgnoredActor.Add(this);
	UGameplayStatics::ApplyRadialDamage(GetWorld(), 100, GetActorLocation(), 500, UMyDamageType::StaticClass(), IgnoredActor);
	if (ParticleS) 
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleS, GetActorLocation());
	}
	if (SoundExplode) 
	{
		UGameplayStatics::SpawnSound2D(GetWorld(), SoundExplode);
	}

	Destroy();
}
