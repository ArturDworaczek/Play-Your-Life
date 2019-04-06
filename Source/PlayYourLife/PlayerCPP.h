// Game Created By Artur Dworaczek, Located In Somerset UK.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneCaptureComponent.h"
#include "PlayerControllerCPP.h"
#include "PropertyDoorCPP.h"
#include "PlayerCPP.generated.h"

UCLASS(config = Game)
class APlayerCPP : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cameras, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Third-Person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cameras, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TPCamera;

	/** First-Person Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cameras, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FPCamera;

	/** Map Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cameras, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* MapCamera;

	/** Scene Capture */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cameras, meta = (AllowPrivateAccess = "true"))
		class USceneCaptureComponent2D* SceneCapture;

	/** First-Person Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Chat, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* ChatProximity;

	/** Line Trace Arrow */
	UPROPERTY(VisibleAnywhere, Category = LineTrace)
		class UArrowComponent* LineTArrow;

	// VISUAL
	/** Hat */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cameras, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Hat;
	/** Sunglasses */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cameras, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Sunglasses;
	/** Chain */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cameras, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Chain;
	/** Hoodie */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cameras, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Hoodie;
	/** TShirt */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cameras, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* TShirt;
	/** Trousers */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cameras, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Trousers;
	/** Socks */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cameras, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Socks;
	/** Shoes */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cameras, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Shoes;
public:
	APlayerCPP();

	FTimerHandle JumpTimerHandle;
	FTimerHandle InSprintTimerHandle;
	FTimerHandle DeSprintTimerHandle;

	////////////// NOT REPLICATED VARIABLES
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float SpeedIncrease;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float SpeedDecrease;

	///////////// REPLICATED VARIABLES
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Movement")
		float DefaultSpeed;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Movement")
		bool bJump;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Movement")
		bool bWalkingForward;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Movement")
		bool bWalkingBackward;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Movement")
		bool bFirstPerson;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Movement")
		bool bSprinting;

	UPROPERTY(BlueprintReadOnly, Category = "Multiplayer Chat")
		TArray<APlayerCPP*> PlayersInRange;
