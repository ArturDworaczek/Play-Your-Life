// Game Created By Artur Dworaczek, Located In Somerset UK.

#include "PlayerCPP.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/SceneCaptureComponent.h"
#include "Engine.h"
#include "GameFramework/Character.h"
#include "PlayerStateCPP.h"
#include "Components/InputComponent.h"
#include "PlayerControllerCPP.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "UnrealNetwork.h"

// APlayerCPP
APlayerCPP::APlayerCPP()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 350.f;
	GetCharacterMovement()->AirControl = 0.01f;
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	DefaultSpeed = GetCharacterMovement()->MaxWalkSpeed;
	SpeedIncrease = 25.0f;
	SpeedDecrease = 25.0f;

	// Chat Proximity Range
	ChatProximity = CreateDefaultSubobject<USphereComponent>(TEXT("Chat Proximity Range"));
	ChatProximity->SetSphereRadius(1500.0f);
	ChatProximity->SetupAttachment(GetMesh(), TEXT(""));

	// Create A Arrow To Make Line Traces Forward
	LineTArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Line Trace Arrow"));
	LineTArrow->SetupAttachment(GetMesh(), TEXT("Head"));
	LineTArrow->SetRelativeLocation(FVector(0.0f, 0.0f, 1500.0f));
	LineTArrow->SetRelativeRotation(FRotator(110.6f, -90.0f, -20.6f));

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create A Map Camera
	MapCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MapCamera"));
	MapCamera->SetupAttachment(GetMesh(), TEXT(""));
	MapCamera->SetRelativeLocation(FVector(0.0f, -5.0f, 20.0f));
	MapCamera->SetRelativeRotation(FRotator(0.0f, 90.0f, 90.0f));

	// Scene Capture Component
	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Scene Capture"));
	SceneCapture->SetupAttachment(MapCamera);

	// Create A Third-Person Camera
	TPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TPCamera"));
	TPCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	TPCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	// Create A First-Person Camera
	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	FPCamera->SetupAttachment(GetMesh(), TEXT("Head"));
	FPCamera->bUsePawnControlRotation = true;
	FPCamera->SetRelativeLocation(FVector(0.0f, -5.0f, 20.0f));
	FPCamera->SetRelativeRotation(FRotator(0.0f, 90.0f, 90.0f));
}
// APlayerCPP Multiplayer
void APlayerCPP::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(APlayerCPP, bJump);
	DOREPLIFETIME(APlayerCPP, bSprinting);
	DOREPLIFETIME(APlayerCPP, DefaultSpeed);
	DOREPLIFETIME(APlayerCPP, bWalkingBackward);
	DOREPLIFETIME(APlayerCPP, bWalkingForward);
	DOREPLIFETIME(APlayerCPP, bFirstPerson);
}
// INPUTS
void APlayerCPP::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set Up Gameplay Key Bindings
	check(PlayerInputComponent);

	// ACTION BINDS
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCPP::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCPP::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCPP::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCPP::StopSprinting);
	PlayerInputComponent->BindAction("ZoomCameraIn", IE_Pressed, this, &APlayerCPP::ZoomCameraIn);
	PlayerInputComponent->BindAction("ZoomCameraOut", IE_Pressed, this, &APlayerCPP::ZoomCameraOut);
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &APlayerCPP::Interact);


	// AXIS BINDS
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCPP::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCPP::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

