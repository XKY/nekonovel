// ===============================================================
//  nekoCommands - 2009.7.6
// ===============================================================
// - Purpose : 
// ---------------------------------------------------------------

#ifndef __nekoCommands_h__
#define __nekoCommands_h__

#undef _HAS_ITERATOR_DEBUGGING
#define _HAS_ITERATOR_DEBUGGING 0
/*#undef  _SECURE_SCL
#define _SECURE_SCL 0
#undef  _SECURE_SCL_THROWS
#define _SECURE_SCL_THROWS 1*/
#include <list>
using namespace std;
#include "nekoTimer.h"
#include "nekoIRunnable.h"
#include "nekoIEventListener.h"
#include "nekoLockSection.h"
#include "nekoSaveSpot.h"

namespace neko
{
#	define MAX_COMMAND_SIZE 20

	enum E_COMMAND_TYPE
	{
		ECT_INPUT_WAIT = 0,
		// ��� [�̸�]

		ECT_SOUND,
		// ȿ���� [�̸�] [���ϸ�]
		// �ش� ȿ������ ����Ѵ�.

		ECT_BGMUSIC,
		// ������� [���ϸ�] [�ݺ�Ƚ��=1]
		// �ش� ��������� ����Ѵ�. �ݺ�Ƚ���� -1�̸� ������ ����Ѵ�.

		ECT_PLAY,
		// ��� [�̸�]
		// �����Ǿ��ִ� ���� ����Ѵ�.

		ECT_PAUSE,
		// �Ͻ����� [�̸�]
		// �ش� ��ü�� �Ͻ����� �Ѵ�.

		ECT_STOP,
		// ���� [�̸�]
		// �ش� ��ü�� �����Ѵ�.

		ECT_SET_VARIABLE,
		// ���� �����̸� [=] ��
		// �ش� ������ ���� �Է��Ѵ�.
		// ���� �̸��� @ ���� ������ ��� ���������� �ν��Ѵ�.
		// �����ڿ��� = += -= /= *= �� �ִ�.
		// �������� �Է��Ϸ��� [��] �κп� ����1~7 �� �Է��Ѵ�. ������ ����.

		ECT_MOVIE,
		// ������ [��ü�̸�] [�����̸�] [����ð�=-1]
		// �ش� �������� ����Ѵ�. ����ð��� -1�̸�, ������ ����Ѵ�. 

		ECT_FADEIN,
		// ���̵��� [��ü�̸�] [�ҿ�ð�]
		// �ش� ��ü�� ���̵� �� �Ѵ�.

		ECT_FADEOUT,
		// ���̵�ƿ� [��ü�̸�] [�ҿ�ð�]
		// �ش� ��ü�� ���̵� �ƿ� �Ѵ�.

		ECT_ROTATE,
		// ȸ�� [��ü�̸�] [����] [�ӵ�=0] [���ӵ�=0]
		// �ش� ��ü�� ȸ���Ѵ�.

		ECT_THREAD_START,
		// ���������

		ECT_THREAD_WAIT,
		// ��������,

		ECT_THREAD_DECLARE,
		// ������ [�̸�]

		ECT_CG,
		// CG [�̸�] [�̹������ϸ�=""] [X] [Y]
		// �ش� �̹����� ĳ���͸� ǥ���Ѵ�.

		ECT_SPRITE,
		// ��������Ʈ [�̸�] [�̹���1] [�̹���2] [�̹���3] ...
		// �ش� �̸��� ���ο� ��������Ʈ�� �����.

		ECT_GIF_SPRITE,
		// GIF��������Ʈ [�̸�] [�̹����̸�] [X] [Y]
		// �ش� �̸��� GIF ��������Ʈ�� �����.

		ECT_BOOKMARK,
		// �ϸ�ũ �̸�

		ECT_TIMER,
		// Ÿ�̸� [�̸�] [�ð�] [Ÿ�̸ӿϷ��� ȣ���� ����] [�̵��� �ϸ�ũ�̸�]
		// �ش� �̸��� Ÿ�̸Ӹ� �����.

		ECT_PRINTPOS,
		// ������ġ [X] [Y]

		ECT_TEXT_SPEED,
		// ���ӵ� [��]
		// ��� ��� �ӵ��� �����Ѵ�.
		// �ϳ��� ���ڰ� ��µǴµ� �ɸ��� �ð�(1/1000��)
		
