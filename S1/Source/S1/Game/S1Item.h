// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/S1ItemManager.h"
#include "Protocol.pb.h"
#include "S1Item.generated.h"

UCLASS()
class S1_API AS1Item : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AS1Item();
	virtual ~AS1Item();
protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetItemVisual();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemVisualName;

public:
	void SetItemInfo(const Protocol::ItemInfo& Info);
	void SetPosInfo(const Protocol::PosInfo& Info);
	struct FS1ItemStruct GetItemStruct() { return itemStruct; }
	Protocol::ItemInfo* ItemInfo;
protected:
	//class Protocol::ItemInfo* ItemInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		struct FS1ItemStruct itemStruct;

	Protocol::PosInfo* PosInfo;
	UPROPERTY(BlueprintReadOnly)
	int ItemRarality = 0;
public:
	UFUNCTION(BlueprintNativeEvent)
	void SetNiagaraEffect();
	virtual void SetNiagaraEffect_Implementation();

};
