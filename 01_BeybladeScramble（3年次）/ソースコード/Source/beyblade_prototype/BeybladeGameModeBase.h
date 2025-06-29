// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// BeybladeGameModeBase.h
// ゲームのルールを管理するゲームモードのヘッダ
// 作成日：2025/04/29

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BeybladeGameModeBase.generated.h"

// ゲームの状態を定義する列挙体
UENUM(BlueprintType)
enum class EGameState : uint8
{
	Playing = 0 UMETA(DisplayName = "プレイ中"),
	Paused = 1 UMETA(DisplayName = "一時停止"),
	Finished = 2 UMETA(DisplayName = "終了"),
};

/**
 * ゲームのルールや進行を管理するゲームモードの基底クラス
 */
UCLASS()
class BEYBLADE_PROTOTYPE_API ABeybladeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	// コンストラクタ
	ABeybladeGameModeBase();

	// ゲーム開始時に呼ばれます
	UFUNCTION()
	virtual void BeginPlay() override;

	// 毎フレーム呼ばれます
	UFUNCTION()
	virtual void Tick(float DeltaTime) override;

	// 入力デバイスの接続状態が変化したときに呼ばれます
	UFUNCTION()
	void OnInputDeviceConnectionChange(EInputDeviceConnectionState NewConnectionState, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId);

	// 現在のゲームモード（"CPU"など）の文字列を取得します
	UFUNCTION(BlueprintCallable)
	FString GetModeStr() const
	{
		return ModeStr;
	};

	// 終了ウィジェットを初期化します (Blueprintで実装)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InitFinishWidget(int PlayerIndex);

	// ゲームのポーズ状態を切り替えます
	UFUNCTION(BlueprintCallable)
	void TogglePause(int PlayerIndex);

	// 現在のゲームの状態を取得します
	UFUNCTION(BlueprintCallable)
	EGameState GetGameState() const
	{
		return BeybladeGameState;
	};

	// 停止中のプレイヤーのインデックスを取得します
	UFUNCTION(BlueprintCallable)
	int GetPausedPlayerIndex() const
	{
		return PausedPlayerIndex;
	}

private:
	// ベイブレードのスポーン位置の配列
	TArray<FVector> BeybladeSpawnPos;

	// マルチプレイヤーモードかどうか
	bool IsMultiplayer;

	// スポーンされたベイブレードへのポインタ
	class ABeybladeBase *SpawnedBeyblade;

	// ゲームインスタンスへの参照
	class UBeybladeGameInstance *GameInstance;

	// メインカメラへの参照
	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = true))
	class ACameraActor *MainCamera;

	// カメラの初期位置
	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = true))
	FVector CameraOriginalPos;

	// スポットライトへの参照 (現在は未使用)
	class ASpotLight *SpotLight;

	// 環境マネージャーへの参照
	TObjectPtr<class AEnvironmentManager> EnvironmentManager;

	// モード文字列 ("CPU" または "PVP")
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FString ModeStr;

	// 必殺技UIウィジェットのクラス
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player, HUD and UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> UltraWidgetClass;

	// 終了UIウィジェットのクラス
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player, HUD and UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> FinishWidgetClass;

	// ポーズUIウィジェットのクラス
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player, HUD and UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> PauseWidgetClass;

	// 必殺技UIウィジェットのインスタンス
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player, HUD and UI", meta = (AllowPrivateAccess = true))
	class UUserWidget *UltraWidget;

	// 終了UIウィジェットのインスタンス
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player, HUD and UI", meta = (AllowPrivateAccess = true))
	class UUserWidget *FinishWidget;

	// ポーズUIウィジェットのインスタンス
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player, HUD and UI", meta = (AllowPrivateAccess = true))
	class UUserWidget *PauseWidget;

	// スポーン可能なベイブレードの名前とクラスのマッピング。
	// ブループリントのクラスデフォルトで設定します。
	// 例: Key="seiryu", Value=BP_Beyblade_Seiryu_C (ABeyblade_seiryu のブループリントクラス)
	UPROPERTY(EditDefaultsOnly, Category = "Beyblade Spawning", meta = (AllowPrivateAccess = true))
	TMap<FName, TSubclassOf<class ABeybladeBase>> BeybladeClassMap;

	// 現在のゲームの状態
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GamePlay", meta = (AllowPrivateAccess = true))
	EGameState BeybladeGameState;

	// ポーズをかけたプレイヤーのインデックス
	int PausedPlayerIndex;
};