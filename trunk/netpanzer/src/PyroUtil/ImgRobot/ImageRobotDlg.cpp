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
#include "ImageRobotDlg.h"

#include "ImageRobotGlobals.hpp"
#include "ImageUtils.hpp"
#include <process.h>
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageRobotDlg dialog

CImageRobotDlg::CImageRobotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImageRobotDlg::IDD, pParent)
{
	
    m_FileDialog = new CFileDialog( TRUE , NULL, "", 
                                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR | OFN_ALLOWMULTISELECT,
                                    NULL, this );    

    m_FileDialog->m_ofn.lpstrFile = gFileDialogFileList;

    m_FileDialog->m_ofn.nMaxFile = gFileDialogFileListSize;
     
    //{{AFX_DATA_INIT(CImageRobotDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CImageRobotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageRobotDlg)
	DDX_Control(pDX, IDC_SRC_FILE_COUNT, m_SourceFileCountCap);
	DDX_Control(pDX, IDC_OUT_FILE_COUNT, m_OutputFileCountCap);
	DDX_Control(pDX, IDC_OUTPUTDIR_EDIT, m_OutputDirEdit);
	DDX_Control(pDX, IDC_SOURCELIST, m_SourceListBox);
	DDX_Control(pDX, IDC_OUTPUTLIST, m_OutputListBox);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CImageRobotDlg, CDialog)
	//{{AFX_MSG_MAP(CImageRobotDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_FILES, OnClickFiles)
	ON_BN_CLICKED(IDC_CROP, OnClickCrop)
	ON_BN_CLICKED(IDC_CLEARALL, OnClickClearall)
	ON_BN_CLICKED(IDC_RENAME_BUTTON, OnClickRenameButton)
	ON_BN_CLICKED(IDC_PAKFILE_BUTTON, OnClickPakfileButton)
	ON_BN_CLICKED(IDC_COLOR_PAK_BUTTON, OnColorPakButton)
	ON_BN_CLICKED(IDC_BUTTON_SHADOWPACKER, OnButtonShadowpacker)
	ON_BN_CLICKED(IDC_BUTTON_UNITPACKER, OnButtonUnitpacker)
	ON_BN_CLICKED(IDC_BUTTON_PALTOACT, OnClickButtonPaltoAct)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageRobotDlg message handlers

BOOL CImageRobotDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	// Load the game palette.
	//Palette::init("netp.act");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CImageRobotDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CImageRobotDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CImageRobotDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CImageRobotDlg::ValidateOutputDirectory( void )
 {
  CString output_path;
  char buffer[_MAX_PATH];
   
  _getcwd( buffer, _MAX_PATH );
 
   m_OutputDirEdit.GetWindowText( output_path );
  if ( _chdir( output_path ) == -1 )
   {
    int mesg_result;
    
    _chdir( buffer );
     
    mesg_result = MessageBox( "The specified output directory was not found\n"
                              "Do you wish to create it?",
                              "Output Directory Not Found",
                              MB_YESNO | MB_ICONQUESTION );

    if ( mesg_result == IDYES )
     {
      _mkdir( output_path );
      return( TRUE );  
     }   

    return( FALSE );
   }
  
  _chdir( buffer );
  return( TRUE );
 }

void CImageRobotDlg::SetListBoxFileCounts( void )
 {  
  char file_count_str[256];
  
  sprintf( file_count_str, "File Count: %d", m_SourceListBox.GetCount() );
  m_SourceFileCountCap.SetWindowText( file_count_str );
  sprintf( file_count_str, "File Count: %d", m_OutputListBox.GetCount() );
  m_OutputFileCountCap.SetWindowText( file_count_str );  
 }

void CImageRobotDlg::OnClickFiles() 
{ 
 // TODO: Add your control notification handler code here
 POSITION iterator;
 CString  string;

 char buffer[256];
 char drive[_MAX_DRIVE];
 char dir[_MAX_DIR];
 char fname[_MAX_FNAME];
 char ext[_MAX_EXT];

 memset( gFileDialogFileList, 0, gFileDialogFileListSize );

 if ( m_FileDialog->DoModal() == IDOK  )
  {
   m_SourceListBox.ResetContent();
   gSourceFileList.resetList();
   
   iterator = m_FileDialog->GetStartPosition( );	
   if (iterator == NULL) return;
      
   do 
    {
     string = m_FileDialog->GetNextPathName( iterator );
     

     _splitpath( string, NULL, NULL, fname, ext);
     strcat( fname, ext );
     m_SourceListBox.AddString( fname );
   
    } while( iterator != NULL );

   
   iterator = m_FileDialog->GetStartPosition( );	
   string = m_FileDialog->GetNextPathName( iterator );
   _splitpath( string, drive, dir, fname, ext);
   strcpy( buffer, drive );
   strcat( buffer, dir );
   m_SourceDirPath = buffer;
   strcat( buffer, "output\\" );
   m_OutputDirEdit.SetWindowText( buffer );
  
   CString path;
   CString file_name;
   int file_count = m_SourceListBox.GetCount();
   for( int i = 0; i < file_count; i++ )
    {
     m_SourceListBox.GetText( i, file_name );
     path = m_SourceDirPath + file_name;
     gSourceFileList.addString( path.GetBuffer(_FILESTRING_SIZE) );          
    }
   
   SetListBoxFileCounts();
  }

}