/////////////////// FUNCTIONS
// CALLED EVERY FRAME
void APlayerCPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
// CALLED AT THE BEGINNING OF THE GAME
void APlayerCPP::BeginPlay()
{
	// Call The Base Class
	Super::BeginPlay();
}
/////////////////// AXIS FUNCTIONS
void APlayerCPP::MoveForward(float Value)
{
	APlayerControllerCPP* Controller = Cast<APlayerControllerCPP>(GetController());

	if (Controller->bGesturePlaying == false)
	{
		if (Value < 0.0f)
		{
			SetbWalkingBackward(true);
		}
		else
		{
			SetbWalkingBackward(false);
		}
		if (Value > 0.0f)
		{
			SetbWalkingForward(true);
		}
		else
		{
			SetbWalkingForward(false);
		}
		if ((Controller != NULL) && (Value != 0.0f))
		{
			// Find Out Which Way Is Forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// Get Forward Vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
	}
}
void APlayerCPP::MoveRight(float Value)
{
	APlayerControllerCPP* Controller = Cast<APlayerControllerCPP>(GetController());

	if (Controller->bGesturePlaying == false)
	{
		if ((Controller != NULL) && (Value != 0.0f))
		{
			// Find Out Which Way Is Right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// Get Right Vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// Add Movement
			AddMovementInput(Direction, Value);
		}
	}
}

/////////////////// ACTION FUNCTIONS
// CAMERA ZOOM IN/OUT FUNCTIONS
void APlayerCPP::ZoomCameraIn()
{
	if (CameraBoom->TargetArmLength > 100.0f)
	{
		CameraBoom->TargetArmLength = CameraBoom->TargetArmLength - 10.0f;
		/*if (CameraBoom->TargetArmLength <= 100.0f)
		{
			SetbFirstPerson(true);
			bUseControllerRotationYaw = true;

			FPCamera->SetActive(true, false);
			TPCamera->SetActive(false, false);
		}*/
	}
}
void APlayerCPP::ZoomCameraOut()
{
	if (CameraBoom->TargetArmLength < 500.0f)
	{
		CameraBoom->TargetArmLength = CameraBoom->TargetArmLength + 10.0f;
		/*if (CameraBoom->TargetArmLength > 90.0f)
		{
			SetbFirstPerson(false);
			bUseControllerRotationYaw = false;

			FPCamera->SetActive(false, false);
			TPCamera->SetActive(true, false);
		}*/
	}
}
// JUMPING FUNCTIONS
void APlayerCPP::Jump()
{
	float Speed = FVector::DotProduct(GetVelocity(), GetActorRotation().Vector());

	if (Speed <= 0.0f)
	{
		SetbJump(true);
		GetWorld()->GetTimerManager().SetTimer(JumpTimerHandle, this, &APlayerCPP::JumpIdle, 0.9f, false);
	}
	else
	{
		SetbJump(true);
		ACharacter::Jump();
		GetWorld()->GetTimerManager().SetTimer(JumpTimerHandle, this, &APlayerCPP::StopJumping, 0.1f, false);
	}
}
void APlayerCPP::StopJumping()
{
	ACharacter::StopJumping();
	SetbJump(false);
}
void APlayerCPP::JumpIdle()
{
	ACharacter::Jump();
	StopJumping();
}
// SPRINTING FUNCTIONS
void APlayerCPP::Sprint()
{
	float Speed = FVector::DotProduct(GetVelocity(), GetActorRotation().Vector());
	if (Speed > 0.0f)
	{
		SetbSprinting(true);
		GetWorld()->GetTimerManager().ClearTimer(DeSprintTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(InSprintTimerHandle, this, &APlayerCPP::IncreaseSpeed, 0.1f, true);
	}
}
void APlayerCPP::StopSprinting()
{
	SetbSprinting(false);
	GetWorld()->GetTimerManager().ClearTimer(InSprintTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(DeSprintTimerHandle, this, &APlayerCPP::DecreaseSpeed, 0.2f, true);
}
// TRACE FOR INTERACTION
void APlayerCPP::Interact()
{
	FHitResult Hit;
	FVector Start = TPCamera->GetComponentLocation();
	FVector ForwardVector = TPCamera->GetForwardVector();
	FVector End = ((ForwardVector * 500.f) + Start);
	FCollisionQueryParams CollisionParams;

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.0f, 0, 1.0f);

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionParams))
	{
		if (Hit.GetActor()->GetName().Contains(TEXT("Door")))
		{
			APropertyDoorCPP* DoorCast = Cast<APropertyDoorCPP>(Hit.GetActor());
			
			if (DoorCast != NULL)
			{
				APlayerStateCPP* PS = Cast<APlayerStateCPP>(GetPlayerState());

				if (DoorCast->bPropertyOwned)
				{
					if (PS->PropertyOwned(DoorCast->PropertyID))
					{
						DoorServer(DoorCast->bDoorOpen, DoorCast);
					}
				}
				else
				{
					APlayerControllerCPP* PC = Cast<APlayerControllerCPP>(GetController());
					PC->GetNotification(false, PS->PlayerId, FNotification{ TEXT("THIS PROPERTY "),
																			TEXT("WITH ID "),
																			FString::FromInt(DoorCast->PropertyID),
																			TEXT(" IS FOR SALE FOR $"),
																			FString::SanitizeFloat(DoorCast->PropertyValue) });
				}
			}
		}
	}
}
// DOOR SERVER FUNCTION
void APlayerCPP::DoorServer_Implementation(bool bOpen, APropertyDoorCPP* Door)
{
	DoorMulticast(bOpen, Door);
}
bool APlayerCPP::DoorServer_Validate(bool bOpen, APropertyDoorCPP* Door)
{
	return true;
}
// SET DOOR OPEN/CLOSE (MULTICAST)
void APlayerCPP::DoorMulticast_Implementation(bool bOpen, APropertyDoorCPP* Door)
{
	if (bOpen)
	{
		Door->CloseDoor();
	}
	else
	{
		Door->OpenDoor();
	}
}

