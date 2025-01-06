// Internet.cpp

#include "Internet.h"

// Global variables
Internet g_internet;
EditWindow g_editWindow;
ButtonWindow g_buttonWindow;
ListBoxWindow g_listBoxWindow;
StatusBarWindow g_statusBarWindow;

void TagFunction( LPCTSTR lpszParentUrl, LPCTSTR lpszTag )
{
	HtmlFile htmlFile;

	// Allocate string memory
	LPTSTR lpszTagName = new char[ STRING_LENGTH ];

	// Get tag name
	if( htmlFile.GetTagName( lpszTag, lpszTagName ) )
	{
		// Successfully got tag name

		// See if this is an anchor tag
		if( lstrcmpi( lpszTagName, HTML_FILE_CLASS_ANCHOR_TAG_NAME ) == 0 )
		{
			// This is an anchor tag

			// Allocate string memory
			LPTSTR lpszAttributeValue = new char[ STRING_LENGTH ];

			// Add tag to list box window
			g_listBoxWindow.AddText( lpszTag );

			// Get attribute value
			if( htmlFile.GetAttributeValue( lpszTag, lpszParentUrl, HTML_FILE_CLASS_ANCHOR_TAG_ATTRIBUTE, lpszAttributeValue ) )
			{
				// Successfully got attribute value

				// Add attribute value to list box window
				g_listBoxWindow.AddText( lpszAttributeValue );

			} // End of successfully got attribute value

			// Free string memory
			delete [] lpszAttributeValue;

		} // End of this is an anchor tag

	} // End of successfully got tag name

	// Free string memory
	delete [] lpszTagName;

} // End of function TagFunction

BOOL DownloadFile( LPCTSTR lpszUrl, LPTSTR lpszLocalFilePath )
{
	BOOL bResult = FALSE;

	// Allocate string memory
	LPTSTR lpszStatusMessage = new char[ STRING_LENGTH + sizeof( char ) ];

	// Format status message
	wsprintf( lpszStatusMessage, INTERNET_CLASS_DOWNLOADING_STATUS_MESSAGE_FORMAT_STRING, lpszUrl );

	// Show status message on status bar window
	g_statusBarWindow.SetText( lpszStatusMessage );

	// Download url to local file
	if( g_internet.DownloadFile( lpszUrl, lpszLocalFilePath ) )
	{
		// Successfully downloaded url to local file

		// Format status message
		wsprintf( lpszStatusMessage, INTERNET_CLASS_SUCCESSFULLY_DOWNLOADED_STATUS_MESSAGE_FORMAT_STRING, lpszUrl, lpszLocalFilePath );

		// Update return value
		bResult = TRUE;

	} // End of successfully downloaded url to local file
	else
	{
		// Unable to download url to local file

		// Format status message
		wsprintf( lpszStatusMessage, INTERNET_CLASS_UNABLE_TO_DOWNLOAD_STATUS_MESSAGE_FORMAT_STRING, lpszUrl );

	} // End of unable to download url to local file

	// Show status message on status bar window
	g_statusBarWindow.SetText( lpszStatusMessage );

	// Free string memory
	delete [] lpszStatusMessage;

	return bResult;

} // End of function DownloadFile

void EditWindowUpdateFunction( int nTextLength )
{
	// See if edit window contains text
	if( nTextLength > 0 )
	{
		// Edit window contains text

		// Enable button window
		g_buttonWindow.Enable( TRUE );

	} // Edit window contains text
	else
	{
		// Edit window is empty

		// Enable button window
		g_buttonWindow.Enable( FALSE );

	} // Edit window is empty

} // End of function EditWindowUpdateFunction

void ListBoxWindowSelectionChangedFunction( LPTSTR lpszItemText )
{
	// Show item text on status bat window
	g_statusBarWindow.SetText( lpszItemText );

} // End of function ListBoxWindowSelectionChangedFunction

void ListBoxWindowDoubleClickFunction( LPTSTR lpszItemText )
{
	// Display item text
	MessageBox( NULL, lpszItemText, INFORMATION_MESSAGE_CAPTION, ( MB_OK | MB_ICONINFORMATION ) );

} // End of function ListBoxWindowDoubleClickFunction

void OpenFileFunction( LPCTSTR lpszFilePath )
{
	// Add file to list box window
	g_listBoxWindow.AddText( lpszFilePath );

} // End of function OpenFileFunction

