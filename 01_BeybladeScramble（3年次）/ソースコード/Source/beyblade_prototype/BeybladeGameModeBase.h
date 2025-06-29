// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// BeybladeGameModeBase.h
// �Q�[���̃��[�����Ǘ�����Q�[�����[�h�̃w�b�_
// �쐬���F2025/04/29

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BeybladeGameModeBase.generated.h"

// �Q�[���̏�Ԃ��`����񋓑�
UENUM(BlueprintType)
enum class EGameState : uint8
{
	Playing = 0 UMETA(DisplayName = "�v���C��"),
	Paused = 1 UMETA(DisplayName = "�ꎞ��~"),
	Finished = 2 UMETA(DisplayName = "�I��"),
};

/**
 * �Q�[���̃��[����i�s���Ǘ�����Q�[�����[�h�̊��N���X
 */
UCLASS()
class BEYBLADE_PROTOTYPE_API ABeybladeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	// �R���X�g���N�^
	ABeybladeGameModeBase();

	// �Q�[���J�n���ɌĂ΂�܂�
	UFUNCTION()
	virtual void BeginPlay() override;

	// ���t���[���Ă΂�܂�
	UFUNCTION()
	virtual void Tick(float DeltaTime) override;

	// ���̓f�o�C�X�̐ڑ���Ԃ��ω������Ƃ��ɌĂ΂�܂�
	UFUNCTION()
	void OnInputDeviceConnectionChange(EInputDeviceConnectionState NewConnectionState, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId);

	// ���݂̃Q�[�����[�h�i"CPU"�Ȃǁj�̕�������擾���܂�
	UFUNCTION(BlueprintCallable)
	FString GetModeStr() const
	{
		return ModeStr;
	};

	// �I���E�B�W�F�b�g�����������܂� (Blueprint�Ŏ���)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InitFinishWidget(int PlayerIndex);

	// �Q�[���̃|�[�Y��Ԃ�؂�ւ��܂�
	UFUNCTION(BlueprintCallable)
	void TogglePause(int PlayerIndex);

	// ���݂̃Q�[���̏�Ԃ��擾���܂�
	UFUNCTION(BlueprintCallable)
	EGameState GetGameState() const
	{
		return BeybladeGameState;
	};

	// ��~���̃v���C���[�̃C���f�b�N�X���擾���܂�
	UFUNCTION(BlueprintCallable)
	int GetPausedPlayerIndex() const
	{
		return PausedPlayerIndex;
	}

private:
	// �x�C�u���[�h�̃X�|�[���ʒu�̔z��
	TArray<FVector> BeybladeSpawnPos;

	// �}���`�v���C���[���[�h���ǂ���
	bool IsMultiplayer;

	// �X�|�[�����ꂽ�x�C�u���[�h�ւ̃|�C���^
	class ABeybladeBase *SpawnedBeyblade;

	// �Q�[���C���X�^���X�ւ̎Q��
	class UBeybladeGameInstance *GameInstance;

	// ���C���J�����ւ̎Q��
	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = true))
	class ACameraActor *MainCamera;

	// �J�����̏����ʒu
	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = true))
	FVector CameraOriginalPos;

	// �X�|�b�g���C�g�ւ̎Q�� (���݂͖��g�p)
	class ASpotLight *SpotLight;

	// ���}�l�[�W���[�ւ̎Q��
	TObjectPtr<class AEnvironmentManager> EnvironmentManager;

	// ���[�h������ ("CPU" �܂��� "PVP")
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FString ModeStr;

	// �K�E�ZUI�E�B�W�F�b�g�̃N���X
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player, HUD and UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> UltraWidgetClass;

	// �I��UI�E�B�W�F�b�g�̃N���X
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player, HUD and UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> FinishWidgetClass;

	// �|�[�YUI�E�B�W�F�b�g�̃N���X
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player, HUD and UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> PauseWidgetClass;

	// �K�E�ZUI�E�B�W�F�b�g�̃C���X�^���X
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player, HUD and UI", meta = (AllowPrivateAccess = true))
	class UUserWidget *UltraWidget;

	// �I��UI�E�B�W�F�b�g�̃C���X�^���X
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player, HUD and UI", meta = (AllowPrivateAccess = true))
	class UUserWidget *FinishWidget;

	// �|�[�YUI�E�B�W�F�b�g�̃C���X�^���X
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player, HUD and UI", meta = (AllowPrivateAccess = true))
	class UUserWidget *PauseWidget;

	// �X�|�[���\�ȃx�C�u���[�h�̖��O�ƃN���X�̃}�b�s���O�B
	// �u���[�v�����g�̃N���X�f�t�H���g�Őݒ肵�܂��B
	// ��: Key="seiryu", Value=BP_Beyblade_Seiryu_C (ABeyblade_seiryu �̃u���[�v�����g�N���X)
	UPROPERTY(EditDefaultsOnly, Category = "Beyblade Spawning", meta = (AllowPrivateAccess = true))
	TMap<FName, TSubclassOf<class ABeybladeBase>> BeybladeClassMap;

	// ���݂̃Q�[���̏��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GamePlay", meta = (AllowPrivateAccess = true))
	EGameState BeybladeGameState;

	// �|�[�Y���������v���C���[�̃C���f�b�N�X
	int PausedPlayerIndex;
};