#include "QuadtreeActor.h"
#include "Kismet/KismetMathLibrary.h"

AQuadtreeActor::AQuadtreeActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AQuadtreeActor::BeginPlay()
{
	Super::BeginPlay();
	Root = GetWorld()->SpawnActor<AQuadtreeNode>(AQuadtreeNode::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	Root->Initialize(GetActorLocation(), Size);  // Set the center and size of the root node
	//InsertRandomPoints(StartPoints);

	// Clustered points around a specific area
	FVector CenterPoint(10, 10, 0);
	InsertClusteredPoints(CenterPoint, StartPoints);
}

void AQuadtreeActor::InsertClusteredPoints(FVector CenterPoint, int32 NumPoints)
{
	for (int32 i = 0; i < NumPoints; ++i)
	{
		float Angle = UKismetMathLibrary::RandomFloatInRange(0.0f, 2.0f * PI);
		float Distance = UKismetMathLibrary::RandomFloatInRange(0.0f, Spread);
		FVector RandomPoint = CenterPoint + FVector(FMath::Cos(Angle) * Distance, FMath::Sin(Angle) * Distance, 0.0f);
		InsertPoint(RandomPoint);
	}
}

void AQuadtreeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AQuadtreeActor::InsertPoint(FVector Point)
{
	if (Root)
	{
		Root->Insert(Point);
	}
}

void AQuadtreeActor::ClearQuadtree()
{
	if (Root)
	{
		Root->Clear();
		Root->Destroy();
		Root = GetWorld()->SpawnActor<AQuadtreeNode>(AQuadtreeNode::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		Root->Initialize(GetActorLocation(), Size);
	}
}

void AQuadtreeActor::InsertRandomPoints(int32 NumberOfPoints)
{
	for (int32 i = 0; i < NumberOfPoints; ++i)
	{
		FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), FVector(Size, Size, 0.0f));
		InsertPoint(RandomPoint);
	}
}

TArray<FVector> AQuadtreeActor::QueryPoints(FVector RangeCenter, float RangeSize)
{
	if (Root)
	{
		return Root->Query(RangeCenter, RangeSize);
	}
	return TArray<FVector>();
}
