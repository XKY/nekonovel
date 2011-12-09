#include "nekoPrecompiledHeader.h"
#include "nekoGUIChatBox.h"
#include "NekoNovel.h"
#include "nekoHWResourceManager.h"
#include "nekoUtility.h"

namespace neko
{
	nekoChatBoxTextBlock *nekoGUIChatBox::_GetFirstTextBlock()
	{
		list<nekoChatBoxTextBlock *>::iterator itBlock = mTextBlockList.begin();
		for(;itBlock != mTextBlockList.end();++itBlock)
		{
			if((*itBlock)->mWaitingText.size() > (*itBlock)->mWaitingTextWrittenLen)
				return (*itBlock);
		}

		return 0;
	}

	nekoGUIChatBox::nekoGUIChatBox()
		: mCurrentFont(NULL)
		, mCursorVisible(false)
		, mCursorTickIndex(1)
		, mMaxHeight(0)
		, mSelectedIndex(-1)
		, nekoIBlockInstance(NULL)
		, mChatVisible(true)
		, mLineInterval(0)
		, mSmoothAlpha(0.0f)
		, mTextCount(0)
		, mSmoothTextEnd(-1)
		, mPrintTimer(0)
		, mAutoTurningTimer(2.0f)
	{
		mZOrder = 50;
		mBoundingBox = nekoRectangle(25, 420, 775, 570);
		mPrintingBoundary = nekoRectangle(25 + 10, 420 + 10, 775 - 10, 570 - 10);
		mBackground = GetNekoNovel()->GetHWResourceManager()->LoadTexture("�̹���\\���â��Ų.png");

		mCursor = GetNekoNovel()->GetHWResourceManager()->LoadTexture("�̹���\\Ŀ��.png");
		mCursorTick.SetTime(0.5f);

		SetFont("����ü", 24, true);
	}

	nekoGUIChatBox::~nekoGUIChatBox()
	{
		list<nekoChatBoxFont *>::iterator iter = mFontList.begin();
		for(;iter != mFontList.end();++iter)
		{
			(*iter)->mFont->Release();
			delete (*iter);
		}

		list<nekoChatBoxTextBlock *>::iterator itBlock = mTextBlockList.begin();
		for(;itBlock != mTextBlockList.end();++itBlock)
		{
			delete (*itBlock);
		}
	}

	/// Set Font
	void nekoGUIChatBox::SetFont(const char8 *faceName, int32 height, int32 bold)
	{
		if(!faceName)
			faceName = mCurrentFont->mFaceName.c_str();

		if(height < 0)
			height = mCurrentFont->mHeight;

		if(bold < 0)
			bold = (bool)mCurrentFont->mBold;

		// �ֱ��� ���� ������ ��Ʈ�̸� �׳� ����..
		if(mFontList.empty() == false)
		{
			list<nekoChatBoxFont *>::reverse_iterator iter = mFontList.rbegin();
			if(stricmp((*iter)->mFaceName.c_str(), faceName) == 0 && height == (*iter)->mHeight &&
				(bool)bold == (*iter)->mBold)
			{
				return;
			}
		}

		OSVERSIONINFO osVerInfo;
		::GetVersionEx(&osVerInfo);

		// ���� �߰��ϱ�
		ID3DXFont *font;
		if(FAILED(D3DXCreateFont(GetNekoNovel()->GetVideoDriver()->GetHandle(), height, 0, (bold) ? FW_BOLD : 0, 1, false, DEFAULT_CHARSET
			, OUT_DEFAULT_PRECIS,
			// XP �̻� ClearType ����
			/*(osVerInfo.dwMajorVersion * 10 + osVerInfo.dwMinorVersion >= 51) ? CLEARTYPE_NATURAL_QUALITY : */ANTIALIASED_QUALITY
			, DEFAULT_PITCH|FF_DONTCARE, faceName, &font)))
		{
			LogPrint(LOG_LEVEL_WARN, "'%s'(ũ�� %d) ��Ʈ �ε� ����", faceName, height);
			return;
		}

		nekoChatBoxFont *newNode = nekoNew nekoChatBoxFont;
		newNode->mBold = (bool)bold;
		newNode->mFaceName = faceName;
		newNode->mFont = font;
		newNode->mHeight = height;

		mFontList.push_back(newNode);
		mCurrentFont = newNode;

		PushNewBlock();
	}

	/// Set Skin
	void nekoGUIChatBox::SetSkin(const char8 *fileName)
	{
		if(fileName == NULL)
		{
			mBackground = NULL;
			return;
		}

		mBackground = GetNekoNovel()->GetHWResourceManager()->LoadTexture(nekoUtility::FormString("�̹���\\%s", fileName));
	}

