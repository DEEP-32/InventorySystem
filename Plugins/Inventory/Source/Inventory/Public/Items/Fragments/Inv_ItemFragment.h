#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inv_ItemFragment.generated.h"

USTRUCT(BlueprintType)
struct FInv_ItemFragment {
	GENERATED_BODY()

public:
	FGameplayTag GetFragmentTag() const {
		return FragmentTag;
	}

	void SetFragmentTag(const FGameplayTag& NewFragmentTag) {
		this->FragmentTag = FragmentTag;
	}

public:
	FInv_ItemFragment() {}
	FInv_ItemFragment(const FInv_ItemFragment& Other) = default;
	FInv_ItemFragment& operator=(const FInv_ItemFragment& Other) = default;
	FInv_ItemFragment(FInv_ItemFragment&& Other) = default;
	FInv_ItemFragment& operator=(FInv_ItemFragment&& Other) = default;
	virtual ~FInv_ItemFragment() {}

	

private:
	UPROPERTY(EditAnywhere, Category="Inventory|Fragment Type",meta = (Categories = "Fragment"))
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
};


USTRUCT(BlueprintType)
struct FInv_GridFragment : public FInv_ItemFragment {
	FIntPoint GetGridSize() const {
		return GridSize;
	}

	void SetGridSize(const FIntPoint& NewGridSize) {
		this->GridSize = GridSize;
	}

	float GetGridPadding() const {
		return GridPadding;
	}

	void SetGridPadding(float NewGridPadding) {
		this->GridPadding = GridPadding;
	}

	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	FIntPoint GridSize {1,1};

	UPROPERTY(EditAnywhere, Category="Inventory")
	float GridPadding {0.f};
};
