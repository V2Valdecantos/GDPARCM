// Fill out your copyright notice in the Description page of Project Settings.


#include "ParserBlueprintFunctionLibrary.h"
#include "Engine/StaticMeshActor.h"
#include "Editor.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

bool UParserBlueprintFunctionLibrary::FileSaveString(FString SaveTextB, FString FileNameB)
{
	return FFileHelper::SaveStringToFile(SaveTextB, *(FPaths::ProjectDir() + FileNameB));
}

bool UParserBlueprintFunctionLibrary::FileLoadString(FString FileNameA, FString& SaveTextA)
{
	return FFileHelper::LoadFileToString(SaveTextA, *(FPaths::ProjectDir() + FileNameA));
}

FString TypeIdToPrimitiveType(FString type)
{
	FString newType;

	 if (type == ".?AVCube@GDEngine@@")
		 return "Cube";

	 if (type == ".?AVPlane@GDEngine@@")
		 return "Plane";

	 if (type == ".?AVSphere@GDEngine@@")
		return "Sphere";

	 if (type == ".?AVCylinder@GDEngine@@")
		 return "Cylinder";

	 if (type == ".?AVCapsule@GDEngine@@")
		 return "Capsule";

	 return "Cube";
}

FString PrimitiveToTypeID(FString type)
{
	FString newType;

	if (type.Contains("Cube"))
		return ".?AVCube@GDEngine@@";

	if (type.Contains("Plane"))
		return ".?AVPlane@GDEngine@@";

	if (type.Contains("Sphere"))
		return ".?AVSphere@GDEngine@@";

	if (type.Contains("Cylinder"))
		return ".?AVCylinder@GDEngine@@";

	if (type.Contains("Capsule"))
		return ".?AVCapsule@GDEngine@@";

	return "Cube";
}

bool UParserBlueprintFunctionLibrary::SaveLevel(UWorld* EditorWorld, FString NewLevelData, FString path)
{
	TArray<AActor*> Actors = TArray<AActor*>();
	TArray<FString> levelData = TArray<FString>();

	UGameplayStatics::GetAllActorsOfClass(EditorWorld, AActor::StaticClass(), Actors);

	for (AActor* Actor : Actors) 
	{
		if (Actor->GetName().Contains("StaticMesh")) {
			levelData.Add("{");
			levelData.Add("GameObject");
			levelData.Add(Actor->GetName()); //Name
			FString type = "";
			TArray<UStaticMeshComponent*> meshcomponents;
			Actor->GetComponents<UStaticMeshComponent>(meshcomponents);
			for (UStaticMeshComponent* mesh : meshcomponents) 
			{
				type = mesh->GetStaticMesh()->GetName();
			}
			levelData.Add(PrimitiveToTypeID(type)); //Type
			levelData.Add("1"); //isActive
			levelData.Add(FString::SanitizeFloat(Actor->GetTransform().GetLocation().X) + "," + FString::SanitizeFloat(Actor->GetTransform().GetLocation().Y) + "," + FString::SanitizeFloat(Actor->GetTransform().GetLocation().Z) + ","); // Position
			levelData.Add(FString::SanitizeFloat(Actor->GetActorScale().X) + "," + FString::SanitizeFloat(Actor->GetActorScale().Y) + "," + FString::SanitizeFloat(Actor->GetActorScale().Z) + ","); // Scale
			levelData.Add(FString::SanitizeFloat(Actor->GetTransform().GetRotation().X) + "," + FString::SanitizeFloat(Actor->GetTransform().GetRotation().Y) + "," + FString::SanitizeFloat(Actor->GetTransform().GetRotation().Z) + ","); // Rot

			if (Actor->GetRootComponent()->IsSimulatingPhysics()) 
			{
				levelData.Add("PhysicsComponent " + Actor->GetName());
				levelData.Add("2");
			}

			levelData.Add("}");
		}
	}

	FFileHelper::SaveStringArrayToFile(levelData, *(FPaths::ProjectDir() + path));
	
	return true;
}

