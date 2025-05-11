#pragma once

#include "pch.h"

// Project Name : My_GDIPlus
class CMy_GDIPlusApp : public CWinApp
{
public:
    CMy_GDIPlusApp();
    //아래 부분을 추가.
    ULONG_PTR m_gdiplusToken;
    // Overrides
public:
    virtual BOOL InitInstance();

    // Implementation

    DECLARE_MESSAGE_MAP()
};
