
// MyProject.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMyProjectApp:
// �� Ŭ������ ������ ���ؼ��� MyProject.cpp�� �����Ͻʽÿ�.
//

class CMyProjectApp : public CWinApp
{
public:
	CMyProjectApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMyProjectApp theApp;