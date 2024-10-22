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

	void InsertClusteredPoints(FVector CenterPoint, int32 NumPoints);
public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "QuadTree")
	AQuadtreeNode* Root;

	UPROPERTY(EditAnywhere, Category = "QuadTree")
	float Size = 2500;

	UPROPERTY(EditAnywhere, Category = "QuadTree")
	float StartPoints = 150;
	UPROPERTY(EditAnywhere, Category = "QuadTree")
	float Spread = 2000;

	UFUNCTION(BlueprintCallable, Category = "Quadtree")
	void InsertPoint(FVector Point);

	UFUNCTION(BlueprintCallable, Category = "Quadtree")
	void ClearQuadtree();

	UFUNCTION(BlueprintCallable, Category = "Quadtree")
	void InsertRandomPoints(int32 NumberOfPoints);

	UFUNCTION(BlueprintCallable, Category = "Quadtree")
	TArray<FVector> QueryPoints(FVector RangeCenter, float RangeSize);
};
