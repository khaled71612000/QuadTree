#include "QuadtreePlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "QuadtreeActor.h"
#include "EngineUtils.h"

AQuadTreePlayerController::AQuadTreePlayerController()
{
	bShowMouseCursor = true;
	bCanClick = true;
	ClickCooldown = 0.1f; // Cooldown period in seconds
	LastClickTime = 0.0f;
}

void AQuadTreePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// Check for left mouse button click and cooldown
	if (IsInputKeyDown(EKeys::LeftMouseButton) && bCanClick)
	{
		HandleMouseClick();
	}

	if (IsInputKeyDown(EKeys::RightMouseButton))
	{
		HandleRightHold();
	}

	// Update cooldown timer
	if (!bCanClick)
	{
		LastClickTime += DeltaTime;
		if (LastClickTime >= ClickCooldown)
		{
			bCanClick = true;
			LastClickTime = 0.0f;
		}
	}
}

void AQuadTreePlayerController::HandleMouseClick()
{
	bCanClick = false; // Disable further clicks until cooldown expires
	OnLeftMouseClick();
}

void AQuadTreePlayerController::HandleRightHold()
{

	FVector WorldLocation, WorldDirection;

	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		FPlane Plane(FVector(0, 0, 0), FVector(0, 0, 1));  // XY plane at Z = 0
		FVector IntersectionPoint = FMath::LinePlaneIntersection(WorldLocation, WorldLocation + (WorldDirection * 10000.0f), Plane);

		for (TActorIterator<AQuadtreeActor> It(GetWorld()); It; ++It)
		{
			AQuadtreeActor* QuadtreeActor = *It;
			if (QuadtreeActor)
			{
				IntersectionPoint.Z = QuadtreeActor->GetActorLocation().Z;

				// Define the range for the query
				FVector RangeCenter = IntersectionPoint;
				float RangeSize = 1000.0f; // Adjust this size as needed

				// Draw the query rectangle (bounding box)
				FVector BoxExtent(RangeSize, RangeSize, 0.0f);
				DrawDebugBox(GetWorld(), RangeCenter, BoxExtent, FColor::Yellow, false, -1, 100, 20.f);

				// Query points in the range centered at IntersectionPoint
				TArray<FVector> QueriedPoints = QuadtreeActor->QueryPoints(RangeCenter, RangeSize);
				for (const FVector& Point : QueriedPoints)
				{
					// Visualize the queried points
					DrawDebugPoint(GetWorld(), FVector(Point.X, Point.Y, RangeCenter.Z), 20, FColor::Blue, false, -1, 1000.f);
				}
				break;
			}
		}
	}
}

void AQuadTreePlayerController::OnLeftMouseClick()
{
	FVector WorldLocation, WorldDirection;

	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		// Assuming your 2D grid is in the XY plane at Z = 0
		FPlane Plane(FVector(0, 0, 0), FVector(0, 0, 1));  // XY plane at Z = 0

		// Calculate intersection of the mouse ray with the plane
		FVector IntersectionPoint = FMath::LinePlaneIntersection(WorldLocation, WorldLocation + (WorldDirection * 10000.0f), Plane);

		// Adjust the Z coordinate to match the grid actor's Z
		for (TActorIterator<AQuadtreeActor> It(GetWorld()); It; ++It)
		{
			AQuadtreeActor* QuadtreeActor = *It;
			if (QuadtreeActor)
			{
				IntersectionPoint.Z = QuadtreeActor->GetActorLocation().Z;
				break;
			}
		}

		// Debug draw the intersection point in the editor
		DrawDebugPoint(GetWorld(), IntersectionPoint, 10.0f, FColor::Red, false, -1);

		// Iterate over Quadtree actors
		for (TActorIterator<AQuadtreeActor> It(GetWorld()); It; ++It)
		{
			AQuadtreeActor* QuadtreeActor = *It;
			if (QuadtreeActor)
			{
				// Insert the point in local space
				FVector LocalSpaceLocation = QuadtreeActor->GetActorTransform().InverseTransformPosition(IntersectionPoint);
				QuadtreeActor->InsertPoint(LocalSpaceLocation);

				// Debug draw the local space location
				FVector WorldSpaceLocation = QuadtreeActor->GetActorTransform().TransformPosition(LocalSpaceLocation);
				DrawDebugPoint(GetWorld(), WorldSpaceLocation, 10.0f, FColor::Blue, false, -1);
				break;
			}
		}
	}
}