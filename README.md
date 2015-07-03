# SampleGameEngine - Hot to Settings

1. Download SDL2 SDL2_image SDL2_mixer and Set File CompileAndRun.bat path to Libs.
   (http://www.libsdl.org/ and http://www.libsdl.org/projects)

2. Download Glew and Building Library, Create .bat File and Copy and Paste Code, And Double-Click File .bat
   (http://glew.sourceforge.net/)
::Code::

gcc -DGLEW_NO_GLU -O2 -Wall -W -Iinclude  -DGLEW_BUILD -o src/glew.o -c src/glew.c
gcc -shared -Wl,-soname,libglew32.dll -Wl,--out-implib,lib/libglew32.dll.a    -o lib/glew32.dll src/glew.o -L/mingw/lib -lglu32 -lopengl32 -lgdi32 -luser32 -lkernel32
ar cr lib/libglew32.a src/glew.o

gcc -DGLEW_NO_GLU -DGLEW_MX -O2 -Wall -W -Iinclude  -DGLEW_BUILD -o src/glew.mx.o -c src/glew.c
gcc -shared -Wl,-soname,libglew32mx.dll -Wl,--out-implib,lib/libglew32mx.dll.a -o lib/glew32mx.dll src/glew.mx.o -L/mingw/lib -lglu32 -lopengl32 -lgdi32 -luser32 -lkernel32
ar cr lib/libglew32mx.a src/glew.mx.o

And Set File CompileAndRun.bat path to Libs.

3. Download GLM from Web Site.
   (http://glm.g-truc.net/) 
   And Set File CompileAndRun.bat path to Libs.

*In File CompileAndRun.bat line 10 you can change File for Compile and Run.

Enjoy.
