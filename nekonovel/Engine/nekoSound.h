// ===============================================================
//  nekoSound - 2009.7.6
// ===============================================================
// - Purpose : 
// ---------------------------------------------------------------

#ifndef __nekoSound_h__
#define __nekoSound_h__

#include "nekoIBlock.h"
#include "nekoHWMusic.h"
#include "nekoIBlockInstance.h"

namespace neko
{
	class nekoSoundInstance : public nekoIBlockInstance
	{
	protected:

		nekoAutoPtr<nekoHWMusic> mSound;

	public:

		nekoSoundInstance(nekoIBlock *block);
		virtual ~nekoSoundInstance();

		void SetSound(const char8 *fileName, uint32 repeat = -1);

		virtual void Update(float32 frameTime);
		virtual void Draw();

		void Play();
		void Stop(bool pause = false);

		void SetAlpha(int32 alpha);

		/// ������ �����ϱ�
		result Dump(int32 version, nekoByteBuffer &outBuf);
		result Load(int32 version, nekoByteBuffer &inBuf);
	};

	class nekoSound : public nekoIBlock
	{
	protected:

	public:

		nekoSound();
		virtual ~nekoSound();

		/// �ν��Ͻ��� �����
		virtual nekoIBlockInstance *CreateInstance();

	};
}


#endif // __nekoSound_h__