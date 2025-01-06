// Internet.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Resource.h"

#include "..\Common\Ascii.h"
#include "..\Common\Common.h"

#include "..\Classes\ArgumentListClass.h"
#include "..\Classes\ButtonWindowClass.h"
#include "..\Classes\DroppedFilesClass.h"
#include "..\Classes\EditWindowClass.h"
#include "..\Classes\FontClass.h"
#include "..\Classes\HtmlFileClass.h"
#include "..\Classes\InternetClass.h"
#include "..\Classes\ListBoxWindowClass.h"
#include "..\Classes\MessageClass.h"
#include "..\Classes\MenuClass.h"
#include "..\Classes\StatusBarWindowClass.h"
#include "..\Classes\WindowClass.h"
#include "..\Classes\WindowClassClass.h"

#define MAIN_WINDOW_CLASS_NAME													"Main Internet Window Class"

#define MAIN_WINDOW_CLASS_MENU_NAME												MAKEINTRESOURCE( IDR_MAIN_MENU )

#define MAIN_WINDOW_TEXT														"Internet"

#define BUTTON_WINDOW_TEXT														"Go"
#define BUTTON_WINDOW_WIDTH														30
#define BUTTON_WINDOW_HEIGHT													25
#define BUTTON_WINDOW_ID														WM_USER

#define EDIT_WINDOW_TEXT														"http://www.google.com"

#define ABOUT_MESSAGE_TEXT														"Internet\r\n"						\
																				"\r\n"								\
																				"Written by Jim Smith\r\n"			\
																				"\r\n"								\
																				"January 2025"