	/// Update
	void nekoGUIChatBox::Update(float32 frameTime)
	{
		nekoGameEnvironment &env = GetNekoNovel()->GetCurrentGame()->GetEnvironment();

		if(env.mPhotoMode)
			return;

		// ���߿� �� �κ� ����ȭ�� �������...

		if(env.mSmoothPrint && GetNekoNovel()->GetCurrentGame()->GetGameMode() == GAME_RUN_PLAYING)
		{
			if(mSmoothAlpha < (mTextCount - (mSmoothTextEnd + 1)) * 20.0f + 255.0f)
			{
				mSmoothAlpha += frameTime * 1400.0f;

				// ���� ��ŵ..
				if(!env.mPrintDelay
					|| GetNekoNovel()->GetCurrentGame()->GetFlowState() == EGFS_QUICKSKIPPING
					|| (GetNekoNovel()->IsActive() && GetKeyState(VK_CONTROL) < 0))
					mSmoothAlpha += frameTime * 4400.0f;	
			}
		}

		mCursorTick.Update(frameTime);
		if(mCursorTick.IsCompleted())
		{
			mCursorTickIndex = mCursorTickIndex++;

			if(env.mCursorType == ECCT_TWINKLE)
			{
				mCursorTick.SetTime(0.5f);

				if(mCursorTickIndex > 1)
				{
					mCursorTickIndex = 0;
					mCursorSourceRect.left = mCursorSourceRect.top = 0;
					mCursorSourceRect.right = (mCursor) ? mCursor->GetSize().x : 0;
					mCursorSourceRect.bottom = (mCursor) ? mCursor->GetSize().y : 0;
				}
			}
			else if(env.mCursorType == ECCT_ANIMATION)
			{
				mCursorTick.SetTime(0.1f);

				// ���� Ŀ���� ���ٸ�..
				if(!mCursor)
				{
					mCursorSourceRect.left = mCursorSourceRect.top =
						mCursorSourceRect.right = mCursorSourceRect.bottom = 0;
				}
				else
				{
					if(mCursorTickIndex > env.mCursorAniWidth * env.mCursorAniHeight)
						mCursorTickIndex = 1;

					mCursorSourceRect.left = ((mCursorTickIndex - 1) % env.mCursorAniWidth) * mCursor->GetSize().x / env.mCursorAniWidth;
					mCursorSourceRect.top = (int32)((mCursorTickIndex - 1) / env.mCursorAniWidth) * mCursor->GetSize().y / env.mCursorAniHeight;
					mCursorSourceRect.right = mCursorSourceRect.left + mCursor->GetSize().x / env.mCursorAniWidth;
					mCursorSourceRect.bottom = mCursorSourceRect.top + mCursor->GetSize().y / env.mCursorAniHeight;
				}
			}
		}

		// ����� �ؽ�Ʈ�� ���Ҵ°� ?
		if(IsWaitingTextLeft())
		{
			mPrintTimer += frameTime;

			do
			{
				if(mWaitingForNextPage)
				{
					mCursorVisible = true;

					if(GetNekoNovel()->GetCurrentGame()->GetFlowState() == EGFS_AUTOTURNING)
					{
						// �ڵ� �ѱ�
						mAutoTurningTimer.Update(frameTime);
						if(mAutoTurningTimer.IsCompleted())
						{
							Skip();
						}
					}
					return;
				}
				else
					mCursorVisible = false;

				if(GetNekoNovel()->GetCurrentGame()->GetGameMode() == GAME_RUN_PREVIEW)
				{
					Skip();
					return;
				}

				if(mPrintTimer < (env.GetSpeechSpeed())
					&& !(
					   (GetNekoNovel()->GetCurrentGame()->GetFlowState() == EGFS_QUICKSKIPPING)
					|| (GetNekoNovel()->IsActive() && GetKeyState(VK_CONTROL) < 0))
					&& env.mPrintDelay)
					return;

				// ��� ������ ������ �ִ��� üũ�Ѵ�.
				if(_IsPrintAbleNow() == false)
				{
					mWaitingForNextPage = true;
					return;
				}

				mPrintTimer = 0;


				nekoChatBoxTextBlock *block = _GetFirstTextBlock();
				const char8 *p = block->GetCurrentText();

				// ��� ù �����..
				if(block->mWidthesInitialized == false)
				{
					// ����� �̸� ������ش�.
					if(block->mAlignType == ETAT_CENTER2)
					{
						// �̸� ����س���.
						_PreCalculateForCenter2(block, p);

						// �׸��� ���� ���忡 ũ�� �̸� �Է����ֱ�.
						mWidthPerLine[mWidthPerLine.size() - 1] += block->mWidthPerLine[0];
						for(uint32 i = 1;i < block->mWidthPerLine.size();++i)
						{
							mWidthPerLine.push_back(block->mWidthPerLine[i]);
						}
					}

					block->mWidthesInitialized = true;
				}

				// ���ο� ���ڸ� �����Ѵ�.

				if(nekoUtility::IsMultibyteCode(*p))
				{
					block->mCaption += *p++;
					block->mWaitingTextWrittenLen++;
				}

				block->mCaption += *p;
				block->mWaitingTextWrittenLen++;

			} while(IsWaitingTextLeft());
		}

	}

