// Game Created By Artur Dworaczek, Located In Somerset UK.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PropertyDoorCPP.generated.h"

UCLASS()
class PLAYYOURLIFE_API APropertyDoorCPP : public AActor
{
	GENERATED_BODY()
///////////// PUBLIC STUFF
public:	
	// Sets default values for this actor's properties
	APropertyDoorCPP();

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Multiplayer Property System")
		bool bPropertyOwned;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Multiplayer Property System")
		bool bDoorOpen;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Multiplayer Property System")
		bool bAnimPlaying;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Multiplayer Property System")
		UStaticMesh* Door;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Multiplayer Property System")
		int32 PropertyID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Multiplayer Property System")
		float PropertyValue;

	/** OPEN THE DOOR */
	UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer Property System")
		void OpenDoor();
	/** CLOSE THE DOOR */
	UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer Property System")
		void CloseDoor();
///////////// PRIVATE STUFF
private:
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;
};
