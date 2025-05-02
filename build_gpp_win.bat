set GPP="d:\mingw64\bin\g++.exe"
set  LD="d:\mingw64\bin\ld.exe"

mkdir g_build
cd g_build

%GPP% -std=c++14 -c -I..\include ..\glfwbgi.cpp
%GPP% -std=c++14 -c -I..\include ..\lodepng.cpp

%LD% -r -o libglfwbgi.a glfwbgi.o lodepng.o ..\lib\mingw-w64\x64\libglfw3.a

%GPP% -std=c++14 -o ..\test_gpp.exe  -I. ..\glfwtest.cpp -L. -lglfwbgi -lmingw32 -lopengl32 -lgdi32 -luser32
