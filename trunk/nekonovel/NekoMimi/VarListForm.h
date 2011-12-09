#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace NekoMimi {

	/// <summary>
	/// VarListForm�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class VarListForm : public System::Windows::Forms::Form
	{
	public:
		static VarListForm ^singletonForm = nullptr;

		VarListForm(void)
		{
			InitializeComponent();
			//
			//TODO: ������ �ڵ带 ���⿡ �߰��մϴ�.
			//
		}

	protected:
		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
		/// </summary>
		~VarListForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListView^  lstVariables;
	protected: 

	protected: 
	private: System::Windows::Forms::ColumnHeader^  columnHeader1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader2;
	private: System::Windows::Forms::ColumnHeader^  columnHeader3;

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
			this->lstVariables = (gcnew System::Windows::Forms::ListView());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
			this->SuspendLayout();
			// 
			// lstVariables
			// 
			this->lstVariables->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(3) {this->columnHeader1, 
				this->columnHeader2, this->columnHeader3});
			this->lstVariables->Dock = System::Windows::Forms::DockStyle::Fill;
			this->lstVariables->Location = System::Drawing::Point(0, 0);
			this->lstVariables->Name = L"lstVariables";
			this->lstVariables->Size = System::Drawing::Size(378, 188);
			this->lstVariables->TabIndex = 0;
			this->lstVariables->UseCompatibleStateImageBehavior = false;
			this->lstVariables->View = System::Windows::Forms::View::Details;
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"���� �̸�";
			this->columnHeader1->Width = 104;
			// 
			// columnHeader2
			// 
			this->columnHeader2->Text = L"Ÿ��";
			this->columnHeader2->Width = 67;
			// 
			// columnHeader3
			// 
			this->columnHeader3->Text = L"����";
			this->columnHeader3->Width = 201;
			// 
			// VarListForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(378, 188);
			this->Controls->Add(this->lstVariables);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::SizableToolWindow;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"VarListForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"������� ���� ����Ʈ";
			this->Load += gcnew System::EventHandler(this, &VarListForm::VarListForm_Load);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &VarListForm::VarListForm_FormClosing);
			this->ResumeLayout(false);

		}
#pragma endregion

	private: System::Void VarListForm_Load(System::Object^  sender, System::EventArgs^  e);
	private: System::Void VarListForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e);

	public:
		void OnVariableChanged(const nekoEvent &event);
		void UpdateAllVariables();
};
}
