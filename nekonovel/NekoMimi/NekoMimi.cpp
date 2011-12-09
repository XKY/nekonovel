// NekoMimi.cpp : �⺻ ������Ʈ �����Դϴ�.

#include "stdafx.h"
#include "MainForm.h"
#include "GamePublishForm.h"

using namespace System::Threading;
using namespace NekoMimi;

class MainEventListener : public nekoIEventListener
{
public:


	/// �̺�Ʈ ������
	bool OnEvent(const nekoEvent &event)
	{
		if(event.type == EVENT_LOG)
		{
			frmMain::OnLogReceived(event);
			return true;	
		}
		else if(event.type == EVENT_PUBLISHING_STATUS)
		{
			GamePublishForm::OnStatusUpdated(event);
			return true;
		}
		// ������ �����.
		else if(event.type == EVENT_ENGINE_FREEZED)
		{
			frmMain::self->SaveEmergencyFiles();
			return true;
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
	MainEventListener mainListener;
	nekoNovel->AddEventListener(&mainListener);

	// �� â�� ����� �����մϴ�.
	//Application::Run();
	frmMain ^frm = gcnew frmMain();
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
