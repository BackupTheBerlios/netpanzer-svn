/*
Copyright (C) 1998 Pyrosoft Inc. (www.pyrosoftgames.com), Matthew Bogue

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#if !defined(AFX_IMAGEROBOTDLG_H__59415956_1FF1_11D2_866F_444553540000__INCLUDED_)
#define AFX_IMAGEROBOTDLG_H__59415956_1FF1_11D2_866F_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "CropInfoDialog.h"
#include "RenameInfoDialog.h"
#include "PackFileDialog.h"
#include "ColorPakDialog.h"


/////////////////////////////////////////////////////////////////////////////
// CImageRobotDlg dialog

class CImageRobotDlg : public CDialog
{
// Construction
public:
	CImageRobotDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CImageRobotDlg)
	enum { IDD = IDD_IMAGEROBOT_DIALOG };
	CStatic	m_SourceFileCountCap;
	CStatic	m_OutputFileCountCap;
	CEdit	m_OutputDirEdit;
	CListBox	m_SourceListBox;
	CListBox	m_OutputListBox;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageRobotDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   	HICON m_hIcon;
    CFileDialog      *m_FileDialog;
    CropInfoDialog   *m_CropDialog;
    CColorPakDialog  *m_ColorPakDialog;
    RenameInfoDialog *m_RenameDialog;
    PackFileDialog   *m_PackFileDialog;

    CString m_SourceDirPath;

    void SetListBoxFileCounts( void );
    BOOL ValidateOutputDirectory( void );

    void PicLabCropPics();
    void RenamePics( CString nameStub );
    

    // Generated message map functions
	//{{AFX_MSG(CImageRobotDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClickFiles();
	afx_msg void OnClickCrop();
	afx_msg void OnClickClearall();
	afx_msg void OnClickRenameButton();
	afx_msg void OnClickPakfileButton();
	afx_msg void OnColorPakButton();
	afx_msg void OnButtonShadowpacker();
	afx_msg void OnButtonUnitpacker();
	afx_msg void OnClickButtonPaltoAct();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEROBOTDLG_H__59415956_1FF1_11D2_866F_444553540000__INCLUDED_)
