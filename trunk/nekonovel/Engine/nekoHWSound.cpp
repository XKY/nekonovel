#include "nekoPrecompiledHeader.h"
#include "nekoHWSound.h"
#include "nekoLogger.h"
#include "nekoHWResourceManager.h"
#include <audiere.h>
#include "nekoNovelReal.h"

namespace neko
{
	nekoHWSound::nekoHWSound(nekoHWResourceManager *resMgr, const char8 *fileName)
		: nekoIHWResource(resMgr, EHRT_SOUND)
		, mHandle(NULL)
		, mVolume(1.0f)
		, mPlayWhenLoad(true)
	{
		Catch();
		mName = fileName;
		nekoUtility::TrimRight(mName);

		SetTimeout(30);
	}

	/// Destructor
	nekoHWSound::~nekoHWSound()
	{
		if(mHandle)
		{
			mHandle->stop();
			mHandle->unref();
		}
	}

	/// ������ �ε�/��ε带 �õ��ϴ� �޼ҵ�
	result nekoHWSound::OnCatching()
	{
		mLockSeciton.Begin();

		if(mHandle != NULL)
		{
			mState = EHRS_LOADED;
			mLockSeciton.End();
			return S_OK;
		}
		
		char8 *data;
		int32 length;

		if(FAILED(GetNekoNovel()->GetFileSystem()->LoadData(mName.c_str(), &data, &length)))
		{
			LogPrint(LOG_LEVEL_WARN, "'%s' ���� ������ �ε��� �� �����ϴ�.", mName.c_str());
			mState = EHRS_NONE;
			mLockSeciton.End();
			return (mLastReturnCode = E_NOT_EXIST);
		}

		File *filePtr = hidden::AdrCreateMemoryFile(data, length);
		delete []data;

		mHandle = OpenSoundEffect(mResManager->GetSoundDriver()->GetHandle(), filePtr, MULTIPLE);
		
		//mHandle = OpenSound(mResManager->GetSoundDriver()->GetHandle(), filePtr, true);

		if(!mHandle)
		{
			mState = EHRS_NONE;

			mLastReturnCode = E_FAIL;
			mLockSeciton.End();

			LogPrint(LOG_LEVEL_WARN, "'%s' ���� �ε� ����", mName.c_str());
			return (mLastReturnCode = E_FAIL);
		}

		// ���� Audiere�� �ַ� Auto pointer�� �ޱ� ������ �� ref�� ���־���Ѵ�.
		mHandle->ref();
		mHandle->setVolume(mVolume);
		if(mPlayWhenLoad)
			mHandle->play();

		mLockSeciton.End();
		return nekoIHWResource::OnCatching();
	}

	result nekoHWSound::OnDropping()
	{
		mLockSeciton.Begin();

		if(mHandle == NULL)
		{
			mState = EHRS_NONE;
			mLockSeciton.End();
			return S_OK;
		}

		mHandle->stop();
		mHandle->unref();

		mHandle = NULL;

		mLockSeciton.End();
		return nekoIHWResource::OnDropping();
	}

	/// ���� ��Ź!
	void nekoHWSound::Stop()
	{
		if(mHandle)
			mHandle->stop();

		mPlayWhenLoad = false;
	}

	/// ��� ��Ź!
	void nekoHWSound::Play()
	{
		if(mHandle)
			mHandle->play();

		if(SUCCEEDED(mLastReturnCode))
			Refresh();
	}

	void nekoHWSound::SetVolume(float32 volume)
	{
		mLockSeciton.Begin();

		mVolume = volume;

		if(mHandle)
			mHandle->setVolume(volume);
		mLockSeciton.End();
	}
}