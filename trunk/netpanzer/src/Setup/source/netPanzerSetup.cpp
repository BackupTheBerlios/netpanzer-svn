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
#include "stdafx.hpp"

#include "netPanzerSetup.hpp"
#include <errno.h>
#include <windows.h>
#include <dsetup.h>
#include <resource.h>
#include <process.h>

#define LoadStrFromRes(uID,lpBuffer) \
LoadString (GetModuleHandle(NULL),\
            (uID),\
            (lpBuffer),\
            sizeof((lpBuffer)));


/*
 * globals
 */
static HANDLE   hinst;
static char     GameDirectory[MAX_PATH];    // where the user wants the game
static char     SetupDirectory[MAX_PATH];     // where the user ran setup from


BOOL FAR PASCAL warnDlgProc(HWND hDlg,DWORD dwMessage,DWORD wParam,DWORD lParam);


enum { _launch_netp_install_ok,
       _launch_netp_install_directx_wrong_ver,
       _launch_netp_install_directx_install
     };


int LaunchNetPanzerInstall( HWND hWnd )
 {
  int   ver_result;
  int   mb_result;
  int   exec_result;
  BOOL  version_check_success;

  DWORD direct_x_version;
  DWORD direct_x_revision;
  
  char workBuf[512];
  char strBuf[512];
  char ishield_path[MAX_PATH];

  version_check_success = FALSE;

  ver_result = DirectXSetupGetVersion( &direct_x_version, &direct_x_revision );
  
  if ( ver_result == 0 )
   {
    LoadStrFromRes( IDS_DIRECTX_NOVER_WARN, strBuf );
    mb_result = MessageBox( hWnd, strBuf, "DirectX Version Detection", MB_YESNO | MB_ICONEXCLAMATION );     
    
    if ( mb_result == IDYES )
     { return( _launch_netp_install_directx_install ); }
    else
     { return( _launch_netp_install_directx_wrong_ver ); }
   }
  else
   if ( (HIWORD(direct_x_version) < 4 ) || (LOWORD(direct_x_version) < 5)  )
    {
     LoadStrFromRes( IDS_DIRECTX_OLDVER_WARN, workBuf );
     sprintf( strBuf, workBuf, LOWORD( direct_x_version), HIWORD( direct_x_version), LOWORD( direct_x_version) );
     mb_result = MessageBox( hWnd, strBuf, "DirectX Version Detection", MB_YESNO | MB_ICONEXCLAMATION ); 
     
     if ( mb_result == IDYES )
      { return( _launch_netp_install_directx_install ); }
     else
      { return( _launch_netp_install_directx_wrong_ver ); }

    }
   else
    { version_check_success = TRUE; }
 
  if( version_check_success == FALSE )
   return( _launch_netp_install_directx_wrong_ver );

  strcpy( ishield_path, SetupDirectory );
  strcat( ishield_path, "\\netPDist\\setup.exe" );

  exec_result = execl( ishield_path, ishield_path, NULL );
 
  if( exec_result == -1 )
   {
    if ( errno == ENOENT  )
     {
      LoadStrFromRes( IDS_ISHIELD_NOT_FOUND, strBuf );
      MessageBox( hWnd, strBuf, "Install Shield Launch Error", MB_OK | MB_ICONEXCLAMATION );          
     }
   } 
  
  return( _launch_netp_install_ok );
 }

void DoDirectSetup( HWND hWnd )
 {
  int  mb_result;
  int  result;
  char strBuf[512];
  char dx_setup_path[MAX_PATH];
  
  strcpy( dx_setup_path, SetupDirectory );
  strcat( dx_setup_path, "\\redist" );

  //result = DirectXSetup( hWnd, "d:\\sdk52\\redist", DSETUP_TESTINSTALL );
  result = DirectXSetup( hWnd, dx_setup_path, DSETUP_DXCORE );

  switch ( result )
   { 
    case DSETUPERR_SUCCESS :
     return;
    
    case DSETUPERR_USERHITCANCEL :
     return;
     
    case DSETUPERR_SUCCESS_RESTART :
     {
      LoadStrFromRes( IDS_DIRECTX_REBOOT_MESG, strBuf );
      mb_result = MessageBox( hWnd, strBuf, "DirectX 5 Setup -- Reboot", MB_YESNO | MB_ICONQUESTION );     
      
      if( mb_result == IDYES )
       { ExitWindowsEx( EWX_REBOOT, 0 ); }
      else
       { return; }
     }
    break;
          
    case DSETUPERR_NOTPREINSTALLEDONNT :
     { 
      LoadStrFromRes( IDS_DSETUP_NT_FAIL, strBuf );    
      MessageBox( hWnd, strBuf, "DirectX Setup Error", MB_OK ); 
     }
    break;

    default :
     { MessageBox( hWnd, "A setup error occured and DirectX could not be installed", "DirectX Setup Error", MB_OK ); }
    break;
   }
 
 }


BOOL FAR PASCAL warnDlgProc(HWND hDlg,DWORD dwMessage,DWORD wParam,DWORD lParam)
{
 switch(dwMessage)
  {
   case WM_INITDIALOG:
    {
     char szBuffer[1024];
     LoadStrFromRes( IDS_DIRECTX_WARN, szBuffer );
     SetWindowText( GetDlgItem( hDlg, IDC_WARN_EDIT ), szBuffer );     
    } break;

   case WM_COMMAND:
    switch(wParam)
     {
      case IDCANCEL: 
       {
        EndDialog( hDlg, 0 );
       } break;
     
      case IDOK:
       {
        ShowWindow( hDlg, SW_HIDE );
        DoDirectSetup( hDlg );
        ShowWindow( hDlg, SW_SHOW );
        EndDialog( hDlg, 0 );
       } break;
     } // ** switch WM_COMMAND
   } // ** switch (dwMessage)
 return 0;
}

BOOL FAR PASCAL mainDlgProc(HWND hDlg,DWORD dwMessage,DWORD wParam,DWORD lParam)
{
 switch(dwMessage)
  {
   case WM_COMMAND:
    switch(wParam)
     {
      case ID_EXIT_SETUP: 
       {
        EndDialog( hDlg, 0 );
       } break;
     
      /*
      case IDC_INSTALL_DIRECTX :
       {
        DialogBox( hinst, MAKEINTRESOURCE( IDD_DIRECTX_WARN ) , hDlg, (DLGPROC) warnDlgProc );
       } break;
      */
      
      case IDC_INSTALL_NETPANZER :
       {
        int result;
        result = LaunchNetPanzerInstall( hDlg );
        if( result == _launch_netp_install_directx_install )
         {
          do
           {
            DialogBox( (HINSTANCE) hinst, MAKEINTRESOURCE( IDD_DIRECTX_WARN ) , hDlg, (DLGPROC) warnDlgProc );
            result = LaunchNetPanzerInstall( hDlg );
           } while( result == _launch_netp_install_directx_install );
         }
       
       } break;

     } // ** switch WM_COMMAND
   } // ** switch (dwMessage)
 return 0;
}



int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow)
{
        TCHAR * p;
        TCHAR * x;
        hinst = hInstance;

        /*
         * get our fullpath name and strip the file name
         */
        GetModuleFileName(hInstance, SetupDirectory, sizeof(SetupDirectory));

        for (x=p=SetupDirectory; *p; p=AnsiNext(p))
        {
                if ((*p == '\\') || (*p == '/'))
                        x = p;
        }
        *x = 0;
        
        DialogBox( hInstance, MAKEINTRESOURCE( IDD_MAIN_DIALOG ) , NULL, (DLGPROC) mainDlgProc );

        return 0;
} /* WinMain */
