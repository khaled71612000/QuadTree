#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuadtreeActor.h"
#include "Particle.h"
#include "ParticleSystemActor.generated.h"

UCLASS()
class QUADTREE_API AParticleSystemActor : public AActor
{
	GENERATED_BODY()

public:
	AParticleSystemActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "QuadTree")
	AQuadtreeActor* QuadtreeActor;

	UPROPERTY(EditAnywhere, Category = "QuadTree")
	TArray<AParticle*> Particles;

	UPROPERTY(EditAnywhere, Category = "QuadTree")
	TSubclassOf<AParticle> ParticleClass;

	UPROPERTY(EditAnywhere, Category = "QuadTree")
	int32 ParticlesNum = 50;

	UPROPERTY(EditAnywhere, Category = "QuadTree")
	int32 QueryRange = 300;

	void InitializeParticles(int32 NumParticles);
	void UpdateQuadtree();


	void MeasurePerformance();

private:
	double TimeWithoutQuadtree;
	double TimeWithQuadtree;
};
