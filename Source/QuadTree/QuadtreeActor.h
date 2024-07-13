#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuadtreeNode.h"
#include "QuadtreeActor.generated.h"

UCLASS()
class QUADTREE_API AQuadtreeActor : public AActor
{
	GENERATED_BODY()

public:
	AQuadtreeActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	AQuadtreeNode* Root;

	void InsertPoint(FVector Point);
};
