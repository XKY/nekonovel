// ===============================================================
//  nekoGame - 2009.7.6
// ===============================================================
// - Purpose : 
// ---------------------------------------------------------------

#ifndef __nekoGame_h__
#define __nekoGame_h__

#include <map>
using namespace std;

#include "nekoCommands.h"
#include "nekoIBlockInstance.h"
#include "nekoVariable.h"
#include "nekoGUIManager.h"
#include "nekoBGImage.h"
#include "nekoBGMusic.h"
#include "nekoVoice.h"
#include "nekoGUISelectionButtons.h"
#include "nekoGUIChatBox.h"
#include "nekoMessageLogger.h"
#include "nekoPublisher.h"
#include "nekoLockSection.h"
#include "nekoButton.h"

namespace neko
{
#	define MAX_SAVE_COUNT 400 // 20 * 20
#	define ONE_PAGE_SAVECOUNT 20
#	define QUICK_SAVE_IDX -1

	struct nekoStack
	{
		nekoAutoPtr<nekoCommands> mCommands;
		list<nekoOneCommand *>::iterator mPlayingPos;

		inline void SetPlayingPos(list<nekoOneCommand *>::iterator iter) { mPlayingPos = iter; }
		inline list<nekoOneCommand *>::iterator &GetPlayingPos() { return mPlayingPos; }

		nekoStack(nekoCommands *cmds) : mCommands(cmds) { cmds->AddRef(); mPlayingPos = mCommands->GetCommandList().begin(); }
		~nekoStack() {}
	};

	struct nekoThread : public nekoStack
	{
		string mName;
		bool mRunning; // ������ ���� ���� ����.
		bool mIsStack; // �����ΰ� �������ΰ�!
		bool mValid;

		enum
		{
			TempValueCount = 8,
		};

		/// Command ó���� ���� �ӽ� ������(��Ƽ �����忡 �������� ����.)
		nekoTimer _mTempTimer[TempValueCount];
		int32 _mTempInteger[TempValueCount];
		float64 _mTempFloat[TempValueCount];
		string _mTempString; // �̰��� ���̺� ���Ͽ� ������� ������, ����!
		nekoAutoPtr<nekoIBlockInstance> _mTempInst; // �� �ɾ�� �ν��Ͻ�. �̰͵� ���̺� ���Ͽ� ������� �ʴ´�.

		nekoThread(nekoCommands *cmds, bool isStack = false) : nekoStack(cmds), mRunning(false), mIsStack(isStack), mValid(true) {}
		~nekoThread() {}
	};

	enum E_GAME_RUN_TYPE
	{
		GAME_RUN_PREVIEW = 0,
		// ������ ��忡���� ���� �Է��� ������� �ʴ´�.

		GAME_RUN_PLAYING,
	};

	enum E_GAME_PROGRESSING_TYPE
	{
		EGPT_NORMAL = 0,
		EGPT_AUTO,
		EGPT_SKIP,
	};

	enum E_PROJECT_FLAG_TYPE
	{
		// ä�ùڽ��� ����� �̹����� ǥ���ϱ�
		EPFT_CHATBOX_IN_THUMBNAIL = 0x00000001,

		// Ŀ���� ä�ùڽ� ������ �Ʒ��� ���̱�
		EPFT_NEWTYPE_CURSOR		  = 0x00000002,
	};

	struct NEKONOVEL_API nekoGameEnvironment
	{
		float32 mSpeechSpeed;
		uint32 mSpeechColor;
		uint32 mSelectionColor;
		uint32 mSpeechShadowColor;
		int32 mSpeechShadowType;

		bool mSystemMenuEnabled;
		E_GAME_PROGRESSING_TYPE mProgressingType;

		int32 mPrintX;
		int32 mPrintY;

		/// ������Ʈ ����
		int32 mWidth;
		int32 mHeight;
		bool mDefFullScreen;
		int32 mDefFXVolume;
		int32 mDefBGMVolume;
		int32 mDefVoiceVolume;

		string mLastPublishFolder;
		int32 mLastPublishVersion;
		bool mCompressBGM;

		/// ���� �긲
		bool mPrintDelay;

