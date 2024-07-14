#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particle.generated.h"

UCLASS()
class QUADTREE_API AParticle : public AActor
{
	GENERATED_BODY()

public:
	AParticle();

	UPROPERTY(EditAnywhere, Category = "QuadTree")
	FVector Velocity;

	UPROPERTY(EditAnywhere, Category = "QuadTree")
	FVector Position;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ParticleMesh;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
