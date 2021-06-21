
// HaveFunDrivingPlane.h: HaveFunDrivingPlane 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // 主符号


// CHaveFunDrivingPlaneApp:
// 有关此类的实现，请参阅 HaveFunDrivingPlane.cpp
//

class CHaveFunDrivingPlaneApp : public CWinApp
{
public:
	CHaveFunDrivingPlaneApp() noexcept;


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CHaveFunDrivingPlaneApp theApp;