		/// �ؽ�Ʈ ������ ����(�⺻�� �ƴѵ�, �⺻���� �°�!)
		int32 mTextWindowAlpha;

		/// ���� ���..
		bool mPhotoMode;

		/// ù ��ũ��Ʈ ����
		string mFirstScriptName;

		/// Ŀ�� �ɼ�
		E_CHATBOX_CURSOR_TYPE mCursorType;
		int32 mCursorAniWidth;
		int32 mCursorAniHeight;

		/// ī�޶� �Ӽ� ���� ���� ����
		/// ���� ��������� ��ü���� ����
		bool mCameraIgnored;

		/// ó�� CG�� ��Ÿ���� ���̵��� �ӵ�
		float32 mCGFadeInSpeed;

		/// CG�� ����� �� ���̵�ƿ� �ӵ�
		float32 mCGFadeOutSpeed;

		/// �ε巯�� ���� �긲
		bool mSmoothPrint;

		/// ���� ���� Ƚ��
		uint32 mGameRuntimes;

		/// �ܺ� ��Ʈ ����Ʈ
		string mExternalFonts;

		/// ��� �ΰ��ӵ�
		float32 mTextSubSpeed;

		E_TEXT_ALIGN_TYPE mTextAlign;

		/// ���� ���� ���� ������
		int32 mSlotPage;

		/// �ִ� ������ ��
		int32 mMaxPage;

		/// ������Ʈ �÷���
		uint32 mProjectFlags;

		/// '�����' ���� ?!
		bool mIW2Use;

		/// ������ ���ð�
		float32 mAutoWaitTime;

		/// �� ���� �÷��� �ð�
		float32 mGamePlayTime;

		nekoGameEnvironment(){ Clear(); }
		~nekoGameEnvironment() {}

		/// Clear
		void Clear();

		inline float32 GetSpeechSpeed() { return max(mSpeechSpeed + mTextSubSpeed, 0); }
	};

	struct NEKONOVEL_API nekoSelection
	{
		string mScriptName;
		string mBookMarkName;

		nekoSelection() {}
		~nekoSelection() {}
	};

	enum E_GAME_SYSTEM_STATE
	{
		EGSS_NONE = 0,
		EGSS_SELECTING, /* ��� ���� */
		EGSS_WAIT,
		EGSS_BUTTON_PRESS_WAIT,
		EGSS_INPUT_WAIT,	/* �ؽ�Ʈ�ڽ� �Է´�� */
		EGSS_UNKNOWN,
	};

	enum E_GAME_FLOW_STATE
	{
		EGFS_NORMAL = 0,
		EGFS_QUICKSKIPPING, // ���� ��ŵ
		EGFS_AUTOTURNING, // �ڵ� �ѱ��
	};

	struct nekoMacro
	{
		string mName;
		list<nekoOneCommand *> mCommandList;

		nekoMacro() {}
		~nekoMacro() {}
	};

#define SAVEDATA_CODENAME	"NEKONOVEL_SAVEDATA"
#define SAVEDATA_VERSION	1129
#define PUBLISHEDFILE_VERSION	1000
	enum
	{
		SaveThumnailWidth = 150,
		SaveThumnailHeight = 113,
	};

	class NEKONOVEL_API nekoSaveData : public nekoIUnknown
	{
	protected:

		string mFileName;
		string mDateString;
		string mLiteDateString;
		string mCaption;
		nekoAutoPtr<nekoHWTexture> mPreview;
		nekoByteBuffer mBuffer;
		time_t mTime;
		int32 mVersion;

	public:

		nekoSaveData();
		~nekoSaveData();

		result Initialize(const char8 *fileName);

		result LoadToGame();
		result SaveToFile(const char8 *fileName);

		inline nekoHWTexture *GetPreview() { return mPreview; }
		inline const char8 *GetDateString() { return mDateString.c_str(); }

		// �ʿ��� ���� ����
		const char8 *GetLiteDateString();

		inline const char8 *GetCaption() { return mCaption.c_str(); }
	};

	struct nekoHotKey
	{
		uchar8 mKeyCode;
		string mFileName;
		string mBookMarkName;

