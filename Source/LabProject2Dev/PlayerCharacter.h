// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


struct FInputActionValue;

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	FInventorySlot(){}
	FInventorySlot(class UItem* item, uint8 amount)
	{
		Item = item;
		Amount = amount;
		
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UItem* Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 Amount;
};

UCLASS()
class LABPROJECT2DEV_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class USphereComponent* Collider;
		

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
	class UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
	class UInputAction* ForwardInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
	class UInputAction* RightInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
	class UInputAction* MouseXInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
	class UInputAction* MouseYInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
	class UInputAction* AttackInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
	class UInputAction* InventoryInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
	class UInputAction* UseInput;


	UFUNCTION()
	bool GetIsAttack();

	UFUNCTION()
	void ResetAttack();

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleInventory();

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnComponentEndOverlap(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

private:
	void Forward(const FInputActionValue& input);
	void Right(const FInputActionValue& input);
	void MouseX(const FInputActionValue& input);
	void MouseY(const FInputActionValue& input);
	void Attack(const FInputActionValue& input);
	void Use(const FInputActionValue& input);

	void PickUp();
	void AddItem(UItem* item, uint8 amount);

	void Movement();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
		float InputX;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
		float InputY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		TArray<FInventorySlot> Inventory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		TArray<class ASpawnableItem*> NearbyItem;
		
		
		

private:

	bool IsAttack;

	float Yaw;
	float Pitch;

};


