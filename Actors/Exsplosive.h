// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Exsplosive.generated.h"

/**
 * 
 */
UCLASS()
class ACTORS_API AExsplosive : public AItem
{
	GENERATED_BODY()

		
public:
		AExsplosive();

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float Damage;
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

		virtual void onEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
