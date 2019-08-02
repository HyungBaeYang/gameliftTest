// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include <Engine/GameInstance.h>
#include "Actor/CharacterPC.h"
#include "Packet/PacketConsts.h"
//#include "SocketIONative.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Engine.h"
#include "EngineGlobals.h"

#if WITH_GAMELIFT
#include "GameLiftServerSDK.h"
#endif
#include "NetworkReplayStreaming.h"
#include "GP2GameInstance.generated.h"




class AGameMode;
class UUIManager;

/*
UENUM(BlueprintType)
enum EWebResultEnum
{
	FAIL,
	SUCCESS,
	INVALID_VERSION,
	WRONG_PASSWORD,
	NOT_FOUND_USER,
	ALREADY_IN_CONNECT,
	AUTH_TOKEN_INVALID,
	GAMELIFT_NO_AVAILABLE_PROCESS,
	GAMELIFT_GAMESESSION_FULL,
	GAMELIFT_GAMESESSION_INVALID_STATUS,
	GAMELIFT_GAMESESSION_ACTIVATING_RETRY,
	GAMELIFT_GAMESESSION_TERMINATED,
	GAMELIFT_PLAYERSESSION_ALREADY_CONNECTED,
	GAMELIFT_PLAYERSESSION_DISCONNECT,
	GAMELIFT_PLAYERSESSION_TIMEOUT,
	GAMELIFT_ETC_ERROR,
	DB_ERROR = 500
};
*/
struct FConnectInfo
{

	int     UserSN;          ///
	FString PlayerSessionID; ///

	FConnectInfo() : UserSN (0)
	{
	}

	FConnectInfo(FConnectInfo && connectInfo) : UserSN(0)
	{
	UserSN = connectInfo.UserSN;
	PlayerSessionID = connectInfo.PlayerSessionID;
	}

	FConnectInfo(int UserSN , const FString &PlayerSessionID)
	{
	this->UserSN = UserSN;
	this->PlayerSessionID = PlayerSessionID;
	}

	FConnectInfo & operator=(const FConnectInfo &Other)
	{
	this->UserSN = Other.UserSN;
	this->PlayerSessionID = Other.PlayerSessionID;

	return  *this;           
	}
};

UENUM(BlueprintType)
enum EGAME_MODE
{
	PVP,
	PVE,
	TUTORIAL
};

USTRUCT(BlueprintType)
struct FGP2_ReplayInfo
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadOnly)
		FString ReplayName;

	UPROPERTY(BlueprintReadOnly)
		FString FriendlyName;

	UPROPERTY(BlueprintReadOnly)
		FString ReplaySize;

	UPROPERTY(BlueprintReadOnly)
		FString ReplayCharacterName;

	UPROPERTY(BlueprintReadOnly)
		FString ReplayPlayedMap;

	UPROPERTY(BlueprintReadOnly)
		FDateTime Timestamp;

	UPROPERTY(BlueprintReadOnly)
		int32 LengthInMS;

	UPROPERTY(BlueprintReadOnly)
		bool bIsValid;

	FGP2_ReplayInfo(FString NewName, FString NewFriendlyName, FString Size , FString NewCharacterName, FString NewPlayedMap,  FDateTime NewTimestamp, int32 NewLengthInMS)
	{
		ReplayName = NewName;
		FriendlyName = NewFriendlyName;
		ReplaySize = Size;
		ReplayCharacterName = NewCharacterName;
		ReplayPlayedMap = NewPlayedMap;
		Timestamp = NewTimestamp;
		LengthInMS = NewLengthInMS;
		bIsValid = true;
	}

	FGP2_ReplayInfo()
	{
		ReplayName = "Replay";
		FriendlyName = "Replay";
		Timestamp = FDateTime::MinValue();
		LengthInMS = 0;
		bIsValid = false;
	}
};

//DECLARE_DYNAMIC_DELEGATE_OneParam(FCharacterLoadCompleted, USkeletalMesh*, Mesh);

