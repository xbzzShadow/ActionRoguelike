// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UMInteractionComponent;
class UAnimMontage;
class UMAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API AMCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMCharacter();

protected:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* m_pCameraComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* m_pSpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UMInteractionComponent* InteractComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UMAttributeComponent* AttributeComp;

	UPROPERTY(EditAnywhere,Category=Attack)
	TSubclassOf<AActor> PrimaryAttackProjectileClass;

	UPROPERTY(EditAnywhere,Category=Attack)
	TSubclassOf<AActor> SpecialAttackProjectileClass;

	UPROPERTY(EditAnywhere, Category = Attack)
	UAnimMontage* AttrackAnim;



	FTimerHandle TimerHandle_PrimaryAttack;
	
	float SpecialProjectileDestroyTime = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);

	void PrimaryAttrack();
	void PrimaryInteract();

	void SpecialAttack();

	void PrimatyAttack_TimeElapsed();
	void SpecialAttack_TimeElapsed();

	void SpawnProjectile(TSubclassOf<AActor> SpawnActorClass);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UMAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
};
