#include "QuadtreeNode.h"
#include "DrawDebugHelpers.h"

AQuadtreeNode::AQuadtreeNode()
{
	PrimaryActorTick.bCanEverTick = true;
	Capacity = 4;
	Divided = false;
}

AQuadtreeNode::AQuadtreeNode(FVector InCenter, float InSize)
	: Center(InCenter), Size(InSize), Capacity(4), Divided(false)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AQuadtreeNode::Initialize(FVector InCenter, float InSize)
{
	Center = InCenter;
	Size = InSize;
}

void AQuadtreeNode::BeginPlay()
{
	Super::BeginPlay();
}

void AQuadtreeNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugBox(GetWorld(), Center, FVector(Size, Size, 0), FColor::Green, false, -1, 0, 5);

	if (Divided)
	{
		NE->Tick(DeltaTime);
		NW->Tick(DeltaTime);
		SE->Tick(DeltaTime);
		SW->Tick(DeltaTime);
	}
}

void AQuadtreeNode::Subdivide()
{
	float NewSize = Size / 2;

	NE = GetWorld()->SpawnActor<AQuadtreeNode>(AQuadtreeNode::StaticClass(), FVector(Center.X + NewSize, Center.Y + NewSize, 0), FRotator::ZeroRotator);
	NW = GetWorld()->SpawnActor<AQuadtreeNode>(AQuadtreeNode::StaticClass(), FVector(Center.X - NewSize, Center.Y + NewSize, 0), FRotator::ZeroRotator);
	SE = GetWorld()->SpawnActor<AQuadtreeNode>(AQuadtreeNode::StaticClass(), FVector(Center.X + NewSize, Center.Y - NewSize, 0), FRotator::ZeroRotator);
	SW = GetWorld()->SpawnActor<AQuadtreeNode>(AQuadtreeNode::StaticClass(), FVector(Center.X - NewSize, Center.Y - NewSize, 0), FRotator::ZeroRotator);

	NE->Initialize(FVector(Center.X + NewSize / 2, Center.Y + NewSize / 2, 0), NewSize);
	NW->Initialize(FVector(Center.X - NewSize / 2, Center.Y + NewSize / 2, 0), NewSize);
	SE->Initialize(FVector(Center.X + NewSize / 2, Center.Y - NewSize / 2, 0), NewSize);
	SW->Initialize(FVector(Center.X - NewSize / 2, Center.Y - NewSize / 2, 0), NewSize);

	Divided = true;
}

void AQuadtreeNode::Insert(FVector Point)
{
	if (!ContainsPoint(Point))
	{
		return;
	}

	if (Points.Num() < Capacity)
	{
		Points.Add(Point);
	}
	else
	{
		if (!Divided)
		{
			Subdivide();
		}

		NE->Insert(Point);
		NW->Insert(Point);
		SE->Insert(Point);
		SW->Insert(Point);
	}
}

bool AQuadtreeNode::ContainsPoint(FVector Point)
{
	return Point.X >= Center.X - Size && Point.X <= Center.X + Size &&
		Point.Y >= Center.Y - Size && Point.Y <= Center.Y + Size;
}