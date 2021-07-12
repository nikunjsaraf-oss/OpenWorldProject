// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	Stamina = MaxStamina;
	StaminaDrainRate = 0.1f;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_StaminaRegeneration, this, &AMainCharacter::RegenerateStamina, 1, true);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AMainCharacter::LookRight);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::EndSprint);
}

void AMainCharacter::MoveForward(const float Value)
{
	// AddMovementInput(GetActorForwardVector() * Value);

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::MoveRight(const float Value)
{
	// AddMovementInput(GetActorRightVector() * Value);

	// Controllers are non - physical actors that can possess a Pawn to control its actions.
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// To find which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		// To get the world direction
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::LookUp(const float Value)
{
	AddControllerPitchInput(Value * TurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::LookRight(const float Value)
{
	AddControllerYawInput(Value * TurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::StartSprint()
{
	if (Stamina > 0 && GetMovementComponent()->Velocity.Size() > 0)
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = 2 * DefaultWalkSpeed;
		Stamina -= StaminaDrainRate;

		GetWorldTimerManager().SetTimer(TimerHandle_StaminaCheck, this, &AMainCharacter::StartSprint, 1, true);

		if (Stamina <= 0.00f)
		{
			bIsSprinting = false;
			GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
		}
	}
}

void AMainCharacter::EndSprint()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	GetWorldTimerManager().ClearTimer(TimerHandle_StaminaCheck);
}

void AMainCharacter::RegenerateStamina()
{
	if (Stamina < 1 && !bIsSprinting)
	{
		Stamina += StaminaDrainRate / 2;

		if (Stamina > MaxStamina)
		{
			Stamina = MaxStamina;
		}
	}
}
