#pragma once

using namespace System;
using namespace System::Globalization;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace NekoMimi {

	/// <summary>
	/// NewScriptFileForm�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class NewScriptFileForm : public System::Windows::Forms::Form
	{
	public:
		NewScriptFileForm(void)
		{
			InitializeComponent();
			
			this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
		}

	protected:
		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
		/// </summary>
		~NewScriptFileForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  txtName;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  btAdd;
	private: System::Windows::Forms::Button^  btClose;
	private: System::Windows::Forms::Label^  label2;



	private:
		/// <summary>
		/// �ʼ� �����̳� �����Դϴ�.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����̳� ������ �ʿ��� �޼����Դϴ�.
		/// �� �޼����� ������ �ڵ� ������� �������� ���ʽÿ�.
		/// </summary>
		void InitializeComponent(void)
		{
			this->txtName = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->btAdd = (gcnew System::Windows::Forms::Button());
			this->btClose = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// txtName
			// 
			this->txtName->Location = System::Drawing::Point(80, 27);
			this->txtName->Name = L"txtName";
			this->txtName->Size = System::Drawing::Size(192, 21);
			this->txtName->TabIndex = 0;
			this->txtName->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &NewScriptFileForm::txtName_KeyUp);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(32, 32);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(37, 12);
			this->label1->TabIndex = 1;
			this->label1->Text = L"�̸� :";
			// 
			// btAdd
			// 
			this->btAdd->Location = System::Drawing::Point(136, 88);
			this->btAdd->Name = L"btAdd";
			this->btAdd->Size = System::Drawing::Size(88, 24);
			this->btAdd->TabIndex = 2;
			this->btAdd->Text = L"�߰�(&A)";
			this->btAdd->UseVisualStyleBackColor = true;
			this->btAdd->Click += gcnew System::EventHandler(this, &NewScriptFileForm::btAdd_Click);
			// 
			// btClose
			// 
			this->btClose->Location = System::Drawing::Point(224, 88);
			this->btClose->Name = L"btClose";
			this->btClose->Size = System::Drawing::Size(88, 24);
			this->btClose->TabIndex = 3;
			this->btClose->Text = L"â �ݱ�(&C)";
			this->btClose->UseVisualStyleBackColor = true;
			this->btClose->Click += gcnew System::EventHandler(this, &NewScriptFileForm::btClose_Click);
			// 
			// label2
			// 
			this->label2->Location = System::Drawing::Point(8, 64);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(304, 16);
			this->label2->TabIndex = 4;
			this->label2->Text = L"�� ���� : txt Ȯ���ڴ� �ڵ����� ������ ���������� ^^";
			// 
			// NewScriptFileForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(319, 124);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->btClose);
			this->Controls->Add(this->btAdd);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->txtName);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"NewScriptFileForm";
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"�� ��ũ��Ʈ �߰�";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		public:

			String ^mFileName;

		private: System::Void btAdd_Click(System::Object^  sender, System::EventArgs^  e)
		{
			if(txtName->Text->Length == 0)
			{
				MessageBox::Show(this, "���� �̸��� �Է����ּ���!", MIMI_TITLE, MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
				return;
			}

			String ^fileName = txtName->Text;

			if(fileName->Length < 5 || String::Compare(fileName->Substring(fileName->Length - 4, 4), ".txt", StringComparison::CurrentCultureIgnoreCase) != 0)
				fileName += ".txt";

			// ���� �����ϱ�
			String ^outpath = "��ũ��Ʈ\\" + fileName;

			try
			{
				FileStream ^fileStream = gcnew FileStream(outpath, System::IO::FileMode::Create);

				fileStream->Close();
			} catch(Exception ^)
			{
				MessageBox::Show(this, "'" + outpath + "' ������ ������ �� �����!", MIMI_TITLE, MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
				return;
			}

			mFileName = fileName;
			this->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->Close();
		}

		private: System::Void btClose_Click(System::Object^  sender, System::EventArgs^  e)
		{
			this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->Close();
		}
		private: System::Void txtName_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		{
			if(e->KeyCode == Keys::Enter)
			{
				btAdd_Click(sender, e);
			}
		}
};
}
