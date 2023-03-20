// Fill out your copyright notice in the Description page of Project Settings.


#include "MMagicProjecttile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "MAttributeComponent.h"

// Sets default values
AMMagicProjecttile::AMMagicProjecttile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AMMagicProjecttile::OnActorOverlap);
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->ProjectileGravityScale = 0;
}

// Called when the game starts or when spawned
void AMMagicProjecttile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMMagicProjecttile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AMMagicProjecttile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor !=GetInstigator())
	{
		UMAttributeComponent* AttributeComp = Cast<UMAttributeComponent>(OtherActor->GetComponentByClass(UMAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(DamageAmount);

			Destroy();
		}
	}
}

/////////////////////////


// Sets default values
AMBlackholeProjectile::AMBlackholeProjectile()
	:AMMagicProjecttile()
{
	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(SphereComp);

	EObjectTypeQuery AffectType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ForceComp->RemoveObjectTypeToAffect(AffectType);
	ForceComp->AddCollisionChannelToAffect(ECollisionChannel::ECC_WorldDynamic);

	ForceComp->Radius = 2500.0f;
	ForceComp->ImpulseStrength = -10000.0f;
	ForceComp->bImpulseVelChange = true;
	ForceComp->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void AMBlackholeProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

// Called every frame
void AMBlackholeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AMBlackholeProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	LastTime = GetWorld()->GetTimeSeconds();
	GetWorldTimerManager().SetTimer(SpawnForce, this, &AMBlackholeProjectile::TimerEvent_SpawnForce, 0.1f, true);

	SphereComp->OnComponentHit.AddDynamic(this, &AMBlackholeProjectile::OnActorHit);
}

void AMBlackholeProjectile::TimerEvent_SpawnForce()
{
	//UE_LOG(LogTemp, Log, TEXT("Current Destroy Time: %f  ,  LastTime: %f"), DestroyTime,LastTime);

	ForceComp->FireImpulse();

	DestroyTime += GetWorld()->GetTimeSeconds() - LastTime;
	LastTime = GetWorld()->GetTimeSeconds();

	if (DestroyTime > 5.0f)
		GetWorld()->DestroyActor(this);

}

void AMBlackholeProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("Destroy Actor : %s"), *GetNameSafe(OtherActor));

	GetWorld()->DestroyActor(OtherActor);
}
