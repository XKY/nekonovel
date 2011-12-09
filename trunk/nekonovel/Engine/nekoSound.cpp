#include "nekoPrecompiledHeader.h"
#include "nekoSound.h"
#include "NekoNovel.h"
#include "nekoHWResourceManager.h"
#include "nekoLogger.h"

namespace neko
{
	nekoSoundInstance::nekoSoundInstance(nekoIBlock *block)
		: nekoIBlockInstance(block)
	{

	}

	nekoSoundInstance::~nekoSoundInstance()
	{
		if(mSound)
			mSound->Stop();
	}

	void nekoSoundInstance::SetSound(const char8 *fileName, uint32 repeat)
	{
		string path = (string("ȿ����\\") + fileName);

		// �Ȱ��� �ž� ?
		if(stricmp(path.c_str(), fileName) == 0)
		{
			// �׷� ������ڱ�!
			if(mSound)
			{
				mSound->Play();
				return;
			}
		}

		//  ���� ���� ���� ������ ���� ���..
		if(mSound && mSound->GetRefCount() == 2)
		{
			mSound->Stop();
		}

		if(fileName != NULL)
		{
			mSound = GetNekoNovel()->GetHWResourceManager()->LoadMusic(path.c_str(), repeat
				, GetNekoNovel()->GetCurrentGame()->GetEnvironment().mDefFXVolume / 100.0f);
			if(mSound)
			{
				mAlpha = 255;
			}

			mSound->Play();
		}
		else
			mSound = NULL;
	}

	void nekoSoundInstance::Update(float32 frameTime)
	{

	}

	void nekoSoundInstance::Draw()
	{

	}

	void nekoSoundInstance::Play()
	{
		if(mSound)
			mSound->Play();
	}

	void nekoSoundInstance::Stop(bool pause)
	{
		if(mSound && !pause)
			mSound->Stop();
	}

	void nekoSoundInstance::SetAlpha(int32 alpha)
	{
		nekoIBlockInstance::SetAlpha(alpha);

		if(mSound)
			mSound->SetVolume((float32)alpha / 255.0f *
			(GetNekoNovel()->GetCurrentGame()->GetEnvironment().mDefFXVolume / 100.0f));
	}

	/// ������ �����ϱ�
	result nekoSoundInstance::Dump(int32 version, nekoByteBuffer &outBuf)
	{
		nekoIBlockInstance::Dump(version, outBuf);

		outBuf << ((mSound) ? mSound->GetName() : "");

		if(mSound)
			outBuf << mSound->GetRepeatCount();

		return S_OK;
	}

	result nekoSoundInstance::Load(int32 version, nekoByteBuffer &inBuf)
	{
		nekoIBlockInstance::Load(version, inBuf);

		string path;
		uint32 repeat = 1;
		inBuf >> path;

		if(!path.empty())
		{
			if(version >= 1127)
				inBuf >> repeat;

			mSound = GetNekoNovel()->GetHWResourceManager()->LoadMusic(path.c_str(), repeat
				, (float32)mAlpha / 255.0f *
				(GetNekoNovel()->GetCurrentGame()->GetEnvironment().mDefFXVolume / 100.0f));
			mSound->Stop();
		}

		return S_OK;
	}

	/// Constructor
	nekoSound::nekoSound()
		: nekoIBlock(BLOCK_SOUND)
	{

	}

	/// Destructor
	nekoSound::~nekoSound()
	{

	}

	/// �ν��Ͻ��� �����
	nekoIBlockInstance *nekoSound::CreateInstance()
	{
		return nekoNew nekoSoundInstance(this);
	}
}