// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ASpawnLocationChecker.h"
#include "Utils/Xml.h"
#include <xstring>
// Sets default values
AASpawnLocationChecker::AASpawnLocationChecker()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AASpawnLocationChecker::BeginPlay()
{
	Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("DebugTest"));

}

// Called every frame
void AASpawnLocationChecker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AASpawnLocationChecker::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AASpawnLocationChecker::GenerateNodes(FVector Location)
{
	NodeArr.Add(Location);

}

void AASpawnLocationChecker::SaveXML()
{
    Xml::XMLDocument* document = new Xml::XMLDocument();
    Xml::XMLElement* root = document->NewElement("Edges");
    document->LinkEndChild(root);
    for (auto key : NodeArr)
    {
        Xml::XMLElement* element = document->NewElement("Location");
        root->LinkEndChild(element);

        element->SetAttribute("X", key.X);
        element->SetAttribute("Y", key.Y);
        element->SetAttribute("Z", key.Z);
    }
    document->SaveFile(TCHAR_TO_UTF8(*SaveXmlPath));
    UE_LOG(LogTemp, Warning, TEXT("SaveXml"));
}

