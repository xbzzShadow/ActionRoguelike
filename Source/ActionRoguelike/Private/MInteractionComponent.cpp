// Fill out your copyright notice in the Description page of Project Settings.


#include "MInteractionComponent.h"
#include "MInterface.h"

// Sets default values for this component's properties
UMInteractionComponent::UMInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRot;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRot);
	FVector End = EyeLocation + EyeRot.Vector() * 1000.0f;

	//FHitResult Hit;
	//bool bIsHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, QueryParams);

	float Radius = 30.0f;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	TArray<FHitResult> Hits;
	bool bIsHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, QueryParams, Shape);

	for (int32 i = 0; i < Hits.Num(); i++)
	{
		AActor* HitActor = Hits[i].GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<UMInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);

				IMInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}

			DrawDebugSphere(GetWorld(), End, Radius, 32, FColor::Green, false, 2.0f, 0.0f, 2.0f);
		}
	}


	DrawDebugLine(GetWorld(), EyeLocation, End, FColor::Green, false, 2.0f, 0.0f, 2.0f);
}