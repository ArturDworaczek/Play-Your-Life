// Game Created By Artur Dworaczek, Located In Somerset UK.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Animation/AnimSequence.h"
#include "GameFramework/Pawn.h"
#include "UnrealNetwork.h"
#include "Animation/AnimBlueprint.h"
#include "Classes/Sound/SoundClass.h"
#include "GameFramework/PlayerInput.h"
#include "PlayerControllerCPP.generated.h"

/**
 * 
 */
///////////// ENUMS
UENUM(BlueprintType)
enum EAccountType
{
	AT_Normal			UMETA(DisplayName = "Normal Account"),
	AT_Premium			UMETA(DisplayName = "Premium Account"),
	AT_PremiumPlus		UMETA(DisplayName = "Premium Plus Account"),
	AT_Helper			UMETA(DisplayName = "Helper Account"),
	AT_Moderator		UMETA(DisplayName = "Mod Account"),
	AT_Admin			UMETA(DisplayName = "Admin Account")
};

///////////// STRUCTS
USTRUCT(BlueprintType)
struct FChatMessage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Chat Information")
		int32 PID;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Chat Information")
		FString Sender;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Chat Information")
		FString MessageText;
};

USTRUCT(BlueprintType)
struct FCommands
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Player Information")
		TEnumAsByte<EAccountType> AccountTypeNeeded;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Player Information")
		FString Command;
};

USTRUCT(BlueprintType)
struct FNotification
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Notification Information")
		FString Player;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Notification Information")
		FString Notification;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Notification Information")
		FString Amount;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Notification Information")
		FString From;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Notification Information")
		FString Sender;
};

UCLASS()
class PLAYYOURLIFE_API APlayerControllerCPP : public APlayerController
{
	GENERATED_BODY()

///////////// PRIVATE FUNCTIONS
private:
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;
///////////// PUBLIC FUNCTIONS
public:
	APlayerControllerCPP();

	/** FIND PLAYER BY ID */
	UFUNCTION(BlueprintCallable)
		APlayerCPP* FindPlayer(int32 PlayerIndex);
	/** RETURN ACCOUNT ENUM AS STRING */
	UFUNCTION(BlueprintCallable)
		FString AccountEnumToString(EAccountType AccountType);

	/** SET NEW ACTION KEY */
	UFUNCTION(BlueprintCallable, Category = "Keybinding")
		bool SetNewActionKey(FName ActionName, FKey NewKey, FKey OldKey);
	/** GET ALL ACTION KEY INFO */
	UFUNCTION(BlueprintCallable, Category = "Keybinding")
		bool GetActionKeys();
	/** UPDATE ACTION KEY */
	UFUNCTION(BlueprintImplementableEvent, Category = "Keybinding")
		void UpdateActionKey(FInputActionKeyMapping ActionKeyInformation);
	/** SET NEW SOUND CLASS VOLUME */
	UFUNCTION(BlueprintCallable, Category = "Sound")
		bool SetNewSoundClassVolume(USoundClass* TargetClass, float NewVolume);

	/** GET MESSAGE (SERVER) */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Multiplayer Chat Functions")
		void GetMessage(FChatMessage Message, EAccountType Colour);
		void GetMessage_Implementation(FChatMessage Message, EAccountType Colour);
		bool GetMessage_Validate(FChatMessage Message, EAccountType Colour);
	/** UPDATE CHAT */
	UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer Chat Functions")
		void UpdateChat(FChatMessage Message, EAccountType Colour);

	/** SET BGESTUREPLAYING (SERVER) */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Multiplayer Gestures Functions")
		void SetbGesturePlaying(bool NewBool);
		void SetbGesturePlaying_Implementation(bool NewBool);
		bool SetbGesturePlaying_Validate(bool NewBool);
	/** SET GESTURES (SERVER) */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Multiplayer Gestures Functions")
		void SetGesture(int32 Index, UAnimSequence* Animation);
		void SetGesture_Implementation(int32 Index, UAnimSequence* Animation);
		bool SetGesture_Validate(int32 Index, UAnimSequence* Animation);

