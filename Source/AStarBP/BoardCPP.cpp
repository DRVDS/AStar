// Fill out your copyright notice in the Description page of Project Settings.

#include "BoardCPP.h"
#include "FieldCPP.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InstancedStaticMeshComponent.h"



// Sets default values
ABoardCPP::ABoardCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//static ConstructorHelpers::FObjectFinder<UClass> MazePawnFinder(TEXT("Class'/Game/TopDownBP/Blueprints/TopDownCharacter.TopDownCharacter'"));
	//MazePawn = MazePawnFinder.Object;

	
	b_MazeRunnerCalled = false;
//	width = 10;
//	heigth = 10;

}

void ABoardCPP::SetStartPoint(FHitResult HitResult)
{
	if (HitResult.Actor->IsA(AFieldCPP::StaticClass()))
	{
		// if there is a valid start field reset the current one 
		if (StartField != nullptr)
		{
			StartField->SetFieldType(FieldPropertyMap.Find(TEnumAsByte<EFieldType>::EnumType::Gray), TEnumAsByte<EFieldType>::EnumType::Gray);
		}
		auto ptr = Cast<AFieldCPP>(HitResult.Actor);
		StartField = ptr;
		ptr->SetFieldType(FieldPropertyMap.Find(TEnumAsByte<EFieldType>::EnumType::Start), TEnumAsByte<EFieldType>::EnumType::Start);
	}
	// Spawn Pawn
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FRotator SpawnRotation(0, 0, 0);
	/*
	Instance_MazePawn = GetWorld()->SpawnActor<ACharacter>(MazePawn,
		FVector(StartField->GetActorLocation().X , StartField->GetActorLocation().Y, 95.f), SpawnRotation, SpawnParams);
	UE_LOG(LogTemp,Warning, TEXT(" X + %f : Y + %f "), StartField->GetActorLocation().X, StartField->GetActorLocation().Y)
	*/
	
}

void ABoardCPP::SetEndPoint(FHitResult HitResult)
{
	if (HitResult.Actor->IsA(AFieldCPP::StaticClass()))
	{
		if (EndField != nullptr)
		{
			EndField->SetFieldType(FieldPropertyMap.Find(TEnumAsByte<EFieldType>::EnumType::Gray), TEnumAsByte<EFieldType>::EnumType::Gray);
		}
		auto ptr = Cast<AFieldCPP>(HitResult.Actor);
		EndField = ptr;
		ptr->SetFieldType(FieldPropertyMap.Find(TEnumAsByte<EFieldType>::EnumType::End), TEnumAsByte<EFieldType>::EnumType::End);
		
	}
}

void ABoardCPP::SetObstacle(FHitResult HitResult)
{
	if (HitResult.Actor->IsA(AFieldCPP::StaticClass()))
	{
		auto ptr = Cast<AFieldCPP>(HitResult.Actor);

		if(ptr->FieldProperties.is_obstacle)
			ptr->SetFieldType(FieldPropertyMap.Find(TEnumAsByte<EFieldType>::EnumType::Gray), TEnumAsByte<EFieldType>::EnumType::Gray);
		else
			ptr->SetFieldType(FieldPropertyMap.Find(TEnumAsByte<EFieldType>::EnumType::Obstacle), TEnumAsByte<EFieldType>::EnumType::Obstacle);
	}
}



void ABoardCPP::GetNeighbours(AFieldCPP * InField)
{
	// Remove all possible neighbours that the element might have from previous runs
	InField->Neighbours.Empty();

	AFieldCPP* east = GetField(InField->x + 1, InField->y);
	AFieldCPP* west = GetField(InField->x - 1, InField->y);
	AFieldCPP* north = GetField(InField->x, InField->y + 1);
	AFieldCPP* south = GetField(InField->x, InField->y -1);

	if (east && !east->FieldProperties.is_obstacle) { InField->Neighbours.Add(east); }
	if (west && !west->FieldProperties.is_obstacle) { InField->Neighbours.Add(west); }
	if (north && !north->FieldProperties.is_obstacle) { InField->Neighbours.Add(north); }
	if (south && !south->FieldProperties.is_obstacle) { InField->Neighbours.Add(south); }
}

AFieldCPP* ABoardCPP::GetField(int x, int y) {
	
	for (auto e : Fields) {
		if (e->x == x && e->y == y)
			return e;
	}
	return nullptr;
}

float ABoardCPP::EstimateDistanceToGoal(AFieldCPP * InField)
{
	return sqrt(DistanceBetweenTwoCells(InField, EndField));
		
}

float ABoardCPP::DistanceBetweenTwoCells(AFieldCPP * InField1, AFieldCPP * InField2)
{
	return (InField1->x + InField2->x) * (InField1->x + InField2->x) + (InField1->y + InField2->y) * (InField1->y + InField2->y);
}

void ABoardCPP::ResetBoard()
{
	for (auto e : Fields)
	{
		if (e->FieldType.GetValue() == EFieldType::Path)
		{
			e->SetFieldType(FieldPropertyMap.Find(TEnumAsByte<EFieldType>::EnumType::Gray), TEnumAsByte<EFieldType>::EnumType::Gray);
		}
		e->ResetFieldValues();
	}
}