protected:
	// Called Every Frame
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay();

	// AXIS FUNCTIONS
	/** Called for forwards/backward input */
	void MoveForward(float Value);
	/** Called for side to side input */
	void MoveRight(float Value);

	// ACTION FUNCTIONS
	/** Called To Jump */
	void Jump();
	/** Called To Jump When Idle */
	void JumpIdle();
	/** Called To Stop Jumping */
	void StopJumping();

	/** Called To Zoom Camera In*/
	void ZoomCameraIn();
	/** Called To Zoom Camera Out*/
	void ZoomCameraOut();

	/** Called To Sprint */
	void Sprint();
	/** Called To Stop Sprinting */
	void StopSprinting();

	/** Called To Interact */
	void Interact();

	/** SET DOOR OPEN/CLOSE (SERVER) */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Multiplayer Property System")
		void DoorServer(bool bOpen, APropertyDoorCPP* Door);
		void DoorServer_Implementation(bool bOpen, APropertyDoorCPP* Door);
		bool DoorServer_Validate(bool bOpen, APropertyDoorCPP* Door);
	/** SET DOOR OPEN/CLOSE (MULTICAST) */
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Multiplayer Property System")
		void DoorMulticast(bool bOpen, APropertyDoorCPP* Door);
		void DoorMulticast_Implementation(bool bOpen, APropertyDoorCPP* Door);

	// MULTIPLAYER FUNCTIONS
	/** SET JUMP BOOL (SERVER) */
	UFUNCTION(Server, Reliable, WithValidation, Category = "Multiplayer Movement Functions")
		void SetbJump(bool ToSet);
		void SetbJump_Implementation(bool ToSet);
		bool SetbJump_Validate(bool ToSet);
	/** SET WALKING BACKWARD BOOL (SERVER) */
	UFUNCTION(Server, Reliable, WithValidation, Category = "Multiplayer Movement Functions")
		void SetbWalkingBackward(bool ToSet);
		void SetbWalkingBackward_Implementation(bool ToSet);
		bool SetbWalkingBackward_Validate(bool ToSet);
	/** SET WALKING FORWARD BOOL (SERVER) */
	UFUNCTION(Server, Reliable, WithValidation, Category = "Multiplayer Movement Functions")
		void SetbWalkingForward(bool ToSet);
		void SetbWalkingForward_Implementation(bool ToSet);
		bool SetbWalkingForward_Validate(bool ToSet);
	/** SET FIRST-PERSON BOOL (SERVER) */
	UFUNCTION(Server, Reliable, WithValidation, Category = "Multiplayer Movement Functions")
		void SetbFirstPerson(bool ToSet);
		void SetbFirstPerson_Implementation(bool ToSet);
		bool SetbFirstPerson_Validate(bool ToSet);
	/** SET SPRINTING BOOL (SERVER) */
	UFUNCTION(Server, Reliable, WithValidation, Category = "Multiplayer Movement Functions")
		void SetbSprinting(bool ToSet);
		void SetbSprinting_Implementation(bool ToSet);
		bool SetbSprinting_Validate(bool ToSet);
	/** INCREASE SPEED (SERVER) */
	UFUNCTION(Server, Reliable, WithValidation, Category = "Multiplayer Movement Functions")
		void IncreaseSpeed();
		void IncreaseSpeed_Implementation();
		bool IncreaseSpeed_Validate();
	/** DECREASE SPEED (SERVER) */
	UFUNCTION(Server, Reliable, WithValidation, Category = "Multiplayer Movement Functions")
		void DecreaseSpeed();
		void DecreaseSpeed_Implementation();
		bool DecreaseSpeed_Validate();
	
	/** SET DEFAULT SPEED (CLIENT) */
	UFUNCTION(Client, Reliable, Category = "Multiplayer Movement Functions")
		void SetDefaultSpeedC();
		void SetDefaultSpeedC_Implementation();
	/** INCREASE SPEED (CLIENT) */
	UFUNCTION(Client, Reliable, Category = "Multiplayer Movement Functions")
		void IncreaseSpeedC(float CurrentSpeed);
		void IncreaseSpeedC_Implementation(float CurrentSpeed);
	/** DECREASE SPEED (CLIENT) */
	UFUNCTION(Client, Reliable, Category = "Multiplayer Movement Functions")
		void DecreaseSpeedC(float CurrentSpeed);
		void DecreaseSpeedC_Implementation(float CurrentSpeed);
	/** ADD PLAYER IN RANGE (SERVER) */
	UFUNCTION(BlueprintCallable, Category = "Multiplayer Chat Functions")
		void AddPlayerInRange(APlayerCPP* PlayerToAdd);
	/** REMOVE PLAYER IN RANGE (SERVER) */
	UFUNCTION(BlueprintCallable, Category = "Multiplayer Chat Functions")
		void RemovePlayerInRange(APlayerCPP* PlayerToRemove);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetTPCamera() const { return TPCamera; }
	/** Returns FPCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFPCamera() const { return FPCamera; }
	/** Returns MapCamera subobject **/
	FORCEINLINE class UCameraComponent* GetMapCamera() const { return MapCamera; }
	/** Returns SceneCapture subobject **/
	FORCEINLINE class USceneCaptureComponent2D* GetSceneCapture() const { return SceneCapture; }
	/** Returns ChatProximity subobject **/
	FORCEINLINE class USphereComponent* GetChatProximity() const { return ChatProximity; }

	// VISUAL
	/** Returns Hat subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetHat() const { return Hat; }
	/** Returns Sunglasses subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetSunglasses() const { return Sunglasses; }
	/** Returns Chain subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetChain() const { return Chain; }
	/** Returns Hoodie subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetHoodie() const { return Hoodie; }
	/** Returns TShirt subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetTShirt() const { return TShirt; }
	/** Returns Trousers subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetTrousers() const { return Trousers; }
	/** Returns Socks subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetSocks() const { return Socks; }
	/** Returns Shoes subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetShoes() const { return Shoes; }
};

