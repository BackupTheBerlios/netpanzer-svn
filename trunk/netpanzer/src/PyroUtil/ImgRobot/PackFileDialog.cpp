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

#include "stdafx.h"
#include "ImageRobot.h"
#include "PackFileDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PackFileDialog dialog


PackFileDialog::PackFileDialog(CWnd* pParent /*=NULL*/)
	: CDialog(PackFileDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(PackFileDialog)
	m_PackFileName = _T("");
	//}}AFX_DATA_INIT
}


void PackFileDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PackFileDialog)
	DDX_Text(pDX, IDC_PAKFILE_EDIT, m_PackFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PackFileDialog, CDialog)
	//{{AFX_MSG_MAP(PackFileDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PackFileDialog message handlers