void ABoardCPP::SolveAStar()
{
	// Clean out previous values
	for (auto e : Fields)
	{
		if(e->FieldType.GetValue() == EFieldType::Path)
		{
			e->SetFieldType(FieldPropertyMap.Find(TEnumAsByte<EFieldType>::EnumType::Gray), TEnumAsByte<EFieldType>::EnumType::Gray);
		}
		e->ResetFieldValues();
	}

	if (!StartField || !EndField) {
		UE_LOG(LogTemp, Warning, TEXT("Set StartField And EndField first"));
		return;
	}
	// Start with empty lists 
	
	OpenList.Empty();
	ClosedList.Empty();
	
	//and add StartField
	OpenList.Add(StartField);

	StartField->DistanceFromStart = 0.0f;
	StartField->EstimatedDistanceToGoal = EstimateDistanceToGoal(EndField);

	while (OpenList.Num() != 0) 
	{
			OpenList.Sort([](const AFieldCPP &lhs, const AFieldCPP &rhs) {return lhs.CombinedCost < rhs.CombinedCost; });
			
			CurrentNode = OpenList[0];
			OpenList.Remove(CurrentNode);

			ClosedList.Add(CurrentNode);
			
			// Have we reached the End ???
			if (CurrentNode == EndField)
			{
				UE_LOG(LogTemp, Warning, TEXT("Path found!"));

				PathFound = true;

				// Lets draw the path starting from the Goal going to the Start
				while (CurrentNode != StartField)
				{
						if(CurrentNode != EndField)
							CurrentNode->SetFieldType(FieldPropertyMap.Find(TEnumAsByte<EFieldType>::EnumType::Path), TEnumAsByte<EFieldType>::EnumType::Path);
						
						AFieldCPP* addAsChild = CurrentNode;
						CurrentNode = CurrentNode->Parent;
						CurrentNode->Child = addAsChild;

				}
				
			}
			else
			{
				GetNeighbours(CurrentNode);

				for (auto child : CurrentNode->Neighbours)
				{
					if (ClosedList.Contains(child))
						continue;


					if (!OpenList.Contains(CurrentNode))
					{
						OpenList.Add(child);
						child->Parent = CurrentNode;

						// Create the f, g, and h values
						child->DistanceFromStart = CurrentNode->DistanceFromStart + 1;
						child->EstimatedDistanceToGoal = EstimateDistanceToGoal(child);
						child->CombinedCost = child->DistanceFromStart + child->EstimatedDistanceToGoal;

					}
					else
					{
						// Create the f, g, and h values
						child->DistanceFromStart = CurrentNode->DistanceFromStart + DistanceBetweenTwoCells(child, CurrentNode);
						child->EstimatedDistanceToGoal = EstimateDistanceToGoal(child);
						child->CombinedCost = child->DistanceFromStart + child->EstimatedDistanceToGoal;

						if (OpenList[OpenList.Find(child)]->DistanceFromStart < child->DistanceFromStart)
							child->Parent = CurrentNode;

					}

				}
			}
	}
}

void ABoardCPP::GenerateMaze()
{
	int ran;  
	for (auto e : Fields)
	{
		ran = FMath::RandRange(0, 10);
		if(ran < 2)
			e->SetFieldType(FieldPropertyMap.Find(TEnumAsByte<EFieldType>::EnumType::Obstacle), TEnumAsByte<EFieldType>::EnumType::Obstacle);
	}

}


void ABoardCPP::Tick(float DeltaSeconds)
{
	if (!b_MazeRunnerCalled) 
		return;

	//FVector CurrentPostion = Instance_MazePawn->GetActorLocation();
	//FVector NextPosition = CurrentNode->Child->GetActorLocation();
	//if(Instance_MazePawn)
	//	Instance_MazePawn->SetActorLocation(FMath::Lerp(CurrentPostion, NextPosition, DeltaSeconds));
}

void ABoardCPP::StartMazeRunner(ACharacter* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("StartMazRunner Called"))
	b_MazeRunnerCalled = true;

}


// Called when the game starts or when spawned
void ABoardCPP::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	FRotator SpawnRotation(0, 0, 0);


	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	Gray->SetStaticMesh(PlaneMeshAsset.Object);
	PlaneMeshAsset->SetMaterial(
	Path->SetStaticMesh(PlaneMeshAsset.Object);
	Obstacle->SetStaticMesh(PlaneMeshAsset.Object);

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < heigth; y++)
		{
			auto ptr = GetWorld()->SpawnActor<AFieldCPP>(AFieldCPP::StaticClass(), FVector(x * 100, y * 100, 0), SpawnRotation, SpawnParams);
			
			// set coordinates
			ptr->x = x;
			ptr->y = y;
			//ptr->Plane->AddInstance(FTransform());
			ptr->SetFieldType(FieldPropertyMap.Find(TEnumAsByte<EFieldType>::EnumType::Gray), TEnumAsByte<EFieldType>::EnumType::Gray);

			Fields.Add(ptr);
		}
	}

}



