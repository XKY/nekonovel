// ParticleTool.cpp : �⺻ ������Ʈ �����Դϴ�.

#include "stdafx.h"
#include "MainForm.h"

using namespace ParticleTool;

class MainEventListener : public nekoIEventListener
{
public:


	/// �̺�Ʈ ������
	bool OnEvent(const nekoEvent &event)
	{
		if(event.type == EVENT_LOG)
		{
			// �����!
			if(event.value[1] == (int32)LOG_LEVEL_WARN)
			{
				MessageBox::Show(MainForm::self, ConvertToString(event.GetText(0)), "���ڳ뺧 �˸�!", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
				return true;
			}
		}

		return false;
	}
};

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// ��Ʈ���� ��������� ���� Windows XP �ð� ȿ���� Ȱ��ȭ�մϴ�.
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	NekoNovel *nekoNovel = GetNekoNovel();
	//MainEventListener mainListener;
	//nekoNovel->AddEventListener(&mainListener);

	// �� â�� ����� �����մϴ�.
	//Application::Run();
	MainForm ^frm = gcnew MainForm();
	frm->Show();

	while (frm->Created)
	{
		Application::DoEvents();
		if(!nekoNovel->Update())
			break;
	}

	ReleaseNekoNovel();

	return 0;
}
