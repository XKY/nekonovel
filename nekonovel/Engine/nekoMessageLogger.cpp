#include "nekoPrecompiledHeader.h"
#include "nekoMessageLogger.h"
#include "nekoNovel.h"
#include "nekoHWResourceManager.h"
#include "nekoUtility.h"

namespace neko
{
	void nekoMessageLogger::_UpdateBoundingBoxes()
	{
		ID3DXFont *fontHandle = 0;
		if(mFont)
		{
			mFont->Refresh();
			if(!mFont->GetHandle())
				return;

			fontHandle = mFont->GetHandle();
		}
		else
		{
			fontHandle = GetNekoNovel()->GetVideoDriver()->GetDefaultFont();
		}

		if(!fontHandle)
			return;

		// �ٿ�� �ڽ� �� ���
		list<nekoMessageLogBlock>::iterator iter = mBlocks.begin();

		int32 y = 0;
		int32 x = 40;

		RECT calcRect;
		
		// �� ������ ũ�� ���ϱ�
		calcRect.top = 0;
		calcRect.left = 0;
		calcRect.right = 0;
		calcRect.bottom = 0;
		fontHandle->DrawTextA(0, "��", 2, &calcRect, DT_CALCRECT, 0);
		mCharHeight = calcRect.bottom;

		int32 lastPageMargin = 0;
		for(;iter != mBlocks.end();++iter)
		{
			x = (*iter).mLeftMargin;

			calcRect.left = 0;
			calcRect.right = _GetRealViewportWidth(x);
			calcRect.bottom = 0;
			fontHandle->DrawTextA(0, (*iter).mText.c_str(), -1, &calcRect, DT_WORDBREAK|DT_CALCRECT, 0);

			/*
			// ������ ������ ���� �߰�!
			if((*iter).mText[(*iter).mText.length() - 1] == '\n')
				calcRect.bottom += mCharHeight;
			*/

			(*iter).mBoundingBox.left = 40 + x;
			(*iter).mBoundingBox.top = y;
			(*iter).mBoundingBox.SetSize(_GetRealViewportWidth(x), calcRect.bottom);

			lastPageMargin = (*iter).mPageMargin;
			y += lastPageMargin + calcRect.bottom;
		}

		y -= lastPageMargin;


		mTotalHeight = y;

		mUpdateNeeded = false;
	}

	nekoMessageLogger::nekoMessageLogger()
		: mSize(800, 600)
		, mUpdateNeeded(false)
	{
		Clear();
	}

	nekoMessageLogger::~nekoMessageLogger()
	{

	}

	void nekoMessageLogger::Clear()
	{
		mLeftMargin = 100;
		mNormalLeftMarign = 0;
		mRightMargin = 5;
		mPageMargin = 10;
		mEmptyPage = 0;
		mDefaultColor = 0xffffffff;
		mColor = 0;
		mTotalHeight = 0;
		mFont = 0;
		mBoldFont = 0;
		mCharHeight = NEKO_DEFFONT_HEIGHT;

		ClearBuffer();
	}

	void nekoMessageLogger::ClearBuffer()
	{
		mTotalHeight = 0;
		mBlocks.clear();
	}

	void nekoMessageLogger::WriteText(const char8 *nameLabel, const char8 *voiceName, const char8 *text, bool append)
	{
		if(append && !mBlocks.empty())
		{
			// ������ �༮ ��������
			(*(--mBlocks.end())).mText += text;
		}
		else
		{
			string newStr;
			if(nameLabel && nameLabel[0])
			{
				for(int32 i = 0;i < mEmptyPage;++i)
					newStr += '\n';
			}
			newStr += text;

			mBlocks.push_back(nekoMessageLogBlock(
				(nameLabel && nameLabel[0]) ? nameLabel : ""
				, (voiceName) ? voiceName : ""
				, newStr.c_str()
				, (nameLabel && nameLabel[0]) ? mLeftMargin : mNormalLeftMarign
				, mPageMargin
				, (mColor) ? mColor : mDefaultColor
				)
				);

			if(mTotalHeight)
				mTotalHeight += mPageMargin;
		}

		ID3DXFont *fontHandle =	 0;
		if(mFont)
		{
			mFont->Refresh(true);
			mBoldFont->Refresh(true);

			fontHandle = mFont->GetHandle();
		}
		else
		{
			fontHandle = GetNekoNovel()->GetVideoDriver()->GetDefaultFont();
		}

		if(!fontHandle)
			return;

		nekoMessageLogBlock &block = (*(--mBlocks.end()));

		int32 x = block.mLeftMargin;

		int32 oldHeight = block.mBoundingBox.GetHeight();

		RECT calcRect;
		calcRect.top = 0;
		calcRect.left = 0;
		calcRect.right = _GetRealViewportWidth(x);
		calcRect.bottom = 0;
		fontHandle->DrawTextA(0, block.mText.c_str(), -1, &calcRect, DT_WORDBREAK|DT_CALCRECT, 0);

		/*
		// ������ ������ ���� �߰�!
		if(block.mText[block.mText.length() - 1] == '\n')
			calcRect.bottom += mCharHeight;
		*/

		block.mBoundingBox.left = 40 + x;
		block.mBoundingBox.top = mTotalHeight - oldHeight;
		block.mBoundingBox.SetSize(_GetRealViewportWidth(x), calcRect.bottom);

		mTotalHeight += calcRect.bottom - oldHeight;
	}

