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


#include <config.h>
#include "ImageRobot.h"
#include "RenameInfoDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RenameInfoDialog dialog


RenameInfoDialog::RenameInfoDialog(CWnd* pParent /*=0*/)
	: CDialog(RenameInfoDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(RenameInfoDialog)
	m_NameStub = _T("");
	//}}AFX_DATA_INIT
}


void RenameInfoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RenameInfoDialog)
	DDX_Text(pDX, IDC_EDIT1, m_NameStub);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RenameInfoDialog, CDialog)
	//{{AFX_MSG_MAP(RenameInfoDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RenameInfoDialog message handlers
