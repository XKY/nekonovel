// ===============================================================
//  nekoHWTexture - 2009.7.6
// ===============================================================
// - Purpose : 
// ---------------------------------------------------------------

#ifndef __nekoHWMusic_h__
#define __nekoHWMusic_h__

#include "nekoPoint.h"
#include "nekoAutoPtr.h"
#include "nekoIHWResource.h"

namespace audiere
{
	class OutputStream;
}

using namespace audiere;

namespace neko
{
	class nekoAudiereListener;
	class nekoHWMusic : public nekoIHWResource
	{
	protected:

		OutputStream *mHandle;
		float32 mVolume;
		uint32 mRepeatCount;
		bool mPlayWhenLoad;

		nekoAudiereListener *mListener;

	public:

		/// Constructor
		nekoHWMusic(nekoHWResourceManager *resMgr, const char8 *fileName, uint32 repeat, float32 volume);

		/// Destructor
		~nekoHWMusic();

		inline OutputStream *GetHandle() { return mHandle; }

		/// On Device Lost
		virtual void OnDeviceLost();

		/// On Device Reset
		virtual void OnDeviceReset();

		/// ������ �ε�/��ε带 �õ��ϴ� �޼ҵ�
		result OnCatching();
		result OnDropping();

		/// �ݺ� ����
		void SetRepeat(uint32 repeat);
		inline bool IsRepeat() { return (mRepeatCount > 1); }
		inline uint32 GetRepeatCount() { return mRepeatCount; }

		/// ���� ����
		void SetVolume(float32 volume);
		inline float32 GetVolume() { return mVolume; }

		/// ������ΰ� ? Ȥ��, ����� �����ΰ� ?
		bool IsPlaying();

		/// ���� ��Ź!
		void Stop(bool pause = false);

		/// ��� ��Ź!
		void Play();

		void UpdateInThread(float32 frameTime);

		/// �÷��� �����!
		void OnEnded();
	};
}


#endif // __nekoHWTexture_h__