// Fill out your copyright notice in the Description page of Project Settings.


#include "ColliderMovementComponent.h"


void UColliderMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime,TickType,ThisTickFunction);

	if (!PawnOwner|| !UpdatedComponent || ShouldSkipUpdate(DeltaTime )) // early exit,if conditions are true do nothing
	{
			return;
	}

	FVector DesireMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f);// return the value of the input vector  squish it down to 1 then clears it out to 0

	if (!DesireMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesireMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

		// if we bumb into something , slide along the side of it
		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesireMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
			UE_LOG(LogTemp, Warning, TEXT("valid blocking hit"));
		}
	}
}
