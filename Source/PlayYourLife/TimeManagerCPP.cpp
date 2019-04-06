// Game Created By Artur Dworaczek, Located In Somerset UK.

#include "TimeManagerCPP.h"
#include "UnrealNetwork.h"
#include "Engine.h"


// Sets default values
ATimeManagerCPP::ATimeManagerCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// AMOUNT IN MINUTES TO COMPLETE 1 DAY.
	DayLength = 24;
}

/** START THE DAY NIGHT CYCLE ON CLIENT (CLIENT) */
void ATimeManagerCPP::StartCycle_Implementation(int32 DayToStart, float HourToStart)
{
	// CALCULATIONS FOR THE DAY CYCLE
	OneDay = DayLength * 60;
	OneHour = OneDay / 24;
	OneMinute = OneHour / 60;
	OneSecond = OneMinute / 60;

	// DEFAULT START
	GlobalDay = DayToStart;
	GlobalHour = HourToStart;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATimeManagerCPP::TimeManagerCycle, OneSecond, true);
}
/** MAIN DAY NIGHT CYLCE FUNCTION */
void ATimeManagerCPP::TimeManagerCycle()
{
	if (GlobalHour >= 24)
	{
		GlobalDay++;
		GlobalHour = 0.0f;
	}
	else if (GlobalHour >= 6 && GlobalHour < 18)
	{
		CurrentWorldState = EDayNight::E_Day;
	}
	else
	{
		CurrentWorldState = EDayNight::E_Night;
	}

	GlobalHour += OneSecond / OneHour;
	SunAngleRot.Pitch = GlobalHour / 6 * 90 + 90;

	UpdateSunLocation(SunAngleRot);
}