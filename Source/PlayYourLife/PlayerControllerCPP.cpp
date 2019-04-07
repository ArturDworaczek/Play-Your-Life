// Game Created By Artur Dworaczek, Located In Somerset UK.

#include "PlayerControllerCPP.h"
#include "Engine.h"
#include "UnrealNetwork.h"
#include "PlayerCPP.h"
#include "GameFramework/Pawn.h"
#include "Animation/AnimBlueprint.h"
#include "EngineUtils.h"

// SET DEFAULTS
APlayerControllerCPP::APlayerControllerCPP()
{
	// DEFAULT COMMANDS
	Commands = {  
		         FCommands{ AT_Normal, TEXT("SetAccount") },
		         FCommands{ AT_Admin,  TEXT("Inv") },
		         FCommands{ AT_Normal, TEXT("AddCash") },
		         FCommands{ AT_Normal, TEXT("GiveCash") }
	           };

	// DEFAULT AMOUNT OF GESTURES
	GestureNULL = NULL;
	Gestures = { GestureNULL, GestureNULL, GestureNULL, GestureNULL };
}
// REPLICATE VARIABLES
void APlayerControllerCPP::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(APlayerControllerCPP, ChatMessagesArray);
	DOREPLIFETIME(APlayerControllerCPP, AccountType);
	DOREPLIFETIME(APlayerControllerCPP, Gestures);
	DOREPLIFETIME(APlayerControllerCPP, bGesturePlaying);
	DOREPLIFETIME(APlayerControllerCPP, bInvisible);
	DOREPLIFETIME(APlayerControllerCPP, NotificationsArray);
	DOREPLIFETIME(APlayerControllerCPP, bNotifPlaying);
}
// FIND PLAYER BY ID
APlayerCPP* APlayerControllerCPP::FindPlayer(int32 PlayerIndex)
{
	for (TObjectIterator<APlayerCPP> Obj; Obj; ++Obj)
	{
		if (Obj->GetPlayerState()->PlayerId == PlayerIndex)
		{
			return *Obj;
		}
	}
	return NULL;
}
// SET NEW ACTION KEY
bool APlayerControllerCPP::SetNewActionKey(FName ActionName, FKey NewKey, FKey OldKey)
{
	// Get Input Settings
	UInputSettings* Settings = UInputSettings::StaticClass()->GetDefaultObject<UInputSettings>();
	if (!Settings) { return false; }

	// Remove Previous Binding
	FInputActionKeyMapping OldInput(ActionName, OldKey, false, false, false, false);
	Settings->RemoveActionMapping(OldInput);

	//Use New Binding
	FInputActionKeyMapping NewInput(ActionName, NewKey, false, false, false, false);
	Settings->AddActionMapping(NewInput);

	return true;
}
// GET ALL ACTION KEY INFO
bool APlayerControllerCPP::GetActionKeys()
{
	// Get Input Settings
	UInputSettings* Settings = UInputSettings::StaticClass()->GetDefaultObject<UInputSettings>();
	if (!Settings) { return false; }

	// Return Action Mappings
	for (int8 ActionKey = 0; ActionKey < Settings->ActionMappings.Num(); ActionKey++)
	{
		UpdateActionKey(Settings->ActionMappings[ActionKey]);
	}

	return true;
}
// SET NEW SOUND CLASS VOLUME
bool APlayerControllerCPP::SetNewSoundClassVolume(USoundClass* TargetClass, float NewVolume)
{
	if (!TargetClass) { return false; }

	TargetClass->Properties.Volume = NewVolume;
	return true;
}
// RETURN ACCOUNT ENUM AS STRING
FString APlayerControllerCPP::AccountEnumToString(EAccountType at)
{
	if (at == EAccountType::AT_Normal)
	{
		return TEXT("NORMAL");
	}
	else if (at == EAccountType::AT_Premium)
	{
		return TEXT("PREMIUM");
	}
	else if (at == EAccountType::AT_PremiumPlus)
	{
		return TEXT("PREMIUM PLUS");
	}
	else if (at == EAccountType::AT_Helper)
	{
		return TEXT("HELPER");
	}
	else if (at == EAccountType::AT_Moderator)
	{
		return TEXT("MODERATOR");
	}
	else if (at == EAccountType::AT_Admin)
	{
		return TEXT("ADMIN");
	}
	else
	{
		return TEXT("NOT FOUND");
	}
	return TEXT("NOT FOUND");
}
// ADD MESSAGE (SERVER)
void APlayerControllerCPP::GetMessage_Implementation(FChatMessage Message, EAccountType Colour)
{
	ChatMessagesArray.Add(Message);

	APlayerCPP* PlayerCast = Cast<APlayerCPP>(GetPawn());

	for (int32 i = 0; PlayerCast->PlayersInRange.Num() > i; i++)
	{
		APlayerControllerCPP* PC = Cast<APlayerControllerCPP>(PlayerCast->PlayersInRange[i]->GetController());

		PC->UpdateChat(Message, Colour);
	}
}
bool APlayerControllerCPP::GetMessage_Validate(FChatMessage Message, EAccountType Colour)
{
	return true;
}
// SET BGESTUREPLAYING (SERVER)
void APlayerControllerCPP::SetbGesturePlaying_Implementation(bool NewBool)
{
	bGesturePlaying = NewBool;
}
bool APlayerControllerCPP::SetbGesturePlaying_Validate(bool NewBool)
{
	return true;
}
// SET GESTURES (SERVER)
void APlayerControllerCPP::SetGesture_Implementation(int32 Index, UAnimSequence* Animation)
{
	Gestures.Insert(Animation, Index);
}
bool APlayerControllerCPP::SetGesture_Validate(int32 Index, UAnimSequence* Animation)
{
	return true;
}
// PLAY GESTURE (SERVER)
void APlayerControllerCPP::PlayGestureOnServer_Implementation(APawn* ReqPlayer, UAnimSequence* Animation)
{
	if (Animation != NULL)
	{
		SetbGesturePlaying(true);

		for (TObjectIterator<APlayerControllerCPP> PC; PC; ++PC)
		{
			PC->PlayGestureOnClient(ReqPlayer, Animation);
		}
		GetWorld()->GetTimerManager().SetTimer(GestureTimerHandle, this, &APlayerControllerCPP::CallReturnToAnimBP, Animation->SequenceLength, false);
	}
}
bool APlayerControllerCPP::PlayGestureOnServer_Validate(APawn* ReqPlayer, UAnimSequence* Animation)
{
	return true;
}
// PLAY GESTURE (CLIENT)
void APlayerControllerCPP::PlayGestureOnClient_Implementation(APawn* ReqPlayer, UAnimSequence* Animation)
{
	APlayerCPP* PlayerCast = Cast<APlayerCPP>(ReqPlayer);

	PlayerCast->GetMesh()->PlayAnimation(Animation, false);
}
// RETURN TO ANIMBP (SERVER)
void APlayerControllerCPP::ReturnToAnimbpServer_Implementation(APawn* ReqPlayer, UClass* AnimationBP)
{
	if (AnimationBP != NULL)
	{
		SetbGesturePlaying(true);

		for (TObjectIterator<APlayerControllerCPP> PC; PC; ++PC)
		{
			PC->ReturnToAnimbpClient(ReqPlayer, AnimationBP);
		}
	}
}
bool APlayerControllerCPP::ReturnToAnimbpServer_Validate(APawn* ReqPlayer, UClass* AnimationBP)
{
	return true;
}
// RETURN TO ANIMBP (CLIENT)
void APlayerControllerCPP::ReturnToAnimbpClient_Implementation(APawn* ReqPlayer, UClass* AnimationBP)
{
	APlayerCPP* PlayerCast = Cast<APlayerCPP>(ReqPlayer);
	PlayerCast->GetMesh()->SetAnimInstanceClass(AnimationBP);
}
// REQUEST ACCOUNT TYPE CHANGE (SERVER)
void APlayerControllerCPP::RequestAccountTypeChange_Implementation(int32 PlayerAccountIndex, EAccountType NewAccountType)
{
	for (TObjectIterator<APlayerControllerCPP> AccountPC; AccountPC; ++AccountPC)
	{
		if (AccountPC->PlayerState->PlayerId == PlayerAccountIndex)
		{
			AccountPC->AccountType = NewAccountType;
			AccountPC->GetNotification(false, PlayerAccountIndex, FNotification{ TEXT("YOUR "),
																				 TEXT("ACCOUNT TYPE HAS CHANGED TO "),
																				 TEXT(""),
																				 TEXT(""),
																				 AccountEnumToString(NewAccountType) });
			break;
		}
	}
}
bool APlayerControllerCPP::RequestAccountTypeChange_Validate(int32 PlayerAccountIndex, EAccountType NewAccountType)
{
	return true;
}
// GO INVISIBLE (SERVER)
void APlayerControllerCPP::Invisible_Implementation()
{
	APlayerCPP* PlayerCast = Cast<APlayerCPP>(GetPawn());
	bInvisible = !bInvisible;
	PlayerCast->GetMesh()->SetVisibility(!bInvisible);
}
bool APlayerControllerCPP::Invisible_Validate()
{
	return true;
}
// GET NOTIFICATION (SERVER)
void APlayerControllerCPP::GetNotification_Implementation(bool bSendToEveryone, int32 PlayerIndexToSend, FNotification Notification)
{
	if (bSendToEveryone)
	{
		for (TObjectIterator<APlayerControllerCPP> PC; PC; ++PC)
		{
			if (PC->bNotifPlaying == false)
			{
				PC->NotificationsArray.Add(Notification);
				PC->UpdateNotification(Notification);
				PC->bNotifPlaying = true;
			}
			else
			{
				PC->NotificationsArray.Add(Notification);
			}
		}
	}
	else
	{
		APlayerControllerCPP* PlayerCast = Cast<APlayerControllerCPP>(FindPlayer(PlayerIndexToSend)->GetController());

		if (PlayerCast->bNotifPlaying == false)
		{
			PlayerCast->NotificationsArray.Add(Notification);
			PlayerCast->UpdateNotification(Notification);
			PlayerCast->bNotifPlaying = true;
		}
		else
		{
			PlayerCast->NotificationsArray.Add(Notification);
		}
	}

	FString StringNotification;
	StringNotification += Notification.Player += Notification.Notification += Notification.Amount += Notification.From += Notification.Sender;
	UE_LOG(LogTemp, Display, TEXT("%s"), *StringNotification);
}
bool APlayerControllerCPP::GetNotification_Validate(bool bSendToEveryone, int32 PlayerIndexToSend, FNotification Notification)
{
	return true;
}
// DELETE OLD NOTIF AND CHECK FOR MORE (SERVER)
void APlayerControllerCPP::GetMoreNotif_Implementation()
{
	NotificationsArray.RemoveAt(0);
	bNotifPlaying = false;

	if (NotificationsArray.Num() > 0)
	{
		UpdateNotification(NotificationsArray[0]);
		UE_LOG(LogTemp, Warning, TEXT("More Notifications To Play..."));
		bNotifPlaying = true;
	}
	else
	{
		bNotifPlaying = false;
		UE_LOG(LogTemp, Warning, TEXT("No More Notifications To Play..."));
	}
}
bool APlayerControllerCPP::GetMoreNotif_Validate()
{
	return true;
}
// SET BNOTIFPLAYING (SERVER)
void APlayerControllerCPP::SetbNotifPlaying_Implementation(bool NewBool)
{
	bNotifPlaying = NewBool;
}
bool APlayerControllerCPP::SetbNotifPlaying_Validate(bool NewBool)
{
	return true;
}