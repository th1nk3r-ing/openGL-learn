/**
 * @file:   win32.c
 * @note:   2010-2020, <git.oschia.net/think3r>
 * @brief:  windows 平台支持
 * @author:  
 * @date:   2020/3/29
 * @note:
 * @note \n History:
   1.日    期: 2020/3/29
     作    者:  
     修改历史: 创建文件
 */

/*----------------------------------------------*/
/*                  头文件包含                  */
/*----------------------------------------------*/
#include <windows.h>
#include <stdlib.h>
#include "OGL_common.h"

/*----------------------------------------------*/
/*                 宏类型定义                   */
/*----------------------------------------------*/
#ifdef _WIN64
#define GWL_USERDATA GWLP_USERDATA
#endif

// Some old MinGW/CYGWIN distributions don't define this:
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING  0x0004
#endif

/*----------------------------------------------*/
/*                结构体定义                    */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 函数声明                     */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 全局变量                     */
/*----------------------------------------------*/
static HANDLE stdoutHandle;
static DWORD outModeInit;

/*----------------------------------------------*/
/*                 函数定义                     */
/*----------------------------------------------*/

void win_setupConsole(void) 
{
	DWORD outMode = 0;
	stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	if(stdoutHandle == INVALID_HANDLE_VALUE) 
	{
		exit(GetLastError());
	}
	
	if(!GetConsoleMode(stdoutHandle, &outMode)) 
	{
		exit(GetLastError());
	}

	outModeInit = outMode;
	
    // Enable ANSI escape codes
	outMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

	if(!SetConsoleMode(stdoutHandle, outMode)) 
	{
		exit(GetLastError());
	}	
}


void win_restoreConsole(void) 
{	
    // Reset console mode
	if(!SetConsoleMode(stdoutHandle, outModeInit)) {
		exit(GetLastError());
	}
}


/**
 * @function:   ESWindowProc
 * @brief:       Main window procedure
 * @param[in]:  HWND hWnd      
 * @param[in]:  UINT uMsg      
 * @param[in]:  WPARAM wParam  
 * @param[in]:  LPARAM lParam  
 * @param[out]: None
 * @return:     LRESULT WINAPI
 */
LRESULT WINAPI ESWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
   LRESULT  lRet = 1;

   switch ( uMsg )
   {
      case WM_CREATE:
         break;
      case WM_PAINT:
      {
         EGL_Context *esContext = ( EGL_Context * ) ( LONG_PTR ) GetWindowLongPtr ( hWnd, GWL_USERDATA );

         if ( esContext && esContext->drawFunc )
         {
            esContext->drawFunc ( esContext );
            eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
         }

         if ( esContext )
         {
            ValidateRect ( esContext->eglNativeWindow, NULL );
         }
      }
      	break;
      case WM_DESTROY:
         PostQuitMessage ( 0 );
         break;

      case WM_CHAR:
      {
         POINT      point;
         EGL_Context *esContext = ( EGL_Context * ) ( LONG_PTR ) GetWindowLongPtr ( hWnd, GWL_USERDATA );

         GetCursorPos ( &point );

         if ( esContext && esContext->keyFunc )
            esContext->keyFunc ( esContext, ( unsigned char ) wParam,
                                 ( int ) point.x, ( int ) point.y );
      }
      break;
      default:
         lRet = DefWindowProc( hWnd, uMsg, wParam, lParam );
         break;
   }

   return lRet;
}


//////////////////////////////////////////////////////////////////
//
//  Public Functions
//
//

///
//  WinCreate()
//
//      Create Win32 instance and window
//
GLboolean WinCreate(EGL_Context *esContext, const char *title)
{
   WNDCLASS wndclass = {0};
   DWORD    wStyle   = 0;
   RECT     windowRect;
   HINSTANCE hInstance = GetModuleHandle ( NULL );


   wndclass.style         = CS_OWNDC;
   wndclass.lpfnWndProc   = ( WNDPROC ) ESWindowProc;
   wndclass.hInstance     = hInstance;
   wndclass.hbrBackground = ( HBRUSH ) GetStockObject ( BLACK_BRUSH );
   wndclass.lpszClassName = "openglEs2.0";

   if ( !RegisterClass ( &wndclass ) )
   {
      return FALSE;
   }

   wStyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;

   // Adjust the window rectangle so that the client area has
   // the correct number of pixels
   windowRect.left = 0;
   windowRect.top = 0;
   windowRect.right = esContext->s32SurfaceW;
   windowRect.bottom = esContext->s32SurfaceH;

   AdjustWindowRect ( &windowRect, wStyle, FALSE );

   esContext->eglNativeWindow = CreateWindow (
                                   "openglEs2.0",
                                   title,
                                   wStyle,
                                   0,
                                   0,
                                   windowRect.right - windowRect.left,
                                   windowRect.bottom - windowRect.top,
                                   NULL,
                                   NULL,
                                   hInstance,
                                   NULL );

   // Set the EGL_Context* to the GWL_USERDATA so that it is available to the
   // ESWindowProc
#ifdef _WIN64
   //In LLP64 LONG is stll 32bit.
   SetWindowLongPtr( esContext->eglNativeWindow, GWL_USERDATA, ( LONGLONG ) ( LONG_PTR )esContext);
#else
   SetWindowLongPtr ( esContext->eglNativeWindow, GWL_USERDATA, ( LONG ) ( LONG_PTR ) esContext );
#endif


   if ( esContext->eglNativeWindow == NULL )
   {
      return GL_FALSE;
   }

   ShowWindow ( esContext->eglNativeWindow, TRUE );

   return GL_TRUE;
}