	/// Draw
	void nekoGUIChatBox::Draw()
	{
		if(!mVisible)
			return;

		nekoGame *currentGame = GetNekoNovel()->GetCurrentGame();
		int32 alpha = mAlpha * currentGame->GetEnvironment().mTextWindowAlpha / 255.0f;
		nekoVideoDriver *videoDrv = GetNekoNovel()->GetVideoDriver();
		nekoGameEnvironment &env = currentGame->GetEnvironment();
		float32 ZOrder = _GetRealZ();

		if(!mAlpha)
			return;

		videoDrv->EnableCamera(false);

		videoDrv->DrawImage(mBackground, mBoundingBox.left, mBoundingBox.top, mBoundingBox.GetWidth(), mBoundingBox.GetHeight(), D3DCOLOR_ARGB(alpha, 255, 255, 255), 0, 0, -1, -1, 0, 1, 1, ZOrder + REAL_Z_DIFF / 2);

		if(!mChatVisible)
		{
			videoDrv->EnableCamera(true);

			return;
		}

		// �� ���� �ѹ� �׷����..
		list<nekoChatBoxTextBlock *>::iterator iter;
		nekoPoint curPos = nekoPoint(mPrintingBoundary.min.x, mPrintingBoundary.min.y);
		int32 maxHeight = 0;
		SIZE tsize = {0, };
		nekoChatBoxTextBlock *block;
		ID3DXSprite *sprite = videoDrv->GetSprite();
		RECT rect;
		bool exit = false;
		char8 buf[3];
		int32 selectionIndex = -1;

		D3DXMATRIX matIdentity;
		D3DXMatrixIdentity(&matIdentity);
		if(videoDrv->IsTargetRendering())
		{
			videoDrv->EndSprite();

			// ���� ��� ����
			D3DXMATRIX matProj;
			D3DXMatrixOrthoOffCenterLH(&matProj, 0, 320, 240, 0, 0.1f, 1.0f);
			videoDrv->GetDevice()->SetTransform(D3DTS_PROJECTION, &matProj);

			matIdentity._11 = 320.0f/videoDrv->GetParams().BackBufferWidth;
			matIdentity._22 = 240.0f/videoDrv->GetParams().BackBufferHeight;

			videoDrv->BeginSprite();
		}
		matIdentity._43 = ZOrder;
		matIdentity *= videoDrv->GetBaseMatrix();
		sprite->SetTransform(&matIdentity);

		int32 curidx = 0;
		int32 curAlpha;
		uint32 curLine = 0;

		if(mTextBlockList.empty() == false)
		{
			block = (*mTextBlockList.begin());

			// ���Ĺ�� �� ��ġ ����.
			if(block->mAlignType == ETAT_CENTER || block->mAlignType == ETAT_CENTER2)
				curPos.x = mPrintingBoundary.GetCenter().x - mWidthPerLine[curLine] / 2;
			else if(block->mAlignType == ETAT_RIGHT)
				curPos.x = mPrintingBoundary.right - mWidthPerLine[curLine];
		}

		for(iter = mTextBlockList.begin();(!exit && iter != mTextBlockList.end());++iter)
		{
			block = (*iter);

			// �ʱ�ȭ ���� ���� ���� ������� �ʴ´�.
			if(block->mWidthesInitialized == false)
				continue;

			// ������ ������..
			if(block->mType == ETT_SELECTION)
			{
				curPos.y += maxHeight + mLineInterval;
				curPos.x = mPrintingBoundary.min.x;
				maxHeight = 0;
				++selectionIndex;

				block->mRect.MoveTo(block->mRect.left, curPos.y);

				if(selectionIndex == mSelectedIndex)
				{
					/// ���� ���� ��ĥ���ֱ�.
					videoDrv->DrawRectangle(block->mRect.left, block->mRect.top,
						block->mRect.GetWidth(), block->mRect.GetHeight(), D3DCOLOR_ARGB(125, 255, 0, 0), ZOrder);

					/// ���� Sprite Matrix ����
					sprite->SetTransform(&matIdentity);
				}
			}

			uint32 blockCurLine = 0;

			const char8 *p = block->mCaption.c_str();
			while(*p)
			{
				if(nekoUtility::IsMultibyteCode(*p))
				{
					buf[0] = *p++;
					buf[1] = *p++;
					buf[2] = '\0';
				}
				else
				{
					buf[0] = *p++;
					buf[1] = '\0';
				}

				GetTextExtentPoint32(block->mFont->mFont->GetDC(), buf, (buf[1]) ? 2 : 1, &tsize);

				if(buf[0] == '\n')
					tsize.cx = 0;

				// ��ĭ ������
				if(buf[0] == '\n' || curPos.x + tsize.cx > mPrintingBoundary.max.x)
				{
					++curLine;
					++blockCurLine;
					curPos.y += maxHeight + mLineInterval;
					curPos.x = mPrintingBoundary.min.x;
					maxHeight = 0;

					if(block->mWidthPerLine.size() <= blockCurLine)
					{
						block->mWidthPerLine.push_back(0);
					}

					// ���Ĺ�� �� ��ġ ����.
					if(block->mAlignType == ETAT_CENTER || block->mAlignType == ETAT_CENTER2)
						curPos.x = mPrintingBoundary.GetCenter().x - mWidthPerLine[curLine] / 2;
					else if(block->mAlignType == ETAT_RIGHT)
						curPos.x = mPrintingBoundary.right - mWidthPerLine[curLine];
				}

				if(curPos.y + tsize.cy > mPrintingBoundary.max.y)
				{
					exit = true;
					break;
				}

				// �ε巴�� �긮�� �ϱ�.(�������� �翬�� �ƴ���!)
				if(block->mType != ETT_SELECTION 
					 && env.mSmoothPrint && currentGame->GetGameMode() == GAME_RUN_PLAYING
					 && curidx >= mSmoothTextEnd)
				{
					curAlpha = mSmoothAlpha - (curidx - (mSmoothTextEnd + 1)) * 20.0f;
					curAlpha = min(max(curAlpha, 0), 255);
				}
				else
				{
					curAlpha = 255.0f;
				}

				
				switch(block->mShadowType)
				{
				case 1:
					{
						uint32 fadeShadowColor = MixColorsAlpha(block->mShadowColor, mAlpha);
						fadeShadowColor = MixColorsAlpha(fadeShadowColor, curAlpha);
						rect.left = curPos.x + 1; rect.top = curPos.y + 1;
						block->mFont->mFont->DrawTextA(sprite, buf, (buf[1]) ? 2 : 1, &rect, DT_NOCLIP, fadeShadowColor);
						rect.left--; rect.top--;
					}
					break;
				case 3:
					{
						uint32 fadeShadowColor = (block->mShadowColor & 0xffffff) | ((((block->mShadowColor >> 24) & 0xff) / 8) << 24);
						fadeShadowColor = MixColorsAlpha(fadeShadowColor, curAlpha);

						for(int32 sy = -2; sy <= 2;sy += 2)
						{
							for(int32 sx = -2; sx <= 2;sx += 2)
							{
								if(!sx && !sy)
									continue;

								rect.left = curPos.x + sx; rect.top = curPos.y + sy;
								block->mFont->mFont->DrawTextA(sprite, buf, -1, &rect, DT_NOCLIP, fadeShadowColor);
							}
						}
					}
				case 2:
					{
						uint32 fadeShadowColor = MixColorsAlpha(block->mShadowColor, mAlpha);
						fadeShadowColor = MixColorsAlpha(fadeShadowColor, curAlpha);

						for(int32 sy = -1; sy <= 1;++sy)
						{
							for(int32 sx = -1; sx <= 1;++sx)
							{
								if(!sx && !sy)
									continue;

								rect.left = curPos.x + sx; rect.top = curPos.y + sy;
								block->mFont->mFont->DrawTextA(sprite, buf, -1, &rect, DT_NOCLIP, fadeShadowColor);
							}
						}
					
						rect.left = curPos.x; rect.top = curPos.y;
					}
					break;
				case 4:
					{
						uint32 fadeShadowColor = MixColorsAlpha(block->mShadowColor, mAlpha);
						fadeShadowColor = MixColorsAlpha(fadeShadowColor, curAlpha);

						for(int32 sy = -2; sy <= 2;sy++)
						{
							for(int32 sx = -2; sx <= 2;sx++)
							{
								if(!sx && !sy)
									continue;

								rect.left = curPos.x + sx; rect.top = curPos.y + sy;
								block->mFont->mFont->DrawTextA(sprite, buf, -1, &rect, DT_NOCLIP, fadeShadowColor);
							}
						}

						rect.left = curPos.x; rect.top = curPos.y;
					}
					break;
				default:
					rect.left = curPos.x; rect.top = curPos.y;
					break;
				}
				uint32 finalColor = MixColorsAlpha(block->mColor, mAlpha);
				finalColor = MixColorsAlpha(finalColor, curAlpha);
				block->mFont->mFont->DrawTextA(sprite, buf, (buf[1]) ? 2 : 1, &rect, DT_NOCLIP, finalColor);

				curidx++;
				curPos.x += tsize.cx;
				maxHeight = max(maxHeight, tsize.cy);
			}

			rect.left = curPos.x; rect.top = curPos.y;
		}

		// ���� ����!
		matIdentity._43 = 0.0f;
		sprite->SetTransform(&matIdentity);

		if(mCursorVisible && mCursorTickIndex > 0)
		{
			if(currentGame->GetGameMode() == GAME_RUN_PLAYING)
			{
				// ���� �ε巯�� �帲�� ����Ѵٸ�..
				if(env.mSmoothPrint)
				{
					// ������ ������ ������ 255��������!!
					if(curAlpha != 255)
					{
						videoDrv->EnableCamera(true);
						return;
					}
				}

				if(env.mProjectFlags & EPFT_NEWTYPE_CURSOR)
				{
					videoDrv->DrawImage(mCursor, mPrintingBoundary.max.x - mCursorSourceRect.GetWidth(), mPrintingBoundary.max.y - mCursor->GetSize().y
						, -1, -1, (mAlpha << 24) | 0xffffff, mCursorSourceRect.left, mCursorSourceRect.top, mCursorSourceRect.GetWidth(), mCursorSourceRect.GetHeight(), 0, 1, 1, ZOrder);
				}
				else
				{
					videoDrv->DrawImage(mCursor, curPos.x, max(curPos.y + maxHeight - mCursor->GetSize().y, mPrintingBoundary.min.y)
						, -1, -1, (mAlpha << 24) | 0xffffff, mCursorSourceRect.left, mCursorSourceRect.top, mCursorSourceRect.GetWidth(), mCursorSourceRect.GetHeight(), 0, 1, 1, ZOrder);
				}
			}
		}

		videoDrv->EnableCamera(true);
	}

