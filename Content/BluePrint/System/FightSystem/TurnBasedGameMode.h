
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TurnBasedGameMode.generated.h"

class ATurnBasedCharacter;

UENUM(BlueprintType)
enum class ECombatState : uint8
{
    VE_PlayerTurn   UMETA(DisplayName="PlayerTurn"),
    VE_EnemyTurn    UMETA(DisplayName="EnemyTurn"),
    VE_CombatEnd    UMETA(DisplayName="CombatEnd")
};

UCLASS()
class BEBORNEDINHORROR_API ATurnBasedGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ATurnBasedGameMode();

protected:
    virtual void BeginPlay() override;

    void StartCombat();
    void NextTurn();
    void CheckCombatEnd();
    void EndCombat(bool bPlayerWon);

    // 根据速度属性对角色排序
    void SortTurnOrder();

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    ECombatState CurrentState;

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    TArray<ATurnBasedCharacter*> TurnOrder;

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    int32 CurrentTurnIndex;
};
