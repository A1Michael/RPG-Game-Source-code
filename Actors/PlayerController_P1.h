// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerController_P1.generated.h"

/**
 * 
 */
UCLASS()
class ACTORS_API APlayerController_P1 : public APlayerController
{
	GENERATED_BODY()

public:



	/** Refrence to the UMG asset in the editor*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<class UUserWidget> HUDOverlayAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		UUserWidget* HUDOverlay;

protected:
	virtual void BeginPlay() override;
	
};
