// ===============================================================
//  nekoImage - 2009.7.7
// ===============================================================
// - Purpose : 
// ---------------------------------------------------------------

#ifndef __nekoImage_h__
#define __nekoImage_h__

#include "nekoIBlock.h"
#include "nekoIBlockInstance.h"
#include "nekoHWTexture.h"

namespace neko
{
	enum E_IMAGE_TYPE
	{
		IMAGE_NORMAL = 0,
		IMAGE_FACECG,
	};
	class nekoImageInstance : public nekoIBlockInstance
	{
	protected:

		uint32 hue;
		bool mIncludeInSnapShot;
		E_IMAGE_TYPE mImageType;
		nekoAutoPtr<nekoHWTexture> mTexture;

		// �̹��� ��ȯ�� ���Ǵ� ������
		nekoAutoPtr<nekoHWTexture> mNextTexture;
		float32 mTransAlpha;

		// �ʱ� CG ���̵��� ��...
		bool mFirstFadeIn;
		bool mLastFadeOut;

	public:

		nekoImageInstance(nekoIBlock *block);
		virtual ~nekoImageInstance();

		inline void SetHue(uint32 hue) { this->hue = hue | 0xff000000; }
		inline uint32 GetHue() { return this->hue; }
		void SetImage(const char8 *fileName);

		void SetName(const char8 *name);

		void Update(float32 frameTime);
		void Draw();

		void SetIncludedInSnapShot(bool value) { mIncludeInSnapShot = value; }
		bool IsIncludedInSnapShot() { return mIncludeInSnapShot; }

		/// ������ �����ϱ�
		result Dump(int32 version, nekoByteBuffer &outBuf);
		result Load(int32 version, nekoByteBuffer &inBuf);

		/// ���� ��û
		bool RequestToRemove();

		bool IsZeroAlphaDeleting() const { return (mImageType == IMAGE_NORMAL); }
		inline bool IsDoing() { return (mNextTexture || mFirstFadeIn || mLastFadeOut); }
		inline void IgnoreAutoFadeInOut() { mLastFadeOut = mFirstFadeIn = false; }
	};

	class nekoImage : public nekoIBlock
	{
	protected:

	public:
		
		nekoImage();
		virtual ~nekoImage();

		/// �ν��Ͻ��� �����
		virtual nekoIBlockInstance *CreateInstance();
		
	};
}


#endif // __nekoImage_h__