		nekoHotKey(char8 keyCode, const string &fileName, const string &bookMarkName) 
			: mKeyCode(keyCode), mFileName(fileName), mBookMarkName(bookMarkName) {}
		~nekoHotKey() {}
	};
	
	struct nekoVariableTrigger
	{
		string mVarName;
		string mFileName;
		string mBookMarkName;

		nekoVariableTrigger(const string &varName, const string &fileName, const string &bookMarkName) 
			: mVarName(varName), mFileName(fileName), mBookMarkName(bookMarkName) {}
		~nekoVariableTrigger() {}
	};

	// ������
	struct nekoOmake
	{
		string mName;
		string mFileName;

		nekoOmake() {}
		~nekoOmake() {}
	};

	enum E_CUSTOM_EVENT_TYPE
	{
		ECET_LOADED = 0,
		ECET_SAVED,
		ECET_RECOLLECTED,
		ECET_EXIT_SKIPMODE,
		ECET_GOTO_PREV_SLOT,
		ECET_GOTO_NEXT_SLOT,
		ECET_MSGBOX,
		ECET_COUNT,
	};

	// Ŀ���� �̺�Ʈ
	struct nekoCustomEventListener
	{
		string mName;
		string mBookMarkName;

		nekoCustomEventListener() {}
		~nekoCustomEventListener() {}
	};

	enum E_GAME_FLAG_TYPE
	{
		GAME_FLAG_VARTRACE		= 0x00000001,

		// ������ '���' ���ķ� ��ŵ �Ǿ��°� ?
		GAME_FLAG_LAST_SKIPPED	= 0x00000002,
	};

	enum E_MESSAGEBOX_TYPE
	{
		EMT_LOAD		= 0,	// �ش� �ε� �����͸� �ҷ��� ���ΰ�?
		EMT_OVERSAVE,			// �ش� ���̺� �����͸� ��� ������ ?
		EMT_REMOVE,				// �ش� �����͸� ���� ������?
		EMT_QUICKLOAD,			// ���ε带 �� ������ ?
		EMT_QUICKSAVE,			// �����̺긦 �� ������ ?
		EMT_CUSTOM,				// Ŀ����
		EMT_UNKNOWN,
	};

	class NEKONOVEL_API NekoNovel;
	class NEKONOVEL_API nekoGame : public nekoIEventListener
	{
	protected:

		uint32 mGameFlags;

		/// �⺻ ����Ʈ
		E_BGIMAGE_EFFECT_TYPE mBGImageEffect;
		nekoAutoPtr<nekoHWTexture> mBGImageEffectRule;
		string mBGImageEffectRuleName;
		float32 mBGImageEffectSpeed;

		string mName;
		string mMadeBy;
		string mIdentifier;
		string mRunPath;
		NekoNovel *mNekoCore;
		list<nekoThread> mStackList;
		list<nekoThread> mThreadList;
		bool mIsPlaying;
		bool mThreadUpdating;
		E_GAME_RUN_TYPE mRunType;

		bool mSkipAble;

		/// �ν��Ͻ���...
		list<nekoIBlockInstance *> mSkipAbleInstances;
		list<nekoIBlockInstance *> mBlockInstanceList;
		map<string, nekoVariable> mVariables;
		nekoAutoPtr<nekoVoiceInstance> mVoice;
		nekoAutoPtr<nekoBGMusicInstance> mBGMusic;
		list<nekoIBlockInstance *> mTimerList;

		list<nekoCommands *> mLoadedCommands;

		nekoGUIManager mGUIManager;
		nekoAutoPtr<nekoGUIChatBox> mChatBox;
		nekoAutoPtr<nekoGUISelectionButtons> mSelectionButtons;

		// �޼��� �ΰ�
		nekoMessageLogger mMessageLogger;

		// ���� ȯ�� ����
		nekoGameEnvironment mPlayingEnv;

		// ���� ����
		E_GAME_SYSTEM_STATE mGameState;
		E_GAME_FLOW_STATE mFlowState;

		list<nekoSelection *> mSelectionList;

		/// ��ŷ�� �ν��Ͻ�
		nekoIBlockInstance *mPickedInstance;