/////////////////// COLLISION FUNCTIONS
// ADD PLAYER IN RANGE
void APlayerCPP::AddPlayerInRange(APlayerCPP* PlayerToAdd)
{
	if (PlayerToAdd != NULL)
	{
		PlayersInRange.Add(PlayerToAdd);
	}
}
// REMOVE PLAYER IN RANGE
void APlayerCPP::RemovePlayerInRange(APlayerCPP* PlayerToRemove)
{
	APlayerStateCPP* State = Cast<APlayerStateCPP>(PlayerToRemove->GetPlayerState());
	FString ToRemove = State->GetPlayerName();

	for (int32 i = 0; PlayersInRange.Num() >= i; i++)
	{
		APlayerStateCPP* RState = Cast<APlayerStateCPP>(PlayersInRange[i]->GetPlayerState());
		FString RName = RState->GetPlayerName();

		if (ToRemove == RName)
		{
			PlayersInRange.RemoveAt(i);
			break;
		}
	}
}

/////////////////// MULTIPLAYER FUNCTIONS
// SET JUMP BOOL (SERVER)
void APlayerCPP::SetbJump_Implementation(bool ToSet)
{
	bJump = ToSet;
}
bool APlayerCPP::SetbJump_Validate(bool ToSet)
{
	if (Role == ROLE_Authority)
	{
		return true;
	}
	else
	{
		return false;
	}
}
// SET WALKING BACKWARD BOOL (SERVER)
void APlayerCPP::SetbWalkingBackward_Implementation(bool ToSet)
{
	bWalkingBackward = ToSet;
}
bool APlayerCPP::SetbWalkingBackward_Validate(bool ToSet)
{
	if (Role == ROLE_Authority)
	{
		return true;
	}
	else
	{
		return false;
	}
}
// SET WALKING FORWARD BOOL (SERVER)
void APlayerCPP::SetbWalkingForward_Implementation(bool ToSet)
{
	bWalkingForward = ToSet;
}
bool APlayerCPP::SetbWalkingForward_Validate(bool ToSet)
{
	if (Role == ROLE_Authority)
	{
		return true;
	}
	else
	{
		return false;
	}
}
// SET FIRST-PERSON BOOL (SERVER)
void APlayerCPP::SetbFirstPerson_Implementation(bool ToSet)
{
	bFirstPerson = ToSet;
}
bool APlayerCPP::SetbFirstPerson_Validate(bool ToSet)
{
	if (Role == ROLE_Authority)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// SPRINTING FUNCTIONS
// SET DEFAULT SPEED (CLIENT)
void APlayerCPP::SetDefaultSpeedC_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
}
// SET SPRINTING BOOL (SERVER)
void APlayerCPP::SetbSprinting_Implementation(bool ToSet)
{
	bSprinting = ToSet;
}
bool APlayerCPP::SetbSprinting_Validate(bool ToSet)
{
	if (Role == ROLE_Authority)
	{
		return true;
	}
	else
	{
		return false;
	}
}
// INCREASE CHARACTER SPEED (SERVER/CLIENT)
void APlayerCPP::IncreaseSpeed_Implementation()
{
	float CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
	float Speed = FVector::DotProduct(GetVelocity(), GetActorRotation().Vector());

	if (Speed > 50.0f)
	{
		if ((CurrentSpeed < 600.0f) && (bSprinting == true))
		{
			GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed + SpeedIncrease;
			IncreaseSpeedC(CurrentSpeed);
		}
	}
	else
	{
		StopSprinting();
	}
}
bool APlayerCPP::IncreaseSpeed_Validate()
{
	if (Role == ROLE_Authority)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void APlayerCPP::IncreaseSpeedC_Implementation(float CurrentSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed + SpeedIncrease;
}
// DECREASE CHARACTER SPEED (SERVER/CLIENT)
void APlayerCPP::DecreaseSpeed_Implementation()
{
	float CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;

	if ((CurrentSpeed > 200.0f) && (bSprinting == false))
	{
		GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed - SpeedDecrease;
		DecreaseSpeedC(CurrentSpeed);

		float Speed = FVector::DotProduct(GetVelocity(), GetActorRotation().Vector());
		if (Speed < 50.0f)
		{
			GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
			SetDefaultSpeedC();
		}
	}
}
bool APlayerCPP::DecreaseSpeed_Validate()
{
	if (Role == ROLE_Authority)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void APlayerCPP::DecreaseSpeedC_Implementation(float CurrentSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed - SpeedDecrease;
}