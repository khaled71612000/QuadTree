#include "QuadtreeActor.h"

AQuadtreeActor::AQuadtreeActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AQuadtreeActor::BeginPlay()
{
	Super::BeginPlay();
	Root = GetWorld()->SpawnActor<AQuadtreeNode>(AQuadtreeNode::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	Root->Initialize(GetActorLocation(), 200.0f);  // Set the center and size of the root node
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