		ECT_TEXT_COLOR,
		// ������ [R] [G] [B] [A]
		// ����� ������ �����Ѵ�.

		ECT_NEW_LINE,
		// ������
		// ���â���� �����ٷ� �Ѿ��.

		ECT_TEXT_SIZE,
		// ���ũ�� [��]
		// ��µ� ����� ũ���̴�.

		ECT_TEXT_FONT,
		// �����Ʈ [�̸�]
		// ��µ� ����� ��Ʈ�� �����Ѵ�.

		ECT_CLEAR_TEXT,
		// ��������
		// ��µ� ��� ��縦 �����.

		ECT_MOVE,
		// �̵� [�̸�] [X] [Y] [�ӵ�=0] [���ӵ�=0]
		// ��µ� ���� �ű��.
		// �ӵ��� 1px �̵��ϴµ� �ɸ��� �ð����� 1/1000�� �̴�.

		ECT_TEXT_SKIN,
		// ���â��Ų [�����̸�]
		// ���â�� ��Ų�� �����Ѵ�.

		ECT_TEXT_SHADOW_COLOR,
		// ���׸��ڻ��� [R] [G] [B] [A=255]
		
		ECT_TEXT_SHADOW_TYPE,
		// ���׸��� [��]
		// 0~4���� ���� ����
		
		ECT_TEXT_WINDOW_RECT,
		// ���â���� [X] [Y] [����ũ��] [����ũ��]

		ECT_TEXT_PRINTING_RECT,
		// �����¿��� [X] [Y] [����ũ��] [����ũ��]

		// �ڡڡڡ�

		ECT_APTEXT,
		ECT_TEXT,
		// ��� ��¼����¼�� ����~~
		// ����ձ� ��¼����¼�� ����~

		ECT_DELAY,
		// ������ [��]
		// 1/1000�� ������ �����̸� �ش�.

		ECT_JUMP,
		// ���� [�����̸�] [�ϸ�ũ�̸�]
		// �ش� ������ �ϸ�ũ�� �̵��Ѵ�.
		// �ϸ�ũ�̸��� ù�� �̸� ù�ٷ� �̵��Ѵ�.
		// ���� [�ϸ�ũ�̸�]
		// ���� ���� ������ �ش� �ϸ�ũ�� �̵��Ѵ�.

		ECT_SELECTION_COLOR,
		// ���������� [R] [G] [B] [A=255]

		ECT_SELECTION,
		// ������ [�����̸�] [�ϸ�ũ�̸�] [������ ����]

		ECT_SYSTEM_MENU_ENABLE,
		// �ý��۸޴�Ȱ��ȭ [��/�ƴ�]

		ECT_CLEAR_ALL,
		// ��������
		// ���� ���̴� ��� �͵��� �����Ѵ�.

		ECT_EXIT,
		// ���α׷��� �����Ѵ�.

		ECT_SCALE,
		// ũ�� [�̸�] [���ι��] [���ι��] [�ӵ�=0]

		ECT_SKIP_START,
		// ��ŵ����
		// ESCŰ�� ����Ű�� ��ŵ ����.

		ECT_SKIP_END,
		// ��ŵ��

		ECT_GET_TEXT,
		// �Է� [������] [X] [Y] [����ũ��] [����ũ��] [��ư �̹���] [���콺 �ø� ȿ����] [�������� ȿ����] [��Ʈũ��=-1] [��Ʈ����=-1] [��Ʈ�̸�=""]

		ECT_MACRO_START, // ������
		// ��ũ�ν��� [�̸�]
		// ��ũ�θ� �����Ѵ�.

		ECT_FUNC_EXECUTE,
		// �Լ� [�̸�] [�μ�1] [�μ�2] [�μ�3] ...
		// �μ��� ������ �Լ��� �����Ѵ�.

		ECT_BUTTON_WAIT,
		// ��ư���

		ECT_BUTTON,
		// ��ư [X] [Y] [�ʺ�] [����] [Ÿ��] ... [����] [���콺�� �÷������� ��] [���콺�� ������ ���� ��] [���콺�� �÷��� �� ���� ȿ����] [������ �� ȿ����]
		// [Ÿ��]���� ������ ���� �ִ�.
		// ���� [���ϸ�] [�ϸ�ũ��]
		// ��ũ [���ͳ��ּ�]
		// ����
		// ����� [���ϸ�]
		// ȿ���� [���ϸ�]
		// ��ũ��Ʈ [���ϸ�]
		// ����

