#include "QuadtreePlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "QuadtreeActor.h"
#include "EngineUtils.h"

AQuadTreePlayerController::AQuadTreePlayerController()
{
	bShowMouseCursor = true;
}

void AQuadTreePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &AQuadTreePlayerController::OnLeftMouseClick);
}

void AQuadTreePlayerController::OnLeftMouseClick()
{
	FVector WorldLocation, WorldDirection;

	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		FVector End = WorldLocation + (WorldDirection * 10000.0f);

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, End, ECC_Visibility);

		if (HitResult.bBlockingHit)
		{
			FVector HitLocation = HitResult.Location;

			// Find the quadtree actor and insert the point
			for (TActorIterator<AQuadtreeActor> It(GetWorld()); It; ++It)
			{
				AQuadtreeActor* QuadtreeActor = *It;
				if (QuadtreeActor)
				{
					QuadtreeActor->InsertPoint(HitLocation);
					DrawDebugPoint(GetWorld(), HitLocation, 10, FColor::Red, false, 2.0f);
					break;
				}
			}
		}
	}
}
