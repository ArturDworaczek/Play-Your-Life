// Game Created By Artur Dworaczek, Located In Somerset UK.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/DirectionalLightComponent.h"
#include "TimeManagerCPP.generated.h"

// ENUM
UENUM(BlueprintType)
enum EDayNight
{
	E_Day		UMETA(DisplayName = "Day"),
	E_Night		UMETA(DisplayName = "Night")
};

UCLASS()
class PLAYYOURLIFE_API ATimeManagerCPP : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimeManagerCPP();

	FTimerHandle TimerHandle;

	/** TIME FOR A FULL DAY TO PASS */
	UPROPERTY(BlueprintReadOnly, Category = "Multiplayer Time Manager")
		float OneDay;
	/** TIME FOR A HOUR TO PASS */
	UPROPERTY(BlueprintReadOnly, Category = "Multiplayer Time Manager")
		float OneHour;
	/** TIME FOR A MINUTE TO PASS */
	UPROPERTY(BlueprintReadOnly, Category = "Multiplayer Time Manager")
		float OneMinute;
	/** TIME FOR A MINUTE TO PASS */
	UPROPERTY(BlueprintReadOnly, Category = "Multiplayer Time Manager")
		float OneSecond;
	/** CURRENT SUN ANGLE */
	UPROPERTY(BlueprintReadOnly, Category = "Multiplayer Time Manager")
		FRotator SunAngleRot;

	/** CURRENT DAY */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Multiplayer Time Manager")
		int32 GlobalDay;
	/** CURRENT GAME HOUR */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Multiplayer Time Manager")
		float GlobalHour;
	/** DAY LENGTH IN MINUTES */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Multiplayer Time Manager")
		float DayLength;

	/** CURRENT WORLD STATE */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Multiplayer Time Manager")
		TEnumAsByte<EDayNight> CurrentWorldState;

	/** UPDATE SUN LOCATION */
	UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer Time Manager")
		void UpdateSunLocation(FRotator SunRotation);
	/** START THE DAY NIGHT CYCLE ON CLIENT (CLIENT) */
	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "Multiplayer Time Manager")
		void StartCycle(int32 DayToStart, float HourToStart);
		void StartCycle_Implementation(int32 DayToStart, float HourToStart);
	/** MAIN DAY NIGHT CYLCE FUNCTION */
	UFUNCTION(BlueprintCallable, Category = "Multiplayer Time Manager")
		void TimeManagerCycle();
};
