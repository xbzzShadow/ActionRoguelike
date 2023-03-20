// Fill out your copyright notice in the Description page of Project Settings.


#include "MDashProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

AMDashProjectile::AMDashProjectile()
{
	TeleportDelay = 0.0f;
	DetonateDelay = 0.0f;

	MovementComp->InitialSpeed = 6000.0f;
}

void AMDashProjectile::Explode_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Dash Projectile Explode"));

	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	EffectComp->DeactivateSystem();

	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &AMDashProjectile::TeleportInstigator, TeleportDelay);
}

void AMDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &AMDashProjectile::Explode, DetonateDelay);
}

void AMDashProjectile::TeleportInstigator()
{
	UE_LOG(LogTemp, Warning, TEXT("Teleport After Dash Projectile Explode"));

	AActor* InstigatorActor = GetInstigator();
	if (ensure(InstigatorActor))
	{
		InstigatorActor->TeleportTo(GetActorLocation(), GetActorRotation());
	}
}