	/// Clear all
	void nekoGUIChatBox::Clear()
	{
		mLineInterval = 0;
		mChatVisible = true;
		mCursorVisible = false;
		mAlpha = 255;
		mBoundingBox = nekoRectangle(25, 420, 775, 570);
		mPrintingBoundary = nekoRectangle(25 + 10, 420 + 10, 775 - 10, 570 - 10);
		mBackground = GetNekoNovel()->GetHWResourceManager()->LoadTexture("�̹���\\���â��Ų.png");

		ClearBuffer();
	}

	/// Clear the buffer
	void nekoGUIChatBox::ClearBuffer()
	{
		mCursorPos = nekoPoint();
		mCursorVisible = false;
		mMaxHeight = 0;
		mTextCount = 0;
		mSmoothAlpha = 0.0f;
		mSmoothTextEnd = -1;
		mPrintTimer = 0;
		mWaitingForNextPage = false;

		list<nekoChatBoxFont *>::iterator iter = mFontList.begin();
		for(;iter != mFontList.end();)
		{
			if(mCurrentFont == (*iter))
			{
				++iter;
				continue;
			}

			(*iter)->mFont->Release();
			delete (*iter);
			mFontList.erase(iter++);
		}

		// �� ����������...
		list<nekoChatBoxTextBlock *>::iterator iterBlock = mTextBlockList.begin();
		for(;iterBlock != mTextBlockList.end();++iterBlock)
		{
			delete (*iterBlock);
		}
		mTextBlockList.clear();

		PushNewBlock();

		// ���κ� ũ�� �ʱ�ȭ
		mWidthPerLine.clear();
		mWidthPerLine.push_back(0);
	}

