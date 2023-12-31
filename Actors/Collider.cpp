// Fill out your copyright notice in the Description page of Project Settings.


#include "Collider.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ColliderMovementComponent.h"





// Sets default values
ACollider::ACollider()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject <USphereComponent>(TEXT("SphereComponent"));
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	//SphereComponent->SetupAttachment(GetRootComponent());

	SetRootComponent(SphereComponent);
	SphereComponent->InitSphereRadius(40.f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	MeshComponent = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (MeshComponentAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);
		MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -40.f));
		MeshComponent->SetWorldScale3D(FVector (0.8f, 0.8f, 0.8f));
	}

	SpringArm = CreateDefaultSubobject <USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, -0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	
	ourMovementComponent = CreateDefaultSubobject<UColliderMovementComponent>(TEXT("OurMovementComponent"));
	ourMovementComponent->UpdatedComponent = RootComponent;
	CameraInput = FVector2D(0.f, 0.f);

	
	AutoPossessPlayer = EAutoReceiveInput::Player0; 
}

// Called when the game starts or when spawned
void ACollider::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += CameraInput.X;
	SetActorRotation(NewRotation);

	FRotator NewSpringArmRotation = SpringArm-> GetComponentRotation();
	NewSpringArmRotation.Pitch = FMath::Clamp(NewSpringArmRotation.Pitch += CameraInput.Y, -80.f, -15.f);
	SpringArm->SetWorldRotation(NewSpringArmRotation);
}

// Called to bind functionality to input
void ACollider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACollider::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACollider::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &ACollider::YawCamera);
	PlayerInputComponent->BindAxis(TEXT("CamerapITCH"), this, &ACollider::PitchCamera);



}

void ACollider::MoveForward(float input)
{
	FVector Forward = GetActorForwardVector();
	AddMovementInput(input * Forward);
	if (ourMovementComponent)
	{
		ourMovementComponent->AddInputVector(Forward * input);
	}
}
void ACollider::MoveRight(float input)
{
	FVector Right = GetActorRightVector();
	AddMovementInput(input * Right);
	if (ourMovementComponent)
	{
		ourMovementComponent->AddInputVector(Right * input);
	}
	
}

void ACollider::PitchCamera(float AXisValue)
{
	CameraInput.Y = AXisValue;

}

void ACollider::YawCamera(float AXisValue)
{

	CameraInput.X = AXisValue;
}

UPawnMovementComponent* ACollider::GetMovementComponent() const
{

	return ourMovementComponent;
}