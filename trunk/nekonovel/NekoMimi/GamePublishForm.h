#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace NekoMimi {

	/// <summary>
	/// GamePublishForm�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class GamePublishForm : public System::Windows::Forms::Form
	{
	public:
		bool mPublishing;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::CheckBox^  cbAutoUpdate;


	public: 
		static GamePublishForm ^self;

	public:
		GamePublishForm(void)
			: mPublishing(false)
		{
			InitializeComponent();

			GamePublishForm::self = this;

			nekoGame *game = GetNekoNovel()->GetCurrentGame();
			nekoGameEnvironment &env = game->GetEnvironment();

			txtLastPublished->Text = ConvertToString(env.mLastPublishFolder.c_str());
			txtVersion->Text = String::Format("{0}", env.mLastPublishVersion);

			cbCompressBGM->Checked = env.mCompressBGM;
		}

	protected:
		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
		/// </summary>
		~GamePublishForm()
		{
			delete self;
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Button^  btPublish;
	private: System::Windows::Forms::Button^  btClose;
	protected: 


	private: System::Windows::Forms::TextBox^  txtLastPublished;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  btBrowse;

	private: System::Windows::Forms::CheckBox^  cbCompressBGM;
	private: System::Windows::Forms::ProgressBar^  pbar;


	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  txtVersion;

	private: System::Windows::Forms::Label^  label3;


	protected: 

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
			this->btPublish = (gcnew System::Windows::Forms::Button());
			this->btClose = (gcnew System::Windows::Forms::Button());
			this->txtLastPublished = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->btBrowse = (gcnew System::Windows::Forms::Button());
			this->cbCompressBGM = (gcnew System::Windows::Forms::CheckBox());
			this->pbar = (gcnew System::Windows::Forms::ProgressBar());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->txtVersion = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->cbAutoUpdate = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// btPublish
			// 
			this->btPublish->Location = System::Drawing::Point(264, 216);
			this->btPublish->Name = L"btPublish";
			this->btPublish->Size = System::Drawing::Size(136, 32);
			this->btPublish->TabIndex = 0;
			this->btPublish->Text = L"���� �����ϱ�(&S)";
			this->btPublish->UseVisualStyleBackColor = true;
			this->btPublish->Click += gcnew System::EventHandler(this, &GamePublishForm::btPublish_Click);
			// 
			// btClose
			// 
			this->btClose->Location = System::Drawing::Point(400, 216);
			this->btClose->Name = L"btClose";
			this->btClose->Size = System::Drawing::Size(136, 32);
			this->btClose->TabIndex = 1;
			this->btClose->Text = L"�ݱ�(&C)";
			this->btClose->UseVisualStyleBackColor = true;
			this->btClose->Click += gcnew System::EventHandler(this, &GamePublishForm::btClose_Click);
			// 
			// txtLastPublished
			// 
			this->txtLastPublished->Location = System::Drawing::Point(146, 18);
			this->txtLastPublished->Name = L"txtLastPublished";
			this->txtLastPublished->Size = System::Drawing::Size(310, 21);
			this->txtLastPublished->TabIndex = 2;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(8, 22);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(137, 12);
			this->label1->TabIndex = 3;
			this->label1->Text = L"���� ������ ���� ���� : ";
			// 
			// btBrowse
			// 
			this->btBrowse->Location = System::Drawing::Point(456, 16);
			this->btBrowse->Name = L"btBrowse";
			this->btBrowse->Size = System::Drawing::Size(80, 24);
			this->btBrowse->TabIndex = 4;
			this->btBrowse->Text = L"ã�ƺ���(&B)";
			this->btBrowse->UseVisualStyleBackColor = true;
			this->btBrowse->Click += gcnew System::EventHandler(this, &GamePublishForm::btBrowse_Click);
			// 
			// cbCompressBGM
			// 
			this->cbCompressBGM->AutoSize = true;
			this->cbCompressBGM->Location = System::Drawing::Point(160, 56);
			this->cbCompressBGM->Name = L"cbCompressBGM";
			this->cbCompressBGM->Size = System::Drawing::Size(144, 16);
			this->cbCompressBGM->TabIndex = 5;
			this->cbCompressBGM->Text = L"������� ���� ����(&1)";
			this->cbCompressBGM->UseVisualStyleBackColor = true;
			// 
			// pbar
			// 
			this->pbar->Location = System::Drawing::Point(8, 192);
			this->pbar->Name = L"pbar";
			this->pbar->Size = System::Drawing::Size(528, 16);
			this->pbar->TabIndex = 6;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(8, 56);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(65, 12);
			this->label2->TabIndex = 7;
			this->label2->Text = L"���� ���� :";
			// 
			// txtVersion
			// 
			this->txtVersion->Location = System::Drawing::Point(80, 51);
			this->txtVersion->MaxLength = 5;
			this->txtVersion->Name = L"txtVersion";
			this->txtVersion->Size = System::Drawing::Size(72, 21);
			this->txtVersion->TabIndex = 8;
			this->txtVersion->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &GamePublishForm::txtVersion_KeyPress);
			// 
			// label3
			// 
			this->label3->Location = System::Drawing::Point(8, 88);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(528, 24);
			this->label3->TabIndex = 9;
			this->label3->Text = L"�� ���� : ������ �����Ͻñ� ���� �������� ���� ������ ����ֽô� ���� ���� ��õ�մϴ�!!";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->textBox1);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->cbAutoUpdate);
			this->groupBox1->Location = System::Drawing::Point(8, 120);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(528, 64);
			this->groupBox1->TabIndex = 10;
			this->groupBox1->TabStop = false;
			// 
			// textBox1
			// 
			this->textBox1->Enabled = false;
			this->textBox1->Location = System::Drawing::Point(152, 19);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(352, 21);
			this->textBox1->TabIndex = 3;
			this->textBox1->Text = L"http://localhost/patch/";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Enabled = false;
			this->label4->Location = System::Drawing::Point(8, 24);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(139, 12);
			this->label4->TabIndex = 1;
			this->label4->Text = L"version.ini ������ �ּ� :";
			// 
			// cbAutoUpdate
			// 
			this->cbAutoUpdate->AutoSize = true;
			this->cbAutoUpdate->Enabled = false;
			this->cbAutoUpdate->Location = System::Drawing::Point(0, 0);
			this->cbAutoUpdate->Name = L"cbAutoUpdate";
			this->cbAutoUpdate->Size = System::Drawing::Size(128, 16);
			this->cbAutoUpdate->TabIndex = 0;
			this->cbAutoUpdate->Text = L"�ڵ� ������Ʈ ���";
			this->cbAutoUpdate->UseVisualStyleBackColor = true;
			// 
			// GamePublishForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(543, 259);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->txtVersion);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->pbar);
			this->Controls->Add(this->cbCompressBGM);
			this->Controls->Add(this->btBrowse);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->txtLastPublished);
			this->Controls->Add(this->btClose);
			this->Controls->Add(this->btPublish);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"GamePublishForm";
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"������ �����ؿ�!";
			this->Load += gcnew System::EventHandler(this, &GamePublishForm::GamePublishForm_Load);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &GamePublishForm::GamePublishForm_FormClosing);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		private: System::Void GamePublishForm_Load(System::Object^  sender, System::EventArgs^  e)
		{


		}
		private: System::Void btClose_Click(System::Object^  sender, System::EventArgs^  e)
		{
			nekoGame *game = GetNekoNovel()->GetCurrentGame();
			nekoGameEnvironment &env = game->GetEnvironment();

			env.mLastPublishFolder = ConvertToChar(txtLastPublished->Text);
			env.mLastPublishVersion = Int32::Parse(txtVersion->Text);
			env.mCompressBGM = cbCompressBGM->Checked;

			this->Close();
		}
		private: System::Void btPublish_Click(System::Object^  sender, System::EventArgs^  e)
		{
			/// �ϴ� ���� ����..
			nekoGame *game = GetNekoNovel()->GetCurrentGame();
			nekoGameEnvironment &env = game->GetEnvironment();

			if(!mPublishing)
			{
				env.mLastPublishFolder = ConvertToChar(txtLastPublished->Text);
				env.mLastPublishVersion = Int32::Parse(txtVersion->Text);
				env.mCompressBGM = cbCompressBGM->Checked;

				if(FAILED(game->Build()))
				{
					MessageBox::Show(this, "Game.nkpublished ���� �ۼ��� ������ ������ϴ�.",
						MIMI_TITLE, MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
					return;
				}

				if(FAILED(game->StartPublishing()))
				{
					MessageBox::Show(this, "���� �۾��� ������ �� �����ϴ�!",
						MIMI_TITLE, MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
					return;
				}

				mPublishing = true;
				btPublish->Text = "�۾� �ߴ��ϱ�(&X)";
				btClose->Enabled = false;
			}
			else
			{
				game->StopPublishing();
			}
		}
		private: System::Void btBrowse_Click(System::Object^  sender, System::EventArgs^  e)
		{
			FolderBrowserDialog ^dlg = gcnew FolderBrowserDialog();

			String ^mRunPath = Environment::CurrentDirectory;
			dlg->SelectedPath = mRunPath;
			if(dlg->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				txtLastPublished->Text = dlg->SelectedPath;
			}
			Directory::SetCurrentDirectory(mRunPath);
		}
		private: System::Void txtVersion_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e)
		{
			if (!(Char::IsDigit(e->KeyChar) || e->KeyChar == Convert::ToChar(Keys::Back)))
			{
				e->Handled = true;
			}
		}

		private:
			delegate void TestDelegate();
			int32 mLastStatusValue;

		void OnStatusUpdatedReal()
		{
			// �۾� ����
			if(mLastStatusValue < 0)
			{
				self->btClose->Enabled = true;
				self->mPublishing = false;
				self->btPublish->Text = "���� �����ϱ�(&S)";
				self->pbar->Value = 0;

				if(mLastStatusValue == -2)
				{
					MessageBox::Show(this, "���� �۾��� �����ϴ� ���� ������ �߻��Ͽ����ϴ�.",
						MIMI_TITLE, MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
					this->Close();
				}
				else if(mLastStatusValue == -1)
				{
					MessageBox::Show(this, "���������� ���� �۾��� ���ƽ��ϴ�!",
						MIMI_TITLE, MessageBoxButtons::OK, MessageBoxIcon::Information);
					this->Close();
				}
				else if(mLastStatusValue == -3)
				{
					MessageBox::Show(this, "���� �۾��� �ߴ��߽��ϴ�!",
						MIMI_TITLE, MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
					this->Close();
				}
				return;
			}

			self->pbar->Value = mLastStatusValue;
		}

		public:
		static void OnStatusUpdated(const nekoEvent &e)
		{
			try
			{
				self->mLastStatusValue = e.value[0];

				TestDelegate ^realPut = gcnew TestDelegate(self , &GamePublishForm::OnStatusUpdatedReal);
				//self->Invoke(realPut, gcnew array<Object ^>{str});
				self->Invoke(realPut);
			} catch(Exception ^)
			{

			}
		}
		private: System::Void GamePublishForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
		{
			if(mPublishing)
				e->Cancel = true;
		}
};
}
