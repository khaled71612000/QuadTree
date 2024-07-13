#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "QuadTreePlayerController.generated.h"

UCLASS()
class QUADTREE_API AQuadTreePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AQuadTreePlayerController();

protected:
	virtual void SetupInputComponent() override;

	void OnLeftMouseClick();
};
