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
#include "ColorPakDialog.h"
#include "Palette.hpp"
#include "ImageRobotGlobals.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorPakDialog dialog


CColorPakDialog::CColorPakDialog(CWnd* pParent /*=0*/)
	: CDialog(CColorPakDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorPakDialog)
	//}}AFX_DATA_INIT
}


void CColorPakDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorPakDialog)
	DDX_Control(pDX, IDC_ORIGINAL_BUTTON, m_originalButton);
	DDX_Control(pDX, IDC_PERCENT_RED, m_percentRed);
	DDX_Control(pDX, IDC_PERCENT_GREEN, m_percentGreen);
	DDX_Control(pDX, IDC_PERCENT_GRAY, m_percentGray);
	DDX_Control(pDX, IDC_PERCENT_BLUE, m_percentBlue);
	DDX_Control(pDX, IDC_GRADIENT, m_Gradient);
	DDX_Control(pDX, IDC_RED_BUTTON, m_redButton);
	DDX_Control(pDX, IDC_GREEN_BUTTON, m_greenButton);
	DDX_Control(pDX, IDC_GRAY_BUTTON, m_grayButton);
	DDX_Control(pDX, IDC_BLUE_BUTTON, m_blueButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorPakDialog, CDialog)
	//{{AFX_MSG_MAP(CColorPakDialog)
	ON_BN_CLICKED(IDC_GRAY_BUTTON, OnGrayButton)
	ON_BN_CLICKED(IDC_GREEN_BUTTON, OnGreenButton)
	ON_BN_CLICKED(IDC_RED_BUTTON, OnRedButton)
	ON_BN_CLICKED(IDC_BLUE_BUTTON, OnBlueButton)
	ON_BN_CLICKED(IDC_ORIGINAL_BUTTON, OnOriginalButton)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RED, OnDeltaposSpinRed)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_GREEN, OnDeltaposSpinGreen)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_GRAY, OnDeltaposSpinGray)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BLUE, OnDeltaposSpinBlue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorPakDialog message handlers

void CColorPakDialog::OnGrayButton() 
{
	m_Gradient.setGrayColorTable();
	m_Gradient.DrawMe();
	m_Gradient.Invalidate();
	//m_outputPakColorControl.SetWindowText("Gray");
}

void CColorPakDialog::OnGreenButton() 
{
	m_Gradient.setGreenColorTable();
	m_Gradient.DrawMe();
	m_Gradient.Invalidate();
}

void CColorPakDialog::OnRedButton() 
{
	m_Gradient.setRedColorTable();
	m_Gradient.DrawMe();
	m_Gradient.Invalidate();
}

bool CColorPakDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (!m_Gradient.init()) return false;
	
///////////
//HBITMAP CreateDIBSection( HDC hdc,  // handle to device context 
//CONST BITMAPINFO *pbmi,  // pointer to structure containing bitmap size, format, and color data 
//UINT iUsage,  // color data type indicator: RGB values or palette indices 
//VOID *ppvBits,  // pointer to variable to receive a pointer to the bitmap’s bit values 
//HANDLE hSection,  // optional handle to a file mapping object 
//DWORD dwOffset  // offset to the bitmap bit values within the file mapping object 
//); 
////////////
	
	return true;  // return true unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return false
}

void CColorPakDialog::OnApplyColorPak() 
{
//	// TODO: Add your control notification handler code here
}

COwnerDraw::COwnerDraw()
{
	setBltOriginal();
	setGrayColorTable();

	packedBody.load("abramsBody.pak");
	packedBody.setFPS(12);
	packedBody.setOffsetCenter();
	packedTurret.load("abramsTurret.pak");
	packedTurret.setFrame(12);
	packedTurret.setFPS(12);
	packedTurret.setOffsetCenter();

	grassSurface.loadBMP("grass.bmp");

	maxPercent       = 2.5f;
	minPercent       = 1.5f;
	incrementPercent = 0.5f;
	grayPercent      = 2.0f;
	redPercent       = 2.0f;
	greenPercent     = 2.0f;
	bluePercent      = 2.0f;

	rebuildGrayColorTable();
	rebuildRedColorTable();
	rebuildGreenColorTable();
	rebuildBlueColorTable();
}

COwnerDraw::~COwnerDraw()
{
}

