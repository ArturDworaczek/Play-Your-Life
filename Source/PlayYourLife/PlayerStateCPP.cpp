// Game Created By Artur Dworaczek, Located In Somerset UK.

#include "PlayerStateCPP.h"
#include "Engine.h"
#include "PlayerCPP.h"
#include "PlayerControllerCPP.h"
#include "UnrealNetwork.h"

// DEFAULTS
APlayerStateCPP::APlayerStateCPP()
{

}
// REPLICATION
void APlayerStateCPP::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(APlayerStateCPP, Cash);
	DOREPLIFETIME(APlayerStateCPP, Reputation);
	DOREPLIFETIME(APlayerStateCPP, OwnedVehicles);
	DOREPLIFETIME(APlayerStateCPP, Inventory);
}
// ADD REPUTATION (SERVER)
void APlayerStateCPP::AddReputation_Implementation(int32 ReputationToAdd)
{
	Reputation += ReputationToAdd;
}
bool APlayerStateCPP::AddReputation_Validate(int32 ReputationToAdd)
{
	return true;
}
// ADD CASH (SERVER)
void APlayerStateCPP::AddCash_Implementation(int32 PlayerReceiving, float Amount, FNotification Notification)
{
	APlayerCPP* PPlayerReceiving = NULL;
	bool PlayerFound = false;

	for (TObjectIterator<APlayerCPP> Obj; Obj; ++Obj)
	{
		APlayerCPP* PlayerCast = Cast<APlayerCPP>(*Obj);

		if (PlayerCast->GetPlayerState()->PlayerId == PlayerReceiving)
		{
			PlayerFound = true;
			PPlayerReceiving = PlayerCast;
			break;
		}
	}

	if (PlayerFound)
	{
		APlayerStateCPP* PSReceiving = Cast<APlayerStateCPP>(PPlayerReceiving->GetPlayerState());
		APlayerControllerCPP* PCReceiving = Cast<APlayerControllerCPP>(PPlayerReceiving->GetController());

		PSReceiving->Cash += Amount;
		PCReceiving->GetNotification(false, PlayerReceiving, Notification);
	}
}
bool APlayerStateCPP::AddCash_Validate(int32 PlayerReceiving, float Amount, FNotification Notification)
{
	return true;
}
// TRANSFER MONEY (SERVER)
void APlayerStateCPP::TransferCash_Implementation(int32 PlayerIndexReceiving, APawn* PlayerWantingToTransfer, float Amount)
{
	if (Cash >= Amount && PlayerId != PlayerIndexReceiving)
	{
		APlayerCPP* PlayerTransfering = Cast<APlayerCPP>(PlayerWantingToTransfer);
		bool bPlayerFoundInRange = false;
		APlayerStateCPP* PlayerReceiving = NULL;

		for (int32 i = 0; PlayerTransfering->PlayersInRange.Num() > i; i++)
		{
			APlayerStateCPP* PState = Cast<APlayerStateCPP>(PlayerTransfering->PlayersInRange[i]->GetPlayerState());

			if (PState->PlayerId == PlayerIndexReceiving)
			{
				PlayerReceiving = PState;
				bPlayerFoundInRange = true;
				break;
			}
		}

		if (bPlayerFoundInRange)
		{
			AddCash(PlayerTransfering->GetPlayerState()->PlayerId, Amount * -1, FNotification{ TEXT("YOU "),
																						  TEXT("HAVE TRANSFERRED $"),
																						  FString::SanitizeFloat(Amount),
																					      TEXT(" TO "),
																						  PlayerReceiving->GetPlayerName() });

			PlayerReceiving->AddCash(PlayerIndexReceiving, Amount, FNotification{ TEXT("YOU "),
																				  TEXT("HAVE RECIEVED $"),
																				  FString::SanitizeFloat(Amount),
																				  TEXT(" FROM "),
																				  PlayerTransfering->GetPlayerState()->GetPlayerName() });
		}
	}
}
bool APlayerStateCPP::TransferCash_Validate(int32 PlayerIndexReceiving, APawn* PlayerWantingToTransfer, float Amount)
{
	return true;
}
// ADD VEHICLE (SERVER)
void APlayerStateCPP::AddVehicle_Implementation(FVehicleStructure VehicleToAdd)
{
	OwnedVehicles.Add(VehicleToAdd);
}
bool APlayerStateCPP::AddVehicle_Validate(FVehicleStructure VehicleToAdd)
{
	return true;
}
// REMOVE VEHICLE (SERVER)
void APlayerStateCPP::RemoveVehicle_Implementation(FVehicleStructure VehicleToRemove)
{
	//OwnedVehicles.RemoveAt(OwnedVehicles.Find(VehicleToRemove));
}
bool APlayerStateCPP::RemoveVehicle_Validate(FVehicleStructure VehicleToRemove)
{
	return true;
}
// ADD / REMOVE ITEM (SERVER)
void APlayerStateCPP::ItemToInventory_Implementation(bool bRemove, FItemStructure Item)
{
	if (bRemove)
	{
		for (int32 Index = 0; Index <= Inventory.Num(); Index++)
		{
			if (Inventory[Index].ItemID == Item.ItemID)
			{
				Inventory.RemoveAt(Index);
				RefreshInventory();
				break;
			}
		}
	}
	else
	{
		Inventory.Add(Item);
		RefreshInventory();
	}
}
bool APlayerStateCPP::ItemToInventory_Validate(bool bRemove, FItemStructure Item)
{
	return true;
}
// ADD / REMOVE PROPERTY (SERVER)
void APlayerStateCPP::Property_Implementation(bool bRemove, int32 PropertyID)
{
	if (bRemove)
	{
		for (int32 Index = 0; Index <= OwnedProperties.Num(); Index++)
		{
			if (OwnedProperties[Index] == PropertyID)
			{
				OwnedProperties.RemoveAt(Index);
				break;
			}
		}
	}
	else
	{
		OwnedProperties.Add(PropertyID);
	}
}
bool APlayerStateCPP::Property_Validate(bool bRemove, int32 PropertyID)
{
	return true;
}
// CHECK IF YOU OWN THE PROPERTY
bool APlayerStateCPP::PropertyOwned(int32 PropertyID)
{
	for (int32 Index = 0; Index <= OwnedProperties.Num(); Index++)
	{
		if (OwnedProperties[Index] == PropertyID)
		{
			return true;
		}
	}
	return false;
}