		/// ������
		bool mJumped;

		nekoAutoPtr<nekoPublisher> mPublisher;
		nekoAutoPtr<nekoSaveData> mSaveDatas[MAX_SAVE_COUNT]; /// ���̺� �����͵�..
		nekoAutoPtr<nekoSaveData> mQuickSave;

		list<nekoHotKey> mHotKeyList; // ��Ű ������
		list<nekoVariableTrigger> mVarTriggerList; // ���� Ʈ���� ������
		list<nekoOmake> mOmakeList;	// ������ ����Ʈ
		map<E_CUSTOM_EVENT_TYPE, nekoCustomEventListener> mCustomEventLtnList; // Ŀ���� �̺�Ʈ ����Ʈ

		inline bool _IsMouseEventReceivable(const nekoIBlockInstance *inst)
		{
			if(IsMsgBoxShown())
				return (inst->GetType() == BLOCK_BUTTON
				&& (static_cast<const nekoButtonInstance *>(inst)->GetButtonType() == EBIT_MSGBOX_OK
				|| static_cast<const nekoButtonInstance *>(inst)->GetButtonType() == EBIT_MSGBOX_CANCEL));

			return ((inst->GetType() >= BLOCK_BUTTON && inst->GetType() <= BLOCK_TRACKBAR) 
				|| (inst->GetType() >= BLOCK_TEXTBOX && inst->GetType() <= BLOCK_MESSAGELOG));
		}

		/// �޼����ڽ� ����
		string mMsgBoxCaption;
		E_MESSAGEBOX_TYPE mMsgBoxType;
		int32 mMsgBoxArgs;

	public:
		
		/// Constructor
		nekoGame(NekoNovel *nekoCore);
		
		/// Destructor
		virtual ~nekoGame();

		inline void AddFlags(uint32 flags) { mGameFlags |= flags; }
		inline void RemoveFlags(uint32 flags) { mGameFlags &= ~flags; }
		inline bool IsTurnOn(uint32 flags) { return ((mGameFlags & flags) > 0); }

		result Initialize();

		nekoIBlockInstance *AddInstance(nekoIBlockInstance *inst);
		nekoIBlockInstance *CreateInstance(E_BLOCK_TYPE blockType, const char8 *name, bool *firstInitialize = 0);
		bool RemoveInstance(nekoIBlockInstance *inst);
		nekoIBlockInstance *GetInstance(const char8 *name);
		void ClearInstances(E_BLOCK_TYPE blockType, nekoIBlockInstance *without = NULL, bool excludesNonZeroAlphaDeleting = false);

		void ClearSkipAbleInstances();

		////////////////////////////////
		////////// ������....
		////////////////////////////////

		/// Load
		/// projectInfoData�� ������ ��Ÿ������ ����.
		result Load(const char8 *projectInfoData = NULL);

		/// Save
		result Save();

		////////////////////////////////
		///////// ���� ������....
		////////////////////////////////

		/// Build it!
		result Build();

		/// Run
		result Run();

		/// Save slot
		result SaveToSlot(int32 slotNo);

		/// Load from slot
		result LoadFromSlot(int32 slotNo);

		/// Remove slot
		result RemoveSlot(int32 slotNo);

		/// Save auto to slot
		result SaveAutoToSlot();

		/// Create Commands
		nekoCommands *CreateCommands(const char8 *name);

		/// Remove Commands
		bool RemoveCommands(nekoCommands *commands);

		/// Get Commands by name
		nekoCommands *GetCommands(const char8 *name);

		/// Change Commands' order.
		bool ChangeCommandsOrder(nekoCommands *commands, uint32 index);

		/// Play commands
		result PlayCommands(const char8 *name, const char8 *bookMarkName = NULL, bool onlyInLoadedScripts = false, bool internalRequest = false);

		/// Get Commands list
		inline list<nekoCommands *> &GetCommandsList() { return mLoadedCommands; }


		
		nekoVariable *AddVariable(const char8 *name);
		bool RemoveVariable(const nekoVariable *var);
		nekoVariable *GetVariable(const char8 *name);
		map<string, nekoVariable> &GetVariableList() { return mVariables; }


