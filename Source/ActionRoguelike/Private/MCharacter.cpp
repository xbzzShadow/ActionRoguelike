// Fill out your copyright notice in the Description page of Project Settings.


#include "MCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MAttributeComponent.h"
#include "MInteractionComponent.h"

// Sets default values
AMCharacter::AMCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pSpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	m_pSpringArmComp->bUsePawnControlRotation = true;
	m_pSpringArmComp->SetupAttachment(RootComponent);

	m_pCameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	m_pCameraComp->SetupAttachment(m_pSpringArmComp);

	InteractComp = CreateDefaultSubobject<UMInteractionComponent>("InteractComp");

	AttributeComp = CreateDefaultSubobject<UMAttributeComponent>("AttributeComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void AMCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMCharacter::MoveForward(float value)
{
	FRotator ControllerRot = GetControlRotation();
	ControllerRot.Pitch = 0.0f;
	ControllerRot.Roll = 0.0f;

	AddMovementInput(ControllerRot.Vector(),value);
}

void AMCharacter::MoveRight(float value)
{
	FRotator ControllerRot = GetControlRotation();
	ControllerRot.Pitch = 0.0f;
	ControllerRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControllerRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, value);
}

void AMCharacter::PrimaryAttrack()
{
	PlayAnimMontage(AttrackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AMCharacter::PrimatyAttack_TimeElapsed, 0.2f);

}

void AMCharacter::PrimaryInteract()
{
	if (InteractComp)
	{
		InteractComp->PrimaryInteract();
	}
}

void AMCharacter::SpecialAttack()
{
	PlayAnimMontage(AttrackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AMCharacter::SpecialAttack_TimeElapsed, 0.2f);
}

void AMCharacter::PrimatyAttack_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);

	FActorSpawnParameters SpawnParms;
	SpawnParms.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParms.Instigator = this;

	GetWorld()->SpawnActor<AActor>(PrimaryAttackProjectileClass, SpawnTM, SpawnParms);
}

void AMCharacter::SpecialAttack_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);

	FActorSpawnParameters SpawnParms;
	SpawnParms.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParms.Instigator = this;

	AActor* NewSpawnActor = GetWorld()->SpawnActor<AActor>(SpecialAttackProjectileClass, SpawnTM, SpawnParms);
}

void AMCharacter::SpawnProjectile(TSubclassOf<AActor> SpawnActorClass)
{
	if (ensureAlways(SpawnActorClass))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		FVector TraceStart = m_pCameraComp->GetComponentLocation();
		FVector TraceEnd = m_pCameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 5000);

		FHitResult Hit;

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

		FCollisionShape CollsionShape;
		CollsionShape.SetSphere(30.0f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams,CollsionShape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjectileRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTM = FTransform(ProjectileRotation, HandLocation);

		GetWorld()->SpawnActor<AActor>(SpawnActorClass, SpawnTM, SpawnParams);
	}
}

void AMCharacter::OnHealthChanged(AActor* InstigatorActor, UMAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (NewHealth <= 0.0f)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);
	}
}

void AMCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChange.AddDynamic(this,&AMCharacter::OnHealthChanged);
}

// Called every frame
void AMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Lookup", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttrack", IE_Pressed, this, &AMCharacter::PrimaryAttrack);
	PlayerInputComponent->BindAction("SpecialAttack", IE_Pressed, this, &AMCharacter::SpecialAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AMCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
}