void CImageRobotDlg::OnClickCrop() 
{
 // TODO: Add your control notification handler code here
 m_CropDialog = new CropInfoDialog( this );	
  
 m_CropDialog->m_MinX = gCropMinX;
 m_CropDialog->m_MinY = gCropMinY;
 m_CropDialog->m_MaxX = gCropMaxX;
 m_CropDialog->m_MaxY = gCropMaxY;
 m_CropDialog->m_ImageX = gCropImageX;
 m_CropDialog->m_ImageY = gCropImageY;
 
 if ( m_CropDialog->DoModal() == IDOK )
  {
   gCropMinX = m_CropDialog->m_MinX; 
   gCropMinY = m_CropDialog->m_MinY;
   gCropMaxX = m_CropDialog->m_MaxX;
   gCropMaxY = m_CropDialog->m_MaxY;
   gCropImageX = m_CropDialog->m_ImageX;
   gCropImageY = m_CropDialog->m_ImageY;
 
   PicLabCropPics();
   
   SetListBoxFileCounts();
  }

 delete(m_CropDialog);
}

void CImageRobotDlg::PicLabCropPics()
 {
  CString output_dir;
  CString output_path;
  CString source_path;
  CString output_file;
  CString source_file;

  int file_count;
  int file_index;
  
  if ( ValidateOutputDirectory() == FALSE )
   return;

  FILE *fp;

  fp = fopen( "piclab.prg", "wt" );

  m_OutputDirEdit.GetWindowText( output_dir );
  m_OutputListBox.ResetContent();

  file_count = m_SourceListBox.GetCount();
  
  for( file_index = 0; file_index < file_count; file_index++ )
   {
    source_path = m_SourceDirPath;
    m_SourceListBox.GetText( file_index, source_file );   
    source_path = source_path + source_file;     
    
    output_path = output_dir;
    m_SourceListBox.GetText( file_index, output_file );   
    output_path = output_path + output_file;       
    
    int min_x, min_y, max_x, max_y;

    min_x = gCropMinX;
    min_y = gCropImageY - gCropMaxY;
    
    max_x = gCropMaxX;
    max_y = gCropImageY - gCropMinY;

    PicLabCrop( source_path, output_path, min_x, min_y, max_x, max_y, fp );

    m_OutputListBox.AddString( output_file );
   }
 
  fclose( fp );

  _spawnl( _P_NOWAIT, "piclab\\pl", "piclab\\pl", "piclab.prg", NULL );
 
 }


void CImageRobotDlg::RenamePics( CString nameStub )
 {
  CString output_dir;
  CString output_path;
  CString source_path;
  CString output_file;
  CString source_file;

  int file_count;
  int file_index;

 char fname[_MAX_FNAME];
 char ext[_MAX_EXT];
  

  m_OutputDirEdit.GetWindowText( output_dir );
  m_OutputListBox.ResetContent();

  file_count = m_SourceListBox.GetCount();
  
  for( file_index = 0; file_index < file_count; file_index++ )
   {
    source_path = m_SourceDirPath;
    m_SourceListBox.GetText( file_index, source_file );   
    source_path = source_path + source_file;     
    
    _splitpath( source_file, NULL, NULL, fname, ext);

    char buffer[256];
    sprintf( buffer , "%s%04d%s", nameStub, file_index, ext );

    output_path = m_SourceDirPath;
    output_path = output_path + CString( buffer );       
    
    rename( source_path, output_path );  

    m_OutputListBox.AddString( CString( buffer ) );
   }
 
 
  SetListBoxFileCounts();
 }

void CImageRobotDlg::OnClickClearall() 
{
  // TODO: Add your control notification handler code here
  m_OutputListBox.ResetContent();
  m_SourceListBox.ResetContent();
  gSourceFileList.resetList();
  m_OutputDirEdit.SetWindowText( "" );	
  SetListBoxFileCounts();
}

void CImageRobotDlg::OnClickRenameButton() 
{
 // TODO: Add your control notification handler code here
 m_RenameDialog = new RenameInfoDialog( this );
	
 if( m_RenameDialog->DoModal() == IDOK )
  {
   CString name_stub;

   name_stub = m_RenameDialog->m_NameStub;
   
   RenamePics( name_stub );
  }
 
 delete( m_RenameDialog );
}

