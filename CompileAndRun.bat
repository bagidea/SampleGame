@ECHO OFF

CLS

SET Include=-IC:\SDL\SDL2-2.0.3\include -IC:\SDL\SDL2-2.0.3\i686-w64-mingw32\include\SDL2 -IC:\SDL\SDL2-2.0.3\x86_64-w64-mingw32\include\SDL2
SET Library=-LC:\SDL\SDL2-2.0.3\lib -LC:\SDL\SDL2-2.0.3\i686-w64-mingw32\lib -LC:\SDL\SDL2-2.0.3\x86_64-w64-mingw32\lib
SET Flag_Compile=-w -Wl,-subsystem,windows
SET Flag_Library=-lmingw32 -lSDL2main -lSDL2

SET Source=main.cpp GameWindow.cpp

IF NOT EXIST "bin\" (
	MKDIR bin
)

g++ %Source% %Include% %Library% %Flag_Compile% %Flag_Library% -o bin/Game

START /WAIT bin/Game
RMDIR /S /Q bin