	nekoChatBoxTextBlock *nekoGUIChatBox::PushNewBlock()
	{
		nekoGameEnvironment &env = GetNekoNovel()->GetCurrentGame()->GetEnvironment();

		if(mTextBlockList.empty() == false)
		{
			nekoChatBoxTextBlock *lastBlock = *(--mTextBlockList.end());

			// �� �������� �ִ� ���� ����̰�, ����ִٸ�...
			if(lastBlock->mType == ETT_NORMAL
				&& lastBlock->mWaitingText.empty())
			{
				delete lastBlock;
				mTextBlockList.pop_back();
			}
		}
		
		// ���ο� �� �߰�
		nekoChatBoxTextBlock *newBlock = nekoNew nekoChatBoxTextBlock;
		newBlock->mType = ETT_NORMAL;	
		newBlock->mFont = mCurrentFont;
		newBlock->mColor = env.mSpeechColor;
		newBlock->mShadowColor = env.mSpeechShadowColor;
		newBlock->mShadowType = env.mSpeechShadowType;
		newBlock->mAlignType = env.mTextAlign;

		mTextBlockList.push_back(newBlock);
		return newBlock;
	}

	/// Write text
	void nekoGUIChatBox::WriteText(const char8 *sentence, E_TEXTBLOCK_TYPE blockType)
	{
		if(!sentence)
			return;

		nekoGameEnvironment &env = GetNekoNovel()->GetCurrentGame()->GetEnvironment();

		if(blockType == ETT_NORMAL)
		{
			if(mTextCount > 0)
				OnSmoothTextEnd();

			nekoChatBoxTextBlock *lastBlock = *(--mTextBlockList.end());

			if(lastBlock && lastBlock->mType == ETT_NORMAL)
			{
				lastBlock->mWaitingText += sentence;

				// Center2 �϶����� �߰�������, ũ�⸦ ������Ʈ���ش�.
				if(lastBlock->mWidthesInitialized &&
					lastBlock->mAlignType == ETAT_CENTER2)
				{
					// Center2�� �ش��ϴ� ���ڿ����� ũ�⸦ ���ٰ� �ٽ� �����ش�.


					for(uint32 i = 1;i < lastBlock->mWidthPerLine.size();++i)
					{
						mWidthPerLine.pop_back();
					}
					mWidthPerLine[mWidthPerLine.size() - 1] -= lastBlock->mWidthPerLine[0];

					_PreCalculateForCenter2(lastBlock, sentence);
					mWidthPerLine[mWidthPerLine.size() - 1] += lastBlock->mWidthPerLine[0];
					for(uint32 i = 1;i < lastBlock->mWidthPerLine.size();++i)
					{
						mWidthPerLine.push_back(lastBlock->mWidthPerLine[i]);
					}
				}
			}
			else
			{
				(lastBlock = PushNewBlock())->mWaitingText = sentence;
			}
		}
		else if(blockType == ETT_SELECTION)
		{
			uint32 i = 0;
			char8 buf[3];
			SIZE tsize;
			nekoRectangle rect;

			/// ������ �� ��� ���� �� ��ȯ
			if(mCursorPos.x)
			{
				mCursorPos.x = 0;
				mCursorPos.y += mMaxHeight + mLineInterval;
				mMaxHeight = 0;
			}

			rect.left = mCursorPos.x + mPrintingBoundary.left; rect.top = mCursorPos.y + mPrintingBoundary.top;
			for(;sentence[i] != '\0';)
			{
				if(nekoUtility::IsMultibyteCode(sentence[i]))
				{
					buf[0] = sentence[i];
					buf[1] = sentence[i + 1];
					buf[2] = '\0';
				}
				else
				{
					buf[0] = sentence[i];
					buf[1] = '\0';
				}

				GetTextExtentPoint32(mCurrentFont->mFont->GetDC(), buf, (buf[1]) ? 2 : 1, &tsize);

				if(buf[0] == '\n')
					tsize.cx = 0;

				// ��ĭ ������
				if(buf[0] == '\n' || mCursorPos.x + tsize.cx > mPrintingBoundary.GetWidth())
				{
					mCursorPos.y += mMaxHeight + mLineInterval;
					mCursorPos.x = 0;
					mMaxHeight = 0;
				}

				if(mCursorPos.y + tsize.cy > mPrintingBoundary.GetHeight())
				{
					break;
				}

				if(nekoUtility::IsMultibyteCode(sentence[i]))
					i += 2;
				else
					i ++;

				++mTextCount;
				mCursorPos.x += tsize.cx;
				mMaxHeight = max(mMaxHeight, tsize.cy);
			}
			rect.right = mPrintingBoundary.right;
			rect.bottom = mPrintingBoundary.top + mCursorPos.y + mMaxHeight;

			if(!i)
				return;

			nekoChatBoxTextBlock *newBlock = nekoNew nekoChatBoxTextBlock;
			newBlock->mType = blockType;
			newBlock->mCaption = sentence;
			newBlock->mFont = mCurrentFont;
			newBlock->mColor = env.mSelectionColor;
			newBlock->mShadowColor = env.mSpeechShadowColor;
			newBlock->mShadowType = env.mSpeechShadowType;
			newBlock->mRect = rect;
			newBlock->mWidthesInitialized = true;

			mTextBlockList.push_back(newBlock);

			OnSmoothTextEnd();
		}
	}

