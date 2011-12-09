// ===============================================================
//  nekoGUIChatBox - 2009.7.11
// ===============================================================
// - Purpose : 
// ---------------------------------------------------------------

#ifndef __nekoGUIChatBox_h__
#define __nekoGUIChatBox_h__

#include <vector>
#include "nekoHWTexture.h"
#include "nekoIBlockInstance.h"
#include "nekoHWFont.h"

namespace neko
{
	enum E_TEXTBLOCK_TYPE
	{
		ETT_NORMAL = 0,
		ETT_SELECTION
	};

	enum E_CHATBOX_CURSOR_TYPE
	{
		ECCT_TWINKLE = 0,
		ECCT_ANIMATION,
	};

	enum E_TEXT_ALIGN_TYPE
	{
		ETAT_LEFT  = 0,
		ETAT_CENTER,
		ETAT_CENTER2,
		ETAT_RIGHT,
	};

	struct nekoChatBoxFont
	{
		ID3DXFont *mFont;
		string mFaceName;
		int32 mHeight;
		bool mBold;

		nekoChatBoxFont() {}
		~nekoChatBoxFont() {}
	};

	struct nekoChatBoxTextBlock
	{
		E_TEXTBLOCK_TYPE mType;
		nekoChatBoxFont *mFont;
		string mCaption;
		uint32 mColor;
		uint32 mShadowColor;
		char8 mShadowType;
		nekoRectangle mRect; // ������������ ����.
		E_TEXT_ALIGN_TYPE mAlignType;

		/// ��� �������� �ؽ�Ʈ
		string mWaitingText;

		/// ��� �������� �ؽ�Ʈ�� ������ ����
		uint32 mWaitingTextWrittenLen;

		/// ���κ� ũ��
		vector<uint32> mWidthPerLine;

		/// ���κ� ũ�Ⱑ �ʱ�ȭ �Ǿ��°� ?
		bool mWidthesInitialized;

		nekoChatBoxTextBlock()
			: mWaitingTextWrittenLen(0), mAlignType(ETAT_LEFT)
			, mWidthesInitialized(false)
		{
			mWidthPerLine.push_back(0);
		}
		~nekoChatBoxTextBlock()
		{
			
		}

		inline const char8 *GetCurrentText() { return mWaitingText.c_str() + mWaitingTextWrittenLen; }
	};

	class NEKONOVEL_API nekoGUIChatBox : public nekoIBlockInstance
	{
	protected:

		nekoAutoPtr<nekoHWTexture> mBackground;
		nekoAutoPtr<nekoHWTexture> mCursor;

		nekoRectangle mPrintingBoundary;

		list<nekoChatBoxFont *> mFontList;
		list<nekoChatBoxTextBlock *> mTextBlockList;

		int32 mSelectedIndex;
		int32 mMaxHeight;
		nekoPoint mCursorPos;

		/// ���� ���õ� ��Ʈ
		nekoChatBoxFont *mCurrentFont;

		/// Ŀ�� ���� ?
		bool mCursorVisible;

		/// Ŀ�� ������ üũ
		int32 mCursorTickIndex;
		nekoTimer mCursorTick;
		nekoRectangle mCursorSourceRect;

		/// ��縸 ���� ����
		bool mChatVisible;

		/// �� ����
		int32 mLineInterval;

		/// ��� �ε巴�� �긲�� ���İ�
		float32 mSmoothAlpha;

		/// ��� �ε巴�� �긲���� ���������� ���� ���
		int32 mSmoothTextEnd;

		/// �� �۾� ��
		int32 mTextCount;

		/// ��� ��� Ÿ�̸�
		float32 mPrintTimer;

		/// ���� �������� �Ѿ�⸦ ��ٸ��� �ִ°� ?
		bool mWaitingForNextPage;
		nekoTimer mAutoTurningTimer;

		/// ���κ� ���ڿ� ��
		vector<int32> mWidthPerLine;

		/// ��� ������ ������ �ִ��� üũ
		inline bool _IsPrintAbleNow()
		{
			return (_PreCalculateText(false) > 0);
		}

		/// ���� �������� �ѱ��
		void _GoNextPage();

		// ������ ���� ���
		uint32 _PreCalculateText(bool skipping = false, nekoChatBoxTextBlock **outBlock = 0);

		// Center2�� ���� ���
		void _PreCalculateForCenter2(nekoChatBoxTextBlock *block, const char8 *text);

		nekoChatBoxTextBlock *_GetFirstTextBlock();

	public:

		nekoGUIChatBox();
		virtual ~nekoGUIChatBox();

		/// Set Font
		void SetFont(const char8 *faceName, int32 height, int32 bold);

		/// Set Skin
		void SetSkin(const char8 *fileName);

		/// Update
		void Update(float32 frameTime);

		/// Draw
		void Draw();

		/// Refresh the texture
		void RefreshTexture();

		/// Clear all
		void Clear();

		/// Clear the buffer
		void ClearBuffer();

		inline nekoChatBoxFont *GetLastFont() { return (*(--mFontList.end())); }
		nekoChatBoxTextBlock *PushNewBlock();

		/// Write text
		void WriteText(const char8 *sentence, E_TEXTBLOCK_TYPE blockType = ETT_NORMAL);

		/// �ε巯�� �ؽ�Ʈ�� ���� ��.
		inline void OnSmoothTextEnd() { mSmoothTextEnd = mTextCount; mSmoothAlpha = 0.0f; }

		/// On Event
		bool OnEvent(const nekoEvent &event);

		inline void SetCursorVisible(bool value) { mCursorVisible = value; mCursorTickIndex = 1; mCursorTick.Clear(); }

		inline nekoRectangle &GetPrintingBox() { return mPrintingBoundary; }
		inline void SetChatVisible(bool value) { mChatVisible = value; }
		inline void SetLineInterval(int32 pixels) { mLineInterval = pixels; }

		bool IsWaitingTextLeft();
		bool IsSmoothTexting();

		/// ��ŵ�ϱ�
		bool Skip();

		result Dump(int32 version, nekoByteBuffer &outBuf);
		result Load(int32 version, nekoByteBuffer &inBuf);

	};
}


#endif // __nekoGUIChatBox_h__