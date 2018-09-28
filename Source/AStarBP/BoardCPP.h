// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FieldCPP.h"

#include "BoardCPP.generated.h"

class AFieldCPP;
class ACharacter;
class UInstancedStaticMeshComponent;


UCLASS()
class ASTARBP_API ABoardCPP : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoardCPP();

	UPROPERTY(VisibleAnywhere, Category = "MazeRunner")
	UInstancedStaticMeshComponent* Gray;
	
	UPROPERTY(VisibleAnywhere, Category = "MazeRunner")
	UInstancedStaticMeshComponent* Path;

	UPROPERTY(VisibleAnywhere, Category = "MazeRunner")
	UInstancedStaticMeshComponent* Obstacle;

	virtual void Tick(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = "MazeRunner")
	void StartMazeRunner(ACharacter* Character);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MazeRunner")
	ACharacter * Instance_MazePawn;

	UPROPERTY(VisibleAnywhere, Category = "MazeRunner")
	bool b_MazeRunnerCalled;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AStar")
	TSubclassOf<ACharacter> MazePawn;

	UFUNCTION(BlueprintCallable, Category="AStar")
	void GetNeighbours(AFieldCPP* InField);

	UFUNCTION(BlueprintCallable, Category = "AStar")
	AFieldCPP* GetField(int x, int y);

	UFUNCTION(BlueprintCallable, Category = "AStar")
	float EstimateDistanceToGoal(AFieldCPP* InField);

	UFUNCTION(BlueprintCallable, Category = "AStar")
	float DistanceBetweenTwoCells(AFieldCPP * InField1, AFieldCPP * InField2);

	UFUNCTION(BlueprintCallable, Category = "AStar")
	void SolveAStar();

	UFUNCTION(BlueprintCallable, Category = "AStar")
	void GenerateMaze();

	// helper function

	UFUNCTION(BlueprintCallable, Category = "AStar")
	void SetStartPoint(FHitResult HitResult);

	UFUNCTION(BlueprintCallable, Category = "AStar")
	void SetEndPoint(FHitResult HitResult);

	UFUNCTION(BlueprintCallable, Category = "AStar")
	void SetObstacle(FHitResult HitResult);
	
	UFUNCTION(BlueprintCallable, Category = "AStar")
	void ResetBoard();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AStar")
	TMap<TEnumAsByte<EFieldType>, FFieldMaterialProperties> FieldPropertyMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AStar")
	int heigth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AStar")
	int width;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	UPROPERTY(VisibleAnywhere, Category = "AStar")
	bool PathFound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AStar")
	AFieldCPP* StartField;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AStar")
	AFieldCPP* EndField;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AStar")
	AFieldCPP* CurrentNode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AStar")
	TArray<AFieldCPP*> Fields;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AStar")
	TArray<AFieldCPP*> OpenList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AStar")
	TArray<AFieldCPP*> ClosedList;


	
};
