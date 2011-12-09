// ===============================================================
//  nekoTrackBar - 2009.7.18
// ===============================================================
// - Purpose : 
// ---------------------------------------------------------------

#ifndef __nekoTrackBar_h__
#define __nekoTrackBar_h__


#include "nekoIBlock.h"
#include "nekoIBlockInstance.h"
#include "nekoHWTexture.h"
#include "nekoHWSound.h"

namespace neko
{
	// * 
	enum E_TRACK_BAR_INSTANCE_TYPE
	{
		ETBIT_BGM = 0,
		ETBIT_FX,
		ETBIT_VOICE,
		ETBIT_TEXTWINDOW,

		ETBIT_VARIABLE,
		ETBIT_SUB_TEXTSPEED,
		ETBIT_AUTO_DELAY,
		ETBIT_NOTHING,
	};

	class nekoTrackBarInstance : public nekoIBlockInstance
	{
	protected:

		int32 mMaxValue;
		int32 mValue;
		nekoAutoPtr<nekoHWTexture> mBarTexture;
		E_TRACK_BAR_INSTANCE_TYPE mTrackBarType;

		/// Ʈ���� ���� ������ ����
		string mVarName;

	public:
		
		nekoTrackBarInstance(nekoIBlock *baseBlock);
		~nekoTrackBarInstance();
		
		inline void SetMaxValue(int32 value) { mMaxValue = value; }
		inline void SetValue(int32 value) { mValue = min(max(value, 1), mMaxValue); }
		void SetBarTexture(const string &fileName);
		inline void SetTrackBarVariable(const string &varName) { mVarName = varName; }

		void SetTrackBarType(E_TRACK_BAR_INSTANCE_TYPE type);

		void OnValueUpdated();

		void Update(float32 frameTime);
		void Draw();

		bool OnEvent(const nekoEvent &event);

		/// ������ �����ϱ�
		result Dump(int32 version, nekoByteBuffer &outBuf);
		result Load(int32 version, nekoByteBuffer &inBuf);
	};


	class nekoTrackBar : public nekoIBlock
	{
	protected:

	public:

		nekoTrackBar();
		virtual ~nekoTrackBar();

		/// �ν��Ͻ��� �����
		virtual nekoIBlockInstance *CreateInstance();

	};
}


#endif // __nekoTrackBar_h__