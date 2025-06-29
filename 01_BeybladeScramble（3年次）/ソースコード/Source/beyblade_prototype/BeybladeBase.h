// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// ABeybladeBase.h
// ベイブレードの基本クラス
// 作成日：2025/03/04

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/Texture2D.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "BeybladeBase.generated.h"

// ベイブレードのステータスを定義する列挙体
UENUM(BlueprintType)
enum class EBeybladeStatus : uint8
{
	Ready = 0 UMETA(DisplayName = "待機"),
	Normal = 1 UMETA(DisplayName = "通常"),
	Stopped = 2 UMETA(DisplayName = "停止"),
	Ultra = 3 UMETA(DisplayName = "必殺"),
};

// ベイブレードのコントローラータイプを定義する列挙体
UENUM(BlueprintType)
enum class EBeybladeControllerType : uint8
{
	None = 0,
	P1 = 1,
	P2 = 2,
	AI = 3,
};

UCLASS()
class BEYBLADE_PROTOTYPE_API ABeybladeBase : public ACharacter
{
	GENERATED_BODY()

	// 関数
public:
	// このキャラクターのプロパティのデフォルト値を設定します
	ABeybladeBase();

protected:
	// ゲーム開始時または生成された時に呼ばれます
	virtual void BeginPlay() override;

	// コントローラーの割り当てを確認します
	void CheckController();

	// ダメージを与えたときに呼び出されます
	virtual void OnDamageDealt();

public:
	// 毎フレーム呼ばれます
	virtual void Tick(float DeltaTime) override;

	// 入力機能をバインドするために呼ばれます
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	// 回転速度を取得します
	UFUNCTION(BlueprintCallable)
	virtual float GetRotationSpeed() const { return RotationSpeed; }

	// ベイブレードの移動速度を取得します
	UFUNCTION(BlueprintCallable)
	virtual float GetMoveSpeed() const { return MoveSpeed; }

	// ベイブレードの現在HPを取得します
	UFUNCTION(BlueprintCallable)
	virtual float GetHP() const { return HP; }

	// 必殺技が既に使用されたかどうかを取得します
	UFUNCTION(BlueprintCallable)
	virtual bool GetIsUltimateUsed() const { return IsUltimateUsed; }

	// ベイブレードの現在のステータスを取得します
	UFUNCTION(BlueprintCallable)
	virtual EBeybladeStatus GetStatus() const { return Status; }

	// 必殺技のタイトルテクスチャを取得します
	UFUNCTION(BlueprintCallable)
	virtual UTexture2D *GetUltimateTitleTexture() const { return UltimateTitle; }

	// このベイブレードのHP割合を取得します
	UFUNCTION(BlueprintCallable)
	float GetHPPercentage() const { return HP / MaxHP; }

	// このベイブレードのブーストゲージの割合を取得します
	UFUNCTION(BlueprintCallable)
	float GetBoostPercentage() const { return Boost / BoostMax; }

	// このベイブレードの名前を取得します
	UFUNCTION(BlueprintCallable)
	FString GetName() const { return BeybladeName; }

	// このベイブレードのコントローラータイプを取得します
	UFUNCTION(BlueprintCallable)
	EBeybladeControllerType GetControllerType() const { return ControllerType; }

	// このベイブレードがシュートされたかどうかを取得します
	UFUNCTION(BlueprintCallable)
	bool IsShot() const { return bIsShot; }

	// 必殺技が使用可能かどうかを返します
	UFUNCTION(BlueprintCallable)
	bool CanUseUltimate() const
	{
		return (!(IsUltimateUsed || Status != EBeybladeStatus::Normal || GetHPPercentage() > 0.5f));
	}

protected:
	// ブースト入力がトリガーされた時の処理です
	void BoostTriggered();

	// 移動入力がトリガーされた時の処理です
	void MoveTriggered(const FInputActionInstance &Instance);

    void Move(const FVector2D &MoveValue);

    // ブースト入力が完了した時の処理です
	void BoostCompleted();

	// シュート入力がトリガーされた時の処理です
	void LaunchTriggered();

	// ポーズ入力がトリガーされた時の処理です
	void PauseTriggered();

	// カメラ操作入力がトリガーされた時の処理です
	void CameraTriggered(const FInputActionInstance &Instance);

	// ダメージを受けた時の処理です
	UFUNCTION()
	void AnyDamage(AActor *DamagedActor, float Damage, const class UDamageType *DamageType, class AController *InstigatedBy, AActor *DamageCauser);

	// 2つのベクトル間の角度を求めます
	UFUNCTION(BlueprintCallable)
	float FindAngleBetweenTwoVectors(FVector VecA, FVector VecB);

	// 予測ラインを更新します
	UFUNCTION(BlueprintCallable)
	void UpdatePrediction();

	// 予測ラインを削除します
	UFUNCTION(BlueprintCallable)
	void DeletePrediction();

	// 敗北したベイブレードをゲームインスタンスから削除します
	UFUNCTION(BlueprintCallable)
	void RemoveLoser();

