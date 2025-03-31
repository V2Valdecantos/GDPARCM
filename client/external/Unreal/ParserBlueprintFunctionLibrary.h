// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ParserBlueprintFunctionLibrary.generated.h"

UCLASS()
class GDENG03_FINALS_API UParserBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
private:
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "File IO")
	static bool FileSaveString(FString SaveTextB, FString FileNameB);

	UFUNCTION(BlueprintCallable, Category = "File IO")
	static bool FileLoadString(FString FileNameA, FString& SaveTextA);

	UFUNCTION(BlueprintCallable, Category = "File IO")
	static bool SaveLevel(UWorld* EditorWorld, FString NewLevelData, FString path);

	UFUNCTION(BlueprintCallable, Category = "File IO")
	static TArray<AStaticMeshActor*> LoadLevel(UWorld* EditorWorld, FString LevelData);

	UFUNCTION(BlueprintCallable, Category = "File IO")
	static FString GetProjectDir();

	UFUNCTION(BlueprintPure, Category = "File IO")
	static FVector StringToVector(FString Input);

	UFUNCTION(BlueprintCallable, Category = "File IO")
	static AStaticMeshActor* SpawnStaticMeshActor(UWorld* EditorWorld, FString name, FVector pos, FVector scale, FRotator rot, FString type, bool active);

	UFUNCTION(BlueprintCallable, Category = "File IO")
	static bool SetMeshAsset(AStaticMeshActor* object, FString type);
	
};
