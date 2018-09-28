// Fill out your copyright notice in the Description page of Project Settings.

#include "FieldCPP.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AFieldCPP::AFieldCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Plane = CreateDefaultSubobject<UStaticMeshComponent>(FName("Plane"));
	//Plane = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("Plane"));
	RootComponent = Plane;
	

	// need to set this here because the fields are being created procedurally

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	Plane->SetStaticMesh(PlaneMeshAsset.Object);

}

void AFieldCPP::ResetFieldValues()
{
	visited = false;
	DistanceFromStart = EstimatedDistanceToGoal = CombinedCost = 9999.f;
	Parent = nullptr;
	Child = nullptr;
	Neighbours.Empty();
	
}

void AFieldCPP::SetFieldType(FFieldMaterialProperties* NewProperties, TEnumAsByte<EFieldType>::EnumType FType)
{
	this->FieldProperties = *NewProperties;
	this->FieldType = FType;
	Plane->SetMaterial(0, this->FieldProperties.FieldMaterial);

}

// Called when the game starts or when spawned
void AFieldCPP::BeginPlay()
{
	Super::BeginPlay();
	
}





