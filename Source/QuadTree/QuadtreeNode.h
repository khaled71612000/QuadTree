#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuadtreeNode.generated.h"

UCLASS()
class QUADTREE_API AQuadtreeNode : public AActor
{
	GENERATED_BODY()

public:
	AQuadtreeNode();

	AQuadtreeNode(FVector InCenter, float InSize);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void Subdivide();
	void Insert(FVector Point);
	bool ContainsPoint(FVector Point);
	void Initialize(FVector InCenter, float InSize);

private:
	UPROPERTY(EditAnywhere)
	FVector Center;

	UPROPERTY(EditAnywhere)
	float Size;

	UPROPERTY(EditAnywhere)
	int Capacity;

	TArray<FVector> Points;

	bool Divided;

	AQuadtreeNode* NE;
	AQuadtreeNode* NW;
	AQuadtreeNode* SE;
	AQuadtreeNode* SW;
};
