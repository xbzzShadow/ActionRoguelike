// Fill out your copyright notice in the Description page of Project Settings.


#include "MExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AMExplosiveBarrel::AMExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	BaseMesh->SetSimulatePhysics(true);
	RootComponent = BaseMesh;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(BaseMesh);
	ForceComp->SetAutoActivate(false);

	ForceComp->Radius = 2000.0f;
	ForceComp->ImpulseStrength = 2500.0f;
	ForceComp->bImpulseVelChange = true;

	ForceComp->AddCollisionChannelToAffect(ECollisionChannel::ECC_WorldDynamic);
}

// Called when the game starts or when spawned
void AMExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BaseMesh->OnComponentHit.AddDynamic(this, &AMExplosiveBarrel::OnActorHit);
}

void AMExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();

	UE_LOG(LogTemp, Log, TEXT("OnActorHit In ExplosiveBarrel"));

	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	FString CombineString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombineString, nullptr, FColor::Red, 2.0f);
}