	// 停止した時に一度だけ呼び出されます
	UFUNCTION(BlueprintCallable)
	void Stop();

	// 必殺技入力がトリガーされた時の処理です
	UFUNCTION(BlueprintCallable)
	void UltraTriggered();

	// コンポーネントがヒットした時の処理です
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);

	// ベイブレードの移動入力を追加します(AI)
	UFUNCTION(BlueprintCallable)
    void AddMoveInput(FVector2D MoveDirection, bool bIsBoosting);

	// ベイブレードをシュートします(AI)
	UFUNCTION(BlueprintCallable)
    void PerformLaunch(FVector AimDirection, float Power);

	// ベイブレードのブーストステータスを設定します(AI)
	UFUNCTION(BlueprintCallable)
	void SetBoostStatus(bool bNewBoostStatus);

protected:
	// 必殺技を発動する際に一度だけ呼ばれます
	virtual void ActiveUltimate();

	// 必殺技が発動中の毎フレーム呼ばれる処理です
	virtual void UltimateTick(float Deltatime);

	// プロパティ
public:
	// このベイブレードのインデックス
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	int32 BeybladeIndex = -1;

protected:
	// Niagaraシステムのクラス
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	class UNiagaraSystem *HitEffectSystemClass;

	// ベイブレードのコントローラータイプ
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay")
	EBeybladeControllerType ControllerType = EBeybladeControllerType::None;

	// シュートされたかどうか
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	bool bIsShot;

	// コントローラーが割り当てられたかどうか
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	bool bIsControllerAssigned;

	// Stopが実行されたかどうか
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	bool bHasStopExecuted = false;

	// ベイブレードの名前
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gameplay")
	FString BeybladeName = "NoName";

	// ゲームインスタンスへの参照
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
	class UBeybladeGameInstance *bbGameInstance;

	// 回転速度
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
	float RotationSpeed = 15000.0f;

	// このベイブレードの移動速度
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
	float MoveSpeed = 15000.0f;

	// このベイブレードの体力
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
	float HP = 100.0f;

	// このベイブレードの現在のステータス
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
	EBeybladeStatus Status;

	// 必殺技のタイトルテクスチャ
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Assets")
	UTexture2D *UltimateTitle;

	// このベイブレードがブースト中かどうか
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	bool IsBoost;

	// 必殺技が使用されたかどうか
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	bool IsUltimateUsed;

	// 必殺技発動までの遅延時間
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	float UltimateCastingDelay = 0.0f;

	// シュート時にベイブレードに加わる力
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	float LaunchPower = 12000.0f;

	// シュート角度の最大値
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	float ShootAngleMax = 1.7f;

	// シュート角度の最小値
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	float ShootAngleMin = -1.7f;

	// ダメージを与える際の係数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float DamageCoefficient = 0.001f;

	// 単一の攻撃で与えることができる最大ダメージ量
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	float MaxDamage = 15.0f;

	// ダメージを受けた後のクールダウン時間
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	float DamageCD = 0.2f;

	// ダメージを受けた後のクールダウンタイマー
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float DamageCDTimer = 0.f;

	// HPの自然減少に関する係数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float DurabilityCoefficient = 1.0f;

	// 予測線の照準速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float AimSpeed = 0.1f;

	// 最大HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float MaxHP;

	// ブーストゲージの残量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float Boost = 100.0f;

	// ブーストゲージの最大値
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float BoostMax = 100.0f;

	// ブースト中の1秒あたりの消費量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float BoostCost = 20.0f;

	// ブーストゲージの回復速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float BoostRegen = 10.0f;

	// デルタタイムを保存するための変数
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	float InternalDeltaTime = 0.0f;

	// 必殺ステータスの残り時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float UltimateTimer;

	// 被ダメージ時の軽減係数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float DamageReduction = 1.0f;

	// シュート前の待機位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector StandbyPos;

	// シュート前の照準方向
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector AimVector;

	// 予測用メッシュ
	UPROPERTY(EditDefaultsOnly, Category = "Assets")
	UStaticMesh *PredictionMesh;

	// 予測用メッシュのマテリアル
	UPROPERTY(EditDefaultsOnly, Category = "Assets")
	UMaterial *PredictionMat;

	// コンポーネント
protected:
	// CapsuleComponentコンポーネント
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent *BeybladeCapsuleComponent;

	// StaticMeshComponentコンポーネント
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent *StaticMeshComponent;

		// StaticMeshComponentコンポーネント
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent *MeshPitchAndRollComponent;

	// NiagaraComponentコンポーネント
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraComponent *NiagaraComponent;

	// SplineComponentコンポーネント
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USplineComponent *SplineComponent;

	// SplineMeshComponentコンポーネントの配列
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class USplineMeshComponent *> SplineMeshComponentArray;

	// TGU_AxisLockerコンポーネント
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTGU_AxisLocker *AxisLocker;

	/** 入力アクション */
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction *BoostInput;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction *CameraInput;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction *LaunchInput;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction *MoveInput;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction *UltraInput;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction *PauseInput;
};