		/// Change Game Mode
		inline E_GAME_RUN_TYPE GetGameMode() { return mRunType; }
		void ChangeGameMode(E_GAME_RUN_TYPE runType);

		/// Update
		void Update(float32 frameTime);

		/// Draw
		void Draw();

		void EnableButtonWait(); // ��ư ��� ����
		void OnButtonPressed(bool forMsgBox = false); // ��ư�� ���� �Ǿ���.

		inline void SetGameSystemState(E_GAME_SYSTEM_STATE state) { mGameState = state; }
		inline void SetGameFlowState(E_GAME_FLOW_STATE state) { mFlowState = state; }

		void WriteText(const char8 *prevVoiceName, const char8 *txt, bool append);

		/// GUI ���!
		inline nekoGUIChatBox *GetChatBox() { return mChatBox; }
		inline nekoGUISelectionButtons *GetSelectionButtons() { return mSelectionButtons; }

		/// ��ŵ ����...
		inline void EnableSkipAble() { mSkipAble = true; }
		void DisableSkipAble();
		inline bool IsSkipAble() { return mSkipAble; }

		/// �� ���͹� -_-a!!
		inline nekoVoiceInstance *GetVoice() { return mVoice; }
		inline nekoBGMusicInstance *GetBGMusic() { return mBGMusic; }
		inline nekoIBlockInstance *GetPickedInstance() { return mPickedInstance; }
		inline list<nekoIBlockInstance *> &GetBlockInstanceList() { return mBlockInstanceList; }
		inline void SetName(const char8 *name) { mName = name; }
		inline const char8 *GetName() { return mName.c_str(); }
		inline void SetMadeBy(const char8 *value) { mMadeBy = value; }
		inline const char8 *GetMadeBy() { return mMadeBy.c_str(); }
		inline nekoGameEnvironment &GetEnvironment() { return mPlayingEnv; }
		inline E_GAME_SYSTEM_STATE GetSystemState() { return mGameState; }
		inline E_GAME_FLOW_STATE GetFlowState() { return mFlowState; }

		/// �ش� ��ġ�� �̵��ϱ�
		/// clearAll�� mRunType�� GAME_RUN_PUBLISHED�� ���� ��ȿ��.
		/// mRunType�� GAME_RUN_PREVIEW������ clearAll�� �׻� true �̴�.
		void GotoLine(nekoCommands *commands, uint32 lineNumber, bool clearAll);

		/// �ش� ��ġ���� ����.
		void DoFrom(int32 lineNumber);

		/// �����ϱ�!
		result JumpTo(const char8 *commandsName, const char8 *bookMarkName);

		bool OnEvent(const nekoEvent &event);

		void ClearAllInstances();

		/// ������ ����...
		void AddSelection(const string &fileName, const string &bookMarkName); /// ������ �߰�
		void OnSelectionCompleted(int32 index);
		void ClearSelections();

		/// ������ ����
		result AddThread(nekoThread &thread);
		result RemoveThread(const char8 *name);
		result StartThreads();
		bool IsExistThread(const char8 *name);
		inline uint32 GetThreadCount() { return mThreadList.size(); }

		/// ����Ʈ �߰�
		result SetEffectToBG(E_BGIMAGE_EFFECT_TYPE fadeType, float32 speed, bool fadeIn);
		result SetRuleEffectToBG(const string &ruleFileName, float32 speed, bool fadeIn);
		inline nekoHWTexture *GetBGEffectRule() { return mBGImageEffectRule; }
		bool HasBGEffect();	// ����Ʈ�� �������ΰ�..
		inline E_BGIMAGE_EFFECT_TYPE GetFadeInEffect() { return mBGImageEffect; }
		
		/// �����ϱ�
		result StartPublishing();
		void StopPublishing();

		// ������ ����
		inline bool IsSelecting() { return (!mSelectionList.empty());}

		// �⺻
		inline const char8 *GetRunPath() { return mRunPath.c_str(); }

		// ���̺� & �ε�
		void LoadSaveDatas();
		inline nekoSaveData *GetSaveData(int32 slotNo);

