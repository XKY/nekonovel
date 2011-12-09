#include "StdAfx.h"
#include "VarListForm.h"

namespace NekoMimi
{
	class VarTraceEventListener : public nekoIEventListener
	{
	public:


		/// �̺�Ʈ ������
		bool OnEvent(const nekoEvent &event)
		{
			if(event.type == EVENT_VARIABLE_CHANGED)
			{
				VarListForm::singletonForm->OnVariableChanged(event);
				return true;
			}

			return false;
		}
	};

	static VarTraceEventListener varTraceListener;



	System::Void VarListForm::VarListForm_Load(System::Object^  sender, System::EventArgs^  e)
	{
		 if(singletonForm)
		 {
			 this->Close();
			 return;
		 }

		 singletonForm = this;

		 UpdateAllVariables();
		 GetNekoNovel()->GetCurrentGame()->AddFlags(GAME_FLAG_VARTRACE);
		 GetNekoNovel()->GetCurrentGame()->AddEventListener(&varTraceListener);
	}
	System::Void VarListForm::VarListForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) 
	{
		 if(singletonForm == this)
		 {
			 GetNekoNovel()->GetCurrentGame()->RemoveEventListener(&varTraceListener);
			 GetNekoNovel()->GetCurrentGame()->RemoveFlags(GAME_FLAG_VARTRACE);

			 singletonForm = nullptr;
		 }
	}

	void VarListForm::OnVariableChanged(const nekoEvent &event)
	{
		// �������� �����ϰ� ��� ����..
		if(event.value[0] == 0 && event.value[1] == 0)
		{
			UpdateAllVariables();
			return;
		}

		// ����Ʈ���� ã��
		ListViewItem ^item = this->lstVariables->FindItemWithText(ConvertToString(event.GetText(0)));

		// ���� ���� ��û..
		if(event.value[1] == 0)
		{
			if(item == nullptr)
				return;

			this->lstVariables->Items->Remove(item);
		}
		// ���� ���� ���� ��û.
		else
		{
			// ������ �߰�!
			if(item == nullptr)
			{
				item = this->lstVariables->Items->Add(ConvertToString(event.GetText(0)));
				item->SubItems->Add("");
				item->SubItems->Add("");
			}

			nekoVariable *var = (nekoVariable *)event.value[1];
			item->SubItems[1]->Text = ConvertToString(var->GetTypeString());
			item->SubItems[2]->Text = ConvertToString(var->GetValue());
		}
	}

	void VarListForm::UpdateAllVariables()
	{
		this->lstVariables->Items->Clear();

		nekoGame *currentGame = GetNekoNovel()->GetCurrentGame();

		// ���� ��� �ʱ�ȭ �ϱ�
		map<string, nekoVariable> &lst = currentGame->GetVariableList();
		map<string, nekoVariable>::iterator iter = lst.begin();
		for(;iter != lst.end();++iter)
		{
			ListViewItem ^item = this->lstVariables->Items->Add(ConvertToString((*iter).first.c_str()));
			item->SubItems->Add(ConvertToString((*iter).second.GetTypeString()));
			item->SubItems->Add(ConvertToString((*iter).second.GetValue()));
		}

	}

}