	/// ���� �������� �ѱ��
	void nekoGUIChatBox::_GoNextPage()
	{
		mCursorPos = nekoPoint();
		mMaxHeight = 0;
		mSelectedIndex = -1;
		mSmoothAlpha = 0;
		mSmoothTextEnd = -1;
		mTextCount = 0;
		mPrintTimer = 0.0f;
		mAutoTurningTimer.Clear();
		mWidthPerLine.clear();
		mWidthPerLine.push_back(0);

		if(!mTextBlockList.empty())
		{
			list<nekoChatBoxTextBlock *>::iterator iter2 = mTextBlockList.begin();
			int32 i;
			int32 blockCount = mTextBlockList.size() - 1;
			for(i = 0;i < blockCount;++i)
			{
				delete (*iter2);
				mTextBlockList.erase(iter2++);
			}

			nekoChatBoxTextBlock *block = (*(--mTextBlockList.end()));

			block->mWidthPerLine.clear();
			block->mWidthPerLine.push_back(0);

			block->mCaption.clear();
			if(block->mAlignType == ETAT_CENTER2)
			{
				block->mWidthesInitialized = false;
			}
		}
	}

	uint32 nekoGUIChatBox::_PreCalculateText(bool skipping, nekoChatBoxTextBlock **outBlock)
	{
		nekoChatBoxTextBlock *block = _GetFirstTextBlock();
		const char8 *p = block->GetCurrentText();
		uint32 i = 0;
		char8 buf[3];
		SIZE tsize;

		if(outBlock)
			*outBlock = block;

		while(*p)
		{
			if(nekoUtility::IsMultibyteCode(*p))
			{
				buf[0] = *p++;
				buf[1] = *p++;
				buf[2] = '\0';
			}
			else
			{
				buf[0] = *p++;
				buf[1] = '\0';
			}

			GetTextExtentPoint32(block->mFont->mFont->GetDC(), buf, (buf[1]) ? 2 : 1, &tsize);

			if(buf[0] == '\n')
				tsize.cx = 0;

			// ��ĭ ������
			if(buf[0] == '\n' || mCursorPos.x + tsize.cx > mPrintingBoundary.GetWidth())
			{

				// ���� �ٿ� ���ڿ��� ���� �� ���� ���°� �ȴٸ�..
				if(mCursorPos.y + mMaxHeight + mLineInterval + tsize.cy > mPrintingBoundary.GetHeight())
				{
					if(block->mAlignType != ETAT_CENTER2)
						block->mWidthPerLine.push_back(0);
					if(skipping)
						mWaitingForNextPage = true;
					break;
				}

				if(block->mAlignType != ETAT_CENTER2)
				{
					block->mWidthPerLine.push_back(0);
					this->mWidthPerLine.push_back(0);
				}

				mCursorPos.y += mMaxHeight + mLineInterval;
				mCursorPos.x = 0;
				mMaxHeight = 0;
			}

			if(buf[1])
				i += 2;
			else
				i ++;

			++mTextCount;
			if(block->mAlignType != ETAT_CENTER2)
			{
				block->mWidthPerLine[block->mWidthPerLine.size() - 1] += tsize.cx;
				this->mWidthPerLine[this->mWidthPerLine.size() - 1] += tsize.cx;
			}
			mCursorPos.x += tsize.cx;
			mMaxHeight = max(mMaxHeight, tsize.cy);

			// �� �ܾ ���� ���..
			if(!skipping)
				break;
		}

		return i;
	}

	void nekoGUIChatBox::_PreCalculateForCenter2(nekoChatBoxTextBlock *block, const char8 *text)
	{

		char8 buf[3];
		SIZE tsize = {0, };
		nekoPoint curPos = mCursorPos;
		int32 maxHeight = mMaxHeight;

		curPos.x = block->mWidthPerLine[block->mWidthPerLine.size() - 1];

		while(*text)
		{
			if(nekoUtility::IsMultibyteCode(*text))
			{
				buf[0] = *text++;
				buf[1] = *text++;
				buf[2] = '\0';
			}
			else
			{
				buf[0] = *text++;
				buf[1] = '\0';
			}

			GetTextExtentPoint32(block->mFont->mFont->GetDC(), buf, (buf[1]) ? 2 : 1, &tsize);

			if(buf[0] == '\n')
				tsize.cx = 0;

			// ��ĭ ������
			if(buf[0] == '\n' || curPos.x + tsize.cx > mPrintingBoundary.GetWidth())
			{
				if(curPos.y + mLineInterval + maxHeight + tsize.cy > mPrintingBoundary.GetHeight())
				{
					break;
				}

				block->mWidthPerLine.push_back(0);

				curPos.y += maxHeight + mLineInterval;
				curPos.x = 0;
				maxHeight = 0;
			}

			curPos.x += tsize.cx;
			block->mWidthPerLine[block->mWidthPerLine.size() - 1] += tsize.cx;
			maxHeight = max(maxHeight, tsize.cy);
		}
	}

