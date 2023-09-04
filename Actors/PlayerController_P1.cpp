// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController_P1.h"
#include "Blueprint/UserWidget.h"




void APlayerController_P1::BeginPlay()
{


	Super::BeginPlay();

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}

	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);
}
