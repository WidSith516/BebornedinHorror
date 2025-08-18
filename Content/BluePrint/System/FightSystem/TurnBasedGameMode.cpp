
#include "TurnBasedGameMode.h"
#include "TurnBasedCharacter.h"
#include "Kismet/GameplayStatics.h"

ATurnBasedGameMode::ATurnBasedGameMode()
{
    CurrentTurnIndex = 0;
    CurrentState = ECombatState::VE_PlayerTurn;
}

void ATurnBasedGameMode::BeginPlay()
{
    Super::BeginPlay();
    StartCombat();
}

void ATurnBasedGameMode::StartCombat()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATurnBasedCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        ATurnBasedCharacter* Character = Cast<ATurnBasedCharacter>(Actor);
        if (Character)
        {
            TurnOrder.Add(Character);
        }
    }

    SortTurnOrder();

    if (TurnOrder.Num() > 0)
    {
        CurrentTurnIndex = 0;
        ATurnBasedCharacter* CurrentChar = TurnOrder[CurrentTurnIndex];
        CurrentState = CurrentChar->bIsPlayerTeam ? ECombatState::VE_PlayerTurn : ECombatState::VE_EnemyTurn;
        UE_LOG(LogTemp, Warning, TEXT("Combat Started! It's %s's turn."), *CurrentChar->CharacterStats.CharacterName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No characters found for combat!"));
    }
}

void ATurnBasedGameMode::SortTurnOrder()
{
    TurnOrder.Sort([](const ATurnBasedCharacter& A, const ATurnBasedCharacter& B) {
        return A.CharacterStats.Speed > B.CharacterStats.Speed;
    });
}

void ATurnBasedGameMode::NextTurn()
{
    if (CurrentState == ECombatState::VE_CombatEnd) return;

    CurrentTurnIndex = (CurrentTurnIndex + 1) % TurnOrder.Num();
    ATurnBasedCharacter* CurrentChar = TurnOrder[CurrentTurnIndex];

    // 跳过已死亡的角色
    while (CurrentChar->IsDead())
    {
        CurrentTurnIndex = (CurrentTurnIndex + 1) % TurnOrder.Num();
        CurrentChar = TurnOrder[CurrentTurnIndex];
    }

    CurrentState = CurrentChar->bIsPlayerTeam ? ECombatState::VE_PlayerTurn : ECombatState::VE_EnemyTurn;
    UE_LOG(LogTemp, Warning, TEXT("Next Turn! It's %s's turn."), *CurrentChar->CharacterStats.CharacterName);

    CheckCombatEnd();
}

void ATurnBasedGameMode::CheckCombatEnd()
{
    int32 PlayerTeamAlive = 0;
    int32 EnemyTeamAlive = 0;

    for (ATurnBasedCharacter* Char : TurnOrder)
    {
        if (!Char->IsDead())
        {
            if (Char->bIsPlayerTeam)
            {
                PlayerTeamAlive++;
            }
            else
            {
                EnemyTeamAlive++;
            }
        }
    }

    if (PlayerTeamAlive == 0)
    {
        EndCombat(false); // 玩家失败
    }
    else if (EnemyTeamAlive == 0)
    {
        EndCombat(true); // 玩家胜利
    }
}

void ATurnBasedGameMode::EndCombat(bool bPlayerWon)
{
    CurrentState = ECombatState::VE_CombatEnd;
    if (bPlayerWon)
    {
        UE_LOG(LogTemp, Warning, TEXT("Combat Over! Player team wins!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Combat Over! Enemy team wins!"));
    }
    // 这里可以添加返回主菜单、显示胜利/失败UI等逻辑
}