		//ECT_MAP_BUTTON,
		// ������ư [CG�̸�] [X] [Y] [�ʺ�] [����] [���콺�� �÷��� ������] [���콺�� ������ ���� ��] [���콺�� �÷����� ���� ȿ����] [������ �� ȿ����]
		
		ECT_TRACKBAR,
		// Ʈ���� [X] [Y] [����ũ��] [����ũ��] [Ÿ��] ... [�� �̹���]
		// ���� [�ִ밪] [������]
		// ���̽�
		// ȿ����
		// �����
		// ���â����

		ECT_SCRIPT,
		// ��ũ��Ʈ [���ϸ�]
		// ��ũ��Ʈ�� �ҷ��´�.

		ECT_BGFADEIN,
		// ������̵��� [ȿ����ȣ] [�ӵ�=1000]
		// 1 ���̵�
		// 2 ��
		// 3 Ŀư.
		// 4 ���ʺ��� ���̵�
		// 5 �����ʺ��� ���̵�
		// 6 ���ʺ��� ���̵�
		// 7 �Ʒ��ʺ��� ���̵�

		ECT_BGFADEOUT,
		// ������̵�ƿ� [ȿ����ȣ] [�ӵ�=1000]
		// 1 ���̵�
		// 2 ��
		// 3 Ŀư
		// 4 ���ʺ��� ���̵�
		// 5 �����ʺ��� ���̵�
		// 6 ���ʺ��� ���̵�
		// 7 �Ʒ��ʺ��� ���̵�

		ECT_BACKGROUND,
		// ���

		ECT_FACECG,
		// ���̽�CG

		ECT_SCREENCOLOR,
		// ȭ����� [R] [G] [B] [A]

		ECT_BOUNCE,
		// Ƣ������� [�̸�] [X1] [Y1] [X2] [Y2] [�ӵ�=1000]

		ECT_ALPHA,
		// ���� [�̸�] [���İ�]

		ECT_SYSTEMSCREEN,
		// �ý���ȭ�� [����]
		// [����]���� �ε�, ���̺� �� �ִ�.
		// ������ ������ ȭ���� ���ش�.

		ECT_SYSTEMBUTTON,
		// �ý��۹�ư [X] [Y] [�ʺ�] [����] [Ÿ��] ... [����] [���콺�� �÷��� ���� ��] [���콺�� ������ ���� ��] [���콺�� �÷��� �� ���� ȿ����] [������ �� ȿ����]
		// [Ÿ��]���� ������ �ִ�.
		// ȸ��
		// ���� [�����̸�] [�ϸ�ũ] // �����̸��� '����' �Է½� ���� ������ �ȴ�.
		// �ڵ��ѱ�� [��/�ƴ�]
		// ������ŵ [��/�ƴ�]
		// ���������� // ȭ���� �����ϴ� ��쿡��
		// ���������� // ȭ���� �����ϴ� ��쿡��
		// ���� [������] [��]
		// ȭ���� [������/Ǯ��ũ��]
		// �����긲 [��/�ƴ�]
		// �⺻��Ʈ [�̸�]
		// �׻��� [��/�ƴ�]
		// ����

		ECT_SYSTEMBUTTON_BYE,
		// �ý��۹�ư����

		ECT_VIBE,
		// ���� [�̸�] [X����] [Y����] [�ð�=1000] [�ӵ�=1000] [X����=0] [Y����=0]

		ECT_BUTTON_BYE,
		// ��ư����
		// ��µ� �Ϲ� ��ư���� ��� �����Ѵ�.

		ECT_KEYPROCESS,
		// Űó�� [Ű�ڵ�] [�����̸�] [�ϸ�ũ�̸�]
		// �ش� Ű�� ������ �ش� ��ġ�� �̵��Ѵ�.

		ECT_HIDE_ONLYCHAT,
		// ��縸���߱� [��/�ƴ�]

		ECT_IF,
		// ���� [A] [=] [B] [�����̸�] [�ϸ�ũ�̸�] [�ƴϸ�] [�����̸�] [�ϸ�ũ�̸�]
		// [���]���� ������ �ִ�.
		//  [���ϸ�] [�ϸ�ũ�̸�]

