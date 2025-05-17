mkdir build_macos
cp libglfw3.a build_macos

cd build_macos

#Building lib
g++ -std=c++14 -c -I../ ../glfwbgi.cpp
g++ -std=c++14 -c -I../include ../freetype.cpp
g++ -std=c++14 -c -I../include ../lodepng.cpp

#Creating static lib
ld -r -o libglfwbgi.a glfwbgi.o libglfw3.a freetype.o lodepng.o


#Building test app
g++ -std=c++14 -stdlib=libc++ libglfwbgi.a -lfreetype -framework CoreVideo -framework OpenGL -framework IOKit -framework Cocoa -framework Carbon glfwtest.cpp -o glfwtest