	/// On Event
	bool nekoGUIChatBox::OnEvent(const nekoEvent &event)
	{
		if(event.type == EVENT_WINDOW_MESSAGE)
		{
			if(event.uMsg == WM_MOUSEMOVE)
			{
				mSelectedIndex = -1;

				POINT pt = {LOWORD(event.lParam), HIWORD(event.lParam)};

				list<nekoChatBoxTextBlock *>::iterator iter = mTextBlockList.begin();
				int32 i = 0;
				for(;iter != mTextBlockList.end();++iter)
				{
					if((*iter)->mType == ETT_SELECTION)
					{
						if((*iter)->mRect.IsPointIn(pt.x, pt.y))
						{
							mSelectedIndex = i;
							return true;
						}
						++i;
					}
				}
			} else if(event.uMsg == WM_LBUTTONDOWN)
			{
				if(mSelectedIndex == -1)
					return false;

				GetNekoNovel()->GetCurrentGame()->OnSelectionCompleted(mSelectedIndex);
			}
		}

		return false;
	}

	result nekoGUIChatBox::Dump(int32 version, nekoByteBuffer &outBuf)
	{
		nekoIBlockInstance::Dump(version, outBuf);

		outBuf << ((mBackground) ? mBackground->GetName() : "")
		<< mMaxHeight
		<< mCursorPos
		<< mCursorVisible
		<< mChatVisible
		<< mPrintingBoundary
		<< mLineInterval
	
		// 1115
		<< mSmoothAlpha
		<< mSmoothTextEnd
		<< mTextCount
		<< mWaitingForNextPage;

		uint32 selectedFont = 0;

		outBuf << (uint32)mFontList.size();
		outBuf << selectedFont;

		list<nekoChatBoxFont *>::iterator itFont = mFontList.begin();
		uint32 i = 0;
		for(;itFont != mFontList.end();++itFont,++i)
		{
			outBuf << (*itFont)->mBold;
			outBuf << (*itFont)->mFaceName;
			outBuf << (*itFont)->mHeight;

			if((*itFont) == mCurrentFont)
				selectedFont = i;
		}

		outBuf << (uint32)mTextBlockList.size();

		list<nekoChatBoxTextBlock *>::iterator itBlock = mTextBlockList.begin();
		for(;itBlock != mTextBlockList.end();++itBlock)
		{
			outBuf << (*itBlock)->mCaption;
			outBuf << (*itBlock)->mColor;
			outBuf << (*itBlock)->mRect;
			outBuf << (*itBlock)->mShadowColor;
			outBuf << (*itBlock)->mShadowType;
			outBuf << (*itBlock)->mType

			// 1115

			<< (*itBlock)->mWaitingText
			<< (*itBlock)->mWaitingText.length()

			// 1116
			<< (*itBlock)->mAlignType
			<< (*itBlock)->mWidthPerLine.size();

			for(uint32 i = 0;i < (*itBlock)->mWidthPerLine.size();++i)
				outBuf << (*itBlock)->mWidthPerLine[i];

			outBuf << (*itBlock)->mWidthesInitialized;

			itFont = mFontList.begin();
			uint32 i = 0;
			for(;itFont != mFontList.end();++itFont, ++i)
			{
				if((*itFont) == (*itBlock)->mFont)
					break;
			}

			outBuf << i;
		}

		outBuf << mWidthPerLine.size();
		for(uint32 i = 0;i < mWidthPerLine.size();++i)
			outBuf << mWidthPerLine[i];

		return S_OK;
	}