		ECT_OMAKE_ADD,
		// �������߰� [�̸�] [�����̸�]

		ECT_OMAKE_CG,
		// ������CG [�̸�] [X] [Y] [����ũ��] [����ũ��] [������ ǥ���� ����]
		// �ش� �̸��� ������ �̹����� �ҷ��� ǥ�����ݴϴ�.
		// ǥ�õ� �����ɴ� CG�� ���� ����� �մϴ�.

		ECT_GAME_STATE,
		// ���ӻ��� [Ÿ��]
		// �ڵ��ѱ��
		// ������ŵ

		ECT_SET_EVENT,
		// �̺�Ʈ [Ÿ��] [�����̸�] [�ϸ�ũ�̸�]
		// [Ÿ��]���� ������ �ִ�.
		// �ε���
		// ���̺���

		ECT_VOICE,
		/// ���̽� [�����̸�]
		/// ������ ��Ҹ��� ����Ѵ�.
		/// 

		ECT_TEXT_LINEINTERVAL,
		/// ����ٰ��� [�ȼ�]
		/// ����ٰ����� �����մϴ�.
		/// ������ ���� �� �ֽ��ϴ�.

		ECT_PRINTING_ORDER,
		/// ��¼��� [�̸�] [��]
		/// 0~200���� ��� ������ ���մϴ�.
		/// CG�� ��ɾ�� �ΰ��� �׸��� ������� �� �յ� ��ġ�� Ȯ���� ���ֱ� ���� ����մϴ�.
		/// �⺻���� 100�̰�, �������� ������ ���ɴϴ�.

		ECT_LABEL,
		/// �� [�̸�] [X] [Y] [R=255] [G=255] [B=255] [A=255] [�ؽ�Ʈ]
		/// ȭ�鿡 �ؽ�Ʈ�� ����մϴ�.
		/// ������ {{�����̸�}}�� ����մϴ�.

		ECT_CAMERA,
		/// ī�޶� [�ӵ�] [���ӵ�] [��ǥX] [��ǥY] [�ʴ� ����������] [Ȯ�����X] [Ȯ�����Y] [�ʴ� ����������] [ȸ������]

		ECT_HIDE_CHATBOX,
		/// ���â���߱� [�ð�]
		/// ��縦 �����, ���â�� ����ϴ�. �ð��� 1/1000�� �����̸�, �Է����� ������ �ٷ� ���߾����ϴ�.

		ECT_SHOW_CHATBOX,
		/// ���â���̱� [�ð�]
		/// ���â�� �����ݴϴ�. �ð��� 1/1000�� �����̸�, �Է����� ������ �ٷ� �������ϴ�.

		ECT_TEXT_BOLD,
		/// ��籽�� [��,�ƴ�]
		/// ��� �۾��� ���� �Ѵ�.

		ECT_FLASH,
		/// �÷��� [�̸�] [�����̸�] [X] [Y] [����ũ��] [����ũ��]
		///
		/// �÷��ø� ȭ��� ����մϴ�.
		///
		/// ��)
		/// // �ҷ�����
		/// �÷��� �����䳢 �����䳢.swf 0 0 800 600
		///
		/// // ���ֱ�
		/// �÷��� �����䳢

		ECT_BG_RULE_FADEIN,
		/// �������̵��� [�����̸�] [�ӵ�]

		ECT_BG_RULE_FADEOUT,
		/// �������̵�ƿ� [�����̸�] [�ӵ�]

		ECT_PARTICLE,
		/// ��ƼŬ [�̸�] [�����̸�]

		ECT_SELECTION_BUTTON,
		/// ��������ư [����] [���콺�� �÷������� ��] [���콺�� ������ ���� ��] [���콺�� �÷��� �� ���� ȿ����] [���콺�� ������ �� ���� ȿ����] [�����̸�] [�ϸ�ũ�̸�] [����]

		ECT_SELECTION_BUTTON_RECT,
		/// ��������ư���� [X] [Y] [����ũ��] [����ũ��]

		ECT_LABEL_PROPERTY,
		/// �󺧼Ӽ� [�̸�] [��Ʈũ��] [����] [��Ʈ�̸�]
		/// ���� �۾� �Ӽ��� �����մϴ�.