void CImageRobotDlg::OnClickPakfileButton() 
{
 // TODO: Add your control notification handler code here
 m_PackFileDialog = new PackFileDialog( this );

 m_PackFileDialog->m_PackFileName = "";

 if ( m_PackFileDialog->DoModal() == IDOK )
  {
   CString output_path;
   
   if ( ValidateOutputDirectory() == FALSE )
    return;
  
   m_OutputDirEdit.GetWindowText( output_path );
   output_path = output_path + m_PackFileDialog->m_PackFileName + ".pak"; 
   
   if ( PakFiles(gSourceFileList, output_path ) == _pak_files_ok ) 
    {
     m_OutputListBox.AddString( (m_PackFileDialog->m_PackFileName + ".pak") );    
    }
  
   SetListBoxFileCounts();
  } 

 delete( m_PackFileDialog );
}

void CImageRobotDlg::OnColorPakButton() 
{
	// TODO: Add your control notification handler code here
	m_ColorPakDialog = new CColorPakDialog(this);

	if (m_ColorPakDialog->DoModal() == IDOK)
	{
		if (!CImageRobotDlg::ValidateOutputDirectory()) { return; }
		
		CString outputDirectory;

		m_OutputDirEdit.GetWindowText(outputDirectory);
		m_OutputListBox.ResetContent();

		int fileCount = m_SourceListBox.GetCount();
  
		for(int i = 0; i < fileCount; i++)
		{
			CString sourceFile;
			CString outputFile;

			m_SourceListBox.GetText(i, sourceFile);   
			m_SourceListBox.GetText(i, outputFile);

			CString sourcePath = m_SourceDirPath + sourceFile;     
			CString outputPath = outputDirectory + outputFile;       

			//void _splitpath( const char *path, char *drive, char *dir, char *fname, char *ext );
			//char extension[256];
			//_splitpath(outputPath, NULL, NULL, NULL, extension)
			//m_OutputListBox.AddString(outputFile);

			PackedSurface tempPack;
			tempPack.load(sourcePath);

			Surface tempSurface;
			tempSurface.create(tempPack.getPixX(), tempPack.getPixY(), tempPack.getPixX(), tempPack.getFrameCount());

			for (int j = 0; j < tempSurface.getFrameCount(); j++)
			{
				tempSurface.setFrame(j);
				tempPack.setFrame(j);

				tempSurface.fill(Color::white);
				tempPack.blt(tempSurface, 0, 0);

				tempSurface.bltLookup(iRect(tempSurface.getRect().min, tempSurface.getRect().max + 1), colorTable->getColorArray());
			}

			tempPack.pack(tempSurface);
			tempPack.save(outputPath);
			m_OutputListBox.AddString(outputFile);
		
			//surface.blt(transSurface);
			//grassSurface.blt(surface);
			//transSurface.bltTrans(surface);
		}

		SetListBoxFileCounts();
	}

	delete m_ColorPakDialog;
	
}

void CImageRobotDlg::OnButtonShadowpacker() 
{
	// TODO: Add your control notification handler code here
	m_PackFileDialog = new PackFileDialog( this );

	m_PackFileDialog->m_PackFileName = "";

	if (m_PackFileDialog->DoModal() == IDOK)
	{
		CString output_path;

		if (ValidateOutputDirectory() == FALSE)
		{
			return;
		}

		m_OutputDirEdit.GetWindowText( output_path );
		
		output_path = output_path + m_PackFileDialog->m_PackFileName + ".pak";

		if (ShadowPakFiles(gSourceFileList, output_path ) == _pak_files_ok)
		{
			m_OutputListBox.AddString((m_PackFileDialog->m_PackFileName + ".pak"));
		}

		SetListBoxFileCounts();
	} 

	delete( m_PackFileDialog );
}

void CImageRobotDlg::OnButtonUnitpacker() 
{
	m_PackFileDialog = new PackFileDialog( this );

	m_PackFileDialog->m_PackFileName = "";

	if (m_PackFileDialog->DoModal() == IDOK)
	{
		CString output_path;

		if (ValidateOutputDirectory() == FALSE)
		{
			return;
		}

		m_OutputDirEdit.GetWindowText( output_path );
		
		output_path = output_path + m_PackFileDialog->m_PackFileName + ".pak";

		if (UnitPakFiles(gSourceFileList, output_path ) == _pak_files_ok)
		{
			m_OutputListBox.AddString((m_PackFileDialog->m_PackFileName + ".pak"));
		}

		SetListBoxFileCounts();
	} 

	delete( m_PackFileDialog );
}

void CImageRobotDlg::OnClickButtonPaltoAct() 
{
 CString output_dir;
 m_OutputDirEdit.GetWindowText( output_dir );

 NewJascPalette( output_dir );	
}
