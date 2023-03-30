// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SphereComponent.h"
#include "InputTriggers.h"
#include "SpawnableItem.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create Spring Arm and attach it to the Root(CapsuleComponent)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 400.f; // Distance from player
	SpringArm->bUsePawnControlRotation = true; // Rotate arm based on controller

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetupAttachment(GetRootComponent());
	Collider->InitSphereRadius(100.f);
	
	Collider->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnComponentBeginOverlap);
	Collider->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnComponentEndOverlap);
	

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	Inventory = TArray<FInventorySlot>();

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = 330.f;

	// Add the mapping context
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->AddMappingContext(MappingContext, 0);

		}
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Fix Some Movement in Air
	if (GetCharacterMovement()->IsFalling())
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	// Pulled this out to its own function
	Movement();


	AddControllerYawInput(Yaw);
	AddControllerPitchInput(Pitch);

}

void APlayerCharacter::Movement()
{
	//Movement
	FRotator ControlRotation = Controller->GetControlRotation();

	ControlRotation.Roll = 0.f;
	ControlRotation.Pitch = 0.f;
	
	//Getting the direction we're looking, and the right vector = cross product of forward and up vectors
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ControlRotation);
	FVector RightVector = UKismetMathLibrary::GetRightVector(ControlRotation);

	ForwardVector *= InputX;
	RightVector *= InputY;

	if (!FMath::IsNearlyZero(InputX))
	{
		AddMovementInput(ForwardVector);
	}
	if (!FMath::IsNearlyZero(InputY))
	{
		AddMovementInput(RightVector);
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Get the player controller
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (UEnhancedInputComponent* EnhanceInputCom = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhanceInputCom->BindAction(ForwardInput, ETriggerEvent::Triggered, this, &APlayerCharacter::Forward);
		EnhanceInputCom->BindAction(RightInput, ETriggerEvent::Triggered, this, &APlayerCharacter::Right);
		EnhanceInputCom->BindAction(ForwardInput, ETriggerEvent::Completed, this, &APlayerCharacter::Forward);
		EnhanceInputCom->BindAction(RightInput, ETriggerEvent::Completed, this, &APlayerCharacter::Right);

		EnhanceInputCom->BindAction(AttackInput, ETriggerEvent::Started, this, &APlayerCharacter::Attack);

		EnhanceInputCom->BindAction(MouseXInput, ETriggerEvent::Started, this, &APlayerCharacter::MouseX);
		EnhanceInputCom->BindAction(MouseYInput, ETriggerEvent::Started, this, &APlayerCharacter::MouseY);
		EnhanceInputCom->BindAction(MouseXInput, ETriggerEvent::Triggered, this, &APlayerCharacter::MouseX);
		EnhanceInputCom->BindAction(MouseYInput, ETriggerEvent::Triggered, this, &APlayerCharacter::MouseY);
		EnhanceInputCom->BindAction(MouseXInput, ETriggerEvent::Completed, this, &APlayerCharacter::MouseX);
		EnhanceInputCom->BindAction(MouseYInput, ETriggerEvent::Completed, this, &APlayerCharacter::MouseY);

		EnhanceInputCom->BindAction(InventoryInput, ETriggerEvent::Started, this, &APlayerCharacter::ToggleInventory);
		EnhanceInputCom->BindAction(UseInput, ETriggerEvent::Started, this, &APlayerCharacter::Use);

	}

}

bool APlayerCharacter::GetIsAttack()
{
	return IsAttack;
}

void APlayerCharacter::Attack(const FInputActionValue& input)
{
	IsAttack = true;
}

void APlayerCharacter::ResetAttack()
{
	IsAttack = false;
}

void APlayerCharacter::Use(const FInputActionValue& input)
{
	if(NearbyItem.Num() > 0)
	{
		PickUp();
		return;
	}
	
}

void APlayerCharacter::PickUp()
{
	AddItem(NearbyItem[0]->Item, NearbyItem[0]->Amount);

	ASpawnableItem* ItemToDestroy = NearbyItem[0];
	NearbyItem.RemoveAt(0);
	ItemToDestroy->Kill();
}

void APlayerCharacter::AddItem(UItem* item, uint8 amount)
{
	for (int i{}; i < Inventory.Num(); i++)
	{
		if (Inventory[i].Item == item)
		{
			Inventory[i].Amount += amount;
			return;
		}
	}
	Inventory.Add(FInventorySlot(item, amount));
}

void APlayerCharacter::Forward(const FInputActionValue& input)
{
	InputX = input.Get<float>();
}

void APlayerCharacter::Right(const FInputActionValue& input)
{
	InputY = input.Get<float>();
}

void APlayerCharacter::MouseX(const FInputActionValue& input)
{
	Yaw = input.Get<float>();
}

void APlayerCharacter::MouseY(const FInputActionValue& input)
{
	Pitch = input.Get<float>();
}
void APlayerCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->IsA<ASpawnableItem>())
	{
		NearbyItem.Add(Cast<ASpawnableItem>(OtherActor));
	}
	
}

void APlayerCharacter::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->IsA<ASpawnableItem>())
	{
		NearbyItem.Remove(Cast<ASpawnableItem>(OtherActor));
	}
}