		ECT_CLEAR_RECOLLECTION,
		/// ȸ���ʱ�ȭ
		/// ȸ�� �ִ� ������ ��� ����ϴ�.

		ECT_IGNORE_CAMERA,
		/// ī�޶󹫽�
		/// ī�޶� ���� ���θ� �����մϴ�.

		ECT_VARIABLE_TRIGGER,
		// ����Ʈ���� [�����̸�] [�����̸�] [�ϸ�ũ�̸�]
		// ������ ���� �ٲ�� �ش� ��ũ��Ʈ�� �����ϰ�, �ٽ� ���ƿɴϴ�.

		ECT_COLOR,
		// ���� [�̸�] [R] [G] [B] [�ҿ�ð�=0]
		// ĳ������ ������ ������ �� �ֽ��ϴ�.
		
		ECT_TEXT_ALIGN,
		// �ؽ�Ʈ���� [���]
		// [���] ���� ����, ���, ���2, ������ �� ������, ������ ��� �������� �����˴ϴ�.

		ECT_CG_FADEIN,
		// CG���̵��� [�ӵ�=0]
		// CG ��ɾ� ���� �ڵ������� ���̵��� ��ŵ�ϴ�.
		// 0�� ���, CG�� ���̵��� ���� �ʰ� �ٷ� ��Ÿ���ϴ�.

		ECT_CG_FADEOUT,
		// CG���̵�ƿ� [�ӵ�=0]
		// 0�� ���, CG�� ���̵�ƿ� ���� �ʰ� �ٷ� ���۴ϴ�.

		ECT_TEXT_FADE,
		// ������̵� [��/�ƴ�]
		// ������̵� ����� �Ѱ� �� �� �ֽ��ϴ�.

		ECT_SYSTEM_OPTION,
		// �ý��ۿɼ� [�ɼǸ�] [��]
		// �ý��ۿɼ��� ������ �� �ֽ��ϴ�.
		// [�ɼǸ�]���� ������ �ֽ��ϴ�.
		// ���ѱ�� [��/�ƴ�] : ���콺 Ŭ���� ��簡 �ѹ��� �Ѿ�� ����� �Ѱ� ���ϴ�.
		// �̹�������ŵ [��/�ƴ�] : ����ŵ�� �̹��� � ������ ��ŵ�� �� �ֵ��� �մϴ�.


		ECT_SET_ONE_VARIABLE,
		// ������
		// ������ ����� ������, �������� ���� ��쿡�� �����ϴ� �����̴�.

		ECT_INITIALIZE_RECOLLECTION,
		// ȸ��ù���� [����ũ��] [����ũ��] [��Ʈũ��] [��Ʈ�̸�]

		ECT_RECOLLECTION_TEXT,
		// ȸ�� [�̸�] [X] [Y] [��ũ�ѹ�Ÿ��=1,2] [��ũ�ѹٹ�ư����] [��Ų����]

		ECT_RECOLLECTION_SETTING,
		// ȸ���� [��/��簣 ����] [������ �� ����] [�� �ٻ���=0] [R] [G] [B]
		// �Է����� ������ �⺻ ������ �����մϴ�.

		ECT_RECOLLECTION_COLOR,
		// ȸ����� [R] [G] [B]
		// �Է����� ������ �⺻ ��������..

		ECT_MSGBOX,
		// �޼����ڽ� [�۱�]

		ECT_MSGBOX_WAIT,
		// �޼����ڽ����

		ECT_INPUT_WAIT2,
		// �����
		// ���� ������ ���

		ECT_IW2_USE,
		// ������� [��/�ƴ�]

		ECT_QUICK_LOAD,
		// �����̺��� ������ �ε��մϴ�.

		ECT_QUICK_SAVE,
		// ���� ���� ���� ��Ȳ�� ���̺� �մϴ�.

		ECT_THREAD_TERMINIATE,
		// ���������� [�̸�]
		// �ش� �̸��� �����带 ���� ��ŵ�ϴ�.

		ECT_NOTHING,
		ECT_UNKNOWN,
		ECT_COUNT
	};

	enum E_EDITOR_COLOR_TYPE
	{
		EECT_BACKCOLOR = 0,
		EECT_1 = 1,
		EECT_10 = 10,
		EECT_COMMENT,
		EECT_ENABLED, // ������ ����ϴ°� ? 0 or 1
		EECT_COUNT,
	};