bool COwnerDraw::init() {

	HDC hdc = CreateCompatibleDC(0);
	if (hdc == 0) return false;

	CRect rect;
	GetWindowRect(&rect);
	int x    = rect.Width();
	int y    = rect.Height();

	memset(&pbmi, 0, sizeof(pbmi));
	pbmi.bmiHeader.biSize          = sizeof(pbmi.bmiHeader);
	pbmi.bmiHeader.biWidth         = x;
	pbmi.bmiHeader.biHeight        = y;
	pbmi.bmiHeader.biPlanes        = 1;
	pbmi.bmiHeader.biBitCount      = 8;
	pbmi.bmiHeader.biCompression   = BI_RGB;
	pbmi.bmiHeader.biSizeImage     = 0;
	pbmi.bmiHeader.biXPelsPerMeter = 30;
	pbmi.bmiHeader.biYPelsPerMeter = 30;
	pbmi.bmiHeader.biClrUsed       = 0;
	pbmi.bmiHeader.biClrImportant  = 0;

	for (int i = 0 ; i < 256 ; i++)
	{
		pbmi.pal[i].rgbBlue = Palette::color[i].blue;
		pbmi.pal[i].rgbGreen = Palette::color[i].green;
		//pbmi.pal[i].rgbGreen = i;
		pbmi.pal[i].rgbRed = Palette::color[i].red;
		pbmi.pal[i].rgbReserved = 0;
	}

	void *dibSectionPtr = 0;
	surface.free();
	dibSection = CreateDIBSection(	hdc,
									(BITMAPINFO *)&pbmi,
									DIB_RGB_COLORS,
									&dibSectionPtr,
									0,
									0);

	DeleteDC(hdc);

	if (dibSection == 0) return false;
	surface.setTo(dibSectionPtr, iXY(x, y), x, 1);
	transSurface.copy(surface);

	return true;
}

void COwnerDraw::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) {

//typedef struct tagDRAWITEMSTRUCT {    UINT   CtlType;    UINT   CtlID;
//    UINT   itemID;    UINT   itemAction;    UINT   itemState;
//    HWND   hwndItem;    HDC    hDC;    RECT   rcItem;    DWORD  itemData;
//} DRAWITEMSTRUCT

	DrawMe();

	SetDIBitsToDevice(
		lpDrawItemStruct->hDC,
		0,
		0,
		surface.getPixX(),
		surface.getPixY(),
		0,
		0,
		0,
		surface.getPixY(),
		surface.rowPtr(0),
		(BITMAPINFO *)&pbmi,
		DIB_RGB_COLORS
	);
} 

void COwnerDraw::DrawMe()
{
	if (bltColorTable)
	{
		surface.fill(Color::white);
		packedBody.blt(surface, 50, 70);
		packedTurret.blt(surface, 50, 70);
		surface.bltLookup(surface.getRect(), colorTable->getColorArray());
	} else
	{
		surface.fill(0);
		packedBody.blt(surface, 50, 70);
		packedTurret.blt(surface, 50, 70);
	}
	
	surface.flipVertical();

	surface.blt(transSurface);
	grassSurface.blt(surface);
	transSurface.bltTrans(surface);
}

void CColorPakDialog::OnBlueButton() 
{
	m_Gradient.setBlueColorTable();
	m_Gradient.DrawMe();
	m_Gradient.Invalidate();
}

void CColorPakDialog::OnOriginalButton() 
{
	m_Gradient.setBltOriginal();
	m_Gradient.DrawMe();
	m_Gradient.Invalidate();
}

void CColorPakDialog::OnDeltaposSpinRed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if (pNMUpDown->iDelta < 0)
	{
		m_Gradient.increaseRedPercent();
	}
	else if (pNMUpDown->iDelta > 0)
	{
		m_Gradient.decreaseRedPercent();
	}

	*pResult = 0;
}

void CColorPakDialog::OnDeltaposSpinGreen(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	if (pNMUpDown->iDelta < 0)
	{
		m_Gradient.increaseGreenPercent();
	}
	else if (pNMUpDown->iDelta > 0)
	{
		m_Gradient.decreaseGreenPercent();
	}
	
	*pResult = 0;
}

void CColorPakDialog::OnDeltaposSpinGray(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	if (pNMUpDown->iDelta < 0)
	{
		m_Gradient.increaseGrayPercent();
	}
	else if (pNMUpDown->iDelta > 0)
	{
		m_Gradient.decreaseGrayPercent();
	}
	
	*pResult = 0;
}

void CColorPakDialog::OnDeltaposSpinBlue(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	if (pNMUpDown->iDelta < 0)
	{
		m_Gradient.increaseBluePercent();
	}
	else if (pNMUpDown->iDelta > 0)
	{
		m_Gradient.decreaseBluePercent();
	}
	
	*pResult = 0;
}
