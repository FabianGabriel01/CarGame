// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RocketLauncher.generated.h"

class USkeletalMeshComponent;
class USphereComponent;
class UParticleSystem;
class USoundBase;
class UProjectileMovementComponent;
class UNiagaraComponent;

UCLASS()
class RACINGGAME_API ARocketLauncher : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARocketLauncher();

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere)
		UParticleSystem* ParticleS;

	UPROPERTY(EditAnywhere)
		USoundBase* SoundExplode;

	UPROPERTY(EditAnywhere)
		UProjectileMovementComponent* ProjectileMomvement;

	UPROPERTY(EditAnywhere)
		UNiagaraComponent* NiagaraSmoke;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void SphereCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