	struct nekoThread;
	struct NEKONOVEL_API nekoOneCommand
	{
		E_COMMAND_TYPE cmdType;
		uint32 startLine;
		list<string> arguments;

		nekoOneCommand() {}
		virtual ~nekoOneCommand() {}

		void Begin(const list<nekoOneCommand *>::iterator &selfIter, nekoThread *thread, bool internalRequest);
		bool Update(nekoThread *cmds, float32 frameTime);
		void End(const list<nekoOneCommand *>::iterator &selfIter, nekoThread *cmds, bool internalRequest);
		bool OnEvent(nekoThread *thread, const nekoEvent &event);

		// ������ �ε��Ǿ��� ��
		void OnGameLoaded(nekoThread *cmds);
	};

	class NEKONOVEL_API nekoGame;
	class NEKONOVEL_API nekoCommands : public nekoIEventListener
	{
	protected:

		enum
		{
			LinesPerVisualSpot = 100,
		};

		string mName;
		nekoLockSection mLockSection;
		list<nekoOneCommand *> mCommandList;
		list<nekoSaveSpot> mVisualSpotList;

		/// �ش� �������� �̵�
		/// internalRequest�� true�̸� ��ɾ�� �ܼ���
		/// ���� ó���� ���ؼ��� ���ȴ�.
		void _GotoLine(nekoThread *thread, bool internalRequest, uint32 lineNumber);

		friend class nekoGame;

	public:
		
		/// Constructor
		nekoCommands(const char8 *name);
		
		/// Destructor
		~nekoCommands();

		/// �̸� ���
		inline const char8 *GetName() const { return mName.c_str(); }

		/// ���Ϸ� ���� ��ũ��Ʈ ��������
		result LoadFromFile(const char8 *fileName);

		//////////////////////////////////////
		/// ���� ���� ��带 ���� �޼ҵ�
		//////////////////////////////////////
		result OnLineUpdated(uint32 lineNumber, const char8 *message); /// ������ �����.
		result OnLineRemoved(uint32 startLine, uint32 lineCount); /// �� ������ �����Ǿ���.
		result OnLineInserted(uint32 lineNumber, const char8 *message); /// �� ������ �ԷµǾ���.
		void UpdateLineCount(uint32 lineCount);

		/// �� Ŀ�ǵ� �߰�
		result AddCommand(uint32 lineNumber, const char8 *message);

		/// ���� Ŀ�ǵ� ����
		result UpdateCommand(uint32 lineNumber, const char8 *message);

		/*
		/// ���־� ���� �߰�
		result AddVisualSpot(uint32 lineNumber);
		*/

		/// Ŀ�ǵ� Ÿ�� ���
		static result GetCommandType(const char8 *message, E_COMMAND_TYPE *cmdType, list<string> &arguments);

		/*
		/// �� �ɱ�
		inline void Lock()
		{
			mLockedCount++;
		}*/

		/*inline void Unlock() { mLockedCount = max(mLockedCount - 1, 0); }
		inline bool IsLocked() { return (mLockedCount > 0); }*/

		inline list<nekoOneCommand *>::iterator GetStartingPos() { return mCommandList.begin(); }
		inline list<nekoOneCommand *>::iterator GetEndingPos() { return mCommandList.end(); }
		inline list<nekoOneCommand *> &GetCommandList() { return mCommandList; }

		list<nekoOneCommand *>::iterator GetPosByLine(uint32 lineNum, bool findAround = false);
		list<nekoOneCommand *>::iterator SearchFrom(list<nekoOneCommand *>::iterator from, E_COMMAND_TYPE cmdType);
		result SearchBookMark(const char8 *name, list<nekoOneCommand *>::iterator &itOut);

		bool OnEvent(nekoThread *thread, const nekoEvent &event);

		static E_COMMAND_TYPE GetCommandByName(const char8 *name);
		static const char8 *GetCommandString(int32 i);
		static const char8 **GetCommandHelpStrings();

		static result LoadCommandReplacements(uint32 editorColors[] = 0, uint32 commandColors[] = 0);

		inline bool IsVoicePlaying()
		{
			//if(mVoice)
			return false;
		}
	};
}


#endif // __nekoCommands_h__