UCLASS( Blueprintable , BlueprintType )
class GP2PROJECT_API UGP2GameInstance
	:
	public UGameInstance
{
	GENERATED_BODY()

private:
//	TSharedPtr<FSocketIONative> NativeClient;
	EGAME_MODE        m_eGameMode;        
	bool              m_bGameStart;       
	bool              m_bLocal;           
	bool              m_bLobby;           
	int8              m_byEnterUserCount; 
	int32             m_iRoomNumber;      
	FString           m_Subject;          
	FString           m_GameSessionID;    
	FString           m_ServerSystemInfo; 
	FString           m_WebServerIP;      
	FString           m_Map;              

	TArray< FString > ServerIPList;       


#if WITH_GAMELIFT
	       TSharedPtr <FGameLiftServerSDKModule> gameLiftSdkModule;            

	       int                                   m_port;                       
	       FTimerHandle                          m_timer;                      
	       int8                                  m_EmptyGameSessionCheckCount; 
	       bool                                  m_bCheckEmptyGameSession;     

	       TMap<int32, FConnectInfo>             m_ConnectUserList;            
	static int8                                  byInitGameLiftCount;          

	       bool                                  m_bActiveGameLiftSession;     
	       bool                                  m_bSettingMap;                

public:

	void EmptyGameSessionCheckTick();
#endif

	UFUNCTION(BlueprintCallable)
	void GameLiftStartProcess();

	void InitGameData();

	virtual void OnStart();

//<--  test server function
	void WebReq_ServerReg();
	void WebRes_ServerReg(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void WebReq_ServerReuse();
	void WebRes_ServerReuse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void WebReq_RoomEnterUser();
	void WebRes_RoomEnterUser(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void WebReq_RoomOutUser();
	void WebRes_RoomOutUser(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void WebReq_GameStart();
	void WebRes_GameStart(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
//-->
	void WebReq_GameEnd();
	void WebRes_GameEnd(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

public:

	UFUNCTION(BlueprintCallable)
	void LoginSessionCheck(const int &PlayerID, const FString &PlayerSessionID, const int &UserSN, bool &CheckResult, int &errorType);

	UFUNCTION(BlueprintCallable)
	void LogoutProcess(const int &PlayerID);

	UFUNCTION( BlueprintPure )
	bool IsWithEditor();

	UFUNCTION( BlueprintCallable , meta = ( HidePin = "WorldContextObject" , DefaultToSelf = "WorldContextObject" ) )
	static EWebResultEnum ToWebResultEnum(UObject* WorldContextObject, float fValue);

	UFUNCTION(BlueprintCallable)
	bool getGameStart();

	UFUNCTION(BlueprintPure)
	EGAME_MODE getGameMode();

	UFUNCTION(BlueprintPure)
	int32 getRoomNumber();

	UFUNCTION(BlueprintPure)
	const FString & getRoomSubject();

	UFUNCTION(BlueprintPure)
	const FString & getGameSession();

	UFUNCTION(BlueprintPure)
	const FString & getServerSystemInfo();

	UFUNCTION(BlueprintPure)
	const FString & getWebURL();

	UFUNCTION(BlueprintPure)
	int32 getCurrnetTickCount();

	UFUNCTION(BlueprintImplementableEvent)
	void LevelEnterFail(const FString &FailMessage);

	void HandleNetworkFailure(UWorld *World, UNetDriver *NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	void HandleNetworkError(UWorld * World, UNetDriver * NetDriver, ENetworkFailure::Type FailureType, const FString & ErrorString);

	int8 getEnterUserCount();
	bool getLocal();
	bool getGameLiftActive();


public:
	UFUNCTION( BlueprintPure , Category = "DefaultGame" )
	const bool GetAutoLogin();

	UFUNCTION( BlueprintPure , Category = "DefaultGame" )
	const FString& GetAutoLoginId();

	UFUNCTION( BlueprintPure , Category = "DefaultGame" )
	const bool GetLocalMode();

	UFUNCTION( BlueprintPure , Category = "DefaultGame" )
	const int GetDefaultCharacter();

	UFUNCTION( BlueprintPure , Category = "DefaultGame" )
	const bool GetSkipTutorial();

	UFUNCTION( BlueprintPure , Category = "DefaultGame" )
	const bool GetSkipAdvent();

	UFUNCTION( BlueprintPure , Category = "DefaultGame" )
	const bool GetShowDebugArrow();


public:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly , Category = "UI" )
	UUIManager* UIManager;

public:
	/// ������
	UGP2GameInstance();

	/// �Ҹ���
	virtual ~UGP2GameInstance();


	///-------------------------------------------------------------------------------------------------
	/// UGameInstance's virtual functions
	///-------------------------------------------------------------------------------------------------
	virtual void Init() override;

	virtual void Shutdown() override;

	///-------------------------------------------------------------------------------------------------
	/// 
	///-------------------------------------------------------------------------------------------------
	void Clear() {}
	void NetxWorld() {}
	void MoveToTitle() {}
	void MoveToSelectCharacter() {}


	///-------------------------------------------------------------------------------------------------
	/// getter & setter
	///-------------------------------------------------------------------------------------------------
	APlayerController* GetPlayerController() const;

	/// PC �� ��ȯ�Ѵ�.
	ACharacterPC* GetPlayerCharacterPC();

	/// ĳ���͸� ��ȯ�Ѵ�.
	ACharacter* GetPlayerCharacter();

	/// GameMode�� ��ȯ�Ѵ�.
	AGameMode* GetGP2GameMode();

	/// UI �Ŵ����� ��ȯ�Ѵ�.
	UUIManager* GetUIManager() const { return UIManager; }
public:

	// �񵿱� �ε带 ����ϱ� ���� ���·δ�
	//UPROPERTY()
	FStreamableManager AssetLoader;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FStringAssetReference> CharacterAssets;

	//// ĳ���� �ε����� �ɸ��� �񵿱� �ε�.
	//UFUNCTION()
	//void AsyncCharacterLoad(int CharacterIndex, FCharacterLoadCompleted CompletedFunc);


	/**
	* Replay can save infinitly if number is zero.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Replays")
		int32 NumberOfSaveableReplay = 0;

	////// Replay //////

public:

	/** BP���� ����� �����մϴ�.ReplayName = ��ũ���� ���� �̸�, FriendlyName = UI������ ��� �̸� */
	UFUNCTION(BlueprintCallable, DisplayName = "StartRecordingReplay" ,Category = "Replays")
		void StartRecordingReplayFromBP(FString ReplayName, FString FriendlyName);

	/** BP���� �������� ���÷��̸� ��ȭ�ϰ� ����. */
	UFUNCTION(BlueprintCallable, DisplayName = "StopRecordingReplay", Category = "Replays")
		void StopRecordingReplayFromBP();

	/** BP���� ������ ���� �� Replay�� ���� ��� ���� */
	UFUNCTION(BlueprintCallable, DisplayName = "PlayReplay", Category = "Replays")
		void PlayReplayFromBP(FString ReplayName);

	/** �ϵ� ����̺꿡�� ���÷��� �˻� / ã�� ���� */
	UFUNCTION(BlueprintCallable, Category = "Replays")
		void FindReplays();

	/** �� �̸��� ����� �����մϴ�(UI �� �ش�) */
	UFUNCTION(BlueprintCallable, Category = "Replays")
		void RenameReplay(const FString & ReplayName, const FString & NewFriendlyReplayName);

	/** ������ ����� ��� ���� */
	UFUNCTION(BlueprintCallable, Category = "Replays")
		void DeleteReplay(const FString & ReplayName);

	UFUNCTION(BlueprintCallable, Category = "Replays")
		void SaveGameInfo(const FString & ReplayName, const FString& PlayerCharacterName, const FString& PlayedMap);

	UFUNCTION(BlueprintPure, Category = "Replays")
		bool IsReplayPlaying();

	UFUNCTION(BlueprintPure, Category = "Replays")
		bool IsRecording();

	UFUNCTION(BlueprintPure, Category = "GamePad")
		static bool IsGamePadconnected();

	FOnGotoTimeDelegate GoToTimeCompleteDelegate;
	
protected:

	/** ���÷��� �˻��� �Ϸ�Ǹ� ȣ��� */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "FindReplayComplete", Category = "Replays")
		void BP_OnFindReplaysComplete(const TArray <FGP2_ReplayInfo> & AllReplays);


	UFUNCTION(BlueprintImplementableEvent, DisplayName = "GotoTimeFinished", Category = "Replays")
		void BP_OnGotoTimeFinished(const bool Finished);

private:

	TArray<FString> FindingGameSaveInfo;

	void FindReplayGameInfo(const FString & ReplayName);

	// ���÷��̸� ��û�ϴ� Streamer
	TSharedPtr<INetworkReplayStreamer> EnumerateStreamsPtr;

	FOnEnumerateStreamsComplete OnEnumerateStreamsCompleteDelegate;

	//FindReplays() �� ������ִ� delegate
	void OnEnumerateStreamsComplete(const TArray <FNetworkReplayStreamInfo> & StreamInfos);

	FOnDeleteFinishedStreamComplete OnDeleteFinishedStreamCompleteDelegate;

	//DeleteReplay �� ������ִ� delegate
	void OnDeleteFinishedStreamComplete(const bool bDeleteSucceeded);


	//GoToTimeCompleteDelegate �� ����Ǿ��ִ� �Լ�
	void OnGotoTimeFinished(const bool FinishRoad);


public:
	UFUNCTION( BlueprintCallable , Category = "Json Security" )
	const bool GetJsonSecurity( int InIndex );

public:
	UFUNCTION( BlueprintCallable , Category = "Server" )
	FString GetServerAddress( int InIndex );
};

