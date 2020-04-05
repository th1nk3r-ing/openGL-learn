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
#include "common.h"
#include "build_time.h"
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

/**
 * @function:   win_SetupConsole
 * @brief:      配置 cmd 控制台
 * @param[in]:  void
 * @param[out]: None
 * @return:     void
 */
void win_SetupConsole(void)
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

	return;
}

/**
 * @function:   win_RestoreConsole
 * @brief:      复位 cmd 控制台
 * @param[in]:  void
 * @param[out]: None
 * @return:     void
 */
void win_RestoreConsole(void)
{
	// Reset console mode
	if(!SetConsoleMode(stdoutHandle, outModeInit)) {
		exit(GetLastError());
	}

	return;
}



/**
 * @function:   __win_windowProcCallback
 * @brief:      Main window procedure
 * @param[in]:  HWND hWnd
 * @param[in]:  UINT uMsg
 * @param[in]:  WPARAM wParam
 * @param[in]:  LPARAM lParam
 * @param[out]: None
 * @return:     LRESULT WINAPI
 */
static LRESULT CALLBACK __win_windowProcCallback
                            (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT  lRet = 1;
	EGL_Context *pstEglContext = (EGL_Context *)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);

	switch (uMsg)
	{
		case WM_CREATE:
		{
			Cprintf_yellow("[%s %d]  WM_CREATE\n", __func__, __LINE__);
			break;
		}
		case WM_DESTROY:
		{
			Cprintf_yellow("[%s %d]  WM_DESTROY\n", __func__, __LINE__);
			PostQuitMessage (0);
			break;
		}
		case WM_PAINT:
		{
			// printf("[%s %d]  WM_PAINT\n", __func__, __LINE__);
			if ( pstEglContext && pstEglContext->drawFunc )
			{
				pstEglContext->drawFunc(pstEglContext);
			}
			if(pstEglContext)
			{
				ValidateRect( pstEglContext->eglNativeWindow, NULL );
			}
			break;
		}
		case WM_CHAR:
		{
			// printf("[%s %d]  WM_CHAR\n", __func__, __LINE__);
			POINT      point;
			GetCursorPos ( &point );
			if ( pstEglContext && pstEglContext->keyFunc )
			{
				pstEglContext->keyFunc( pstEglContext,
						(unsigned char)wParam,
						(int)point.x,
						(int)point.y );
			}
			break;
		}
		case WM_SIZE:
		{
		    if((SIZE_RESTORED == wParam)
		        && (pstEglContext)
		        && (NULL != pstEglContext->resizeWinFunc))
			{
				RECT windowRect = {0};
				if(GetWindowRect(hWnd, &windowRect))
				{
                    pstEglContext->resizeWinFunc(pstEglContext,
                                                 windowRect.right,
                                                 windowRect.bottom);
				}
				else
				{
                    Cprintf_red("[%s %d]  error:[%ld]!\n", __func__, __LINE__, GetLastError());
				}
			}
			break;
		}
		default:
			lRet = DefWindowProc( hWnd, uMsg, wParam, lParam );
			break;
	}

	return lRet;
}

/**
 * @function:   win_CreateWindow
 * @brief:      Create Win32 instance and window
 * @param[in]:  EGL_Context *pstEglContext
 * @param[in]:  const char *title
 * @param[out]: None
 * @return:     GLboolean
 */
GLboolean win_CreateWindow(EGL_Context *pstEglContext, const char *title)
{
	WNDCLASS wndclass = {0};
	DWORD    wStyle   = 0;
	RECT     windowRect;
	HINSTANCE hInstance = GetModuleHandle ( NULL );


	wndclass.style         = CS_OWNDC;
	wndclass.lpfnWndProc   = ( WNDPROC ) __win_windowProcCallback;
	wndclass.hInstance     = hInstance;
	wndclass.hbrBackground = ( HBRUSH ) GetStockObject ( BLACK_BRUSH );
	wndclass.lpszClassName = "OpenGL-ES2.0";

	if ( !RegisterClass ( &wndclass ) )
	{
		return FALSE;
	}

	wStyle = WS_VISIBLE | WS_SIZEBOX | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;

	// Adjust the window rectangle so that the client area has
	// the correct number of pixels
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = pstEglContext->s32SurfaceW;
	windowRect.bottom = pstEglContext->s32SurfaceH;

	AdjustWindowRect ( &windowRect, wStyle, FALSE );

	pstEglContext->eglNativeWindow = CreateWindow (
                            			"OpenGL-ES2.0",
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
	// __win_windowProcCallback
#ifdef _WIN64
	//In LLP64 LONG is stll 32bit.
	SetWindowLongPtr( pstEglContext->eglNativeWindow, GWL_USERDATA, ( LONGLONG ) ( LONG_PTR )pstEglContext);
#else
	SetWindowLongPtr ( pstEglContext->eglNativeWindow, GWL_USERDATA, ( LONG ) ( LONG_PTR ) pstEglContext );
#endif


	if ( pstEglContext->eglNativeWindow == NULL )
	{
		return GL_FALSE;
	}

	ShowWindow(pstEglContext->eglNativeWindow, TRUE );

	return GL_TRUE;
}


/**
 * @function:   win_WinLoop
 * @brief:      Start main windows loop
 * @param[in]:  EGL_Context *pstEglContext
 * @param[out]: None
 * @return:     void
 */
void win_WinLoop(EGL_Context *pstEglContext)
{
	MSG msg = { 0 };
	int done = 0;
	DWORD lastTime = GetTickCount();

	while( !done )
	{
		int gotMsg = (PeekMessage ( &msg, NULL, 0, 0, PM_REMOVE ) != 0);
		DWORD curTime = GetTickCount();
		float deltaTime = ( float ) ( curTime - lastTime ) / 1000.0f;
		lastTime = curTime;

		if( gotMsg )
		{
			if ( msg.message == WM_QUIT )
			{
				done = 1;
			}
			else
			{
				TranslateMessage ( &msg );
				DispatchMessage ( &msg );
			}
		}
		else
		{
			SendMessage(pstEglContext->eglNativeWindow, WM_PAINT, 0, 0 );
		}

		// Call update function if registered
		if ( pstEglContext->updateFunc != NULL )
		{
			pstEglContext->updateFunc( pstEglContext, deltaTime );
		}

	    sleep_ms(39);
	}


}