	result nekoGUIChatBox::Load(int32 version, nekoByteBuffer &inBuf)
	{
		nekoIBlockInstance::Load(version, inBuf);

		string path;

		inBuf >> path
		>> mMaxHeight
		>> mCursorPos
		>> mCursorVisible
		>> mChatVisible
		>> mPrintingBoundary;

		if(version >= 1100)
			inBuf >> mLineInterval;

		if(version >= 1115)
		{
			inBuf >> mSmoothAlpha
			>> mSmoothTextEnd
			>> mTextCount
			>> mWaitingForNextPage;
		}

		if(!path.empty())
			mBackground = GetNekoNovel()->GetHWResourceManager()->LoadTexture(path.c_str());
		else
			mBackground = NULL;

		uint32 count;
		
		// ��� ��Ʈ �����
		list<nekoChatBoxFont *>::iterator iter = mFontList.begin();
		for(;iter != mFontList.end();++iter)
		{
			(*iter)->mFont->Release();
			delete (*iter);
		}

		mFontList.clear();
		mCurrentFont = 0;

		// �ؽ�Ʈ �� ��� �����

		list<nekoChatBoxTextBlock *>::iterator iterBlock = mTextBlockList.begin();
		for(;iterBlock != mTextBlockList.end();++iterBlock)
		{
			delete (*iterBlock);
		}
		mTextBlockList.clear();

		inBuf >> count;
		uint32 selIndex;
		inBuf >> selIndex;

		OSVERSIONINFO osVerInfo;
		::GetVersionEx(&osVerInfo);

		nekoChatBoxFont *newNode = 0;
		for(uint32 i = 0;i < count;++i)
		{
			bool bold;
			int32 height;
			inBuf >> bold >> path >> height;

			// ���� �߰��ϱ�
			ID3DXFont *font;
			if(FAILED(D3DXCreateFont(GetNekoNovel()->GetVideoDriver()->GetHandle(), height, 0, (bold) ? FW_BOLD : 0, 1, false, DEFAULT_CHARSET
				, OUT_DEFAULT_PRECIS, 
				// XP �̻� ClearType ����
				/*(osVerInfo.dwMajorVersion * 10 + osVerInfo.dwMinorVersion >= 51) ? CLEARTYPE_NATURAL_QUALITY : */ANTIALIASED_QUALITY
				, DEFAULT_PITCH|FF_DONTCARE, path.c_str(), &font)))
			{
				LogPrint(LOG_LEVEL_WARN, "'%s'(ũ�� %d) ��Ʈ �ε� ����", path.c_str(), height);
				continue;
			}

			newNode = nekoNew nekoChatBoxFont;
			newNode->mFaceName = path;
			newNode->mFont = font;
			newNode->mHeight = height;
			newNode->mBold = bold;

			mFontList.push_back(newNode);

			if(i == selIndex)
				mCurrentFont = newNode;
		}

		// �⺻ ��Ʈ ����.
		if(!mCurrentFont)
			mCurrentFont = newNode;

		inBuf >> count;

		uint32 sCount;

		list<nekoChatBoxFont *>::iterator itFont;
		for(uint32 i = 0;i < count;++i)
		{
			nekoChatBoxTextBlock *block = nekoNew nekoChatBoxTextBlock;
			inBuf >> block->mCaption
			>> block->mColor
			>> block->mRect
			>> block->mShadowColor
			>> block->mShadowType
			>> block->mType;

			if(version >= 1115)
			{
				inBuf >> block->mWaitingText
					>> block->mWaitingTextWrittenLen;
			}

			if(version >= 1116)
			{
				inBuf >> block->mAlignType;

				inBuf >> sCount;

				block->mWidthPerLine.clear();
				for(uint32 k = 0;k < sCount;++k)
					block->mWidthPerLine.push_back(inBuf.Read<uint32>());
			}

			if(version >= 1128)
			{
				inBuf >> block->mWidthesInitialized;
			}
			else
			{
				block->mWidthesInitialized = false;
			}

			inBuf >> selIndex;

			itFont = mFontList.begin();
			uint32 a = 0;
			for(;itFont != mFontList.end();++itFont, ++a)
			{
				if(a == selIndex)
					break;
			}

			block->mFont = (*itFont);

			mTextBlockList.push_back(block);
		}

		// ���κ� ũ�� �ҷ�����!!

		mWidthPerLine.clear();
		if(version >= 1128)
		{
			uint32 cnt;

			inBuf >> cnt;
			for(uint32 i = 0;i < cnt;++i)
				mWidthPerLine.push_back(inBuf.Read<int32>());
		}
		else
		{
			mWidthPerLine.push_back(0);
		}

		return S_OK;
	}

	bool nekoGUIChatBox::IsWaitingTextLeft()
	{
		list<nekoChatBoxTextBlock *>::iterator iter2 = mTextBlockList.begin();
		for(;iter2 != mTextBlockList.end();++iter2)
		{
			if((*iter2)->mWaitingText.size() > (*iter2)->mWaitingTextWrittenLen)
				return true;
		}

		return false;
	}

	bool nekoGUIChatBox::IsSmoothTexting()
	{
		if(GetNekoNovel()->GetCurrentGame()->GetEnvironment().mSmoothPrint)
		{
			if(mSmoothAlpha < (mTextCount - (mSmoothTextEnd + 1)) * 20.0f + 255.0f)
				return true;
		}

		return false;
	}

	/// ��ŵ�ϱ�
	bool nekoGUIChatBox::Skip()
	{
		if(IsWaitingTextLeft() == false)
			return true;

		// ���� �������� �Ѿ��
		if(mWaitingForNextPage)
		{
			_GoNextPage();
			mWaitingForNextPage = false;
			return false;
		}

		nekoChatBoxTextBlock *block;
		block = _GetFirstTextBlock();

		if(block->mWidthesInitialized == false)
		{

			// ����� �̸� ������ش�.
			if(block->mAlignType == ETAT_CENTER2)
			{
				// �̸� ����س���.
				_PreCalculateForCenter2(block, block->GetCurrentText());

				// �׸��� ���� ���忡 ũ�� �̸� �Է����ֱ�.
				mWidthPerLine[mWidthPerLine.size() - 1] += block->mWidthPerLine[0];
				for(uint32 i = 1;i < block->mWidthPerLine.size();++i)
				{
					mWidthPerLine.push_back(block->mWidthPerLine[i]);
				}
			}


			block->mWidthesInitialized = true;
		}

		uint32 i = _PreCalculateText(true, &block);

		if(!i)
			return true;

		// �׸�ŭ ������ ����!!
		block->mCaption += string(block->GetCurrentText()).substr(0, i);
		block->mWaitingTextWrittenLen += i;

		OnSmoothTextEnd();
		return true;
	}
}