// ------------------------------------------------
// PlayerCharacter.h
// プレーヤーキャラクターを制御するクラス
// 作成者：李朗曦、浅川皓亮
// 作成日時：2024-11-18
// ------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/TimelineComponent.h"
#include "Components/SphereComponent.h"
#include "ActorBase.h"
#include "GravityCharacterMovementComp.h"
#include "Logging/LogMacros.h"
#include "Blueprint/UserWidget.h"
#include "ChangeGravityComponent.h"
#include "PlayerCharacter.generated.h"

// 前方宣言
class UInputMappingContext;
class UInputAction;
class APlayerAttackRange;
class USoundCue;
struct FInputActionValue;

// ログカテゴリーの定義
DECLARE_LOG_CATEGORY_EXTERN(pLogTemplateCharacter, Log, All);

// 重力変更目標地点の構造体
USTRUCT(BlueprintType)
struct FTargetGravitySpotInfo
{
	GENERATED_USTRUCT_BODY()

	// コンストラクタ
	FTargetGravitySpotInfo()
	{
		bIsHit = false;
		SpotLocation = FVector::ZeroVector;
		SpotNormal = FVector::ZeroVector;
	}

	// 当たっているのか
	UPROPERTY(BlueprintReadWrite)
	bool bIsHit;

	// 目標地点
	UPROPERTY(BlueprintReadWrite)
	FVector SpotLocation;

	// 目標法線
	UPROPERTY(BlueprintReadWrite)
	FVector SpotNormal;
};

// プレーヤーの状態を定義するenum
UENUM(BlueprintType)
enum class EPlayerStatus : uint8
{
	// 一般
	Normal = (0x0),
	// 重力変更中
	ChangingDirection = (1 << 0),
	// 攻撃
	Attacking = (1 << 1),
	// 着地
	Grounding = (1 << 2),
	// ダメージ受け
	Damaged = (1 << 3),
	// 死亡
	Died = (1 << 4),
	// 落下中
	Falling = (1 << 5),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FExplosionDelegate, int, Power);

// クラスの宣言
UCLASS()
class ANTIGRAVITY_API APlayerCharacter : public AActorBase
{
	GENERATED_BODY()
public:
	// コンストラクタ
	APlayerCharacter(const FObjectInitializer &ObjectInitializer);
	// デストラクタ
	virtual ~APlayerCharacter();

public:
	// 初期化
	void Init() override {};

	// 更新
	virtual void Update() override;

	// 元の重力方向に戻る
	void BackToNormalGravity();

	// 重力変更関数
	void ChangeGravity();

	// クイック重力変更できるかどうかをチェック
	bool CheckIsAbleToChangeGravityQuick();

	// 重力変更関数
	void ChangeGravityQuick();

	// 重力変更モード変更
	void ActivateSlowMotion();

	// 　重力ゲージを取得用B
	UFUNCTION(BlueprintCallable)
	float GetGravityGage() const { return mGravityGage; }

	// 着地地点の情報
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gravity, meta = (AllowPrivateAccess = "true"))
	struct FTargetGravitySpotInfo GravitySpot;

	// カメラ感度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraOption, meta = (AllowPrivateAccess = "true"))
	float mCameraSensitivity = 1.0f;

	// 爆風を生成関数デリケート
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegate")
	FExplosionDelegate SpawnExplosionDelegate;

	// 状態を取得用
	UFUNCTION(BlueprintCallable)
	EPlayerStatus GetStatus() const { return mPlayerStatus; }

	// 状態を取得用
	UFUNCTION(BlueprintCallable)
	void SetStatus(EPlayerStatus newStatus) { mPlayerStatus = newStatus; }

	// 重力ゲージの割合を取得用
	UFUNCTION(BlueprintCallable)
	float GetGravityPercent() const { return mGravityGage / mMaxGage; }

	// HPを取得用
	UFUNCTION(BlueprintCallable)
	int GetPlayerHP() const { return mHitPoint; }

	// 落下攻撃経過時間を取得用
	UFUNCTION(BlueprintCallable)
	float GetFallingTime() const { return mChangedGravityTimer; }

	// 落下攻撃レベルを取得用
	UFUNCTION(BlueprintCallable)
	int GetPowerLevel() const { return mPowerLevel; }

	// 現在のカメラを取得
	UFUNCTION(BlueprintCallable)
	UCameraComponent *GetActiveCamera() const { return ActiveCamera; }

	// 重力ゲージの最大値を取得
	UFUNCTION(BlueprintCallable)
	float GetMaxGravityGauge() { return mMaxGage; }

	// 重力ゲージの最大値を調整用関数
	UFUNCTION(BlueprintCallable)
	void IncreaseMaxGravityGauge(const float amount)
	{
		mMaxGage += amount;
	}

	// 重力ゲージの値を調整用関数
	UFUNCTION(BlueprintCallable)
	void IncreaseGravityGauge(const float amount)
	{
		mGravityGage += amount;
	}

protected:
	// 移動
	void Move(const FInputActionValue &Value);

	// 重力変更を取り消し
	void CancelGravity();

	// 重力変更決定
	void ApplyGravity();

	// カメラ操作
	void Look(const FInputActionValue &Value);

	// 一時停止
	void Pause();

	// 一時停止
	void Charge();

	// ビギンプレイ
	virtual void BeginPlay();

	// 入力関数をバインド
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	// HPを減らす関数
	virtual void FluctuationHP(float decrease) override;

