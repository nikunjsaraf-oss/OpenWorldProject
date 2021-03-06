// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class OPENWORLDPROJECT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, Category="Components")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category="Properties")
	float TurnRate = 60;

	UPROPERTY(EditDefaultsOnly, Category="Properties")
	float MaxStamina = 1;

	UPROPERTY(VisibleAnywhere, Category="Properties")
	float Stamina;

	UPROPERTY(EditDefaultsOnly, Category="Properties")
	float StaminaDrainRate;

	UPROPERTY(VisibleAnywhere, Category="Properties")
	bool bIsSprinting = false;

	float DefaultWalkSpeed;
	bool bIsShiftKeyPressed = false;

	FTimerHandle TimerHandle_StaminaCheck;
	FTimerHandle TimerHandle_StaminaRegeneration;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void LookRight(float Value);
	void StartSprint();
	void EndSprint();
	void RegenerateStamina(); 


public:
	// Sets default values for this character's properties
	AMainCharacter();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetStamina() const { return Stamina; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsSprinting() const { return bIsSprinting; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
