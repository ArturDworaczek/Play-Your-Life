// Game Created By Artur Dworaczek, Located In Somerset UK.

#include "PropertyDoorCPP.h"
#include "UnrealNetwork.h"


// SET DEFAULT VALUES
APropertyDoorCPP::APropertyDoorCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}
// REPLICATES VARIABLES
void APropertyDoorCPP::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(APropertyDoorCPP, bPropertyOwned);
	DOREPLIFETIME(APropertyDoorCPP, bDoorOpen);
	DOREPLIFETIME(APropertyDoorCPP, bAnimPlaying);
}