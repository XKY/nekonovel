#pragma once

#include "HelpForm.h"
#include "GamePublishForm.h"
#include "GameSettingForm.h"
#include "MainEventListener.h"
#include "ExtCanvas.h"
#include "ScriptFile.h"
#include "NewScriptFileForm.h"
#include "HelpComboItem.h"
#include "VarListForm.h"
#include "DraggableTabControl.h"

using namespace System;
using namespace System::Text;
using namespace System::IO;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace NekoMimi {


	static uint32 gEditorColors[EECT_COUNT];
	static uint32 gCommandColors[ECT_COUNT];

	/// <summary>
	/// frmMain�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class frmMain : public System::Windows::Forms::Form
	{
	public:
		private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel2;



		private: System::Windows::Forms::ColorDialog^  colorDialog;
		private: System::Windows::Forms::ToolStripMenuItem^  menuEditorUndo;
		private: System::Windows::Forms::ToolStripMenuItem^  menuEditorRedo;


	public: 

		static frmMain ^self;
	private: ScintillaNet::Scintilla^  txtEditor;
	private: System::Windows::Forms::ToolStripMenuItem^  ����VToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ȭ�����SToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �����̱�ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����ũ���3ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �����̱�2ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �۾�����FToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �ּ�CToolStripMenuItem;
	private: System::Windows::Forms::ComboBox^  cbHelp;
	private: System::Windows::Forms::TextBox^  txtLogging;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  txtHelpContent;
	private: System::Windows::Forms::Button^  btPutColor;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem3;
	private: System::Windows::Forms::ToolStripMenuItem^  ���������̵�LToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �����������̵�RToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����ڸ�ɾ�UToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ������κ�������ƮVToolStripMenuItem;
	private: System::Windows::Forms::Button^  btAddScript;
	private: System::Windows::Forms::Button^  btCloseScript;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripMenuItem^  ��ũ��Ʈ��������OToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �۾����ν�ũ��Ʈ�ݱ�CToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem4;
	private: System::Windows::Forms::ToolStripMenuItem^  ������ũ��Ʈ�ΰ���PToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ������ũ��Ʈ�γѾ��NToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ��ũ��Ʈ��������VToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem10;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem9;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem8;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem7;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem6;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem5;
	private: System::Windows::Forms::ToolStripMenuItem^  ����1tCtrl1ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem12;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem11;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem13;










	protected:

	public: 
		static String ^mRunPath;

		frmMain(void)
			: mLastLine(1)
			, mCurrentCommands(NULL)
			, mTestPlaying(false)
			, scriptLoadingNow(false)
			, mZoomLevel(1)
		{
			InitializeComponent();

#ifndef _DEBUG
			frmMain::mRunPath = Application::ExecutablePath->Substring(0, Application::ExecutablePath->LastIndexOf('\\'));
#else
			frmMain::mRunPath = Environment::CurrentDirectory;
#endif
			// �������� �������� �ֱ�
			if(frmMain::mRunPath[frmMain::mRunPath->Length - 1] != L'\\')
				frmMain::mRunPath += L'\\';

			frmMain::self = this;
			mNekoNovel = GetNekoNovel();

			this->picCanvas = (gcnew ExtCanvas());
			this->picCanvas->Location = System::Drawing::Point(8, 52);
			this->picCanvas->Name = L"picCanvas";
			this->picCanvas->Size = System::Drawing::Size(800, 600);
			this->picCanvas->BorderStyle = BorderStyle::None;
			this->picCanvas->TabIndex = 0;
			this->picCanvas->TabStop = false;
			this->picCanvas->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &frmMain::picCanvas_MouseMove);
			this->picCanvas->SizeChanged += gcnew System::EventHandler(this, &frmMain::picCanvas_SizeChanged);
			this->picCanvas->ImeMode = System::Windows::Forms::ImeMode::Hangul;
			this->Controls->Add(this->picCanvas);

#ifndef _DEBUG
			mNekoNovel->SetAppFolderPath(ConvertToChar(frmMain::mRunPath));
#endif

			if(FAILED(mNekoNovel->Initialize((HWND)picCanvas->Handle.ToPointer(), picCanvas->Width, picCanvas->Height, 0)))
				return;

			// ���� ���� �����.	
			mCurrentGame = mNekoNovel->LoadGame();
			if(mCurrentGame == NULL)
			{
				MessageBox::Show(this, "������ ���µ� �־ ����ġ ���� ������ �߻��Ͽ����ϴ�.", MIMI_TITLE, MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
				Application::Exit();
				return;
			}
			// �޸� �ʱ�ȭ �ϱ�
			memset(gEditorColors, 0, sizeof(uint32) * EECT_COUNT);
			memset(gCommandColors, 0, sizeof(uint32) * ECT_COUNT);
			nekoCommands::LoadCommandReplacements(gEditorColors, gCommandColors);

			/// ���� �ҷ�����
			int32 i = 0;
			const char8 *p;
			for(int32 i = 0;i < ECT_COUNT;++i)
			{
				p = nekoCommands::GetCommandString(i);
				if(p == NULL)
					break;
				if(*p == '\0')
					continue;
				HelpComboItem ^item = gcnew HelpComboItem(ConvertToString(p), i);
				cbHelp->Items->Add(item);
			}
			cbHelp->SelectedIndex = 0;

			if(gEditorColors[EECT_BACKCOLOR])
			{
				txtEditor->Styles[0]->BackColor = Color::FromArgb(gEditorColors[EECT_BACKCOLOR]);
				txtEditor->Styles[32]->BackColor = Color::FromArgb(gEditorColors[EECT_BACKCOLOR]);
			}
			for(uint32 i = 1;i < 11;++i)
			{
				if(gEditorColors[i])
				{
					txtEditor->Styles[20 + i]->ForeColor = Color::FromArgb(gEditorColors[i]);
					txtEditor->Styles[20 + i]->BackColor = Color::FromArgb(gEditorColors[EECT_BACKCOLOR]);
				}
			}


			OnGameLoaded();

			/// ��Ÿ���� �����.
			txtEditor->Styles[1]->BackColor = Color::Yellow;
			if(gEditorColors[EECT_COMMENT])
				txtEditor->Styles[2]->ForeColor = Color::FromArgb(gEditorColors[EECT_COMMENT]);
			else
				txtEditor->Styles[2]->ForeColor = Color::FromArgb(0, 128, 0);
			txtEditor->Styles[2]->Bold = true;
			if(gEditorColors[EECT_BACKCOLOR])
				txtEditor->Styles[2]->BackColor = Color::FromArgb(gEditorColors[EECT_BACKCOLOR]);
			txtEditor->Styles[3]->ForeColor = Color::FromArgb(150, 150, 200);
			if(gEditorColors[EECT_BACKCOLOR])
				txtEditor->Styles[3]->BackColor = Color::FromArgb(gEditorColors[EECT_BACKCOLOR]);

			/// ĵ���� ������ ����
			picCanvas->Width = mCurrentGame->GetEnvironment().mWidth;
			picCanvas->Height = mCurrentGame->GetEnvironment().mHeight;
		}

	protected:
		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
		/// </summary>
		~frmMain()
		{
			ClearHGlobal();

			if (components)
			{
				delete components;
			}
		}



	private: System::Windows::Forms::MenuStrip^  menuMain;
	private: System::Windows::Forms::ToolStripMenuItem^  ����FToolStripMenuItem;


	private: System::Windows::Forms::ToolStripMenuItem^  �����������SToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  ���α׷�����XToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����HToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ���ڳ뺧�����ؼ�ToolStripMenuItem;


	private: System::Windows::Forms::StatusStrip^  statusBar;


	private: System::Windows::Forms::TabPage^  firstPage;
	private: DraggableTabControl^  tabFiles;
	private: System::Windows::Forms::ToolStripMenuItem^  ����GToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����ġ��������RToolStripMenuItem;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem2;
	private: System::Windows::Forms::ToolStripMenuItem^  ���ӹ����ϱ�PToolStripMenuItem;






	private: System::Windows::Forms::ToolStripMenuItem^  ���Ӽ���RToolStripMenuItem;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1;
	private: System::Windows::Forms::ToolStripMenuItem^  ���ý�ũ��Ʈ������SToolStripMenuItem;
	private: System::Windows::Forms::ContextMenuStrip^  menuTabs;

	private: System::Windows::Forms::ToolStripMenuItem^  ����ũ��ƮAToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ��ũ��Ʈ��������IToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ��ũ��Ʈ����DToolStripMenuItem;

	private: System::ComponentModel::IContainer^  components;
	private:
		/// <summary>
		/// �ʼ� �����̳� �����Դϴ�.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����̳� ������ �ʿ��� �޼����Դϴ�.
		/// �� �޼����� ������ �ڵ� ������� �������� ���ʽÿ�.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->menuMain = (gcnew System::Windows::Forms::MenuStrip());
			this->����FToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->���ý�ũ��Ʈ������SToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�����������SToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->��ũ��Ʈ��������OToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�۾����ν�ũ��Ʈ�ݱ�CToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->���α׷�����XToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����VToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ȭ�����SToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ũ���3ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�����̱�ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�����̱�2ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�۾�����FToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�ּ�CToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ڸ�ɾ�UToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem4 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->������ũ��Ʈ�ΰ���PToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->������ũ��Ʈ�γѾ��NToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��ũ��Ʈ��������VToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem10 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem9 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem8 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem7 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem6 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem5 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem12 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem11 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem13 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����1tCtrl1ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->������κ�������ƮVToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����GToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ġ��������RToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->���Ӽ���RToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->���ӹ����ϱ�PToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����HToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->���ڳ뺧�����ؼ�ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuEditorUndo = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuEditorRedo = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->statusBar = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripStatusLabel2 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->firstPage = (gcnew System::Windows::Forms::TabPage());
			this->tabFiles = (gcnew DraggableTabControl());
			this->menuTabs = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->����ũ��ƮAToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��ũ��Ʈ��������IToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��ũ��Ʈ����DToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->���������̵�LToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�����������̵�RToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->colorDialog = (gcnew System::Windows::Forms::ColorDialog());
			this->txtEditor = (gcnew ScintillaNet::Scintilla());
			this->cbHelp = (gcnew System::Windows::Forms::ComboBox());
			this->txtLogging = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txtHelpContent = (gcnew System::Windows::Forms::TextBox());
			this->btPutColor = (gcnew System::Windows::Forms::Button());
			this->btAddScript = (gcnew System::Windows::Forms::Button());
			this->btCloseScript = (gcnew System::Windows::Forms::Button());
			this->menuMain->SuspendLayout();
			this->statusBar->SuspendLayout();
			this->tabFiles->SuspendLayout();
			this->menuTabs->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->txtEditor))->BeginInit();
			this->SuspendLayout();
			// 
			// menuMain
			// 
			this->menuMain->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->����FToolStripMenuItem, 
				this->����VToolStripMenuItem, this->����GToolStripMenuItem, this->����HToolStripMenuItem});
			this->menuMain->Location = System::Drawing::Point(0, 0);
			this->menuMain->Name = L"menuMain";
			this->menuMain->Size = System::Drawing::Size(1231, 24);
			this->menuMain->TabIndex = 3;
			this->menuMain->Text = L"menuStrip1";
			// 
			// ����FToolStripMenuItem
			// 
			this->����FToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->���ý�ũ��Ʈ������SToolStripMenuItem, 
				this->�����������SToolStripMenuItem, this->toolStripSeparator2, this->��ũ��Ʈ��������OToolStripMenuItem, this->�۾����ν�ũ��Ʈ�ݱ�CToolStripMenuItem, 
				this->toolStripMenuItem1, this->���α׷�����XToolStripMenuItem});
			this->����FToolStripMenuItem->Name = L"����FToolStripMenuItem";
			this->����FToolStripMenuItem->Size = System::Drawing::Size(84, 20);
			this->����FToolStripMenuItem->Text = L"���ڳ뺧(&N)";
			// 
			// ���ý�ũ��Ʈ������SToolStripMenuItem
			// 
			this->���ý�ũ��Ʈ������SToolStripMenuItem->Name = L"���ý�ũ��Ʈ������SToolStripMenuItem";
			this->���ý�ũ��Ʈ������SToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::S));
			this->���ý�ũ��Ʈ������SToolStripMenuItem->Size = System::Drawing::Size(273, 22);
			this->���ý�ũ��Ʈ������SToolStripMenuItem->Text = L"�������� ��ũ��Ʈ�� ����(&S)";
			this->���ý�ũ��Ʈ������SToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::���ý�ũ��Ʈ������SToolStripMenuItem_Click);
			// 
			// �����������SToolStripMenuItem
			// 
			this->�����������SToolStripMenuItem->Name = L"�����������SToolStripMenuItem";
			this->�����������SToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>(((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Shift) 
				| System::Windows::Forms::Keys::S));
			this->�����������SToolStripMenuItem->Size = System::Drawing::Size(273, 22);
			this->�����������SToolStripMenuItem->Text = L"���� ���� ����(&S)";
			this->�����������SToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::�����������SToolStripMenuItem_Click);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(270, 6);
			// 
			// ��ũ��Ʈ��������OToolStripMenuItem
			// 
			this->��ũ��Ʈ��������OToolStripMenuItem->Name = L"��ũ��Ʈ��������OToolStripMenuItem";
			this->��ũ��Ʈ��������OToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::O));
			this->��ũ��Ʈ��������OToolStripMenuItem->Size = System::Drawing::Size(273, 22);
			this->��ũ��Ʈ��������OToolStripMenuItem->Text = L"��ũ��Ʈ ��������(&O)";
			this->��ũ��Ʈ��������OToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::��ũ��Ʈ��������OToolStripMenuItem_Click);
			// 
			// �۾����ν�ũ��Ʈ�ݱ�CToolStripMenuItem
			// 
			this->�۾����ν�ũ��Ʈ�ݱ�CToolStripMenuItem->Name = L"�۾����ν�ũ��Ʈ�ݱ�CToolStripMenuItem";
			this->�۾����ν�ũ��Ʈ�ݱ�CToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::F4));
			this->�۾����ν�ũ��Ʈ�ݱ�CToolStripMenuItem->Size = System::Drawing::Size(273, 22);
			this->�۾����ν�ũ��Ʈ�ݱ�CToolStripMenuItem->Text = L"�۾����� ��ũ��Ʈ �ݱ�(&C)";
			this->�۾����ν�ũ��Ʈ�ݱ�CToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::�۾����ν�ũ��Ʈ�ݱ�CToolStripMenuItem_Click);
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(270, 6);
			// 
			// ���α׷�����XToolStripMenuItem
			// 
			this->���α׷�����XToolStripMenuItem->Name = L"���α׷�����XToolStripMenuItem";
			this->���α׷�����XToolStripMenuItem->Size = System::Drawing::Size(273, 22);
			this->���α׷�����XToolStripMenuItem->Text = L"���α׷� ����(&X)";
			this->���α׷�����XToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::���α׷�����XToolStripMenuItem_Click);
			// 
			// ����VToolStripMenuItem
			// 
			this->����VToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(8) {this->ȭ�����SToolStripMenuItem, 
				this->�۾�����FToolStripMenuItem, this->toolStripMenuItem4, this->������ũ��Ʈ�ΰ���PToolStripMenuItem, this->������ũ��Ʈ�γѾ��NToolStripMenuItem, 
				this->��ũ��Ʈ��������VToolStripMenuItem, this->toolStripSeparator1, this->������κ�������ƮVToolStripMenuItem});
			this->����VToolStripMenuItem->Name = L"����VToolStripMenuItem";
			this->����VToolStripMenuItem->Size = System::Drawing::Size(59, 20);
			this->����VToolStripMenuItem->Text = L"����(&V)";
			// 
			// ȭ�����SToolStripMenuItem
			// 
			this->ȭ�����SToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->����ũ���3ToolStripMenuItem, 
				this->�����̱�ToolStripMenuItem, this->�����̱�2ToolStripMenuItem});
			this->ȭ�����SToolStripMenuItem->Name = L"ȭ�����SToolStripMenuItem";
			this->ȭ�����SToolStripMenuItem->Size = System::Drawing::Size(291, 22);
			this->ȭ�����SToolStripMenuItem->Text = L"ȭ�� ����(&S)";
			// 
			// ����ũ���3ToolStripMenuItem
			// 
			this->����ũ���3ToolStripMenuItem->Name = L"����ũ���3ToolStripMenuItem";
			this->����ũ���3ToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::D1));
			this->����ũ���3ToolStripMenuItem->Size = System::Drawing::Size(197, 22);
			this->����ũ���3ToolStripMenuItem->Text = L"���� ũ���(&1)";
			this->����ũ���3ToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::����ũ���3ToolStripMenuItem_Click);
			// 
			// �����̱�ToolStripMenuItem
			// 
			this->�����̱�ToolStripMenuItem->Name = L"�����̱�ToolStripMenuItem";
			this->�����̱�ToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::D2));
			this->�����̱�ToolStripMenuItem->Size = System::Drawing::Size(197, 22);
			this->�����̱�ToolStripMenuItem->Text = L"3/4�� ���̱�(&2)";
			this->�����̱�ToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::�����̱�ToolStripMenuItem_Click);
			// 
			// �����̱�2ToolStripMenuItem
			// 
			this->�����̱�2ToolStripMenuItem->Name = L"�����̱�2ToolStripMenuItem";
			this->�����̱�2ToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::D3));
			this->�����̱�2ToolStripMenuItem->Size = System::Drawing::Size(197, 22);
			this->�����̱�2ToolStripMenuItem->Text = L"2/4�� ���̱�(&3)";
			this->�����̱�2ToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::�����̱�2ToolStripMenuItem_Click);
			// 
			// �۾�����FToolStripMenuItem
			// 
			this->�۾�����FToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->�ּ�CToolStripMenuItem, 
				this->����ڸ�ɾ�UToolStripMenuItem});
			this->�۾�����FToolStripMenuItem->Name = L"�۾�����FToolStripMenuItem";
			this->�۾�����FToolStripMenuItem->Size = System::Drawing::Size(291, 22);
			this->�۾�����FToolStripMenuItem->Text = L"����â ���� ���(&F)";
			this->�۾�����FToolStripMenuItem->Visible = false;
			// 
			// �ּ�CToolStripMenuItem
			// 
			this->�ּ�CToolStripMenuItem->Name = L"�ּ�CToolStripMenuItem";
			this->�ּ�CToolStripMenuItem->Size = System::Drawing::Size(166, 22);
			this->�ּ�CToolStripMenuItem->Text = L"�ּ�(&C)";
			this->�ּ�CToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::�ּ�CToolStripMenuItem_Click);
			// 
			// ����ڸ�ɾ�UToolStripMenuItem
			// 
			this->����ڸ�ɾ�UToolStripMenuItem->Name = L"����ڸ�ɾ�UToolStripMenuItem";
			this->����ڸ�ɾ�UToolStripMenuItem->Size = System::Drawing::Size(166, 22);
			this->����ڸ�ɾ�UToolStripMenuItem->Text = L"����� ��ɾ�(&U)";
			// 
			// toolStripMenuItem4
			// 
			this->toolStripMenuItem4->Name = L"toolStripMenuItem4";
			this->toolStripMenuItem4->Size = System::Drawing::Size(288, 6);
			// 
			// ������ũ��Ʈ�ΰ���PToolStripMenuItem
			// 
			this->������ũ��Ʈ�ΰ���PToolStripMenuItem->Name = L"������ũ��Ʈ�ΰ���PToolStripMenuItem";
			this->������ũ��Ʈ�ΰ���PToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>(((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Shift) 
				| System::Windows::Forms::Keys::Tab));
			this->������ũ��Ʈ�ΰ���PToolStripMenuItem->Size = System::Drawing::Size(291, 22);
			this->������ũ��Ʈ�ΰ���PToolStripMenuItem->Text = L"���� ��ũ��Ʈ�� ����(&P)";
			this->������ũ��Ʈ�ΰ���PToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::������ũ��Ʈ�ΰ���PToolStripMenuItem_Click);
			// 
			// ������ũ��Ʈ�γѾ��NToolStripMenuItem
			// 
			this->������ũ��Ʈ�γѾ��NToolStripMenuItem->Name = L"������ũ��Ʈ�γѾ��NToolStripMenuItem";
			this->������ũ��Ʈ�γѾ��NToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Tab));
			this->������ũ��Ʈ�γѾ��NToolStripMenuItem->Size = System::Drawing::Size(291, 22);
			this->������ũ��Ʈ�γѾ��NToolStripMenuItem->Text = L"���� ��ũ��Ʈ�� ����(&N)";
			this->������ũ��Ʈ�γѾ��NToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::������ũ��Ʈ�γѾ��NToolStripMenuItem_Click);
			// 
			// ��ũ��Ʈ��������VToolStripMenuItem
			// 
			this->��ũ��Ʈ��������VToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(10) {this->toolStripMenuItem10, 
				this->toolStripMenuItem9, this->toolStripMenuItem8, this->toolStripMenuItem7, this->toolStripMenuItem6, this->toolStripMenuItem5, 
				this->toolStripMenuItem12, this->toolStripMenuItem11, this->toolStripMenuItem13, this->����1tCtrl1ToolStripMenuItem});
			this->��ũ��Ʈ��������VToolStripMenuItem->Name = L"��ũ��Ʈ��������VToolStripMenuItem";
			this->��ũ��Ʈ��������VToolStripMenuItem->Size = System::Drawing::Size(291, 22);
			this->��ũ��Ʈ��������VToolStripMenuItem->Text = L"��ũ��Ʈ �� ����Ű(&T)";
			// 
			// toolStripMenuItem10
			// 
			this->toolStripMenuItem10->Name = L"toolStripMenuItem10";
			this->toolStripMenuItem10->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::D1));
			this->toolStripMenuItem10->Size = System::Drawing::Size(172, 22);
			this->toolStripMenuItem10->Tag = L"1";
			this->toolStripMenuItem10->Text = L"1�� ��(&1)";
			this->toolStripMenuItem10->Click += gcnew System::EventHandler(this, &frmMain::toolStripMenuItem10_Click);
			// 
			// toolStripMenuItem9
			// 
			this->toolStripMenuItem9->Name = L"toolStripMenuItem9";
			this->toolStripMenuItem9->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::D2));
			this->toolStripMenuItem9->Size = System::Drawing::Size(172, 22);
			this->toolStripMenuItem9->Tag = L"2";
			this->toolStripMenuItem9->Text = L"2�� ��(&2)";
			this->toolStripMenuItem9->Click += gcnew System::EventHandler(this, &frmMain::toolStripMenuItem10_Click);
			// 
			// toolStripMenuItem8
			// 
			this->toolStripMenuItem8->Name = L"toolStripMenuItem8";
			this->toolStripMenuItem8->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::D3));
			this->toolStripMenuItem8->Size = System::Drawing::Size(172, 22);
			this->toolStripMenuItem8->Tag = L"3";
			this->toolStripMenuItem8->Text = L"3�� ��(&3)";
			this->toolStripMenuItem8->Click += gcnew System::EventHandler(this, &frmMain::toolStripMenuItem10_Click);
			// 
			// toolStripMenuItem7
			// 
			this->toolStripMenuItem7->Name = L"toolStripMenuItem7";
			this->toolStripMenuItem7->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::D4));
			this->toolStripMenuItem7->Size = System::Drawing::Size(172, 22);
			this->toolStripMenuItem7->Tag = L"4";
			this->toolStripMenuItem7->Text = L"4�� ��(&4)";
			this->toolStripMenuItem7->Click += gcnew System::EventHandler(this, &frmMain::toolStripMenuItem10_Click);
			// 
			// toolStripMenuItem6
			// 
			this->toolStripMenuItem6->Name = L"toolStripMenuItem6";
			this->toolStripMenuItem6->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::D5));
			this->toolStripMenuItem6->Size = System::Drawing::Size(172, 22);
			this->toolStripMenuItem6->Tag = L"5";
			this->toolStripMenuItem6->Text = L"5�� ��(&5)";
			this->toolStripMenuItem6->Click += gcnew System::EventHandler(this, &frmMain::toolStripMenuItem10_Click);
			// 
			// toolStripMenuItem5
			// 
			this->toolStripMenuItem5->Name = L"toolStripMenuItem5";
			this->toolStripMenuItem5->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::D6));
			this->toolStripMenuItem5->Size = System::Drawing::Size(172, 22);
			this->toolStripMenuItem5->Tag = L"6";
			this->toolStripMenuItem5->Text = L"6�� ��(&6)";
			this->toolStripMenuItem5->Click += gcnew System::EventHandler(this, &frmMain::toolStripMenuItem10_Click);
			// 
			// toolStripMenuItem12
			// 
			this->toolStripMenuItem12->Name = L"toolStripMenuItem12";
			this->toolStripMenuItem12->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::D7));
			this->toolStripMenuItem12->Size = System::Drawing::Size(172, 22);
			this->toolStripMenuItem12->Tag = L"7";
			this->toolStripMenuItem12->Text = L"7�� ��(&7)";
			this->toolStripMenuItem12->Click += gcnew System::EventHandler(this, &frmMain::toolStripMenuItem10_Click);
			// 
			// toolStripMenuItem11
			// 
			this->toolStripMenuItem11->Name = L"toolStripMenuItem11";
			this->toolStripMenuItem11->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::D8));
			this->toolStripMenuItem11->Size = System::Drawing::Size(172, 22);
			this->toolStripMenuItem11->Tag = L"8";
			this->toolStripMenuItem11->Text = L"8�� ��(&8)";
			this->toolStripMenuItem11->Click += gcnew System::EventHandler(this, &frmMain::toolStripMenuItem10_Click);
			// 
			// toolStripMenuItem13
			// 
			this->toolStripMenuItem13->Name = L"toolStripMenuItem13";
			this->toolStripMenuItem13->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::D9));
			this->toolStripMenuItem13->Size = System::Drawing::Size(172, 22);
			this->toolStripMenuItem13->Tag = L"9";
			this->toolStripMenuItem13->Text = L"9�� ��(&9)";
			this->toolStripMenuItem13->Click += gcnew System::EventHandler(this, &frmMain::toolStripMenuItem10_Click);
			// 
			// ����1tCtrl1ToolStripMenuItem
			// 
			this->����1tCtrl1ToolStripMenuItem->Name = L"����1tCtrl1ToolStripMenuItem";
			this->����1tCtrl1ToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::D0));
			this->����1tCtrl1ToolStripMenuItem->Size = System::Drawing::Size(172, 22);
			this->����1tCtrl1ToolStripMenuItem->Tag = L"10";
			this->����1tCtrl1ToolStripMenuItem->Text = L"10�� ��(&0)";
			this->����1tCtrl1ToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::toolStripMenuItem10_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(288, 6);
			// 
			// ������κ�������ƮVToolStripMenuItem
			// 
			this->������κ�������ƮVToolStripMenuItem->Name = L"������κ�������ƮVToolStripMenuItem";
			this->������κ�������ƮVToolStripMenuItem->ShortcutKeys = System::Windows::Forms::Keys::F4;
			this->������κ�������ƮVToolStripMenuItem->Size = System::Drawing::Size(291, 22);
			this->������κ�������ƮVToolStripMenuItem->Text = L"������� ���� ����Ʈ(&V)";
			this->������κ�������ƮVToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::������κ�������ƮVToolStripMenuItem_Click);
			// 
			// ����GToolStripMenuItem
			// 
			this->����GToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->����ġ��������RToolStripMenuItem, 
				this->toolStripMenuItem2, this->���Ӽ���RToolStripMenuItem, this->���ӹ����ϱ�PToolStripMenuItem});
			this->����GToolStripMenuItem->Name = L"����GToolStripMenuItem";
			this->����GToolStripMenuItem->Size = System::Drawing::Size(59, 20);
			this->����GToolStripMenuItem->Text = L"����(&G)";
			// 
			// ����ġ��������RToolStripMenuItem
			// 
			this->����ġ��������RToolStripMenuItem->Name = L"����ġ��������RToolStripMenuItem";
			this->����ġ��������RToolStripMenuItem->ShortcutKeys = System::Windows::Forms::Keys::F5;
			this->����ġ��������RToolStripMenuItem->Size = System::Drawing::Size(230, 22);
			this->����ġ��������RToolStripMenuItem->Text = L"�� ��ġ���� ����/����(&R)";
			this->����ġ��������RToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::����ġ��������RToolStripMenuItem_Click);
			// 
			// toolStripMenuItem2
			// 
			this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
			this->toolStripMenuItem2->Size = System::Drawing::Size(227, 6);
			// 
			// ���Ӽ���RToolStripMenuItem
			// 
			this->���Ӽ���RToolStripMenuItem->Name = L"���Ӽ���RToolStripMenuItem";
			this->���Ӽ���RToolStripMenuItem->Size = System::Drawing::Size(230, 22);
			this->���Ӽ���RToolStripMenuItem->Text = L"���� ����(&R)";
			this->���Ӽ���RToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::���Ӽ���RToolStripMenuItem_Click);
			// 
			// ���ӹ����ϱ�PToolStripMenuItem
			// 
			this->���ӹ����ϱ�PToolStripMenuItem->Name = L"���ӹ����ϱ�PToolStripMenuItem";
			this->���ӹ����ϱ�PToolStripMenuItem->ShortcutKeys = System::Windows::Forms::Keys::F9;
			this->���ӹ����ϱ�PToolStripMenuItem->Size = System::Drawing::Size(230, 22);
			this->���ӹ����ϱ�PToolStripMenuItem->Text = L"���� �����ϱ�(&P)";
			this->���ӹ����ϱ�PToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::���ӹ����ϱ�PToolStripMenuItem_Click);
			// 
			// ����HToolStripMenuItem
			// 
			this->����HToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->���ڳ뺧�����ؼ�ToolStripMenuItem});
			this->����HToolStripMenuItem->Name = L"����HToolStripMenuItem";
			this->����HToolStripMenuItem->Size = System::Drawing::Size(72, 20);
			this->����HToolStripMenuItem->Text = L"����(&H)";
			// 
			// ���ڳ뺧�����ؼ�ToolStripMenuItem
			// 
			this->���ڳ뺧�����ؼ�ToolStripMenuItem->Name = L"���ڳ뺧�����ؼ�ToolStripMenuItem";
			this->���ڳ뺧�����ؼ�ToolStripMenuItem->Size = System::Drawing::Size(203, 22);
			this->���ڳ뺧�����ؼ�ToolStripMenuItem->Text = L"���� �뺧�� ���ؼ�...(&A)";
			this->���ڳ뺧�����ؼ�ToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::���ڳ뺧�����ؼ�ToolStripMenuItem_Click);
			// 
			// menuEditorUndo
			// 
			this->menuEditorUndo->Name = L"menuEditorUndo";
			this->menuEditorUndo->Size = System::Drawing::Size(32, 19);
			// 
			// menuEditorRedo
			// 
			this->menuEditorRedo->Name = L"menuEditorRedo";
			this->menuEditorRedo->Size = System::Drawing::Size(32, 19);
			// 
			// statusBar
			// 
			this->statusBar->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->toolStripStatusLabel1, 
				this->toolStripStatusLabel2});
			this->statusBar->Location = System::Drawing::Point(0, 772);
			this->statusBar->Name = L"statusBar";
			this->statusBar->Size = System::Drawing::Size(1231, 22);
			this->statusBar->TabIndex = 7;
			this->statusBar->Text = L"statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
			this->toolStripStatusLabel1->AutoSize = false;
			this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
			this->toolStripStatusLabel1->Size = System::Drawing::Size(200, 17);
			// 
			// toolStripStatusLabel2
			// 
			this->toolStripStatusLabel2->AutoSize = false;
			this->toolStripStatusLabel2->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
			this->toolStripStatusLabel2->Name = L"toolStripStatusLabel2";
			this->toolStripStatusLabel2->Size = System::Drawing::Size(122, 17);
			// 
			// firstPage
			// 
			this->firstPage->Location = System::Drawing::Point(4, 22);
			this->firstPage->Name = L"firstPage";
			this->firstPage->Padding = System::Windows::Forms::Padding(3);
			this->firstPage->Size = System::Drawing::Size(1035, 0);
			this->firstPage->TabIndex = 0;
			this->firstPage->Text = L"���ο� ���� *";
			this->firstPage->UseVisualStyleBackColor = true;
			// 
			// tabFiles
			// 
			this->tabFiles->ContextMenuStrip = this->menuTabs;
			this->tabFiles->Controls->Add(this->firstPage);
			this->tabFiles->Location = System::Drawing::Point(8, 32);
			this->tabFiles->Name = L"tabFiles";
			this->tabFiles->SelectedIndex = 0;
			this->tabFiles->Size = System::Drawing::Size(1043, 21);
			this->tabFiles->TabIndex = 2;
			this->tabFiles->Selecting += gcnew System::Windows::Forms::TabControlCancelEventHandler(this, &frmMain::tabFiles_Selecting);
			this->tabFiles->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &frmMain::tabFiles_MouseDown);
			this->tabFiles->SelectedIndexChanged += gcnew System::EventHandler(this, &frmMain::tabFiles_SelectedIndexChanged);
			// 
			// menuTabs
			// 
			this->menuTabs->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->����ũ��ƮAToolStripMenuItem, 
				this->��ũ��Ʈ��������IToolStripMenuItem, this->��ũ��Ʈ����DToolStripMenuItem, this->toolStripMenuItem3, this->���������̵�LToolStripMenuItem, this->�����������̵�RToolStripMenuItem});
			this->menuTabs->Name = L"menuTabs";
			this->menuTabs->Size = System::Drawing::Size(186, 120);
			// 
			// ����ũ��ƮAToolStripMenuItem
			// 
			this->����ũ��ƮAToolStripMenuItem->Name = L"����ũ��ƮAToolStripMenuItem";
			this->����ũ��ƮAToolStripMenuItem->Size = System::Drawing::Size(185, 22);
			this->����ũ��ƮAToolStripMenuItem->Text = L"�� ��ũ��Ʈ(&A)";
			this->����ũ��ƮAToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::����ũ��ƮAToolStripMenuItem_Click);
			// 
			// ��ũ��Ʈ��������IToolStripMenuItem
			// 
			this->��ũ��Ʈ��������IToolStripMenuItem->Name = L"��ũ��Ʈ��������IToolStripMenuItem";
			this->��ũ��Ʈ��������IToolStripMenuItem->Size = System::Drawing::Size(185, 22);
			this->��ũ��Ʈ��������IToolStripMenuItem->Text = L"��ũ��Ʈ ��������(&I)";
			this->��ũ��Ʈ��������IToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::��ũ��Ʈ��������IToolStripMenuItem_Click);
			// 
			// ��ũ��Ʈ����DToolStripMenuItem
			// 
			this->��ũ��Ʈ����DToolStripMenuItem->Name = L"��ũ��Ʈ����DToolStripMenuItem";
			this->��ũ��Ʈ����DToolStripMenuItem->Size = System::Drawing::Size(185, 22);
			this->��ũ��Ʈ����DToolStripMenuItem->Text = L"��ũ��Ʈ �ݱ�(&C)";
			this->��ũ��Ʈ����DToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::��ũ��Ʈ����DToolStripMenuItem_Click);
			// 
			// toolStripMenuItem3
			// 
			this->toolStripMenuItem3->Name = L"toolStripMenuItem3";
			this->toolStripMenuItem3->Size = System::Drawing::Size(182, 6);
			// 
			// ���������̵�LToolStripMenuItem
			// 
			this->���������̵�LToolStripMenuItem->Name = L"���������̵�LToolStripMenuItem";
			this->���������̵�LToolStripMenuItem->Size = System::Drawing::Size(185, 22);
			this->���������̵�LToolStripMenuItem->Text = L"�������� �̵�(&L)";
			this->���������̵�LToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::���������̵�LToolStripMenuItem_Click);
			// 
			// �����������̵�RToolStripMenuItem
			// 
			this->�����������̵�RToolStripMenuItem->Name = L"�����������̵�RToolStripMenuItem";
			this->�����������̵�RToolStripMenuItem->Size = System::Drawing::Size(185, 22);
			this->�����������̵�RToolStripMenuItem->Text = L"���������� �̵�(&R)";
			this->�����������̵�RToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::�����������̵�RToolStripMenuItem_Click);
			// 
			// txtEditor
			// 
			this->txtEditor->Location = System::Drawing::Point(816, 64);
			this->txtEditor->Margins->Left = 0;
			this->txtEditor->Margins->Margin0->Width = 30;
			this->txtEditor->Margins->Margin3->Width = -10;
			this->txtEditor->Name = L"txtEditor";
			this->txtEditor->Size = System::Drawing::Size(408, 696);
			this->txtEditor->Styles->LineNumber->BackColor = System::Drawing::SystemColors::ActiveBorder;
			this->txtEditor->TabIndex = 13;
			this->txtEditor->TextDeleted += gcnew System::EventHandler<ScintillaNet::TextModifiedEventArgs^ >(this, &frmMain::txtEditor_TextDeleted);
			this->txtEditor->BeforeTextInsert += gcnew System::EventHandler<ScintillaNet::TextModifiedEventArgs^ >(this, &frmMain::txtEditor_BeforeTextInsert);
			this->txtEditor->BeforeTextDelete += gcnew System::EventHandler<ScintillaNet::TextModifiedEventArgs^ >(this, &frmMain::txtEditor_BeforeTextDelete);
			this->txtEditor->SelectionChanged += gcnew System::EventHandler(this, &frmMain::txtEditor_SelectionChanged);
			this->txtEditor->TextInserted += gcnew System::EventHandler<ScintillaNet::TextModifiedEventArgs^ >(this, &frmMain::txtEditor_TextInserted);
			this->txtEditor->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &frmMain::txtEditor_KeyUp);
			this->txtEditor->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &frmMain::txtEditor_KeyDown);
			// 
			// cbHelp
			// 
			this->cbHelp->DropDownHeight = 365;
			this->cbHelp->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cbHelp->FormattingEnabled = true;
			this->cbHelp->IntegralHeight = false;
			this->cbHelp->Location = System::Drawing::Point(501, 664);
			this->cbHelp->Name = L"cbHelp";
			this->cbHelp->Size = System::Drawing::Size(192, 20);
			this->cbHelp->Sorted = true;
			this->cbHelp->TabIndex = 20;
			this->cbHelp->SelectedIndexChanged += gcnew System::EventHandler(this, &frmMain::cbHelp_SelectedIndexChanged);
			// 
			// txtLogging
			// 
			this->txtLogging->Location = System::Drawing::Point(8, 664);
			this->txtLogging->Multiline = true;
			this->txtLogging->Name = L"txtLogging";
			this->txtLogging->ReadOnly = true;
			this->txtLogging->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->txtLogging->Size = System::Drawing::Size(432, 96);
			this->txtLogging->TabIndex = 19;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::Color::Transparent;
			this->label1->Location = System::Drawing::Point(448, 670);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(49, 12);
			this->label1->TabIndex = 23;
			this->label1->Text = L"���� :";
			// 
			// txtHelpContent
			// 
			this->txtHelpContent->Location = System::Drawing::Point(448, 688);
			this->txtHelpContent->Multiline = true;
			this->txtHelpContent->Name = L"txtHelpContent";
			this->txtHelpContent->ReadOnly = true;
			this->txtHelpContent->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->txtHelpContent->Size = System::Drawing::Size(360, 72);
			this->txtHelpContent->TabIndex = 21;
			// 
			// btPutColor
			// 
			this->btPutColor->Location = System::Drawing::Point(698, 662);
			this->btPutColor->Name = L"btPutColor";
			this->btPutColor->Size = System::Drawing::Size(104, 24);
			this->btPutColor->TabIndex = 22;
			this->btPutColor->Text = L"���� �ֱ�(&C)";
			this->btPutColor->UseVisualStyleBackColor = true;
			this->btPutColor->Click += gcnew System::EventHandler(this, &frmMain::btPutColor_Click);
			// 
			// btAddScript
			// 
			this->btAddScript->Location = System::Drawing::Point(1057, 31);
			this->btAddScript->Name = L"btAddScript";
			this->btAddScript->Size = System::Drawing::Size(84, 23);
			this->btAddScript->TabIndex = 24;
			this->btAddScript->Text = L"����(&O)";
			this->btAddScript->UseVisualStyleBackColor = true;
			this->btAddScript->Click += gcnew System::EventHandler(this, &frmMain::btAddScript_Click);
			// 
			// btCloseScript
			// 
			this->btCloseScript->Location = System::Drawing::Point(1140, 31);
			this->btCloseScript->Name = L"btCloseScript";
			this->btCloseScript->Size = System::Drawing::Size(84, 23);
			this->btCloseScript->TabIndex = 25;
			this->btCloseScript->Text = L"�ݱ�(&L)";
			this->btCloseScript->UseVisualStyleBackColor = true;
			this->btCloseScript->Click += gcnew System::EventHandler(this, &frmMain::btCloseScript_Click);
			// 
			// frmMain
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1231, 794);
			this->Controls->Add(this->btCloseScript);
			this->Controls->Add(this->btAddScript);
			this->Controls->Add(this->cbHelp);
			this->Controls->Add(this->txtLogging);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->txtHelpContent);
			this->Controls->Add(this->btPutColor);
			this->Controls->Add(this->statusBar);
			this->Controls->Add(this->tabFiles);
			this->Controls->Add(this->menuMain);
			this->Controls->Add(this->txtEditor);
			this->MainMenuStrip = this->menuMain;
			this->Name = L"frmMain";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"���ڳ뺧 �̹� Ver 1.0 ( ���� Ver 1.0 )";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &frmMain::frmMain_FormClosing);
			this->Resize += gcnew System::EventHandler(this, &frmMain::frmMain_Resize);
			this->menuMain->ResumeLayout(false);
			this->menuMain->PerformLayout();
			this->statusBar->ResumeLayout(false);
			this->statusBar->PerformLayout();
			this->tabFiles->ResumeLayout(false);
			this->menuTabs->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->txtEditor))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	protected:


		ExtCanvas^  picCanvas;
		ScriptFile ^currentScriptFile;

		int32 mLastLine;
		nekoGame *mCurrentGame;
		nekoCommands *mCurrentCommands;
		NekoNovel *mNekoNovel;
		bool mTestPlaying;
		bool scriptLoadingNow;
		float32 mZoomLevel;

	protected:

		/// Ÿ��Ʋ ����
		void RefreshTitle()
		{
			this->Text = ConvertToString(mCurrentGame->GetName()) + " - ���ڳ뺧 �̹� Ver " + String::Format("{0:##.00}", MIMI_VERSION * 0.01f)
				+ " ( ���� Ver " + String::Format("{0:##.00}", mNekoNovel->GetVersion() * 0.01f) + " )";

			if(mTestPlaying)
				this->Text += " [�׽�Ʈ �÷�����]";
		}

		/// ������ ���� �ε� �Ǿ��� ��
		void OnGameLoaded()
		{
			RefreshTitle();

			tabFiles->TabPages->Clear();

			list<nekoCommands *> &lst = mCurrentGame->GetCommandsList();
			list<nekoCommands *>::iterator iter = lst.begin();

			if(lst.size() > 0)
				mCurrentCommands = (*iter);
			else
				mCurrentCommands = NULL;

			TabPage ^page;
			ScriptFile ^sf;
			nekoCommands *cmds;
			for(;iter != lst.end();)
			{
				cmds = (*iter++);

				page = gcnew TabPage(ConvertToString(cmds->GetName()));
				sf = gcnew ScriptFile();

				// ���⼭ ���� ���������.
				{
					try
					{
						StreamReader ^streamReader = gcnew StreamReader(frmMain::mRunPath + "��ũ��Ʈ\\" + ConvertToString(cmds->GetName()), System::Text::Encoding::GetEncoding("EUC-KR"));
						sf->sourceFile = streamReader->ReadToEnd();

						streamReader->Close();
					} catch(FileNotFoundException ^)
					{	
						MessageBox::Show(this, "'" + ConvertToString(cmds->GetName()) + "' ��ũ��Ʈ ������ �� �� �����ϴ�!", MIMI_TITLE, MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
						mCurrentGame->RemoveCommands(cmds);

						continue;
					}
				}

				sf->commands = cmds;
				page->Tag = sf;
				tabFiles->TabPages->Add(page);
				tabFiles->SelectedTab = page;
			}

			if(lst.size() > 0)
			{
				/// 0�� �������� �̵�.
				tabFiles->SelectedIndex = 0;

				/// ���� �Ѱ� �ΰ�� �� ������ ȿ�� ����.
				if(lst.size() == 1)
				{
					tabFiles_Selecting(tabFiles, gcnew TabControlCancelEventArgs(tabFiles->SelectedTab, 0, false, TabControlAction::Selecting));
					tabFiles_SelectedIndexChanged(tabFiles, nullptr);
				}

				//txtEditor->Parent = tabFiles->SelectedTab;
				txtEditor->Visible = true;
				//txtEditor->Dock = DockStyle::Right;
				currentScriptFile = (ScriptFile ^)tabFiles->SelectedTab->Tag;
			}
		}

	private: System::Void tabFiles_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if(tabFiles->IsUpdating)
					 return;

				 if(tabFiles->SelectedTab)
				 {
					 //txtEditor->Parent = tabFiles->SelectedTab;
					 txtEditor->Visible = true;
					 //txtEditor->Dock = DockStyle::Fill;
					 currentScriptFile = (ScriptFile ^)tabFiles->SelectedTab->Tag;

					 txtEditor->Selection->Start = currentScriptFile->selectionStart;
					 txtEditor->Selection->Length = currentScriptFile->selectionLength;
					 //SetScrollPos((HWND)txtEditor->Handle.ToPointer(), SB_HORZ, currentScriptFile->hScroll, true);
					 //SetScrollPos((HWND)txtEditor->Handle.ToPointer(), SB_VERT, currentScriptFile->vScroll, true);
					 txtEditor->Scrolling->ScrollBy(0, currentScriptFile->vScroll);
				 }
				 else
				 {
					 txtEditor->Visible = false;
				 }

				 //txtEditor->Select(0, 0);
				 //txtEditor->Selection->Start = 0;
				 //txtEditor->Selection->Length = 0;
				 //txtEditor->ClearUndo();
				 txtEditor->UndoRedo->EmptyUndoBuffer();
				 txtEditor->Focus();

				 mLastLine = 1;
			 }
			private: String ^nkANSISubString(int32 startIndex, int32 endIndex)
			 {
				 return txtEditor->GetRange(startIndex, endIndex)->Text;
			 }
			private: System::Void txtEditor_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
			 {
				 int32 curLine = txtEditor->Lines->Current->Number + 1;

				 // ���� ������ �Ѿ�� ��
				 if(mLastLine != curLine)
				 {
					 if(e->KeyCode == Keys::Left || e->KeyCode == Keys::Right ||
						 e->KeyCode == Keys::Up || e->KeyCode == Keys::Down)
					 {
						 // ���� �ִ� ���� �������ϱ�
						 if(FAILED(mCurrentCommands->OnLineUpdated(curLine, ConvertToChar(txtEditor->Lines[curLine - 1]->Text))))
						 {
							 txtEditor->GetRange(txtEditor->Lines[curLine - 1]->StartPosition, txtEditor->Lines[curLine - 1]->EndPosition)->SetStyle(1);
						 }
						 else
						 {
							 txtEditor->GetRange(txtEditor->Lines[curLine - 1]->StartPosition, txtEditor->Lines[curLine - 1]->EndPosition)->SetStyle(0);
						 }

						 mLastLine = curLine;

						 return;
					 }

					 //mCurrentGame->GotoLineRequest(mCurrentCommands, curLine);
					 mCurrentGame->GotoLine(mCurrentCommands, curLine, false);
				 }

				 mLastLine = curLine;
			 }
	private: System::Void txtEditor_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
			 {
				 int32 curLine = txtEditor->Lines->Current->Number + 1;

				 if(mLastLine != curLine)
				 {
					 // �������� �ִ� ���� �������ϱ�
					 if(txtEditor->Lines->Count >= mLastLine)
					 {
						 if(FAILED(mCurrentCommands->OnLineUpdated(mLastLine, ConvertToChar(txtEditor->Lines[mLastLine - 1]->Text))))
						 {
							 txtEditor->GetRange(txtEditor->Lines[mLastLine - 1]->StartPosition, txtEditor->Lines[mLastLine - 1]->EndPosition)->SetStyle(1);
						 }
						 else
						 {
							 txtEditor->GetRange(txtEditor->Lines[mLastLine - 1]->StartPosition, txtEditor->Lines[mLastLine - 1]->EndPosition)->SetStyle(0);
						 }
					 }

					 //mCurrentGame->GotoLineRequest(mCurrentCommands, curLine);
					 mCurrentGame->GotoLine(mCurrentCommands, curLine, false);
				 }

				 mLastLine = curLine;
			 }

	private:
		delegate void TestDelegate(String ^txt);

	public:

		void OnLogRealReceived(String ^txt)
		{
			if(txtLogging->Text->Length == 0)
				txtLogging->Text = txt;
			else
				txtLogging->Text += Environment::NewLine + txt;
			txtLogging->Select(txtLogging->Text->Length, 0);
			txtLogging->ScrollToCaret();
		}

		static void OnLogReceived(const nekoEvent &e)
		{
			String ^str = ConvertToString(e.GetText(0));

			try
			{
				self->OnLogRealReceived(str);
			} catch(Exception ^)
			{
				try
				{
					TestDelegate ^realPut = gcnew TestDelegate(self , &frmMain::OnLogRealReceived);
					//self->Invoke(realPut, gcnew array<Object ^>{str});
					self->Invoke(realPut, gcnew array<Object ^>{str});
				} catch(Exception ^)
				{

				}
			}
		}

		/// ��� ������ �����.
		void SaveEmergencyFiles()
		{
			System::Windows::Forms::DialogResult res = 
				MessageBox::Show(this, "�˼��մϴ�. ���ڳ뺧 ������ ������ ������ϴ�.\n���α׷��� ����Ǿ�߸� �ϴ� ��Ȳ�Դϴ�.\n\n���� ������Ʈ�� �����Ͻð� �����Ͻðڽ��ϱ� ?", MIMI_TITLE, MessageBoxButtons::YesNo, MessageBoxIcon::Exclamation);
			if(res == System::Windows::Forms::DialogResult::No)
			{
				return;
			}

			�����������SToolStripMenuItem_Click(nullptr, nullptr);
		}

	private: System::Void ���ڳ뺧�����ؼ�ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 (gcnew HelpForm())->ShowDialog(this);
			 }

	private: System::Void ���ӹ����ϱ�PToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 System::Windows::Forms::DialogResult res = 
					 MessageBox::Show(this, "������ �����ϱ� ���� �����ϴ� ���� �����ϴ�.\n�����Ͻð� ���� �۾��� �Ͻðڽ��ϱ� ?", MIMI_TITLE, MessageBoxButtons::YesNo, MessageBoxIcon::Exclamation);
				 if(res == System::Windows::Forms::DialogResult::Yes)
				 {
					 �����������SToolStripMenuItem_Click(sender, e);
				 }

				 (gcnew GamePublishForm())->ShowDialog(this);
			 }
	private: System::Void frmMain_Resize(System::Object^  sender, System::EventArgs^  e)
			 {
				 tabFiles->Width = ((this->Width - 195) - tabFiles->Left);
				 txtEditor->Width = ((this->Width - 16) - txtEditor->Left);
				 txtEditor->Height = ((this->Height - 64) - txtEditor->Top);

				 txtLogging->Height = ((this->Height - 64) - txtLogging->Top);
				 txtHelpContent->Height = ((this->Height - 64) - txtHelpContent->Top);

				 btAddScript->Left = (this->Width - 190);
				 btCloseScript->Left = (this->Width - 107);
			 }
	private: System::Void ����ġ��������RToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 mTestPlaying = !mTestPlaying;

				 txtEditor->Enabled = !mTestPlaying;
				 btPutColor->Enabled = !mTestPlaying;

				 // �÷��� ���� ?!
				 int32 curLine = txtEditor->Lines->FromPosition(txtEditor->Lines->Current->StartPosition)->Number + 1;
				 if(mTestPlaying)
				 {
					 mCurrentGame->ChangeGameMode(GAME_RUN_PLAYING);
					 mCurrentGame->DoFrom(curLine + 1);
					 tabFiles->Enabled = false;
					 btAddScript->Enabled = false;
					 btCloseScript->Enabled = false;
				 }
				 else
				 {
					 mCurrentGame->ChangeGameMode(GAME_RUN_PREVIEW);
					 mCurrentGame->GotoLine(mCurrentCommands, curLine, false);
					 tabFiles->Enabled = true;
					 btAddScript->Enabled = true;
					 btCloseScript->Enabled = true;
				 }

				 RefreshTitle();
			 }
	private: System::Void ���Ӽ���RToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 (gcnew GameSettingForm())->ShowDialog(this);
				 RefreshTitle();

				 // �ػ� ���� ?
				 picCanvas->Width = mCurrentGame->GetEnvironment().mWidth * mZoomLevel;
				 picCanvas->Height = mCurrentGame->GetEnvironment().mHeight * mZoomLevel;

				 mNekoNovel->OnWindowChanged(mCurrentGame->GetEnvironment().mWidth,
					 mCurrentGame->GetEnvironment().mHeight, false);
			 }
	private: System::Void picCanvas_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
			 {
				 statusBar->Items[0]->Text = String::Format("���콺 ��ǥ : ({0}, {1})", (int)(e->X / mZoomLevel), (int)(e->Y / mZoomLevel));
			 }

	private: System::Void ���α׷�����XToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 Application::Exit();
			 }
	private: System::Void �����������SToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 mCurrentGame->Save();

				 // iterate through all the tab pages
				 for(int i = 0; i < tabFiles->TabCount; i++)
				 {
					 ScriptFile ^sf = (ScriptFile ^)tabFiles->TabPages[i]->Tag;

					 if(sf->changed)
					 {
						 try
						 {
							 // �����ϱ�
							 StreamWriter ^streamWriter = gcnew StreamWriter(frmMain::mRunPath + "��ũ��Ʈ\\" + ConvertToString(sf->commands->GetName()), FALSE, System::Text::Encoding::GetEncoding("EUC-KR"));
							 if(i == tabFiles->SelectedIndex)
								 sf->sourceFile = txtEditor->Text;

							 streamWriter->Write(sf->sourceFile);
							 streamWriter->Close();

							 tabFiles->TabPages[i]->Text = ConvertToString(sf->commands->GetName());
						 } catch(Exception ^)
						 {
							 MessageBox::Show(this, "'" +  ConvertToString(sf->commands->GetName()) + "' ��ũ��Ʈ�� ������ �� �����ϴ�.", MIMI_TITLE, MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
							 return;
						 }

						 sf->changed = false;
					 }
				 }
			 }
	private: System::Void tabFiles_Selecting(System::Object^  sender, System::Windows::Forms::TabControlCancelEventArgs^  e)
			 {
				 if(tabFiles->IsUpdating)
					 return;

				 if(currentScriptFile)
				 {
					 // ���� ������ �Ӽ� ��������
					 ScriptFile ^sf = (ScriptFile ^)currentScriptFile;
					 sf->sourceFile = txtEditor->Text;
					 sf->selectionStart = txtEditor->Selection->Start;
					 sf->selectionLength = txtEditor->Selection->Length;
					 sf->hScroll = GetScrollPos((HWND)txtEditor->Handle.ToPointer(), SB_HORZ);
					 sf->vScroll = txtEditor->Lines->FromPosition(txtEditor->PositionFromPoint(0, 0))->Number;//GetScrollPos((HWND)txtEditor->Handle.ToPointer(), SB_VERT);
					 

					 //txtEditor->Lines->Current->Text->ToCharArray();

					 int32 curLine = (txtEditor->Lines->Current->Number + 1);

					 // ���� �ִ� ���� �������ϱ�
					 mCurrentCommands->OnLineUpdated(curLine, 
						 ConvertToChar(nkANSISubString(txtEditor->Lines[curLine - 1]->StartPosition, txtEditor->Lines[curLine - 1]->EndPosition))
						 );
				 }

				 scriptLoadingNow = true;
				 txtEditor->Text = "";

				 mCurrentCommands = NULL;
				 if(e->TabPage)
				 {
					 ScriptFile ^sf = (ScriptFile ^)e->TabPage->Tag;

					 mCurrentCommands = sf->commands;

					 if(sf->sourceFile)
					 {
						 txtEditor->Text = sf->sourceFile;

						 array<String ^> ^lines = sf->sourceFile->Split('\n');

						 int32 lastIndex = -1;
						 int32 returnCharCount = 0;
						 while((lastIndex = sf->sourceFile->IndexOf('\n', lastIndex + 1)) >= 0)
							 ++returnCharCount;

						 if(returnCharCount > 0)
						 {
							 for(int32 i = 0;i < lines->Length;++i)
							 {
								 String ^str = lines[i];

								 // ���� ���� ���� ������ �ϱ�
								 if(FAILED(mCurrentCommands->OnLineUpdated(i + 1, ConvertToChar(lines[i]))))
								 {
									 txtEditor->GetRange(txtEditor->Lines[i]->StartPosition, txtEditor->Lines[i]->EndPosition)->SetStyle(1);
								 }
								 else
								 {
									 // ���� �ٲٱ�
									 txtEditor->GetRange(txtEditor->Lines[i]->StartPosition, txtEditor->Lines[i]->EndPosition)->SetStyle(0);

									 // �ּ��̱���!
									 if(str->Length >= 2 && str[0] == '/' && str[1] == '/')
									 {
										 txtEditor->GetRange(txtEditor->Lines[i]->StartPosition, txtEditor->Lines[i]->EndPosition)->SetStyle(2);
									 }
									 else
									 {
										 SetCommandFade(i + 1);
									 }
								 }
							 }
						}	
					 }

					 mCurrentGame->GotoLine(mCurrentCommands, 1, false);
				 }

				 scriptLoadingNow = false;
			 }
	private: System::Void frmMain_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
			 {
				 System::Windows::Forms::DialogResult res = MessageBox::Show(this, "���� �۾����� ������ �����ϰ� �����Ͻðھ��?", MIMI_TITLE, MessageBoxButtons::YesNoCancel, MessageBoxIcon::Exclamation);

				 if(res == System::Windows::Forms::DialogResult::Yes)
				 {
					 �����������SToolStripMenuItem_Click(sender, e);
					 return;
				 } else if(res == System::Windows::Forms::DialogResult::No)
				 {
					 return;
				 }

				 e->Cancel = true;
			 }
	private: System::Void ���ý�ũ��Ʈ������SToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 ScriptFile ^sf = (ScriptFile ^)tabFiles->SelectedTab->Tag;

				 try
				 {
					 // �����ϱ�
					 StreamWriter ^streamWriter = gcnew StreamWriter(frmMain::mRunPath + "��ũ��Ʈ\\" + ConvertToString(sf->commands->GetName()), false, System::Text::Encoding::GetEncoding("EUC-KR"));
					 streamWriter->Write(txtEditor->Text);
					 streamWriter->Close();

					 tabFiles->SelectedTab->Text = ConvertToString(sf->commands->GetName());
				 } catch(Exception ^exp)
				 {
					 MessageBox::Show(this, "'" +  ConvertToString(sf->commands->GetName()) + "' ��ũ��Ʈ�� ������ �� �����ϴ�.\n���� : " + exp->ToString(), MIMI_TITLE, MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
					 return;
				 }

				 sf->changed = false;
			 }
	private: System::Void ����ũ��ƮAToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 NewScriptFileForm ^frm = gcnew NewScriptFileForm();
				 if(frm->ShowDialog(this) == System::Windows::Forms::DialogResult::Cancel)
					 return;

				 nekoCommands *cmds = mCurrentGame->CreateCommands(ConvertToChar(frm->mFileName));
				 if(cmds == NULL)
				 {
					 MessageBox::Show(this, "'" +  frm->mFileName + "' �̸��� ��ũ��Ʈ�� �̹� �����ؿ�.", MIMI_TITLE, MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
					 return;
				 }

				 TabPage ^tp = gcnew TabPage(frm->mFileName);
				 ScriptFile ^sf = gcnew ScriptFile();
				 sf->commands = cmds;
				 tp->Tag = sf;
				 tabFiles->TabPages->Add(tp);
			 }
	private: System::Void ��ũ��Ʈ��������IToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if(mTestPlaying)
					 return;

				 OpenFileDialog ^dlg = gcnew OpenFileDialog();
				 dlg->Title = "��ũ��Ʈ ���� ��������";
				 dlg->Multiselect = true;
				 dlg->Filter = "EUC-KR ������ �ؽ�Ʈ ����(*.txt)|*.txt";

				 if(dlg->ShowDialog(this) == System::Windows::Forms::DialogResult::OK)
				 {
					 String ^fileName;
					 nekoCommands *cmds;
					 TabPage ^page;

					 for(int32 i = 0;i < dlg->FileNames->Length;++i)
					 {
						 fileName = Path::GetFileName(dlg->FileNames[i]);

						 cmds = mCurrentGame->CreateCommands(ConvertToChar(fileName));
						 if(cmds == NULL)
						 {
							 continue;
						 }

						 page = gcnew TabPage(ConvertToString(cmds->GetName()));
						 ScriptFile ^sf = gcnew ScriptFile();

						 try
						 {
							 Stream ^stream = System::IO::File::OpenRead(dlg->FileNames[i]);

							 StreamReader ^streamReader = gcnew StreamReader(stream, System::Text::Encoding::GetEncoding("EUC-KR"));
							 sf->sourceFile = streamReader->ReadToEnd();

							 streamReader->Close();
							 stream->Close();
						 } catch(FileNotFoundException ^)
						 {
							 MessageBox::Show(this, "'" + fileName + "' ��ũ��Ʈ ������ �� �� �����ϴ�!", MIMI_TITLE, MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
							 mCurrentGame->RemoveCommands(cmds);

							 continue;
						 }

						 // �ٸ� ������ ���� ��쿡�� ����.
						 if(String::Compare(dlg->FileNames[i], Application::StartupPath + "\\��ũ��Ʈ\\" + fileName, StringComparison::CurrentCultureIgnoreCase) != 0)
							 System::IO::File::Copy(dlg->FileNames[i], Application::StartupPath + "\\��ũ��Ʈ\\" + fileName, true);

						 sf->commands = cmds;
						 page->Tag = sf;
						 tabFiles->TabPages->Add(page);
					 }

					 Directory::SetCurrentDirectory(mRunPath);
				 }
			 }
	private: System::Void ��ũ��Ʈ����DToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if(mTestPlaying)
					 return;


				 if(!tabFiles->SelectedTab)
					 return;

				 if(tabFiles->TabCount == 1)
				 {
					 MessageBox::Show(this, "������ '�ٻ�' �Դϴ�! ��_��..", MIMI_TITLE, MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
					 return;
				 }

				 // ���� ���� ���� �����.
				 ScriptFile ^sf = (ScriptFile ^)tabFiles->SelectedTab->Tag;

				 if(sf->changed)
				 {
					 System::Windows::Forms::DialogResult res = 
						 MessageBox::Show(this, "�ش� ��ũ��Ʈ ������ �����ϰ� �����ðڽ��ϱ� ?", MIMI_TITLE, MessageBoxButtons::YesNoCancel, MessageBoxIcon::Exclamation);

					 if(res == System::Windows::Forms::DialogResult::Yes)
					 {
						 try
						 {
							 // �����ϱ�
							 StreamWriter ^streamWriter = gcnew StreamWriter(Application::StartupPath + "\\��ũ��Ʈ\\" + ConvertToString(sf->commands->GetName()), false, System::Text::Encoding::GetEncoding("EUC-KR"));
							 streamWriter->Write(txtEditor->Text);

							 tabFiles->SelectedTab->Text = ConvertToString(sf->commands->GetName());
						 } catch(Exception ^)
						 {
							 MessageBox::Show(this, "'" +  ConvertToString(sf->commands->GetName()) + "' ��ũ��Ʈ�� ������ �� �����ϴ�.", MIMI_TITLE, MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
							 return;
						 }
					 }
					 else if(res == System::Windows::Forms::DialogResult::Cancel)
					 {
						 return;
					 }
				 }

				 int32 selectedIndex = tabFiles->SelectedIndex;

				 mCurrentGame->RemoveCommands(sf->commands);
				 tabFiles->TabPages->Remove(tabFiles->SelectedTab);

				 tabFiles->SelectedIndex = max(min(selectedIndex, tabFiles->TabPages->Count - 1), 0);
			 }
	private: System::Void tabFiles_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
			 {
				 // check if the right mouse button was pressed
				 if(e->Button == System::Windows::Forms::MouseButtons::Right)
				 {
					 // iterate through all the tab pages
					 for(int i = 0; i < tabFiles->TabCount; i++)
					 {
						 // get their rectangle area and check if it contains the mouse cursor
						 System::Drawing::Rectangle r = tabFiles->GetTabRect(i);
						 if (r.Contains(e->Location))
						 {
							 tabFiles->SelectedIndex = i;
						 }
					 }
				 }

			 }
	private: System::Void txtEditor_SelectionChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 int32 curLine = (txtEditor->Lines->Current->Number + 1);
				 statusBar->Items[1]->Text = "���� ���� : " + curLine;

				 if(mLastLine != curLine)
				 {
					 /// ���� ������ �������Ѵ�.
					 if(curLine != mLastLine)
					 {
						 String ^str = txtEditor->Lines[mLastLine - 1]->Text;

						 // �������� �ִ� ���� �������ϱ�
						 if(FAILED(mCurrentCommands->OnLineUpdated(mLastLine, ConvertToChar(str))))
						 {
							 txtEditor->GetRange(txtEditor->Lines[mLastLine - 1]->StartPosition, txtEditor->Lines[mLastLine - 1]->EndPosition)->SetStyle(1);
						 }
						 else
						 {
							 txtEditor->GetRange(txtEditor->Lines[mLastLine - 1]->StartPosition, txtEditor->Lines[mLastLine - 1]->EndPosition)->SetStyle(0);

							 // �ּ��̱���!
							 if(str->Length >= 2 &&
								 str[0] == '/' && str[1] == '/')
							 {
								 txtEditor->GetRange(txtEditor->Lines[mLastLine - 1]->StartPosition, txtEditor->Lines[mLastLine - 1]->EndPosition)->SetStyle(2);
							 }
							 else
							 {
								 SetCommandFade(mLastLine);
							 }
						 }

						 //mCurrentGame->GotoLineRequest(mCurrentCommands, curLine);
						 mCurrentGame->GotoLine(mCurrentCommands, curLine, false);
					 }

				 }
				 mLastLine = curLine;

				 // ���� ����

				 if(txtEditor->Selection->Length > 1 && txtEditor->Selection->Length < MAX_COMMAND_SIZE * 3/2)
				 {
					 int32 res = cbHelp->FindString(txtEditor->Selection->Text);
					 if(res < 0)
						 return;

					 cbHelp->SelectedIndex = res;
				 }
			 }
	private: System::Void cbHelp_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 const char8 **cmdHelpStrings = nekoCommands::GetCommandHelpStrings();
				 HelpComboItem ^item = (HelpComboItem ^)cbHelp->SelectedItem;

				 if(cmdHelpStrings[item->mValue] != NULL)
					 txtHelpContent->Text = item->mName + " " + ConvertToString(cmdHelpStrings[item->mValue])->Replace("\n", Environment::NewLine);
				 else
					 txtHelpContent->Text = "";
			 }
	private: System::Void btPutColor_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 colorDialog->AllowFullOpen = true;
				 colorDialog->ShowHelp = true;

				 if(colorDialog->ShowDialog(this) == System::Windows::Forms::DialogResult::OK)
				 {
					 String ^str;
					 str = colorDialog->Color.R.ToString() + " " + colorDialog->Color.G.ToString() + " " + colorDialog->Color.B.ToString();

					 txtEditor->Selection->Text = str;
				 }
			 }
	private:

		private: System::Void txtEditor_BeforeTextInsert(System::Object^  sender, ScintillaNet::TextModifiedEventArgs^  e)
		{
			if(scriptLoadingNow)
				return;

			String^ newData = e->Text;
			int32 returnCharCount = 0;
			int32 lastIndex = -1;

			int32 startIndex = txtEditor->Selection->Start;

			// ���� �༮ ����.
			if(txtEditor->Selection->Length > 0)
			{
				while((lastIndex = txtEditor->Selection->Text->IndexOf('\n', lastIndex + 1)) >= 0)
					++returnCharCount;

				if(returnCharCount > 0)
				{
					int32 selLine = txtEditor->Lines->FromPosition(txtEditor->Selection->Start)->Number + 1;

					mCurrentCommands->OnLineRemoved(selLine + 1, returnCharCount);
				}
			}

			returnCharCount = 0;
			lastIndex = -1;

			int32 selLine = txtEditor->Lines->FromPosition(startIndex)->Number + 1;
		}
		private: System::Void txtEditor_BeforeTextDelete(System::Object^  sender, ScintillaNet::TextModifiedEventArgs^  e)
		{
			if(scriptLoadingNow)
				return;

			int32 returnCharCount = 0;
			int32 lastIndex = -1;

			if(e->Length > 1)
			{
				String ^str = txtEditor->GetRange(e->Position, e->Position + e->Length)->Text;
				while((lastIndex = str->IndexOf('\n', lastIndex + 1)) >= 0)
					++returnCharCount;

				if(returnCharCount > 0)
				{
					int32 selLine = txtEditor->Lines->FromPosition(e->Position)->Number + 1;

					mCurrentCommands->OnLineRemoved(selLine + 1, returnCharCount);
				}
			}
			else
			{
				int32 selLine = txtEditor->Lines->FromPosition(e->Position)->Number + 1;
			
				if(txtEditor->CharAt(e->Position) == '\n')
				{
					mCurrentCommands->OnLineRemoved(selLine + 1, 1);
					mCurrentCommands->OnLineUpdated(selLine, ConvertToChar(txtEditor->Lines[selLine - 1]->Text));
				}
			}
			
		}
		private: void SetCommandFade(int32 lastLine)
		{
			ScintillaNet::Range ^range = txtEditor->FindReplace->Find(gcnew ScintillaNet::Range(txtEditor->Lines[lastLine - 1]->StartPosition,
				txtEditor->Lines[lastLine - 1]->EndPosition, txtEditor), " ", 0);

		 if(!range || !range->Length)
			 txtEditor->GetRange(txtEditor->Lines[lastLine - 1]->StartPosition, txtEditor->Lines[lastLine - 1]->EndPosition)->SetStyle(3);
		 else
		 {
			 
			 ScintillaNet::Range ^range2 = txtEditor->GetRange(txtEditor->Lines[lastLine - 1]->StartPosition, range->Start);
			 E_COMMAND_TYPE cmdType = nekoCommands::GetCommandByName(ConvertToChar(range2->Text));

			 if(cmdType < ECT_NOTHING && gCommandColors[cmdType])
			 {
				 range2->SetStyle(20 + gCommandColors[cmdType]);
			 }
			 else
			 {
				 range2->SetStyle(3);
			 }

			 txtEditor->GetRange(range->Start, txtEditor->Lines[lastLine - 1]->EndPosition)->SetStyle(0);
		 }
		}
		private: System::Void txtEditor_TextInserted(System::Object^  sender, ScintillaNet::TextModifiedEventArgs^  e)
		{
			if(scriptLoadingNow)
				return;

			int32 lastIndex = -1;
			int32 returnCharCount = 0;
			int32 selLine = txtEditor->Lines->FromPosition(e->Position)->Number + 1;

			// �߰�!
			array<String ^> ^lines = e->Text->Split('\n');

			while((lastIndex =  e->Text->IndexOf('\n', lastIndex + 1)) >= 0)
				++returnCharCount;

			if(returnCharCount > 0)
			{
				int32 startIndex;
				int32 endIndex;
				for(int32 i = 0;i < lines->Length;++i)
				{
					startIndex = (txtEditor->Lines[selLine + i - 1])->StartPosition;
					endIndex = (txtEditor->Lines[selLine + i - 1])->EndPosition;

					String ^str;

					if(!i)
						str = txtEditor->Lines[selLine + i - 1]->Text;
					else
						str = lines[i];

					// ���� ���� ���� ������ �ϱ�
					if((!i && FAILED(mCurrentCommands->OnLineUpdated(selLine + i, ConvertToChar(str)))) ||
						(i && (FAILED(mCurrentCommands->OnLineInserted(selLine + i - 1, ConvertToChar(str)))))
						)
					{
						txtEditor->GetRange(startIndex, endIndex)->SetStyle(1);
					}
					else
					{
						txtEditor->GetRange(startIndex, endIndex)->SetStyle(0);

						// �ּ��̱���!
						if(str->Length >= 2 &&
							str[0] == '/' && str[1] == '/')
						{
							txtEditor->GetRange(startIndex, endIndex)->SetStyle(2);
						}
						else
						{
							SetCommandFade(selLine + i);
						}
					}
					
				}
			}
			else
			{
				int32 curLine = selLine;

				/// ���� ������ �������Ѵ�.
				if(curLine != mLastLine)
				{
					String ^str = txtEditor->Lines[mLastLine - 1]->Text;

					// �������� �ִ� ���� �������ϱ�
					if(FAILED(mCurrentCommands->OnLineUpdated(mLastLine, ConvertToChar(str))))
					{
						txtEditor->GetRange(txtEditor->Lines[mLastLine - 1]->StartPosition, txtEditor->Lines[mLastLine - 1]->EndPosition)->SetStyle(1);
					}
					else
					{
						txtEditor->GetRange(txtEditor->Lines[mLastLine - 1]->StartPosition, txtEditor->Lines[mLastLine - 1]->EndPosition)->SetStyle(0);

						// �ּ��̱���!
						if(str->Length >= 2 &&
							str[0] == '/' && str[1] == '/')
						{
							txtEditor->GetRange(txtEditor->Lines[mLastLine - 1]->StartPosition, txtEditor->Lines[mLastLine - 1]->EndPosition)->SetStyle(2);
						}
						else
						{
							SetCommandFade(mLastLine);
						}
					}
				}

				mLastLine = curLine;
			};

			ScriptFile ^sf = (ScriptFile ^)tabFiles->SelectedTab->Tag;

			if(sf->changed == false)
			{
				tabFiles->SelectedTab->Text += " *";
				sf->changed = true;
			}
		}
		private: System::Void txtEditor_TextDeleted(System::Object^  sender, ScintillaNet::TextModifiedEventArgs^  e)
		{
			if(scriptLoadingNow)
				return;

			ScriptFile ^sf = (ScriptFile ^)tabFiles->SelectedTab->Tag;

			if(sf->changed == false)
			{
				tabFiles->SelectedTab->Text += " *";
				sf->changed = true;
			}
		}
		private: System::Void ����ũ���3ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			mZoomLevel = 1;
			picCanvas->Width = mCurrentGame->GetEnvironment().mWidth;
			picCanvas->Height = mCurrentGame->GetEnvironment().mHeight;
		}
		private: System::Void �����̱�ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			mZoomLevel = 0.75f;
			picCanvas->Width = mCurrentGame->GetEnvironment().mWidth * mZoomLevel;
			picCanvas->Height = mCurrentGame->GetEnvironment().mHeight * mZoomLevel;
		}
		private: System::Void �����̱�2ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			mZoomLevel = 0.5f;
			picCanvas->Width = mCurrentGame->GetEnvironment().mWidth * mZoomLevel;
			picCanvas->Height = mCurrentGame->GetEnvironment().mHeight * mZoomLevel;
		}
		private: System::Void picCanvas_SizeChanged(System::Object^  sender, System::EventArgs^  e) {

			mNekoNovel->SetScreenZoom(mZoomLevel);

			cbHelp->Left = picCanvas->Right - 360 + (cbHelp->Left - txtHelpContent->Left);
			btPutColor->Left = picCanvas->Right - 360 + (btPutColor->Left - txtHelpContent->Left);
		    txtHelpContent->Left = picCanvas->Right - 360;
			label1->Left = txtHelpContent->Left;
			txtLogging->Width = txtHelpContent->Left - 20;
			txtEditor->Left = picCanvas->Right + 10;
			txtEditor->Width = ((this->Width - 16) - txtEditor->Left);

			txtLogging->Top = picCanvas->Bottom + 10;
			cbHelp->Top = picCanvas->Bottom + 10;
			btPutColor->Top = picCanvas->Bottom + 10;
			label1->Top = picCanvas->Bottom +  10;
			txtHelpContent->Top = cbHelp->Bottom + 10;

			frmMain_Resize(nullptr, nullptr);
		}
		private: System::Void �ּ�CToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			
		}
		private: System::Void ���������̵�LToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			if(!tabFiles->SelectedTab || tabFiles->SelectedIndex == 0)
				return;

			int32 idx = tabFiles->SelectedIndex;
			TabPage ^page = tabFiles->SelectedTab;
			tabFiles->TabPages->Remove(page);
			tabFiles->TabPages->Insert(idx - 1, page);
			tabFiles->SelectedTab = page;

			mCurrentGame->ChangeCommandsOrder(((ScriptFile ^)page->Tag)->commands, idx - 1);
		}
		private: System::Void �����������̵�RToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			if(!tabFiles->SelectedTab || tabFiles->SelectedIndex == tabFiles->TabPages->Count - 1)
				return;

			int32 idx = tabFiles->SelectedIndex;
			TabPage ^page = tabFiles->SelectedTab;
			tabFiles->TabPages->Remove(page);
			tabFiles->TabPages->Insert(idx + 1, page);
			tabFiles->SelectedTab = page;

			mCurrentGame->ChangeCommandsOrder(((ScriptFile ^)page->Tag)->commands, idx + 1);
		}
		private: System::Void ������κ�������ƮVToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			(gcnew VarListForm())->Show();
		}
		private: System::Void btAddScript_Click(System::Object^  sender, System::EventArgs^  e)
		{
			��ũ��Ʈ��������IToolStripMenuItem_Click(sender, e);
		}
		private: System::Void btCloseScript_Click(System::Object^  sender, System::EventArgs^  e)
		{
			��ũ��Ʈ����DToolStripMenuItem_Click(sender, e);
		}
		private: System::Void ������ũ��Ʈ�ΰ���PToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			if(mTestPlaying)
				return;

			if(tabFiles->SelectedIndex - 1 < 0)
				tabFiles->SelectedIndex = tabFiles->TabPages->Count - 1;
			else
				tabFiles->SelectedIndex--;
		}
		private: System::Void ������ũ��Ʈ�γѾ��NToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			if(mTestPlaying)
				return;

			if(tabFiles->SelectedIndex + 1 >= tabFiles->TabPages->Count)
				tabFiles->SelectedIndex = 0;
			else
				tabFiles->SelectedIndex++;
		}
		private: System::Void �۾����ν�ũ��Ʈ�ݱ�CToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			if(mTestPlaying)
				return;

			��ũ��Ʈ����DToolStripMenuItem_Click(sender, e);
		}
		private: System::Void ��ũ��Ʈ��������OToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			��ũ��Ʈ��������IToolStripMenuItem_Click(sender, e);
		}

		// �ǰ� ���� �̵�
		private: System::Void toolStripMenuItem10_Click(System::Object^  sender, System::EventArgs^  e)
		{
			if(mTestPlaying)
				return;

			int32 newIdx = int::Parse( (String ^)((ToolStripMenuItem ^)sender)->Tag) - 1;
			
			if(newIdx >= tabFiles->TabPages->Count)
				return;

			tabFiles->SelectedIndex = newIdx;
		}
};
}

