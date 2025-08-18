
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterStats.h"
#include "TurnBasedCharacter.generated.h"

UCLASS()
class BEBORNEDINHORROR_API ATurnBasedCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ATurnBasedCharacter();

    // 当游戏开始或Actor生成时调用
    virtual void BeginPlay() override;

    // 应用伤害的函数
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    // --- 战斗函数 ---
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void Attack(ATurnBasedCharacter* Target);

    UFUNCTION(BlueprintPure, Category = "Combat")
    bool IsDead() const;

    // --- 属性 ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    int32 CurrentHealth;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    FCharacterStats CharacterStats;

    // 用于从数据表中查找角色属性的ID
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
    FName CharacterID;

    // 角色所属队伍的标识
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
    bool bIsPlayerTeam;

protected:
    // 用于加载角色属性的数据表
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
    class UDataTable* CharacterStatsTable;

    void InitializeStats();
};
