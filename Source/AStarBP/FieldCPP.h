// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "FieldCPP.generated.h"

class UStaticMeshComponent;
class UMaterialInterface;
class UInstancedStaticMeshComponent;


UENUM(BlueprintType)
enum EFieldType {
	Grass UMETA(DisplayName = "Grass"),
	Rock UMETA(DisplayName = "Rock"),
	Obstacle UMETA(DisplayName = "Obstacle"),
	End UMETA(DisplayName = "End"),
	Start UMETA(DisplayName = "Start"),
	Path UMETA(DisplayName = "Path"),
	Gray UMETA(DisplayName = "Gray")
};



USTRUCT(BlueprintType)
struct FFieldMaterialProperties
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	UMaterialInterface* FieldMaterial;

	UPROPERTY(EditAnywhere)
	bool is_obstacle;

	FFieldMaterialProperties(){}
	
};



UCLASS()
class ASTARBP_API AFieldCPP : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFieldCPP();

	void SetDistanceFromStart(int DistanceFromStart) {this->DistanceFromStart = DistanceFromStart;}
	
	void SetDistanceToGoal(float DistanceToGoal) { this->EstimatedDistanceToGoal = DistanceToGoal; }
	
	void ResetFieldValues();

	float CalculateFCost() { return DistanceFromStart + EstimatedDistanceToGoal; }

	void SetFieldType(FFieldMaterialProperties* NewProperties, TEnumAsByte<EFieldType>::EnumType);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * Plane;
		//UInstancedStaticMeshComponent* Plane;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFieldMaterialProperties FieldProperties;

	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<EFieldType> FieldType;

	UPROPERTY(VisibleAnywhere)
	int x;

	UPROPERTY(VisibleAnywhere)
	int y;

	UPROPERTY(VisibleAnywhere)
	bool visited;

	/*G-Cost*/
	UPROPERTY(VisibleAnywhere)
	float DistanceFromStart;
	/*H-Cost*/
	UPROPERTY(VisibleAnywhere)
	float EstimatedDistanceToGoal;
	/*F-Cost*/
	UPROPERTY(VisibleAnywhere)
	float CombinedCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AFieldCPP* Parent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AFieldCPP* Child;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AFieldCPP*>Neighbours;
	
};
