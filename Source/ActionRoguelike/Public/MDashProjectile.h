// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MProjectileBase.h"
#include "MDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AMDashProjectile : public AMProjectileBase
{
	GENERATED_BODY()
	
public:
	AMDashProjectile();

protected:
	virtual void Explode_Implementation() override;

	virtual void BeginPlay() override;

	void TeleportInstigator();

protected:
	UPROPERTY(EditDefaultsOnly,Category = "Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;

	FTimerHandle TimerHandle_DelayedDetonate;
};
