// Fill out your copyright notice in the Description page of Project Settings.


#include "MItemChest.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AMItemChest::AMItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	GoldMesh = CreateDefaultSubobject<UStaticMeshComponent>("GoldMesh");
	GoldMesh->SetupAttachment(BaseMesh);

	GoldParticle = CreateDefaultSubobject<UParticleSystemComponent>("GoldParticle");
	GoldParticle->SetupAttachment(GoldMesh);
}

void AMItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(110.0f, 0.0f, 0.0f));
}

// Called when the game starts or when spawned
void AMItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

