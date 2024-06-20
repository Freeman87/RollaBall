// Fill out your copyright notice in the Description page of Project Settings.


#include "RollaBallPlayerPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"

#include "InputDataConfig.h"  // Mine

// Sets default values
ARollaBallPlayerPawn::ARollaBallPlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create Components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	RootComponent = Mesh;
	SpringArm->SetupAttachment(Mesh);
	Camera->SetupAttachment(SpringArm);

	Mesh->SetSimulatePhysics(true);

}

// Called when the game starts or when spawned
void ARollaBallPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	// account for mass in force
	MoveForce *= Mesh->GetMass();
	JumpImpulse *= Mesh->GetMass();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			InputSubsystem->AddMappingContext(IMC_Gameplay, 0);
		}
	}

	// Binding OnComponentHit event here
	if (Mesh)
	{
		Mesh->OnComponentHit.AddDynamic(this, &ARollaBallPlayerPawn::OnHitGround);
	}
}

// Called to bind functionality to input
void ARollaBallPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (InputActions)
		{
			if (InputActions->IA_Jump)
			{
				EnhancedInputComponent->BindAction(InputActions->IA_Jump, ETriggerEvent::Triggered, this, &ARollaBallPlayerPawn::JumpAction);

			}
			if (InputActions->IA_Move)
			{
				EnhancedInputComponent->BindAction(InputActions->IA_Move, ETriggerEvent::Triggered, this, &ARollaBallPlayerPawn::MoveAction);
			}
		}
	}
}

void ARollaBallPlayerPawn::MoveAction(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	FVector ForwardVector = Camera->GetForwardVector();
	FVector RightVector = Camera->GetRightVector();

	// Remove vertical component (z-axis) from the forward and right vectors
	ForwardVector.Z = 0.0f;
	RightVector.Z = 0.0f;
	ForwardVector.Normalize();
	RightVector.Normalize();

	// Apply force in the direction the camera is facing
	FVector ForceDirection = (ForwardVector * MovementVector.Y + RightVector * MovementVector.X).GetSafeNormal();
	Mesh->AddForce(ForceDirection * MoveForce);
}

void ARollaBallPlayerPawn::JumpAction(const FInputActionValue& Value)
{
	if (JumpCount >= MaxJumpCount) { return;}

	// Apply an upward force to simulate a jump
	FVector JumpForce = FVector(0.0f, 0.0f, JumpImpulse);
	Mesh->AddImpulse(JumpForce);

	JumpCount++;
}

void ARollaBallPlayerPawn::OnHitGround(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Hit.Normal.Z > 0) // Check if the ball hits the ground
	{
		JumpCount = 0;
	}
}