TArray<AStaticMeshActor*> UParserBlueprintFunctionLibrary::LoadLevel(UWorld* EditorWorld, FString LevelData)
{
	TArray<FString> lines;
	LevelData.ParseIntoArrayLines(lines);

	TArray<AStaticMeshActor*> spawnedObjects;
	AStaticMeshActor* newActor = NULL;

	FString name = "";
	FString type = "";
	bool active = true;
	FVector position = FVector(0);
	FVector scale = FVector(1);
	FRotator rotation = FRotator(0);

	int index = 0;
	for (FString line : lines) {

		if (line.Contains("{")) // {
			index = 0;

		else if (index == 1) // Game Object
		{
			name = "";
			type = "";
			active = true;
			position = FVector(0);
			scale = FVector(1);
			rotation = FRotator(0);
			newActor = NULL;
		}
		else if (index == 2)
		{
			name = EditorWorld->GetName() + EditorWorld->GetMapName() + line;
		}
		else if (index == 3) // Type
		{
			type = TypeIdToPrimitiveType(line);
		}
		else if (index == 4) // Active Status
		{
			active = (line.Contains("1")) ? true : false;
		}
		else if (index == 5) // Position
		{
			position = StringToVector(line);
		}
		else if (index == 6) // Scale
		{
			scale = StringToVector(line);
		}
		else if (index == 7) // Rotation
		{
			FVector vecRotation = StringToVector(line);
			rotation = FRotator(vecRotation.X, vecRotation.Y, vecRotation.Z);
		}
		else if (index == 8) {
			if (line.Contains("}"))
			{
				index = -1;
				newActor = SpawnStaticMeshActor(EditorWorld, name, position, scale, rotation, type, active);
				spawnedObjects.Add(newActor);
			}
			else if (line.Contains("PhysicsComponent"))
			{
				//
			}

		}
		else if (index == 9) {
			// Rigidbody Type
		}
		else if (index == 10)
		{
			if (line.Contains("}")) 
			{
				index = -1;
				newActor = SpawnStaticMeshActor(EditorWorld, name, position, scale, rotation, type, active);
				spawnedObjects.Add(newActor);
			}
		}

		index++;
	}

	return spawnedObjects;
}

FString UParserBlueprintFunctionLibrary::GetProjectDir()
{
	return FPaths::ProjectDir();
}

FVector UParserBlueprintFunctionLibrary::StringToVector(FString Input)
{
	float x, y, z;

	FString temp = "";
	int index = 0;
	for (char i : Input)
	{
		if (i != ',')
			temp += i;
		else if (i == ',')
		{
			if (index == 0)
				x = FCString::Atof(*temp);;
			if (index == 1)
				z = FCString::Atof(*temp);;
			if (index == 2)
				y = FCString::Atof(*temp);;

			index++;
			temp = "";
		}
	}
	FVector vec3 = FVector(x, y, z);

	return vec3;
}

AStaticMeshActor* UParserBlueprintFunctionLibrary::SpawnStaticMeshActor(UWorld* EditorWorld, FString name, FVector pos, FVector scale, FRotator rot, FString type, bool active)
{
	//// Ensure this function is only called in the Editor
	//if (!GEditor || !GEditor->GetEditorWorldContext().World())
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Editor world not found."));
	//	return NULL;
	//}

	//UWorld* EditorWorld = GEditor->GetEditorWorldContext().World();

	if (!EditorWorld)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to get editor world."));
		return NULL;
	}

	FActorSpawnParameters spawnParams;
	spawnParams.Name = FName(name);

	AStaticMeshActor* newActor = EditorWorld->SpawnActor<AStaticMeshActor>(pos, rot, spawnParams);
	newActor->SetActorHiddenInGame(active);
	newActor->SetActorScale3D(scale);
	SetMeshAsset(newActor, type);

	return newActor;
	//return NULL;
}

bool UParserBlueprintFunctionLibrary::SetMeshAsset(AStaticMeshActor* object, FString type)
{
	if (object)
	{
		FString meshAssetPath = "/Engine/BasicShapes/" + type;
		// Load the mesh asset from the provided path
		UStaticMesh* MeshAsset = LoadObject<UStaticMesh>(nullptr, *meshAssetPath);
		if (!MeshAsset)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load mesh at path: %s"), *meshAssetPath);
			return false;
		}

		// Assign the loaded mesh to the actor
		object->GetStaticMeshComponent()->SetStaticMesh(MeshAsset);
		object->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn Cube Actor."));
		return false;
	}
}
