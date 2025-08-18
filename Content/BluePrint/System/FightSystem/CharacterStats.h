
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterStats.generated.h"

// USTRUCT宏使其能被UE的反射系统识别
USTRUCT(BlueprintType)
struct FCharacterStats : public FTableRowBase
{
    GENERATED_BODY()

public:
    // UPROPERTY宏使其可以在蓝图和编辑器中访问
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    FString CharacterName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    int32 MaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    int32 AttackPower;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    int32 Defense;
    
    // 速度可以用来决定行动顺序
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    int32 Speed;
};
