// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "animation/AnimInstance.h"




// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create camera boom (pulls towards the player if there is a collision
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f; // Camera follows at this distance
	CameraBoom->bUsePawnControlRotation = true; // Rotate arm based on controller 

	// set size for collision capsule
	GetCapsuleComponent()->SetCapsuleSize(48.f, 103.f);


	//Create a follow camera 
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of boom and let the boom adjust to match
	// the controller orientation
	FollowCamera->bUsePawnControlRotation = false;
	

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	// dont rotate when the controller roates
	// let the movement just adjust the camera
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // character move in the direction of input....
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);// ... at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 650.f; // jump height
	GetCharacterMovement()->AirControl = 0.2f;

	MaxHealth = 100.f;
	Health = 50.f;
	MaxStamina = 350.f;
	Stamina = 120.f;
	Coins = 0;
	runningSpeed = 650.f;
	SprintSpeed = 950.f;
	bShiftKeyPressedDown = false;
	bLMBDown = false;
	bool bSkillOnePressed = false;
	bool bSkillTwoPressed = false;
	bool bSkillThreePressed = false;
	bool bSkillFourPressed = false;
	bool bSkillFivePressed = false;
	//initialize Enums
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;
	StaminaDrainRate = 25.f;
	MinSprintStamina = 50.f;



}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DeltaStamina = StaminaDrainRate * DeltaTime;
	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:
		if (bShiftKeyPressedDown)
		{
			/**  check if the stamina we about to remove will put us below the min threshold*/
			if (Stamina - DeltaStamina <= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum); // set the satus before removing stamina
				Stamina -= DeltaStamina;

			}

			else
			{
				Stamina -= DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Sprinting);// check to see if we have stamina
		}
		else
		{
			/* else remove stamina and remain in Normal state*/
			if (Stamina -= DeltaStamina >= MaxStamina)
			{
				Stamina += DeltaStamina;

			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);

		}
		

		break;
	case EStaminaStatus::ESS_BelowMinimum:
		if (bShiftKeyPressedDown)
		{
			if (Stamina - DeltaStamina <= 0.f)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				Stamina = 0;
				SetMovementStatus(EMovementStatus::EMS_Normal);

			}
			else
			{
				Stamina -= DeltaStamina;
				
			}
			SetMovementStatus(EMovementStatus::EMS_Sprinting);
		}

		else // shift key up
		{
			if (Stamina + DeltaStamina >= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal );
				Stamina += DeltaStamina;


			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal); // set movement status so animation wont play


		}
		break;
	case EStaminaStatus::ESS_Exhausted:
		if (bShiftKeyPressedDown)
		{
			Stamina = 0.f;

		}

		else // shift key up
		{
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	case EStaminaStatus::ESS_ExhaustedRecovering:

		if (Stamina + DeltaStamina >= MinSprintStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
			Stamina += DeltaStamina;
		}
		else
		{
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;

	default:
		break;
	}

}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpRate);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMain::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMain::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMain::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMain::ShiftKeyReleased);
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMain::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMain::LMBUp);

	PlayerInputComponent->BindAction("SkillOne", IE_Pressed, this, &AMain::SkillOne);
	PlayerInputComponent->BindAction("SKillOne", IE_Released, this, &AMain::SkillOneEnd);

	PlayerInputComponent->BindAction("SkillTwo", IE_Pressed, this, &AMain::SkillTwo);
	PlayerInputComponent->BindAction("SKillTwo", IE_Released, this, &AMain::SkillTwoEnd);

	PlayerInputComponent->BindAction("SkillThree", IE_Pressed, this, &AMain::SkillThree);
	PlayerInputComponent->BindAction("SKillThree", IE_Released, this, &AMain::SkillThreeEnd);

	PlayerInputComponent->BindAction("SkillFour", IE_Pressed, this, &AMain::SkillFour);
	PlayerInputComponent->BindAction("SKillFour", IE_Released, this, &AMain::SkillFourEnd);

	PlayerInputComponent->BindAction("SkillFive", IE_Pressed, this, &AMain::SkillFive);
	PlayerInputComponent->BindAction("SKillFive", IE_Released, this, &AMain::SkillFiveEnd);
	
}

