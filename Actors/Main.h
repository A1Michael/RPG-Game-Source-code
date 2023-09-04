// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),

	EMS_MAX UMETA(DisplayName = "DefaultMax")

};

/***/

UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_BelowMinimum UMETA(DisplayName = "BelowMinimum"),
	ESS_Exhausted UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecovering UMETA(DisplayName = "ExhaustedRecovering"),

	EMS_MAX UMETA(DisplayName = "DefaultMax")

};


UCLASS()	
class ACTORS_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

	
	TArray<FVector> PickUpLocations;
	UFUNCTION(BlueprintCallable)
		void ShowPickUpLocation();

	/** Movementstatus that will be used to change the characters states for animation*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera = "Enums")
		EMovementStatus MovementStatus;

	/** Stamina that will be used to change the characters Condition for sprinting */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera = "Enums")
		EStaminaStatus StaminaStatus;

	void SetMovementStatus(EMovementStatus Status);
	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera = "Movement")
		float StaminaDrainRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera = "Movement")
		float MinSprintStamina;

	/** Camera boom positioning behind player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAcess = "true"))
		class USpringArmComponent* CameraBoom;

	/** follow camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAcess = "true"))
		class UCameraComponent* FollowCamera;

	/** Base turn rate to scale turning functions for the camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = " Running")
		float runningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
		float SprintSpeed;


	/** pressed down to start sprinting*/
	void ShiftKeyDown();
	void ShiftKeyReleased();
	bool bShiftKeyPressedDown;

	/**
	Player Stats
	*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
		float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
		float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
		float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
		float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
		int32 Coins;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// called for forward and backwards inputs
	void MoveForward(float Value);
	// called for side to side input
	void MoveRight(float Value);

	// called via input to turn at a given rate
	// @param Rate , This is a normalised rate, i.e 1.0 means 100% of desired turn rate
	void TurnAtRate(float Rate);

	// called via input to turn at a given rate
	// @param Rate , This is a normalised rate, i.e 1.0 means 100% of desired look up/down rate
	void LookUpRate(float Rate);

	/** Set up camera properties*/
	FORCEINLINE class USpringArmComponent* GetCameraBoom()const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera()const { return FollowCamera; }

	void DecreaseHealth(float Amount);
	void Die();
	void IncreaseCoins(float Amount);
	bool bLMBDown;
	void LMBDown();
	void LMBUp();
	void SkillOne();
	void SkillTwo();
	void SkillThree();
	void SkillFour();
	void SkillFive();
	
	void SkillOneEnd();
	void SkillTwoEnd();
	void SkillThreeEnd();
	void SkillFourEnd();
	void SkillFiveEnd();

public:
	bool bSkillOnePressed;
	bool bSkillTwoPressed;
	bool bSkillThreePressed;
	bool bSkillFourPressed;
	bool bSkillFivePressed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera = "Items")
	class AWeapon* EquipedWeapon;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera = "Items")
	//class AWeapon* LeftEquipedWeapon;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera = "Items")
	//class AWeapon* RightEquipedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera = "Items")
	class AItem* ActiveOverlappingItem;

	void SetEquippedWeapon(AWeapon* WeaponToSet);
	FORCEINLINE void SetActiveOverlappingItem(AItem* Item) { ActiveOverlappingItem = Item; }
	FORCEINLINE AWeapon* GetEquippedWeapon() { return EquipedWeapon;}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category =  "Anims")
	bool bAttacking;
	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;

};
