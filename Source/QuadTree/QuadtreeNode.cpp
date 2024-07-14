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
	DrawDebugBox(GetWorld(), Center, FVector(Size, Size, 0), FColor::Green, false, -1, 0, 10);

	for (const FVector& Point : Points)
	{
		DrawDebugPoint(GetWorld(), Point, 10, FColor::Red, false, -1);
	}

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

	NE = GetWorld()->SpawnActor<AQuadtreeNode>(AQuadtreeNode::StaticClass(), FVector(Center.X + NewSize, Center.Y + NewSize, Center.Z), FRotator::ZeroRotator);
	NW = GetWorld()->SpawnActor<AQuadtreeNode>(AQuadtreeNode::StaticClass(), FVector(Center.X - NewSize, Center.Y + NewSize, Center.Z), FRotator::ZeroRotator);
	SE = GetWorld()->SpawnActor<AQuadtreeNode>(AQuadtreeNode::StaticClass(), FVector(Center.X + NewSize, Center.Y - NewSize, Center.Z), FRotator::ZeroRotator);
	SW = GetWorld()->SpawnActor<AQuadtreeNode>(AQuadtreeNode::StaticClass(), FVector(Center.X - NewSize, Center.Y - NewSize, Center.Z), FRotator::ZeroRotator);

	NE->Initialize(FVector(Center.X + NewSize / 2, Center.Y + NewSize / 2, Center.Z), NewSize);
	NW->Initialize(FVector(Center.X - NewSize / 2, Center.Y + NewSize / 2, Center.Z), NewSize);
	SE->Initialize(FVector(Center.X + NewSize / 2, Center.Y - NewSize / 2, Center.Z), NewSize);
	SW->Initialize(FVector(Center.X - NewSize / 2, Center.Y - NewSize / 2, Center.Z), NewSize);

	Divided = true;
}

bool AQuadtreeNode::Insert(FVector Point)
{
	if (!ContainsPoint(Point))
	{
		return false;
	}

	if (Points.Num() < Capacity)
	{
		Points.Add(Point);
		return true;

	}
	else
	{
		if (!Divided)
		{
			Subdivide();
		}

		if (NE->Insert(Point))
		{
			return true;
		}

		if (NW->Insert(Point))
		{
			return true;
		}		
		if (SE->Insert(Point))
		{
			return true;
		}		
		if (SW->Insert(Point))
		{
			return true;
		}

		return false;
	}
}
//AABB
bool AQuadtreeNode::ContainsPoint(FVector Point)
{
	return Point.X >= Center.X - Size && Point.X <= Center.X + Size &&
		Point.Y >= Center.Y - Size && Point.Y <= Center.Y + Size;
}

TArray<FVector> AQuadtreeNode::Query(FVector RangeCenter, float RangeSize)
{
	TArray<FVector> FoundPoints;

	if (!Intersect(RangeCenter, RangeSize))
	{
		return FoundPoints;
	}

	for (const FVector& Point : Points)
	{
		if (Point.X >= RangeCenter.X - RangeSize && Point.X <= RangeCenter.X + RangeSize &&
			Point.Y >= RangeCenter.Y - RangeSize && Point.Y <= RangeCenter.Y + RangeSize)
		{
			FoundPoints.Add(Point);
		}
	}

	if (Divided)
	{
		FoundPoints.Append(NE->Query(RangeCenter, RangeSize));
		FoundPoints.Append(NW->Query(RangeCenter, RangeSize));
		FoundPoints.Append(SE->Query(RangeCenter, RangeSize));
		FoundPoints.Append(SW->Query(RangeCenter, RangeSize));
	}

	return FoundPoints;
}


bool AQuadtreeNode::Intersect(FVector RangeCenter, float RangeSize)
{
	return !(RangeCenter.X - RangeSize > Center.X + Size ||
		RangeCenter.X + RangeSize < Center.X - Size ||
		RangeCenter.Y - RangeSize > Center.Y + Size ||
		RangeCenter.Y + RangeSize < Center.Y - Size);
}