int ShowAboutMessage( HWND hWndParent )
{
	int nResult = 0;

	MSGBOXPARAMS mbp;

	// Clear message box parameter structure
	ZeroMemory( &mbp, sizeof( mbp ) );

	// Initialise message box parameter structure
	mbp.cbSize		= sizeof( MSGBOXPARAMS );
	mbp.hwndOwner	= hWndParent;
	mbp.hInstance	= NULL;
	mbp.lpszText	= ABOUT_MESSAGE_TEXT;
	mbp.lpszCaption	= ABOUT_MESSAGE_CAPTION;
	mbp.dwStyle		= ( MB_OK | MB_USERICON );
	mbp.lpszIcon	= WINDOW_CLASS_CLASS_DEFAULT_ICON_NAME;

	// Show message box
	nResult = MessageBoxIndirect( &mbp );

	return nResult;

} // End of function ShowAboutMessage

LRESULT CALLBACK MainWindowProcedure( HWND hWndMain, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	LRESULT lr = 0;

	// Select message
	switch( uMessage )
	{
		case WM_CREATE:
		{
			// A create message
			HINSTANCE hInstance;

			// Get instance
			hInstance = ( ( LPCREATESTRUCT )lParam )->hInstance;

			// Create edit window
			if( g_editWindow.Create( hWndMain, hInstance, EDIT_WINDOW_TEXT ) )
			{
				// Successfully created edit window
				Font font;

				// Get font
				font = DEFAULT_GUI_FONT;

				// Set edit window font
				g_editWindow.SetFont( font );

				// Create button window
				if( g_buttonWindow.Create( hWndMain, hInstance, BUTTON_WINDOW_TEXT, BUTTON_WINDOW_ID ) )
				{
					// Successfully created button window

					// Set button window font
					g_buttonWindow.SetFont( font );

					// Create list box window
					if( g_listBoxWindow.Create( hWndMain, hInstance ) )
					{
						// Successfully created list box window

						// Set list box window font
						g_listBoxWindow.SetFont( font );

						// Create status bar window
						if( g_statusBarWindow.Create( hWndMain, hInstance ) )
						{
							// Successfully created status bar window

							// Set status bar window font
							g_statusBarWindow.SetFont( font );

							// Select edit window text
							g_editWindow.SelectText();

						} // End of successfully created status bar window

					} // End of successfully created list box window

				} // End of successfully created button window

			} // End of successfully created edit window

			// Break out of switch
			break;

		} // End of a create message
		case WM_SIZE:
		{
			// A size message
			int nClientWidth;
			int nClientHeight;
			RECT rcStatus;
			int nEditWindowWidth;
			int nButtonWindowLeft;
			int nStatusWindowHeight;
			int nListBoxWindowHeight;
			int nListBoxWindowTop;

			// Store client width and height
			nClientWidth	= ( int )LOWORD( lParam );
			nClientHeight	= ( int )HIWORD( lParam );

			// Size status bar window
			g_statusBarWindow.Size();

			// Get status window size
			g_statusBarWindow.GetWindowRect( &rcStatus );

			// Calculate window sizes
			nStatusWindowHeight		= ( rcStatus.bottom - rcStatus.top );
			nListBoxWindowHeight	= ( nClientHeight - ( nStatusWindowHeight + BUTTON_WINDOW_HEIGHT ) + WINDOW_BORDER_HEIGHT );
			nEditWindowWidth		= ( ( nClientWidth - BUTTON_WINDOW_WIDTH ) + WINDOW_BORDER_WIDTH );

			// Calculate window positions
			nListBoxWindowTop		= ( BUTTON_WINDOW_HEIGHT - WINDOW_BORDER_HEIGHT );
			nButtonWindowLeft		= ( nEditWindowWidth - WINDOW_BORDER_WIDTH );

			// Move control windows
			g_editWindow.Move( 0, 0, nEditWindowWidth, BUTTON_WINDOW_HEIGHT, TRUE );
			g_buttonWindow.Move( nButtonWindowLeft, 0, BUTTON_WINDOW_WIDTH, BUTTON_WINDOW_HEIGHT, TRUE );
			g_listBoxWindow.Move( 0, nListBoxWindowTop, nClientWidth, nListBoxWindowHeight, TRUE );

			// Break out of switch
			break;

		} // End of a size message
		case WM_ACTIVATE:
		{
			// An activate message

			// Focus on edit window
			g_editWindow.SetFocus();

			// Break out of switch
			break;

		} // End of an activate message
		case WM_GETMINMAXINFO:
		{
			// A get min max info message
			MINMAXINFO FAR *lpMinMaxInfo;

			// Get min max info structure
			lpMinMaxInfo = ( MINMAXINFO FAR * )lParam;

			// Update min max info structure
			lpMinMaxInfo->ptMinTrackSize.x = WINDOW_CLASS_DEFAULT_MINIMUM_WIDTH;
			lpMinMaxInfo->ptMinTrackSize.y = WINDOW_CLASS_DEFAULT_MINIMUM_HEIGHT;

			// Break out of switch
			break;

		} // End of a get min max info message
		case WM_DROPFILES:
		{
			// A drop files message
			DroppedFiles droppedFiles;

			// Get dropped files
			if( droppedFiles.Get( wParam ) )
			{
				// Successfully got dropped files

				// Process dropped files
				droppedFiles.Process( &OpenFileFunction );

			} // End of successfully got dropped files

			// Break out of switch
			break;

		} // End of a drop files message
		case WM_COMMAND:
		{
			// A command message

			// Select command
			switch( LOWORD( wParam ) )
			{
				case BUTTON_WINDOW_ID:
				{
					// A button window command

					// Allocate string memory
					LPTSTR lpszUrl = new char[ STRING_LENGTH + sizeof( char ) ];

					// Get url from edit window
					if( g_editWindow.GetText( lpszUrl ) )
					{
						// Successfully got url from edit window

						// Allocate string memory
						LPTSTR lpszLocalFilePath = new char[ STRING_LENGTH + sizeof( char ) ];

						// Download url to local file
						if( DownloadFile( lpszUrl, lpszLocalFilePath ) )
						{
							// Successfully downloaded url to local file
							HtmlFile localFile;

							// Open local file for reading
							if( localFile.CreateRead( lpszLocalFilePath ) )
							{
								// Successfully opened local file for reading

								// Read local file
								if( localFile.Read() )
								{
									// Successfully read local file
									int nTagCount;

									// Allocate string memory
									LPTSTR lpszStatusMessage = new char[ STRING_LENGTH ];

									// Process tags in local file
									nTagCount = localFile.ProcessTags( lpszUrl, &TagFunction );

									// Format status message
									wsprintf( lpszStatusMessage, HTML_FILE_CLASS_PROCESS_TAGS_STATUS_MESSAGE_FORMAT_STRING, lpszUrl, nTagCount );

									// Show status message on status bar window
									g_statusBarWindow.SetText( lpszStatusMessage );

									// Free string memory
									delete [] lpszStatusMessage;

								} // End of successfully read local file
								else
								{
									// Unable to read local local file text

									// Allocate string memory
									LPTSTR lpszErrorMessage = new char[ STRING_LENGTH ];

									// Format error message
									wsprintf( lpszErrorMessage, FILE_CLASS_UNABLE_TO_READ_FILE_ERROR_MESSAGE_FORMAT_STRING, lpszLocalFilePath );

									// Display error message
									MessageBox( hWndMain, lpszErrorMessage, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

									// Free string memory
									delete [] lpszErrorMessage;

								} // End of unable to read local file text

								// Close local file
								localFile.Close();

							} // End of successfully opened local file for reading
							else
							{
								// Unable to open local file for reading

								// Allocate string memory
								LPTSTR lpszErrorMessage = new char[ STRING_LENGTH ];

								// Format error message
								wsprintf( lpszErrorMessage, FILE_CLASS_UNABLE_TO_OPEN_FILE_ERROR_MESSAGE_FORMAT_STRING, lpszLocalFilePath );

								// Display error message
								MessageBox( hWndMain, lpszErrorMessage, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

								// Free string memory
								delete [] lpszErrorMessage;

							} // End of unable to open local file for reading

						} // End of successfully downloaded url to local file

						// Free string memory
						delete [] lpszLocalFilePath;

					} // End of successfully got url from edit window

					// Free string memory
					delete [] lpszUrl;

					// Break out of switch
					break;

				} // End of a button window command
				case IDM_FILE_EXIT:
				{
					// A file exit command

					// Destroy window
					DestroyWindow( hWndMain );

					// Break out of switch
					break;

				} // End of a file exit command
				case IDM_HELP_ABOUT:
				{
					// A help about command

					// Show about message
					ShowAboutMessage( hWndMain );

					// Break out of switch
					break;

				} // End of a help about command
				default:
				{
					// Default command

					// See if command message is from edit window
					if( ( HWND )lParam == g_editWindow )
					{
						// Command message is from edit window

						// Handle command message from edit window
						if( !( g_editWindow.HandleCommandMessage( wParam, lParam, &EditWindowUpdateFunction ) ) )
						{
							// Command message was not handled from edit window

							// Call default procedure
							lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

						} // End of command message was not handled from edit window

					} // End of command message is from list box window
					else if( ( HWND )lParam == g_listBoxWindow )
					{
						// Command message is from list box window

						// Handle command message from list box window
						if( !( g_listBoxWindow.HandleCommandMessage( wParam, lParam, &ListBoxWindowSelectionChangedFunction, &ListBoxWindowDoubleClickFunction ) ) )
						{
							// Command message was not handled from list box window

							// Call default procedure
							lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

						} // End of command message was not handled from list box window

					} // End of command message is from list box window
					else
					{
						// Command message is not from list box window

						// Call default procedure
						lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

					} // End of command message is not from list box window

					// Break out of switch
					break;

				} // End of default command

			}; // End of selection for command

			// Break out of switch
			break;

		} // End of a command message
		case WM_SYSCOMMAND:
		{
			// A system command message

			// Select system command
			switch( LOWORD( wParam ) )
			{
				case IDM_HELP_ABOUT:
				{
					// A help about system command

					// Show about message
					ShowAboutMessage( hWndMain );

					// Break out of switch
					break;

				} // End of a help about system command
				default:
				{
					// Default system command

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

					// Break out of switch
					break;

				} // End of default system command

			}; // End of selection for system command

			// Break out of switch
			break;

		} // End of a system command message
		case WM_NOTIFY:
		{
			// A notify message

			// Call default handler
			lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

			// Break out of switch
			break;

		} // End of a notify message
		case WM_CONTEXTMENU:
		{
			// A context menu message
			Menu popupMenu;

			// Load popup menu
			popupMenu.Load( MAKEINTRESOURCE( IDR_CONTEXT_MENU ) );

			// Show popup menu
			popupMenu.TrackPopupMenu( 0, lParam, hWndMain );

			// Break out of switch
			break;

		} // End of a context menu message
		case WM_CLOSE:
		{
			// A close message

			// Destroy main window
			DestroyWindow( hWndMain );

			// Break out of switch
			break;

		} // End of a close message
		case WM_DESTROY:
		{
			// A destroy message

			// Terminate thread
			PostQuitMessage( 0 );

			// Break out of switch
			break;

		} // End of a destroy message
		default:
		{
			// Default message

			// Call default window procedure
			lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

			// Break out of switch
			break;

		} // End of default message

	}; // End of selection for message

	return lr;

} // End of function MainWindowProcedure

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow )
{
	Message message;

	// Connect to internet
	if( g_internet.Connect() )
	{
		// Successfully connected to internet
		WindowClass mainWindowClass;

		// Initialise main window class
		mainWindowClass.Initialise( MAIN_WINDOW_CLASS_NAME, hInstance, MainWindowProcedure, MAIN_WINDOW_CLASS_MENU_NAME );

		// Register main window class
		if( mainWindowClass.Register() )
		{
			// Successfully registered main window class
			Window mainWindow;

			// Create main window
			if( mainWindow.Create( MAIN_WINDOW_CLASS_NAME, NULL, hInstance, MAIN_WINDOW_TEXT ) )
			{
				// Successfully created main window
				Menu systemMenu;
				ArgumentList argumentList;

				// Get system menu
				systemMenu.GetSystem( mainWindow );

				// Add separator item to system menu
				systemMenu.InsertSeparator( MENU_CLASS_SYSTEM_MENU_SEPARATOR_ITEM_POSITION );

				// Add about item to system menu
				systemMenu.InsertItem( MENU_CLASS_SYSTEM_MENU_ABOUT_ITEM_POSITION, IDM_HELP_ABOUT, MENU_CLASS_SYSTEM_MENU_ABOUT_ITEM_TEXT );

				// Get argument list
				if( argumentList.Get() )
				{
					// Successfully got argument list

					// Process arguments
					argumentList.ProcessArguments( &OpenFileFunction );

				} // End of successfully got argument list

				// Show main window
				mainWindow.Show( nCmdShow );

				// Update main window
				mainWindow.Update();

				// Message loop
				while( message.Get() > 0 )
				{
					// Translate message
					message.Translate();

					// Dispatch message
					message.Dispatch();

				}; // End of message loop

			} // End of successfully created main window
			else
			{
				// Unable to create main window

				// Display error message
				MessageBox( NULL, WINDOW_CLASS_UNABLE_TO_CREATE_WINDOW_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

			} // End of unable to create main window

		} // End of successfully registered main window class
		else
		{
			// Unable to register main window class

			// Display error message
			MessageBox( NULL, WINDOW_CLASS_CLASS_UNABLE_TO_REGISTER_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

		} // End of unable to register main window class

	} // End of successfully connected to internet
	else
	{
		// Unable to connect to internet

		// Display error message
		MessageBox( NULL, INTERNET_CLASS_UNABLE_TO_CONNECT_TO_INTERNET_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

	} // End of unable to connect to internet



	return message;

} // End of function WinMain