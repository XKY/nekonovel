#include "nekoPrecompiledHeader.h"
#include "nekoPublisher.h"
#include "NekoNovel.h"
#include "nekoAutoPtr.h"
#include "nekoUtility.h"

namespace neko
{
	static const char8 *gPackageNames[][3] =
	{
		{"�̹���\\*", "Images.nkpack", "�̹���\\"},
		{"��ũ��Ʈ\\*", "Scripts.nkpack", "��ũ��Ʈ\\"},
		{"Command.xml", "Core.nkpack", ""},
		{"ȿ����\\*", "FX.nkpack", "ȿ����\\"},
		{"��ƼŬ\\*", "Particles.nkpack", "��ƼŬ\\"},
		{"���̽�\\*", "Voice.nkpack", "���̽�\\"},
		{"�������\\*", "BGM.nkpack", "�������\\"},
	};

	nekoPublisher::nekoPublisher()
	{

	}

	nekoPublisher::~nekoPublisher()
	{

	}

	// ��Ű¡ ���� �ʴ� ���ϵ� ����..
	void CopyNoPackageFiles(string name)
	{
		string strSrc, strDest;
		HANDLE hFile;
		WIN32_FIND_DATA foundData;

		name = '\\' + name;
		name += '\\';

		strSrc = GetNekoNovel()->GetAppFolderPath();;
		strSrc += name; strSrc += '*';
		hFile = FindFirstFile(strSrc.c_str(), &foundData);

		if(hFile == NULL)
			return;

		strDest = GetNekoNovel()->GetCurrentGame()->GetEnvironment().mLastPublishFolder;
		strDest += name;
		CreateDirectory(strDest.c_str(), NULL);

		do 
		{
			if(foundData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				continue;

			strSrc = GetNekoNovel()->GetAppFolderPath();
			strSrc += name;
			strSrc += foundData.cFileName;

			strDest = GetNekoNovel()->GetCurrentGame()->GetEnvironment().mLastPublishFolder;
			strDest += name;
			strDest += foundData.cFileName;
			CopyFile(strSrc.c_str(), strDest.c_str(), false);

		} while (FindNextFile(hFile, &foundData));

		FindClose(hFile);
	}

	// ���� ���ϵ� ����
	void CopyLocalFiles(string format)
	{
		string strSrc, strDest;
		HANDLE hFile;
		WIN32_FIND_DATA foundData;

		strSrc = GetNekoNovel()->GetAppFolderPath();
		strSrc += format;
		hFile = FindFirstFile(strSrc.c_str(), &foundData);

		if(hFile == NULL)
			return;

		do 
		{
			if(foundData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				continue;

			strSrc = GetNekoNovel()->GetAppFolderPath();
			strSrc += foundData.cFileName;

			strDest = GetNekoNovel()->GetCurrentGame()->GetEnvironment().mLastPublishFolder;
			strDest += '\\';
			strDest += foundData.cFileName;
			CopyFile(strSrc.c_str(), strDest.c_str(), false);

		} while (FindNextFile(hFile, &foundData));

		FindClose(hFile);
	}

	bool CopyToPublishRoot(const char8 *src, const char8 *dest)
	{
		string strSrc;
		string strDest;

		strSrc = GetNekoNovel()->GetAppFolderPath();
		strSrc += '\\'; strSrc += src;

		strDest = GetNekoNovel()->GetCurrentGame()->GetEnvironment().mLastPublishFolder;
		strDest += '\\'; strDest += dest;
		return (CopyFile(strSrc.c_str(), strDest.c_str(), false));
	}

	/// Do something
	uint32 nekoPublisher::Do()
	{
		nekoFileSystem *fs = GetNekoNovel()->GetFileSystem();

		float32 total = 5;
		string strSrc;
		string strDest;
		nekoEvent e(EVENT_PUBLISHING_STATUS);

		int32 i = 0;
		for(;i <= 5;++i)
		{
			// �ߴ� ��û.
			if(mValid == false)
			{
				e.value[0] = -3;
				GetNekoNovel()->OnEvent(e);
				return -1;
			}

			strSrc = GetNekoNovel()->GetAppFolderPath();
			strSrc += "\\"; strSrc += gPackageNames[i][0];

			strDest = GetNekoNovel()->GetCurrentGame()->GetEnvironment().mLastPublishFolder;
			strDest += "\\"; strDest += gPackageNames[i][1];

			if(FAILED(nekoPackage::GeneratePackage(strSrc.c_str(), strDest.c_str(), gPackageNames[i][2])))
			{
				e.value[0] = -2;
				GetNekoNovel()->OnEvent(e);

				return -1;
			}

			e.value[0] = (int32)((i + 1) / total * 100);
			GetNekoNovel()->OnEvent(e);
		}

		// �ߴ� ��û.
		if(mValid == false)
		{
			e.value[0] = -3;
			GetNekoNovel()->OnEvent(e);
			return -1;
		}

		// ������
		CopyNoPackageFiles("������");

		e.value[0] = (int32)80;
		GetNekoNovel()->OnEvent(e);

		// BGM�� ����.
		if(GetNekoNovel()->GetCurrentGame()->GetEnvironment().mCompressBGM)
		{
			strSrc = GetNekoNovel()->GetAppFolderPath();
			strSrc += "\\"; strSrc += gPackageNames[i][0];

			strDest = GetNekoNovel()->GetCurrentGame()->GetEnvironment().mLastPublishFolder;
			strDest += "\\"; strDest += gPackageNames[i][1];

			if(FAILED(nekoPackage::GeneratePackage(strSrc.c_str(), strDest.c_str(), gPackageNames[i][2])))
			{
				e.value[0] = -2;
				GetNekoNovel()->OnEvent(e);

				return -1;
			}

			e.value[0] = (int32)90;
			GetNekoNovel()->OnEvent(e);
		}
		else
		{
			// ������� 
			CopyNoPackageFiles("�������");

			e.value[0] = (int32)90;
			GetNekoNovel()->OnEvent(e);
		}

		// �ߴ� ��û.
		if(mValid == false)
		{
			e.value[0] = -3;
			GetNekoNovel()->OnEvent(e);
			return -1;
		}

		// �ܺ���Ʈ ���� �����ϱ�
		CopyLocalFiles("*.ttc");
		CopyLocalFiles("*.ttf");

		// ���� ���� �����ϱ�
		static const char8 *neededFiles[] = {"Game.exe", "Engine.dll", "audiere.dll", "d3dx9_40.dll", "Game.exe",
		/*"msvcm80.dll", */"msvcp80.dll", "msvcr80.dll", "Engine.dll.intermediate.manifest",
		"Microsoft.VC80.CRT.manifest", "Game.exe.intermediate.manifest", "BugTrap.dll", "Flash.ocx", 0};
		i = 0;
		while(neededFiles[i])
		{
			CopyToPublishRoot(neededFiles[i], neededFiles[i]);
			++i;
		}

		e.value[0] = 100;
		GetNekoNovel()->OnEvent(e);

		// ������!
		e.value[0] = -1;
		GetNekoNovel()->OnEvent(e);
		return 0;
	}
}