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


#include "ImageRobot.h"
#include "CropInfoDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CropInfoDialog dialog


CropInfoDialog::CropInfoDialog(CWnd* pParent /*=0*/)
	: CDialog(CropInfoDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CropInfoDialog)
	m_ImageX = 0;
	m_ImageY = 0;
	m_MaxX = 0;
	m_MaxY = 0;
	m_MinX = 0;
	m_MinY = 0;
	//}}AFX_DATA_INIT
}


void CropInfoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CropInfoDialog)
	DDX_Text(pDX, IDC_IMAGEXEDIT, m_ImageX);
	DDX_Text(pDX, IDC_IMAGEYEDIT, m_ImageY);
	DDX_Text(pDX, IDC_MAXXEDIT, m_MaxX);
	DDX_Text(pDX, IDC_MAXYEDIT, m_MaxY);
	DDX_Text(pDX, IDC_MINXEDIT, m_MinX);
	DDX_Text(pDX, IDC_MINYEDIT, m_MinY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CropInfoDialog, CDialog)
	//{{AFX_MSG_MAP(CropInfoDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CropInfoDialog message handlers
