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
#if !defined(AFX_COLORPAKDIALOG_H__8E8AC281_2267_11D2_89B5_0040052EEC37__INCLUDED_)
#define AFX_COLORPAKDIALOG_H__8E8AC281_2267_11D2_89B5_0040052EEC37__INCLUDED_

#include "Surface.hpp"
#include "PackedSurface.hpp"
#include "ImageRobotGlobals.hpp"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ColorPakDialog.h : header file
//

class COwnerDraw : public CButton {
public:
	COwnerDraw();
	virtual ~COwnerDraw();

	BOOL init();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct); // override CButton
	virtual void DrawMe();

	void setBltOriginal()
	{
		bltColorTable = false;
	}

	void setGrayColorTable()
	{
		bltColorTable = true;
		colorTable = &gray256;
	}

	void setRedColorTable()
	{
		bltColorTable = true;
		colorTable = &red256;
	}

	void setGreenColorTable()
	{
		bltColorTable = true;
		colorTable = &green256;
	}

	void setBlueColorTable()
	{
		bltColorTable = true;
		colorTable = &blue256;
	}

	void increaseGrayPercent() { increasePercent(grayPercent); rebuildGrayColorTable(); }
	void decreaseGrayPercent() { decreasePercent(grayPercent); rebuildGrayColorTable(); }
	void increaseRedPercent() { increasePercent(redPercent); rebuildRedColorTable(); }
	void decreaseRedPercent() { decreasePercent(redPercent); rebuildRedColorTable(); }
	void increaseGreenPercent() { increasePercent(greenPercent); rebuildGreenColorTable(); }
	void decreaseGreenPercent() { decreasePercent(greenPercent); rebuildGreenColorTable(); }
	void increaseBluePercent() { increasePercent(bluePercent); rebuildBlueColorTable(); }
	void decreaseBluePercent() { decreasePercent(bluePercent); rebuildBlueColorTable(); }

	void increasePercent(float &color)
	{
		if ((color += incrementPercent) > maxPercent)
		{
			color = maxPercent;
		}
		DrawMe();
		Invalidate();
	}

	void decreasePercent(float &color)
	{
		if ((color -= incrementPercent) < minPercent)
		{
			color = minPercent;
		}
		DrawMe();
		Invalidate();
	}

	void rebuildGrayColorTable()
	{
		// 256 shades of gray.
		gray256.init(256);
		for (int num = 0; num < 256; num++)
		{
			int c            = Palette::color[num].getBrightnessInt();
			int nearestColor = Palette::findNearestColor(RGBColor(c * grayPercent, c * grayPercent, c * grayPercent));
			gray256.setColor(num, nearestColor);
		}
		gray256.setColor(255, 0);
	}

	void rebuildRedColorTable()
	{
		// 256 shades of red.
		red256.init(256);
		for (int num = 0; num < 256; num++)
		{
			int c            = Palette::color[num].getBrightnessInt();
			int nearestColor = Palette::findNearestColor(RGBColor(c * redPercent, 0, 0));
			red256.setColor(num, nearestColor);
		}
		red256.setColor(255, 0);
	}

	void rebuildGreenColorTable()
	{
		// 256 shades of green.
		green256.init(256);
		for (int num = 0; num < 256; num++)
		{
			int c            = Palette::color[num].getBrightnessInt();
			int nearestColor = Palette::findNearestColor(RGBColor(0, c * greenPercent, 0));
			green256.setColor(num, nearestColor);
		}
		green256.setColor(255, 0);
	}

	void rebuildBlueColorTable()
	{
		// 256 shades of blue.
		blue256.init(256);
		for (int num = 0; num < 256; num++)
		{
			int c            = Palette::color[num].getBrightnessInt();
			int nearestColor = Palette::findNearestColor(RGBColor(0, 0, c * bluePercent));
			blue256.setColor(num, nearestColor);
		}
		blue256.setColor(255, 0);
	}

	const ColorTable *getColorTable() { return colorTable; }

private:
	bool        bltColorTable;

	struct {
		BITMAPINFOHEADER bmiHeader;
		RGBQUAD pal[256]; // room for the rest of the color table
	} pbmi;

	HBITMAP       dibSection;
	Surface       surface;
	Surface       transSurface;
	Surface       grassSurface;
	PackedSurface packedBody;
	PackedSurface packedTurret;

	float maxPercent;
	float minPercent;
	float incrementPercent;
	float grayPercent;
	float redPercent;
	float greenPercent;
	float bluePercent;

};

/////////////////////////////////////////////////////////////////////////////
// CColorPakDialog dialog

class CColorPakDialog : public CDialog
{
// Construction
public:
	CColorPakDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CColorPakDialog)
	enum { IDD = IDD_COLOR_PAK_DIALOG };
	CButton	m_originalButton;
	CStatic	m_percentRed;
	CStatic	m_percentGreen;
	CStatic	m_percentGray;
	CStatic	m_percentBlue;
	COwnerDraw	m_Gradient;
	CButton	m_redButton;
	CButton	m_lightGrayButton;
	CButton	m_greenButton;
	CButton	m_grayButton;
	CButton	m_darkGrayButton;
	CButton	m_blueButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorPakDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CColorPakDialog)
	afx_msg void OnLightGrayButton();
	afx_msg void OnDarkGrayButton();
	afx_msg void OnGrayButton();
	afx_msg void OnGreenButton();
	afx_msg void OnRedButton();
	afx_msg void OnYellowButton();
	virtual BOOL OnInitDialog();
	afx_msg void OnApplyColorPak();
	afx_msg void OnBlueButton();
	afx_msg void OnOriginalButton();
	afx_msg void OnDeltaposSpinRed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinOriginal(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinGreen(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinGray(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinBlue(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORPAKDIALOG_H__8E8AC281_2267_11D2_89B5_0040052EEC37__INCLUDED_)