	void nekoMessageLogger::Initialize(int32 width, int32 height, int32 rightMargin, const char8 *fontName, int32 fontSize)
	{
		if(mSize.x != width || mSize.y != height || !mFont || (stricmp(mFont->GetName(), fontName) != 0)
			|| mFont->GetHeight() != fontSize || mRightMargin != rightMargin)
		{
			mSize.x = width;
			mSize.y = height;
			mRightMargin = rightMargin;
			mFont = GetNekoNovel()->GetHWResourceManager()->LoadFont(fontName, 0, fontSize, false);
			mBoldFont = GetNekoNovel()->GetHWResourceManager()->LoadFont(fontName, 0, fontSize, true);

			mUpdateNeeded = true;
			_UpdateBoundingBoxes();
		}
	}

	void nekoMessageLogger::Update(float32 frameTime)
	{
		if(mUpdateNeeded)
			_UpdateBoundingBoxes();
	}

	void nekoMessageLogger::Load(int32 version, nekoByteBuffer &buf)
	{
		if(version < 1120)
			return;

		Clear();

		// �̴� ���۷� ��������
		nekoByteBuffer miniBuf;
		int32 size = buf.Read<int32>();
		miniBuf.Append((uchar8 *)buf.GetReadPositionBuffer(), size);
		buf.Read(0, size);

		uint32 blockSize;
		string str;
		int32 temp;

		miniBuf >> blockSize;
		miniBuf >> str;
		miniBuf >> temp;

		if(!str.empty())
		{
			mFont = GetNekoNovel()->GetHWResourceManager()->LoadFont(str.c_str(), 0, temp, false);
			mBoldFont = GetNekoNovel()->GetHWResourceManager()->LoadFont(str.c_str(), 0, temp, true);
		}

		miniBuf >> mLeftMargin
			>> mPageMargin
			>> mEmptyPage
			>> mDefaultColor
			>> mColor
			>> mSize;

		if(version >= 1122)
		{
			miniBuf >> mRightMargin;
		}

		if(version >= 1123)
		{
			miniBuf >> mNormalLeftMarign;
		}

		int32 leftMargin;
		int32 pageMargin;
		string str2, str3;
		uint32 color;

		for(uint32 i = 0;i < blockSize;++i)
		{
			miniBuf >> color
				>> leftMargin
				>> pageMargin
				>> str
				>> str2
				>> str3;

			mBlocks.push_back(nekoMessageLogBlock(str.c_str(), str2.c_str(), str3.c_str(), leftMargin, pageMargin, color));
		}

		mUpdateNeeded = true;
	}

	void nekoMessageLogger::Dump(int32 version, nekoByteBuffer &buf)
	{
		nekoByteBuffer miniBuf;

		miniBuf << (uint32)mBlocks.size();
		miniBuf << ((mFont) ? mFont->GetName() : "");
		miniBuf << ((mFont) ? mFont->GetHeight() : 0);
		miniBuf << mLeftMargin
			<< mPageMargin
			<< mEmptyPage
			<< mDefaultColor
			<< mColor
			<< mSize
			<< mRightMargin
			<< mNormalLeftMarign;

		list<nekoMessageLogBlock>::iterator iter = mBlocks.begin();
		for(;iter != mBlocks.end();++iter)
		{
			miniBuf << (*iter).mColor
				<< (*iter).mLeftMargin
				<< (*iter).mPageMargin
				<< (*iter).mName
				<< (*iter).mVoiceName
				<< (*iter).mText;
		}

		buf << miniBuf.GetLength();
		buf.Append(miniBuf.GetBuffer(), miniBuf.GetLength());
	}
}