private:
	// クイック重力変更可能か
	bool bIsAbleToChangeGravityQuick;

	// 敵に命中時の音声
	USoundCue *HitSoundCue;

	// 音声コンポーネント
	UAudioComponent *AudioComponent;

	// プレイヤーのステータス
	UPROPERTY()
	EPlayerStatus mPlayerStatus = EPlayerStatus::Normal;

	// ヒット処理
	UFUNCTION(BlueprintCallable)
	void OnHit(UPrimitiveComponent *HitCom, AActor *OtherActor, UPrimitiveComponent *OtherComp,
			   FVector NormalImpulse, const FHitResult &Hit);

	// ヒット処理
	UFUNCTION(BlueprintCallable)
	void OnAttackHit(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
					 int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	// 重力変更された時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gravity, meta = (AllowPrivateAccess = "true"))
	float mChangedGravityTimer = 0;

	// プレイヤーのパワーレベルを上がるための秒数
	// インデックスはレベル
	// メンバー変数はその時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<float> mPowerLevelTimeThreshold;

	// スローモーションタイムのタイムスケール
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float mSlowMotionTimeScale;

	// デフォルトの操作マッピング
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext *DefaultMappingContext;

	// ジャンプアクション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *JumpAction;

	// 移動アクション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *MoveAction;

	// カメラ操作アクション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *LookAction;

	// カーソルを表示するアクション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *ShowCursor;

	// 重力変更アクション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *ChangeGravityAction;

	// 重力変更アクション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *QuickGravityChangeAction;

	// 重力チャージアクション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *ChargeAction;

	// 重力チャージアクション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *GravityCancelAction;

	// 重力変更確定アクション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *ApplyGravityAction;

	// 一時停止アクション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *CommandAction;

	// 一時停止メニューウィジェット
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SerializeField, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> mPauseMenuWidget;

	// 一時停止メニューウィジェットのインスタンス
	UPROPERTY()
	UUserWidget *mPauseMenuWidgetInstance;

	// 重力変更可能な最大距離
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SerializeField, meta = (AllowPrivateAccess = "true"))
	float mGreatestGravityChangeableDistance;

	// エディタを使っているかどうか(デバッグ用)
	UFUNCTION(BlueprintPure, Category = Debug)
	bool IsWithEditor() const
	{
#if WITH_EDITOR
		return true;
#else
		return false;
#endif
	}

private:
	// カメラスプリングアーム
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent *CameraBoom;

	// 肩越しカメラスプリングアーム
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent *FirstPersonCameraBoom;

	// カメラコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent *FollowCamera;

	// 肩越しカメラコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent *FirstPersonCamera;

	// 現在のカメラ
	UCameraComponent *ActiveCamera;

	// 重力変更コンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ChangeGravityComp, meta = (AllowPrivateAccess = "true"))
	UChangeGravityComponent *mChangeGravityComponent;

	// 攻撃範囲
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HitComp, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent *mAttackComponent;

	// アイテムとインタラクション範囲
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = OverlapComp, meta = (AllowPrivateAccess = "true"))
	USphereComponent *mInteractComponent;

	// 重力を変化できるスローの状態
	bool bNotChangingGravity = true;

	// カメラが切り替え中かどうか
	bool bIsCameraTransitioning = false;

	// 攻撃中かどうか
	bool mIsAttack = false;

	// デフォルトの重力スケール
	float mDefaultGravityScale;

	// 加速度
	float mAcceleration = 1;

	// 最大重力ゲージ
	float mMaxGage;

	// 重力ゲージ
	float mGravityGage;

	// 現在のパワーレベル
	int mPowerLevel = 0;

	// カメラ切り替え用タイムライン
	// UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FTimeline *FirstPersonTimeline;

	// UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FTimeline *ThirdPersonTimeline;

	// カーブ
	UCurveFloat *TimelineCurve;

	// カメラのデフォルト位置
	FTransform FirstPersonCameraOriginalPosition;
	FTransform ThirdPersonCameraOriginalPosition;

	// 肩越しカメラタイムライン更新時に呼ばれる処理
	UFUNCTION()
	void FirstPersonTimelineStep(float _Value);

	// 一般カメラタイムライン更新時に呼ばれる処理
	UFUNCTION()
	void ThirdPersonTimelineStep(float _Value);

	// 肩越しカメラタイムライン終了時に呼ばれる処理
	UFUNCTION()
	void FirstPersonTimelineFinished();

	// 一般カメラタイムライン終了時に呼ばれる処理
	UFUNCTION()
	void ThirdPersonTimelineFinished();

	// 肩越しカメラのローテーションを更新
	FORCEINLINE
	void UpdateFPSCameraRotation();

	// デフォルト重力方向
	FVector mDefaultGravityDirection;

	// ターゲット重力方向
	FVector mTargetGravityDirection;

	// 重力変換されたかどうか
	bool bIsGravityChanged = false;

	// 着地待ち状態中かどうか
	bool bIsWaitingToHitGround = false;

	// レイキャスト当たっていない時の処理
	FORCEINLINE void bNotHit()
	{
		GravitySpot.bIsHit = false;
		mTargetGravityDirection = FVector::Zero();
		GravitySpot.SpotLocation = FVector::Zero();
		GravitySpot.SpotNormal = FVector::Zero();
	}
};
