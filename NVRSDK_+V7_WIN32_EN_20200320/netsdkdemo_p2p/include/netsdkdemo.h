
// netsdkdemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CnetsdkdemoApp:
// �йش����ʵ�֣������ netsdkdemo.cpp
//

class CnetsdkdemoApp : public CWinAppEx
{
public:
	CnetsdkdemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CnetsdkdemoApp theApp;