		void DumpTo(nekoByteBuffer &buf); /// ���� ��Ű��
		result LoadFrom(int32 version, nekoByteBuffer buf); // ��������
		
		// �̺�Ʈ ����
		void SetHotKey(uchar8 keyCode, const string &fileName, const string &bookMarkName);
		void SetVariableTrigger(const string &varName, const string &fileName, const string &bookMarkName);
		void SetCustomEventListener(E_CUSTOM_EVENT_TYPE type, const string &fileName, const string &bookMarkName);
		bool RemoveCustomEventListener(E_CUSTOM_EVENT_TYPE type);
		result CallCustomEvent(E_CUSTOM_EVENT_TYPE type);
		void OnVariableChanged(const string &varName, nekoVariable *variable);
		void OnMsgBoxCompleted(bool answer);
		inline bool ShowMsgBox(E_MESSAGEBOX_TYPE msgBoxType, const char8 *msg, int32 args)
		{
			mMsgBoxCaption = msg;
			mMsgBoxType = msgBoxType;
			mMsgBoxArgs = args;

			if(FAILED(CallCustomEvent(ECET_MSGBOX)))
			{
				mMsgBoxType = EMT_UNKNOWN;
				return false;
			}

			return true;
		}
		inline bool IsMsgBoxShown() { return (mMsgBoxType != EMT_UNKNOWN); }
		inline E_MESSAGEBOX_TYPE GetMsgBoxType() { return mMsgBoxType; }

		inline void OnTextInputCompleted()
		{
			if(mGameState != EGSS_INPUT_WAIT)
				return;

			mGameState = EGSS_NONE;
			ClearInstances(BLOCK_TEXTBOX);
		}

		LPD3DXBUFFER TakeSaveScreenshot();

		inline nekoMessageLogger *GetMessageLogger() { return &mMessageLogger; }

		inline void GoPrevSlotPage()
		{
			mPlayingEnv.mSlotPage--;
			if(mPlayingEnv.mSlotPage < 1)
			{
				mPlayingEnv.mSlotPage = 1;
				return;
			}
				
			CallCustomEvent(ECET_GOTO_PREV_SLOT);
		}
		inline void GoNextSlotPage()
		{
			mPlayingEnv.mSlotPage++;
			if(mPlayingEnv.mSlotPage > mPlayingEnv.mMaxPage)
			{
				mPlayingEnv.mSlotPage = mPlayingEnv.mMaxPage;
				return;
			}

			CallCustomEvent(ECET_GOTO_NEXT_SLOT);
		}
		inline void GoSlotPage(int32 slotPage)
		{
			slotPage = min(max(slotPage, 1), mPlayingEnv.mMaxPage);
			if(mPlayingEnv.mSlotPage < slotPage)
			{
				mPlayingEnv.mSlotPage = slotPage;
				CallCustomEvent(ECET_GOTO_NEXT_SLOT);
			}
			else if(mPlayingEnv.mSlotPage > slotPage)
			{
				mPlayingEnv.mSlotPage = slotPage;
				CallCustomEvent(ECET_GOTO_PREV_SLOT);
			}
		}

		// ���
		void PlayVoice(const char8 *name);

		///////////////////////////////
		/// ������
		//////////////////////////////
		void AddOmake(const string &name, const string &fileName);
		bool RemoveOmake(const string &name);
		const char8 *GetOmake(const string &name);

		#define ENVDATA_FILENAME "Config.nkenv"
		#define ENVDATA_CODENAME "NEKONOVEL_ENVIONRMENT"

		/////////////////////////////////////
		/// ȯ�� ���� �޼ҵ�
		/////////////////////////////////////
		result SaveEnvironment();
		result LoadEnvironment();
	

		/// ��Ÿ
		void MakeScreenshot();

		/// ���̽� ����
		inline bool IsVoicePlaying() { return (mVoice && mVoice->IsPlaying()); }


		/// �ؽ�Ʈ�� �Ľ��Ѵ�.(������ ������ ġȯ����..)
		const string &ParseString(const char8 *str);

		/// �̺�Ʈ��
		void OnRunCompleted();
	};
}


#endif // __nekoGame_h__