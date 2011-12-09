// ===============================================================
//  nekoHWSingleSound - 2009.7.26
// ===============================================================
// - Purpose : 
// ---------------------------------------------------------------

#ifndef __nekoHWMultipleSound_h__
#define __nekoHWMultipleSound_h__



#include "nekoPoint.h"
#include "nekoAutoPtr.h"
#include "nekoIHWResource.h"

namespace audiere
{
	class SoundEffect;
}

using namespace audiere;

namespace neko
{
	class NEKONOVEL_API nekoHWSingleSound : public nekoIHWResource
	{
	protected:

		SoundEffect *mHandle;
		float32 mVolume;
		bool mPlayWhenLoad;

	public:

		/// Constructor
		nekoHWSingleSound(nekoHWResourceManager *resMgr, const char8 *fileName);

		/// Destructor
		~nekoHWSingleSound();

		inline SoundEffect *GetHandle() { return mHandle; }

		/// ������ �ε�/��ε带 �õ��ϴ� �޼ҵ�
		result OnCatching();
		result OnDropping();

		/// ���� ��Ź!
		void Stop();

		/// ��� ��Ź!
		void Play();

		void SetVolume(float32 volume);
		inline float32 GetVolume() { return mVolume; }
	};
}


#endif // __nekoHWTexture_h__