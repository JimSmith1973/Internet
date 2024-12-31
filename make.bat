cls

del *.exe

windres Resource.rc -o Resource.o

g++ -mwindows -m64 -static -Wall -Wextra Internet.cpp ^
 ^
 ..\Classes\ArgumentListClass.cpp ^
 ..\Classes\ButtonWindowClass.cpp ^
 ..\Classes\DroppedFilesClass.cpp ^
 ..\Classes\EditWindowClass.cpp ^
 ..\Classes\FileClass.cpp ^
 ..\Classes\FontClass.cpp ^
 ..\Classes\InternetClass.cpp ^
 ..\Classes\ListBoxWindowClass.cpp ^
 ..\Classes\MenuClass.cpp ^
 ..\Classes\MessageClass.cpp ^
 ..\Classes\StatusBarWindowClass.cpp ^
 ..\Classes\WindowClass.cpp ^
 ..\Classes\WindowClassClass.cpp ^
 ^
 Resource.o -o Internet.exe -lwininet
