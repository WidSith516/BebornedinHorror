
#include "TurnBasedCharacter.h"
#include "Components/CapsuleComponent.h"

ATurnBasedCharacter::ATurnBasedCharacter()
{
    // 关闭Tick，因为在回合制游戏中角色通常不需要每帧更新逻辑
    PrimaryActorTick.bCanEverTick = false;
    bIsPlayerTeam = true;
}

void ATurnBasedCharacter::BeginPlay()
{
    Super::BeginPlay();
    InitializeStats();
}

void ATurnBasedCharacter::InitializeStats()
{
    if (CharacterStatsTable)
    {
        FCharacterStats* Row = CharacterStatsTable->FindRow<FCharacterStats>(CharacterID, TEXT(""));
        if (Row)
        {
            CharacterStats = *Row;
            CurrentHealth = CharacterStats.MaxHealth;
        }
    }
}

float ATurnBasedCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    CurrentHealth -= ActualDamage;
    CurrentHealth = FMath::Max(0, CurrentHealth); // 确保生命值不为负

    UE_LOG(LogTemp, Warning, TEXT("%s takes %f damage, remaining health: %d"), *CharacterStats.CharacterName, ActualDamage, CurrentHealth);

    if (IsDead())
    {
        UE_LOG(LogTemp, Warning, TEXT("%s has been defeated."), *CharacterStats.CharacterName);
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        // 这里可以添加死亡动画、从战场移除等逻辑
    }

    return ActualDamage;
}

void ATurnBasedCharacter::Attack(ATurnBasedCharacter* Target)
{
    if (!Target || Target->IsDead())
    {
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("%s attacks %s!"), *CharacterStats.CharacterName, *Target->CharacterStats.CharacterName);
    
    // 简单的伤害计算
    float DamageToDeal = FMath::Max(1.0f, (float)CharacterStats.AttackPower - Target->CharacterStats.Defense);
    
    FDamageEvent DamageEvent;
    Target->TakeDamage(DamageToDeal, DamageEvent, GetController(), this);
}

bool ATurnBasedCharacter::IsDead() const
{
    return CurrentHealth <= 0;
}
