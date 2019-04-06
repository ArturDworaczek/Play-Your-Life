// Game Created By Artur Dworaczek, Located In Somerset UK.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerControllerCPP.h"
#include "Runtime/Engine/Classes/Engine/Texture.h"
#include "Runtime/Engine/Classes/Engine/SkeletalMesh.h"
#include "PlayerStateCPP.generated.h"

UENUM(BlueprintType)
enum EItemType
{
	IT_Gesture			UMETA(DisplayName = "Gesture"),
	IT_Hat				UMETA(DisplayName = "Hat"),
	IT_Sunglasses		UMETA(DisplayName = "Sunglasses"),
	IT_Chain			UMETA(DisplayName = "Chain"),
	IT_Hoodie			UMETA(DisplayName = "Hoodie"),
	IT_TShirt			UMETA(DisplayName = "T-Shirt"),
	IT_Trousers			UMETA(DisplayName = "Trousers"),
	IT_Socks			UMETA(DisplayName = "Socks"),
	IT_Shoes			UMETA(DisplayName = "Shoes")
};

USTRUCT(BlueprintType)
struct FVehicleStructure
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Player Information")
		int32 VehicleID;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Player Information")
		FString VehiclePlate;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Player Information")
		AActor* VehicleBP;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Player Information")
		float Mileage;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Player Information")
		int32 BHP;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Player Information")
		float TAX;
};

USTRUCT(BlueprintType)
struct FItemStructure
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Item Structure")
		int32 ItemID;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Item Structure")
		FString ItemName;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Item Structure")
		FString ItemDescription;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Item Structure")
		UTexture* ItemTexture;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Item Structure")
		UAnimSequence* Gesture;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Item Structure")
		USkeletalMesh* Mesh;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Item Structure")
		TEnumAsByte<EItemType> ItemType;
};

UCLASS()
class PLAYYOURLIFE_API APlayerStateCPP : public APlayerState
{
	GENERATED_BODY()

///////////// PRIVATE FUNCTIONS
private:
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;
	APlayerStateCPP();
///////////// PUBLIC FUNCTIONS
public:
	/** ADD REPUTATION (SERVER) */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Multiplayer Reputation Functions")
		void AddReputation(int32 ReputationToAdd);
		void AddReputation_Implementation(int32 ReputationToAdd);
		bool AddReputation_Validate(int32 ReputationToAdd);

	/** ADD MONEY (SERVER) */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Multiplayer Banking Functions")
		void AddCash(int32 PlayerReceiving, float Amount, FNotification Notification);
		void AddCash_Implementation(int32 PlayerReceiving, float Amount, FNotification Notification);
		bool AddCash_Validate(int32 PlayerReceiving, float Amount, FNotification Notification);

	/** TRANSFER CASH (SERVER) */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Multiplayer Banking Functions")
		void TransferCash(int32 PlayerIndexReceiving, APawn* PlayerWantingToTransfer, float Amount);
		void TransferCash_Implementation(int32 PlayerIndexReceiving, APawn* PlayerWantingToTransfer, float Amount);
		bool TransferCash_Validate(int32 PlayerIndexReceiving, APawn* PlayerWantingToTransfer, float Amount);
			
	/** ADD VEHICLE (SERVER) */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Multiplayer Vehicle Functions")
		void AddVehicle(FVehicleStructure VehicleToAdd);
		void AddVehicle_Implementation(FVehicleStructure VehicleToAdd);
		bool AddVehicle_Validate(FVehicleStructure VehicleToAdd);
	/** REMOVE VEHICLE (SERVER) */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Multiplayer Vehicle Functions")
		void RemoveVehicle(FVehicleStructure VehicleToRemove);
		void RemoveVehicle_Implementation(FVehicleStructure VehicleToRemove);
		bool RemoveVehicle_Validate(FVehicleStructure VehicleToRemove);

	/** ADD / REMOVE ITEM (SERVER) */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Multiplayer Inventory")
		void ItemToInventory(bool bRemove, FItemStructure Item);
		void ItemToInventory_Implementation(bool bRemove, FItemStructure Item);
		bool ItemToInventory_Validate(bool bRemove, FItemStructure Item);
	/** UPDATE INVENTORY */
	UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer Inventory")
		void RefreshInventory();

	/** ADD / REMOVE PROPERTY (SERVER) */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Multiplayer Property System")
		void Property(bool bRemove, int32 PropertyID);
		void Property_Implementation(bool bRemove, int32 PropertyID);
		bool Property_Validate(bool bRemove, int32 PropertyID);
	/** CHECK IF YOU OWN THE PROPERTY */
	UFUNCTION(BlueprintPure, Category = "Multiplayer Property System")
		bool PropertyOwned(int32 PropertyID);

///////////// PUBLIC REPLICATED VARIABLES
public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Player Information")
		int32 Reputation;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Player Information")
		float Cash;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Player Information")
		TArray<FVehicleStructure> OwnedVehicles;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Player Information")
		TArray<int32> OwnedProperties;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Player Information")
		TArray<FItemStructure> Inventory;
};