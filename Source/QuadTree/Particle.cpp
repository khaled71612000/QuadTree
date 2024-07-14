#include "Particle.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"

AParticle::AParticle()
{
	PrimaryActorTick.bCanEverTick = true;

	ParticleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParticleMesh"));
	RootComponent = ParticleMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ParticleMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (ParticleMeshAsset.Succeeded())
	{
		ParticleMesh->SetStaticMesh(ParticleMeshAsset.Object);
		ParticleMesh->SetRelativeScale3D(FVector(0.5f));
	}
}

void AParticle::BeginPlay()
{
	Super::BeginPlay();
	Position = GetActorLocation();
}

void AParticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Add a small random offset to the position to simulate vibration
	float ShakeAmount = 15.0f; // Adjust this value to control the amount of shake
	Position.X += FMath::RandRange(-ShakeAmount, ShakeAmount);
	Position.Y += FMath::RandRange(-ShakeAmount, ShakeAmount);


	SetActorLocation(Position);
}
