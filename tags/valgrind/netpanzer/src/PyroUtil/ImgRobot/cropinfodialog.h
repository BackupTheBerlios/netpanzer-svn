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
#if !defined(AFX_ROPINFODIALOG_H__5941595E_1FF1_11D2_866F_444553540000__INCLUDED_)
#define AFX_ROPINFODIALOG_H__5941595E_1FF1_11D2_866F_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ropInfoDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CropInfoDialog dialog

class CropInfoDialog : public CDialog
{
// Construction
public:
	CropInfoDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CropInfoDialog)
	enum { IDD = IDD_CROPINFO_DIALOG };
	UINT	m_ImageX;
	UINT	m_ImageY;
	UINT	m_MaxX;
	UINT	m_MaxY;
	UINT	m_MinX;
	UINT	m_MinY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CropInfoDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CropInfoDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROPINFODIALOG_H__5941595E_1FF1_11D2_866F_444553540000__INCLUDED_)