	/** PLAY GESTURES (SERVER) */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Multiplayer Gestures Functions")
		void PlayGestureOnServer(APawn* ReqPlayer, UAnimSequence* Animation);
		void PlayGestureOnServer_Implementation(APawn* ReqPlayer, UAnimSequence* Animation);
		bool PlayGestureOnServer_Validate(APawn* ReqPlayer, UAnimSequence* Animation);
	/** PLAY GESTURE (CLIENT) */
	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "Multiplayer Gestures Functions")
		void PlayGestureOnClient(APawn* ReqPlayer, UAnimSequence* Animation);
		void PlayGestureOnClient_Implementation(APawn* ReqPlayer, UAnimSequence* Animation);

	/** RETURN TO ANIMBP (SERVER) */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Multiplayer Gestures Functions")
		void ReturnToAnimbpServer(APawn* ReqPlayer, UClass* AnimationBP);
		void ReturnToAnimbpServer_Implementation(APawn* ReqPlayer, UClass* AnimationBP);
		bool ReturnToAnimbpServer_Validate(APawn* ReqPlayer, UClass* AnimationBP);
	/** RETURN TO ANIMBP BP */
	UFUNCTION(BlueprintImplementableEvent)
		void CallReturnToAnimBP();
	/** RETURN TO ANIMBP (CLIENT) */
	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "Multiplayer Gestures Functions")
		void ReturnToAnimbpClient(APawn* ReqPlayer, UClass* AnimationBP);
		void ReturnToAnimbpClient_Implementation(APawn* ReqPlayer, UClass* AnimationBP);

	/** REQUEST ACCOUNT TYPE CHANGE (SERVER) */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Multiplayer Chat Functions")
		void RequestAccountTypeChange(int32 PlayerAccountIndex, EAccountType NewAccountType);
		void RequestAccountTypeChange_Implementation(int32 PlayerAccountIndex, EAccountType NewAccountType);
		bool RequestAccountTypeChange_Validate(int32 PlayerAccountIndex, EAccountType NewAccountType);

	/** REQUEST ACCOUNT TYPE CHANGE (SERVER) */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Multiplayer Chat Functions")
		void Invisible();
		void Invisible_Implementation();
		bool Invisible_Validate();

	/** GET NOTIFICATION (SERVER) */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Multiplayer Chat Functions")
		void GetNotification(bool bSendToEveryone, int32 PlayerIndexToSend, FNotification Notification);
		void GetNotification_Implementation(bool bSendToEveryone, int32 PlayerIndexToSend, FNotification Notification);
		bool GetNotification_Validate(bool bSendToEveryone, int32 PlayerIndexToSend, FNotification Notification);
	/** DELETE OLD NOTIF AND CHECK FOR MORE (SERVER) */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Multiplayer Chat Functions")
		void GetMoreNotif();
		void GetMoreNotif_Implementation();
		bool GetMoreNotif_Validate();
	/** SET BNOTIFPLAYING (SERVER) */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Multiplayer Chat Functions")
		void SetbNotifPlaying(bool NewBool);
		void SetbNotifPlaying_Implementation(bool NewBool);
		bool SetbNotifPlaying_Validate(bool NewBool);
	/** UPDATE NOTIFICATION */
	UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer Chat Functions")
		void UpdateNotification(FNotification Notification);
public:
	FTimerHandle GestureTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Multiplayer Chat Functions")
		TArray<FCommands> Commands;

	// REPLICATED VARIABLES
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Multiplayer Gestures Functions")
		TArray<UAnimSequence*> Gestures;
		UAnimSequence* GestureNULL;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Multiplayer Chat Functions")
		TArray<FChatMessage> ChatMessagesArray;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Multiplayer Chat Functions")
		TArray<FNotification> NotificationsArray;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Multiplayer Chat Functions")
		TEnumAsByte<EAccountType> AccountType;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Multiplayer Chat Functions")
		bool bInvisible;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Multiplayer Gestures Functions")
		bool bGesturePlaying;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Multiplayer Chat Functions")
		bool bNotifPlaying;
};