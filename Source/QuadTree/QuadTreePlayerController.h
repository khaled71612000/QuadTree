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

	virtual void PlayerTick(float DeltaTime) override;

protected:

private:
	void OnLeftMouseClick();
	void HandleMouseClick();
	void HandleRightHold();

	bool bCanClick;
	float ClickCooldown;
	float LastClickTime;
};