void AMain::MoveForward(float Value )
{
	if (Controller != nullptr && Value != 0.0f && (!bAttacking))
	{
		// find out which wa is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}

void AMain::MoveRight(float Value )
{
	if (Controller != nullptr && Value != 0.0f && (!bAttacking))
	{
		// find out which wa is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void AMain::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds()); // move the camera at a smooth rate
}

void AMain::LookUpRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds()); // move the camera at a smooth rate

}

void AMain::DecreaseHealth(float Amount)
{
	if (Health - Amount <= 0.f)
	{
		Health -= Amount;
		Die();
	}
	else
	{
		Health -= Amount;

	}
}

void AMain::Die()
{
}

void AMain::IncreaseCoins( float Amount)
{

	Coins += Amount;
}

void AMain::LMBDown()
{
	bLMBDown = true;
	if (ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem); // cast the overlapping item to Weapon
		if (Weapon)
		{
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}

	else
	{
		if (EquipedWeapon)
		{
			Attack();
			
		 }
	}
}

void AMain::LMBUp()
{
	bLMBDown = false;
}

void AMain::SkillOne()
{
	bSkillOnePressed = true;
}

void AMain::SkillTwo()
{
	bSkillTwoPressed = true;
}

void AMain::SkillThree()
{
	bSkillThreePressed = true;
}

void AMain::SkillFour()
{
	bSkillFourPressed = true;
}

void AMain::SkillFive()
{
	bSkillFivePressed = true;
}

void AMain::SkillOneEnd()
{
	bSkillOnePressed = false;
}

void AMain::SkillTwoEnd()
{
	bSkillTwoPressed = false;
}

void AMain::SkillThreeEnd()
{
	bSkillThreePressed = false;
}

void AMain::SkillFourEnd()
{
	bSkillFourPressed = false;
}

void AMain::SkillFiveEnd()
{
	bSkillFivePressed = false;
}

void AMain::SetEquippedWeapon(AWeapon * WeaponToSet)
{
	
	
	
	if (EquipedWeapon)	
	{
		EquipedWeapon->Destroy();

	}

	EquipedWeapon = WeaponToSet;
}

void AMain::Attack()
{
	if (!bAttacking) // attack and continue if we are pressind down LMB
	{
		bAttacking = true;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			int32 Skills = 5;
			switch (Skills)
			{
			case 0:
					SkillOne();
		
					AnimInstance->Montage_Play(CombatMontage, 1.35f);
					AnimInstance->Montage_JumpToSection(FName("Attack1"), CombatMontage);
					
				break;
			case 1:

					SkillTwo();
				
					AnimInstance->Montage_Play(CombatMontage, 1.35f);
					AnimInstance->Montage_JumpToSection(FName("Attack2"), CombatMontage);
					
				
				break;
			case 2:

					SkillThree();
				
					AnimInstance->Montage_Play(CombatMontage, 1.35f);
					AnimInstance->Montage_JumpToSection(FName("DualWeaponCombo"), CombatMontage);
					
				
				break;

			case 3:
					SkillFour();
				
					AnimInstance->Montage_Play(CombatMontage, 1.35f);
					AnimInstance->Montage_JumpToSection(FName("FireBall"), CombatMontage);
					
		
				break;
			case 4:

					SkillFive();
				
					AnimInstance->Montage_Play(CombatMontage, 1.35f);
					AnimInstance->Montage_JumpToSection(FName("FlipKick"), CombatMontage);
					
				
				break;
			}
		}

		/*UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			int32 Section = FMath::RandRange(0, 4);
			switch (Section)
			{
			case 0:

				AnimInstance->Montage_Play(CombatMontage, 1.35f);
				AnimInstance->Montage_JumpToSection(FName("Attack1"), CombatMontage);
				break;
			case 1:

				AnimInstance->Montage_Play(CombatMontage, 1.35f);
				AnimInstance->Montage_JumpToSection(FName("Attack2"), CombatMontage);
				break;

			case 2:

				AnimInstance->Montage_Play(CombatMontage, 1.0f);
				AnimInstance->Montage_JumpToSection(FName("DualWeaponCombo"), CombatMontage);
				break;
			 
			
			case 3:
				AnimInstance->Montage_Play(CombatMontage, 1.35f); 
				AnimInstance->Montage_JumpToSection(FName("FireBall"), CombatMontage);
				break;
			case 4:

				AnimInstance->Montage_Play(CombatMontage, 1.35f);
				AnimInstance->Montage_JumpToSection(FName("FlipKick"), CombatMontage);
				break;
			
				break;
			
			
			
			default:
				;

		
			}
		
		}*/

	}
	
}

void AMain::AttackEnd()
{
	bAttacking = false;

	if (bLMBDown)
	{
		Attack();
	}
	 
}



void AMain::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = runningSpeed;
	}
}

void AMain::ShiftKeyDown()
{
	bShiftKeyPressedDown = true;
}

void AMain::ShiftKeyReleased()
{
	bShiftKeyPressedDown = false;
}

void AMain::ShowPickUpLocation()
{
	// loop through array and list out evry location of items we picked up and display it on screen
	// same loop as the one below
	/**
	for (int32 i = 0; i <  PickUpLocations.Num(); i++)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, PickUpLocations[i], 24.f, 12, FLinearColor::Blue, 10.f, 0.5f);  // draws a debug sphere on screen, useful for debugging

	}

	*/

	for (FVector Location : PickUpLocations)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, Location, 24.f, 12, FLinearColor::Blue, 10.f, 0.5f);  // draws a debug sphere on screen, useful for debugging

	}
}