#include "nekoPrecompiledHeader.h"
#include "nekoHWMusic.h"
#include "nekoLogger.h"
#include "nekoHWResourceManager.h"
#include <audiere.h>
#include "NekoNovel.h"
#include "nekoFileSystem.h"

namespace neko
{
	class nekoAudiereListener : public RefImplementation<StopCallback>
	{
	protected:

		nekoHWMusic *mMusic;

	public:

		nekoAudiereListener(nekoHWMusic *music)
			: mMusic(music)
		{
			GetNekoNovel()->GetSoundDriver()->GetHandle()->registerCallback(this);
		}

		virtual ~nekoAudiereListener()
		{
			GetNekoNovel()->GetSoundDriver()->GetHandle()->unregisterCallback(this);
		}

		ADR_METHOD(void) streamStopped(StopEvent* event)
		{
			if(event->getReason() == StopEvent::STREAM_ENDED
				&& event->getOutputStream() == mMusic->GetHandle())
				mMusic->OnEnded();
		}
	};

	nekoHWMusic::nekoHWMusic(nekoHWResourceManager *resMgr, const char8 *fileName, uint32 repeat, float32 volume)
		: nekoIHWResource(resMgr, EHRT_MUSIC)
		, mHandle(NULL)
		, mVolume(volume)
		, mRepeatCount(repeat)
		, mPlayWhenLoad(true)
		, mListener(0)
	{
		Catch();
		mName = fileName;
		mSharable = false;
		nekoUtility::TrimRight(mName);
	}

	/// Destructor
	nekoHWMusic::~nekoHWMusic()
	{
		if(mHandle)
		{
			//nekoLogger::Log(LOG_LEVEL_DINFO, "������� '%s' �޸� ��ε�", mName.c_str());

			mHandle->stop();
			mHandle->unref();
		}

		if(mListener)
			mListener->unref();
	}

	/// On Device Lost
	void nekoHWMusic::OnDeviceLost()
	{

	}

	/// On Device Reset
	void nekoHWMusic::OnDeviceReset()
	{

	}

	/// ������ �ε�/��ε带 �õ��ϴ� �޼ҵ�
	result nekoHWMusic::OnCatching()
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

		mHandle = OpenSound(mResManager->GetSoundDriver()->GetHandle(), filePtr, true);

		if(!mHandle)
		{
			LogPrint(LOG_LEVEL_WARN, "'%s' ���� �ε� ����", mName.c_str());
			mState = EHRS_NONE;
			mLockSeciton.End();
			return (mLastReturnCode = E_FAIL);
		}

		// ���� Audiere�� �ַ� Auto pointer�� �ޱ� ������ �� ref�� ���־���Ѵ�.
		mHandle->ref();

		if(mPlayWhenLoad)
			mHandle->play();
		mHandle->setVolume(mVolume);

		if(mRepeatCount > 1)
		{
			if(!mListener)
				mListener = nekoNew nekoAudiereListener(this);
		}

		mLockSeciton.End();

		return nekoIHWResource::OnCatching();
	}

	result nekoHWMusic::OnDropping()
	{
		mLockSeciton.Begin();

		if(mHandle == NULL)
		{
			mState = EHRS_NONE;
			mLockSeciton.End();
			return S_OK;
		}

		if(mHandle->isPlaying())
			mHandle->stop();
		mHandle->unref();
		mHandle = NULL;

		if(mListener)
		{
			mListener->unref();
			mListener = 0;
		}

		mLockSeciton.End();
		return nekoIHWResource::OnDropping();
	}

	/// �ݺ� ����
	void nekoHWMusic::SetRepeat(uint32 repeat)
	{
		mLockSeciton.Begin();

		mRepeatCount = repeat;

		if(mHandle)
		{
			if(mRepeatCount > 1)
			{
				if(!mListener)
					mListener = nekoNew nekoAudiereListener(this);
			}
			else
			{
				if(mListener)
					mListener->unref();
				mListener = 0;
			}
		}

		mLockSeciton.End();
	}

	/// ���� ����
	void nekoHWMusic::SetVolume(float32 volume)
	{
		mLockSeciton.Begin();

		mVolume = volume;

		if(mHandle)
			mHandle->setVolume(volume);

		mLockSeciton.End();
	}

	/// ������ΰ� ? Ȥ��, ����� �����ΰ� ?
	bool nekoHWMusic::IsPlaying()
	{
		return ((!mHandle && mPlayWhenLoad && SUCCEEDED(mLastReturnCode)) || (mHandle && mHandle->isPlaying()));
	}

	/// ���� ��Ź!
	void nekoHWMusic::Stop(bool pause)
	{
		mLockSeciton.Begin();

		if(mHandle)
		{
			if(!pause)
			{
				mHandle->reset();
				mHandle->stop();
			}
			else 
				mHandle->stop();
		}
		else
		{
			mPlayWhenLoad = false;
		}

		mAutoDropping = true;

		mLockSeciton.End();
	}

	/// ��� ��Ź!
	void nekoHWMusic::Play()
	{
		mLockSeciton.Begin();

		if(mHandle)
			mHandle->play();
		else
			mPlayWhenLoad = true;

		mLockSeciton.End();
	}

	void nekoHWMusic::UpdateInThread(float32 frameTime)
	{
		mLockSeciton.Begin();

		if(mHandle && mHandle->isPlaying())
			Refresh();

		mLockSeciton.End();

		nekoIHWResource::UpdateInThread(frameTime);
	}

	/// �÷��� �����!
	void nekoHWMusic::OnEnded()
	{
		mLockSeciton.Begin();
		if(mHandle == 0)
		{
			mLockSeciton.End();
			return;
		}

		if(mRepeatCount > 1)
		{
			mHandle->play();
			--mRepeatCount;
		}

		mLockSeciton.End();
	}
}