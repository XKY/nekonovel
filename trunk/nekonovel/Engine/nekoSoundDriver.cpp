#include "nekoPrecompiledHeader.h"
#include "nekoSoundDriver.h"
#include "nekoLogger.h"

namespace neko
{
	/// Constructor
	nekoSoundDriver::nekoSoundDriver()
	{

	}

	/// Destructor
	nekoSoundDriver::~nekoSoundDriver()
	{
		mDevice = NULL;
	}

	/// Initialize
	result nekoSoundDriver::Initialize()
	{
		/// ����� �ʱ�ȭ
		mDevice = OpenDevice();
		if(mDevice == 0)
		{
			LogPrint(LOG_LEVEL_WARN, "���� ����̹� �ʱ�� �����Ͽ����ϴ�.");
			return E_FAIL;
		}

		LogPrint(LOG_LEVEL_INFO, "���� �ý����� ���������� ���۵Ǿ����!");

		return S_OK;
	}

	/// Update
	void nekoSoundDriver::Update(float32 frameTime)
	{
		if(mDevice == NULL)
			return;

		mLockSection.Begin();
		mDevice->update();
		mLockSection.End();
	}
}