@ECHO OFF

ECHO Creating /lib Directory Structure ...

mkdir x86\Debug
mkdir x86\Release
mkdir x64\Debug
mkdir x64\Release
mkdir "x86\Debug [Static]"
mkdir "x86\Release [Static]"
mkdir "x64\Debug [Static]"
mkdir "x64\Release [Static]"

ECHO Copying SDL2 library files ...

xcopy ..\src\sdl2\VisualC\Win32\Debug\SDL2.lib x86\Debug\ /C /Q /Y 
xcopy ..\src\sdl2\VisualC\Win32\Debug\SDL2main.lib x86\Debug\ /C /Q /Y 
xcopy ..\src\sdl2\VisualC\Win32\Debug\SDL2.dll x86\Debug\ /C /Q /Y 
xcopy ..\src\sdl2\VisualC\Win32\Debug\SDL2.pdb x86\Debug\ /C /Q /Y 

xcopy ..\src\sdl2\VisualC\Win32\Release\SDL2.lib x86\Release\ /C /Q /Y 
xcopy ..\src\sdl2\VisualC\Win32\Release\SDL2main.lib x86\Release\ /C /Q /Y 
xcopy ..\src\sdl2\VisualC\Win32\Release\SDL2.dll x86\Release\ /C /Q /Y 

xcopy ..\src\sdl2\VisualC\x64\Debug\SDL2.lib x64\Debug\ /C /Q /Y 
xcopy ..\src\sdl2\VisualC\x64\Debug\SDL2main.lib x64\Debug\ /C /Q /Y 
xcopy ..\src\sdl2\VisualC\x64\Debug\SDL2.dll x64\Debug\ /C /Q /Y 
xcopy ..\src\sdl2\VisualC\x64\Debug\SDL2.pdb x64\Debug\ /C /Q /Y 

xcopy ..\src\sdl2\VisualC\x64\Release\SDL2.lib x64\Release\ /C /Q /Y 
xcopy ..\src\sdl2\VisualC\x64\Release\SDL2main.lib x64\Release\ /C /Q /Y 
xcopy ..\src\sdl2\VisualC\x64\Release\SDL2.dll x64\Release\ /C /Q /Y 

xcopy "..\src\sdl2\VisualC\Win32\Debug [Static]\SDL2.lib" "x86\Debug [Static]\" /C /Q /Y 
xcopy "..\src\sdl2\VisualC\Win32\Debug\SDL2main.lib" "x86\Debug [Static]\" /C /Q /Y 

xcopy "..\src\sdl2\VisualC\Win32\Release [Static]\SDL2.lib" "x86\Release [Static]\" /C /Q /Y 
xcopy "..\src\sdl2\VisualC\Win32\Release\SDL2main.lib" "x86\Release [Static]\" /C /Q /Y 

xcopy "..\src\sdl2\VisualC\x64\Debug [Static]\SDL2.lib" "x64\Debug [Static]\" /C /Q /Y 
xcopy "..\src\sdl2\VisualC\x64\Debug\SDL2main.lib" "x64\Debug [Static]\" /C /Q /Y 

xcopy "..\src\sdl2\VisualC\x64\Release [Static]\SDL2.lib" "x64\Release [Static]\" /C /Q /Y 
xcopy "..\src\sdl2\VisualC\x64\Release\SDL2main.lib" "x64\Release [Static]\" /C /Q /Y

ECHO Done.