@ECHO OFF

CLS

SETLOCAL ENABLEDELAYEDEXPANSION

SET INCLUDE=-ILIB\SDL2\include -ILIB\glew\include -ILIB\glm -ILIB\assimp\include
SET LIBRARY=-LLIB\SDL2\lib -LLIB\glew\lib -LLIB\assimp\lib

SET FLAG_COMPILE=-w -Wl,-subsystem,windows
SET FLAG_LIBRARY=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lopengl32 -lglew32 -lglew32mx -lassimp

SET HEADER=-IBI3D/include
SET SOURCE=main.cpp

FOR %%F in ("BI3D/*.cpp") DO (
	SET SOURCE=!SOURCE! BI3D\%%F
)

SET DEBUG=n

ECHO :::: BagIdea3D ::::
SET /P DEBUG=Show Debug Enter (y): 

IF %DEBUG% == y (
	SET FLAG_COMPILE=-w
	ECHO Debug Show = true
) ELSE (
	ECHO Debug Show = false
)

IF NOT EXIST "bin\" (
	MKDIR bin
)

ECHO.
ECHO :::: Compile ::::
ECHO Include      : %INCLUDE%
ECHO Library      : %LIBRARY%
ECHO Flag Compile : %FLAG_COMPILE%
ECHO Flag Library : %FLAG_LIBRARY%
ECHO Source       : %SOURCE%

C:\MinGW\bin\windres BI3D\ICON.rc bin\ICON.o
C:\MinGW\bin\g++ %SOURCE% bin\ICON.o %HEADER% %INCLUDE% %LIBRARY% %FLAG_COMPILE% %FLAG_LIBRARY% -o bin\Game

ECHO.
ECHO :::: Status ::::

IF EXIST "bin\Game.exe" (
	ECHO Compile Success.
	ECHO Start Program.
	CD ASSET
	START /WAIT ..\bin\Game
	CD ..
	RMDIR /S /Q bin
) ELSE (
	ECHO Compile Fail.
	RMDIR /S /Q bin
)

ECHO.
ECHO End Program.