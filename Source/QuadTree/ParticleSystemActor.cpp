#include "ParticleSystemActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

AParticleSystemActor::AParticleSystemActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AParticleSystemActor::BeginPlay()
{
	Super::BeginPlay();

	// Ensure QuadtreeActor is assigned in the editor or dynamically spawn it
	if (!QuadtreeActor)
	{
		QuadtreeActor = GetWorld()->SpawnActor<AQuadtreeActor>(AQuadtreeActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		QuadtreeActor->Root->Initialize(GetActorLocation(), 2000.0f); // Assuming 2000 is the size you want
	}

	InitializeParticles(ParticlesNum);

	// Measure performance after a delay to allow initialization
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AParticleSystemActor::MeasurePerformance);
}

void AParticleSystemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool bNeedsUpdate = false;

	for (AParticle* Particle : Particles)
	{
		Particle->Tick(DeltaTime);

		FVector PreviousPosition = Particle->Position;
		Particle->Tick(DeltaTime);  // Ensure to call the correct method to move particles
		if (!bNeedsUpdate && FVector::DistSquared(PreviousPosition, Particle->Position) > FMath::Square(10.0f)) // Threshold can be adjusted
		{
			bNeedsUpdate = true;
		}

		Particle->SetActorLocation(Particle->Position);
	}

	if (bNeedsUpdate)
	{
		UpdateQuadtree();
	}

	for (AParticle* Particle : Particles)
	{
		TArray<FVector> NearbyParticles = QuadtreeActor->QueryPoints(Particle->GetActorLocation(), QueryRange);

		for (const FVector& NearbyParticleLocation : NearbyParticles)
		{
			if (FVector::DistSquared(Particle->GetActorLocation(), NearbyParticleLocation) <= FMath::Square(QueryRange))
			{
				// Perform necessary collision checks or interactions here
				DrawDebugLine(GetWorld(), Particle->GetActorLocation(), NearbyParticleLocation, FColor::Black, false, -1.0f, 100, 10.0f);
			}
		}
	}
}

void AParticleSystemActor::InitializeParticles(int32 NumParticles)
{
	for (int32 i = 0; i < NumParticles; ++i)
	{
		FVector RandomPosition = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), FVector(QuadtreeActor->Size, QuadtreeActor->Size, 0.0f));
		FVector RandomVelocity = UKismetMathLibrary::RandomUnitVector() * UKismetMathLibrary::RandomFloatInRange(100.0f, 500.0f);
		// Ensure Z component is zero for 2D behavior

		RandomPosition.Z = QuadtreeActor->GetActorLocation().Z;

		AParticle* NewParticle = GetWorld()->SpawnActor<AParticle>(AParticle::StaticClass(), RandomPosition, FRotator::ZeroRotator);
		NewParticle->Velocity = RandomVelocity;
		Particles.Add(NewParticle);
	}
}


//FScopedSlowTask is used to display a progress dialog for long-running tasks. It helps provide feedback to the user while the quadtree is being updated or while performance is being measured.
void AParticleSystemActor::UpdateQuadtree()
{
	if (QuadtreeActor && QuadtreeActor->Root)
	{
		//FScopedSlowTask SlowTask(0, FText::FromString(TEXT("Updating Quadtree...")));

		////This method is called to show the progress dialog to the user. It should be called once after initializing FScopedSlowTask.
		//SlowTask.MakeDialog();

		QuadtreeActor->ClearQuadtree();

		for (AParticle* Particle : Particles)
		{
			//This method is called to update the progress. It should be called within the loop or steps where the task progresses, indicating the completion of a unit of work.
			//SlowTask.EnterProgressFrame();
			QuadtreeActor->InsertPoint(Particle->GetActorLocation());
		}
	}
}

void AParticleSystemActor::MeasurePerformance()
{
	//double StartTime;
	//double EndTime;

	//// Measure performance without Quadtree
	////FPlatformTime::Seconds() is used to get the current time in seconds. It is utilized to measure the duration of the performance test by capturing the start and end times of the operations.
	//StartTime = FPlatformTime::Seconds();
	//{
	//	for (AParticle* Particle : Particles)
	//	{
	//		for (AParticle* OtherParticle : Particles)
	//		{
	//			if (Particle != OtherParticle)
	//			{
	//				if (FVector::DistSquared(Particle->GetActorLocation(), OtherParticle->GetActorLocation()) <= FMath::Square(QueryRange))
	//				{
	//					// Perform necessary collision checks or interactions here
	//					DrawDebugLine(GetWorld(), Particle->GetActorLocation(), OtherParticle->GetActorLocation(), FColor::Black, false, -1.0f, 100, 10.0f);
	//				}
	//			}
	//		}
	//	}
	//}
	////measure time after operation and see the difference
	//EndTime = FPlatformTime::Seconds();
	//TimeWithoutQuadtree = EndTime - StartTime;

	//// Measure performance with Quadtree
	//////The use of a quadtree structure for collision detection in the particle system is faster compared to the brute-force method (checking every particle against every other particle) due to its spatial partitioning capabilities. 
	//StartTime = FPlatformTime::Seconds();
	//UpdateQuadtree();
	//{
	//	for (AParticle* Particle : Particles)
	//	{
	//		//Each node in the quadtree represents a bounding box, and particles are stored in the appropriate nodes based on their positions.
	//		//With a quadtree, each particle only needs to be compared against other particles in its local region and neighboring regions, rather than against all other particles.
	//		TArray<FVector> NearbyParticles = QuadtreeActor->QueryPoints(Particle->GetActorLocation(), QueryRange);
	//		for (const FVector& NearbyParticleLocation : NearbyParticles)
	//		{
	//			if (FVector::DistSquared(Particle->GetActorLocation(), NearbyParticleLocation) <= FMath::Square(QueryRange))
	//			{
	//				// Perform necessary collision checks or interactions here
	//				DrawDebugLine(GetWorld(), Particle->GetActorLocation(), NearbyParticleLocation, FColor::Black, false, -1.0f, 100, 10.0f);
	//			}
	//		}
	//	}
	//}
	//EndTime = FPlatformTime::Seconds();
	//TimeWithQuadtree = EndTime - StartTime;

	//// Print the results
	//UE_LOG(LogTemp, Warning, TEXT("Time without Quadtree: %f seconds"), TimeWithoutQuadtree);
	//UE_LOG(LogTemp, Warning, TEXT("Time with Quadtree: %f seconds"), TimeWithQuadtree);
}