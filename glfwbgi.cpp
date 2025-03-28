#include <stdio.h>

#include "glfwbgi.h"

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
//#error apple defined
#include <GLFW/glfw3.h>
#else
#include "glfw\glfw3.h"
#include <gl\GL.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include <fstream>
#include <cstdint>

//#define DBG_OUT
#ifdef DBG_OUT
#define DBG_PRINT printf
#else
#define DBG_PRINT(...)
#endif // DBG_OUT

namespace Graph
{

typedef union 
{
	unsigned long dwColor;

	struct  
	{
		unsigned long r : 8;
		unsigned long g : 8;
		unsigned long b : 8;
		unsigned long : 8;
	} rgb;

} ColorType;

//
// Variables
//
bool g_GraphEnabled = false;
GLFWwindow * g_GraphWindow = nullptr;

int g_ScreenW = 0;
int g_ScreenH = 0;

// Key buffer
const unsigned long KeyBufSize = 32;
unsigned int keyBuf[KeyBufSize];
unsigned long bufCurSize = 0;
unsigned long bufRIndex = 0;
unsigned long bufWIndex = 0;


// VKey buffer
bool g_pressedKeys[GLFW_KEY_LAST];

//
// Callbacks
//
void MyErrorCallback(int code, const char* message)
{
	printf("error: [%u] %s\n", code, message);
}

void MyKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
	DBG_PRINT("Key callback: %d %d %d %d\n", key, scancode, action, mods);

	if( key >= 0 && key <= GLFW_KEY_LAST )
	{
		if( action == GLFW_PRESS )
		{
			g_pressedKeys[key] = true;
		}

		if( action == GLFW_RELEASE )
		{
			g_pressedKeys[key] = false;
		}
	}
}

void MyCharCallback(GLFWwindow* pWindow, unsigned int charCode)
{
	DBG_PRINT("Char callback: %u\n", charCode);

	if( !g_GraphEnabled ) return;

	if( bufCurSize < KeyBufSize )
	{
		keyBuf[bufWIndex] = charCode;
		bufWIndex = (bufWIndex + 1) % KeyBufSize;
		++bufCurSize;
	}
}

void MyResizeCallback(GLFWwindow* pWindow, int, int)
{
	glfwSetWindowSize(pWindow, g_ScreenW, g_ScreenH);
}

unsigned long GetColor(unsigned char r, unsigned char g, unsigned char b)
{
	return ((unsigned long)b << 16) | ((unsigned long)g << 8) | (unsigned long)r;
}

//
// API
//
bool InitGraph(int width, int height, const char * title)
{
	glfwSetErrorCallback(&MyErrorCallback);

	int res = glfwInit();
	if( res == GLFW_FALSE )
	{
		printf("LIB init failed\n");
		return false;
	}

	DBG_PRINT("GLFW initialized\n");

	//* Create a windowed mode window and its OpenGL context */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_DEPTH_BITS, 0);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);

	GLFWwindow * graphWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);

	if( graphWindow == nullptr )
	{
		printf("Create window failed\n");

		glfwTerminate();
		return false;
	}

#ifdef DBG_OUT
	{
		int _width, _height;
		glfwGetFramebufferSize(graphWindow, &_width, &_height);
		printf("glfwGetFramebufferSize returned %d %d\n", _width, _height);
	}
#endif // DBG_OUT

	//glViewport(0, 0, width, height);


	//Make the window's context current */
	glfwMakeContextCurrent(graphWindow);

	glfwSetKeyCallback(graphWindow, &MyKeyCallback);
	glfwSetCharCallback(graphWindow, &MyCharCallback);
	glfwSetWindowSizeCallback(graphWindow, &MyResizeCallback);
	DBG_PRINT("callbacks set\n");

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glScalef(2 / ((float)width), -2 / ((float)height), 1);
	glTranslatef(-((float)width - 2) / 2, -(float)height / 2, 0);

	glLineWidth(1.5f);

	glEnable(GL_TEXTURE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA);
	glEnable(GL_BLEND);	

	glClear(GL_COLOR_BUFFER_BIT);

	g_GraphEnabled = true;
	g_GraphWindow = graphWindow;

	bufCurSize = 0;
	bufRIndex = 0;
	bufWIndex = 0;

	g_ScreenW = width;
	g_ScreenH = height;

	return true;
}

void CloseGraph()
{
	glfwTerminate();

	g_GraphWindow = nullptr;
}

bool ShouldClose()
{
	if( !g_GraphEnabled )
	{
		return true;		
	}

	return (glfwWindowShouldClose(g_GraphWindow) != 0);
}

char ReadKey()
{
	DBG_PRINT("Readkey()\n");

	char key = 0;

	if( g_GraphEnabled )
	{
		while( bufCurSize == 0 && glfwWindowShouldClose(g_GraphWindow) == 0 )
		{
			glfwPollEvents();
		}

		key = keyBuf[bufRIndex];
		bufRIndex = (bufRIndex + 1) % KeyBufSize;
		--bufCurSize;
	}

	return key;
}


bool KeyPressed()
{
	DBG_PRINT("KeyPressed\n");

	if( !g_GraphEnabled ) return false;

	glfwPollEvents();

	if( glfwWindowShouldClose(g_GraphWindow) != 0 )
	{
		//THINK: should it be handled directly
		//halt;
	}

	return bufCurSize > 0;
}

bool IsKeyPressed(unsigned short vkey)
{
	if( !g_GraphEnabled ) return false;

	if( vkey <= GLFW_KEY_LAST )
	{
		return g_pressedKeys[vkey];
	}

	return false;
}

void Delay(long ms)
{
	//DBG_PRINT("Delay(%d)\n", ms);

	if( g_GraphEnabled )
	{
		if( glfwWindowShouldClose(g_GraphWindow) != 0 )
		{
			//THINK: should it be handled directly
			//halt;
			return;
		}

		double curTime = glfwGetTime();
		double endTime = curTime + (ms / 1000.0);

		while( curTime < endTime )
		{
			glfwPollEvents();
			curTime = glfwGetTime();
		}
	}
}

void Rectangled(double x1, double y1, double x2, double y2, unsigned long color, bool bPolygon)
{
	if( !g_GraphEnabled ) return;

	DBG_PRINT("Rectangled %.1f %.1f %.1f %.1f %08lX %d\n", x1,y1,x2,y2,color, bPolygon ? 1 : 0);

	if( x2 < x1 || y2 < y1 )
	{
		DBG_PRINT("Invalid params\n");
		return;
	}

	ColorType &tmpColor = *((ColorType*)&color);

	glBegin(bPolygon ? GL_POLYGON : GL_LINE_LOOP);

	glColor3ub(tmpColor.rgb.r, tmpColor.rgb.g, tmpColor.rgb.b);

	glVertex2d(x1, y1);
	glVertex2d(x2, y1);
	glVertex2d(x2, y2);
	glVertex2d(x1, y2);

	glEnd();
}

enum EllipseType
{
	Arc = 1,
	Sector = 2,
	Chord = 3
};

void Ellipsed(double x, double y, double xradius, double yradius, double startAngle, double stopAngle, unsigned long color, bool bPolygon, EllipseType type)
{
	if( !g_GraphEnabled ) return;

	DBG_PRINT("Ellipsed %.1f %.1f %.1f %.1f %.1f %.1f %08lX %d %d\n", x, y, xradius, yradius, startAngle, stopAngle, color, bPolygon ? 1 : 0, (int)type);

	double fistep;
	if( xradius > yradius )
	{
		fistep = 1 / xradius;
	}
	else
	{
		fistep = 1 / yradius;
	}

	ColorType &tmpColor = *((ColorType*)&color);

	glBegin(bPolygon ?
		GL_POLYGON :
		type == EllipseType::Arc ? GL_LINE_STRIP : GL_LINE_LOOP);

	glColor3ub(tmpColor.rgb.r, tmpColor.rgb.g, tmpColor.rgb.b);

	if( type == EllipseType::Sector )
	{
		glVertex2d(x, y);
	}

	double fi = startAngle;
	while (fi < stopAngle)
	{
		glVertex2d(x + xradius*cos(fi), y - yradius*sin(fi));
		fi += fistep;
	}

	glVertex2d(x + xradius*cos(stopAngle), y-yradius*sin(stopAngle));

	glEnd();
}

enum PolyType
{
	ptLine = 1,
	ptLoop = 2,
	ptPolygon = 3
};

void Poly(Point * points, unsigned short count, unsigned long color, PolyType type)
{
	if( !g_GraphEnabled ) return;

	DBG_PRINT("Poly 0x%p %u %08lX %d\n", points, count, color, (int)type);

	switch( type )
	{
	case Graph::ptLine:
		glBegin(GL_LINE_STRIP);
		break;
	case Graph::ptLoop:
		glBegin(GL_LINE_LOOP);
		break;
	case Graph::ptPolygon:
		glBegin(GL_POLYGON);
		break;
	default:
		return;
	}

	ColorType &tmpColor = *((ColorType*)&color);

	glColor3ub(tmpColor.rgb.r, tmpColor.rgb.g, tmpColor.rgb.b);

	for (unsigned short i = 0 ; i < count ;++i)
	{
		glVertex2i(points[i].x, points[i].y);
	}

	glEnd();
}

void Lined(double x1, double y1, double x2, double y2, unsigned long color)
{
	if( !g_GraphEnabled ) return;

	DBG_PRINT("Line %.1f %.1f %.1f %.1f %08lX\n", x1, y1, x2, y2, color);

	glBegin(GL_LINE_LOOP);
	
	ColorType &tmpColor = *((ColorType*)&color);

	glColor3ub(tmpColor.rgb.r, tmpColor.rgb.g, tmpColor.rgb.b);

	glVertex2d(x1, y1);
	glVertex2d(x2, y2);

	glEnd();
}

void DrawLine(short x1, short y1, short x2, short y2, unsigned long color)
{
	Lined(x1, y1, x2, y2, color);
// 
// 	glBegin(GL_LINES);
// 
// 	ColorType &tmpColor = *((ColorType*)&color);
// 
// 	//glColor3ub(tmpColor.rgb.r, tmpColor.rgb.g, tmpColor.rgb.b);
// 	glColor3ub(0, 0, 0);
// 
// 	glVertex2i(500, 500);
// 	glVertex2i(, y2);
// 
// 	glEnd();
}

void DrawEllipseArc(short x, short y, unsigned short xradius, unsigned short yradius, short startAngle, short stopAngle, unsigned long color)
{
	Ellipsed(x, y, xradius, yradius, startAngle * M_PI / 180, stopAngle * M_PI / 180, color, false, EllipseType::Arc);
}

void DrawEllipseSector(short x, short y, unsigned short xradius, unsigned short yradius, short startAngle, short stopAngle, unsigned long color)
{
	Ellipsed(x, y, xradius, yradius, startAngle * M_PI / 180, stopAngle * M_PI / 180, color, false, EllipseType::Sector);
}

void DrawEllipseChord(short x, short y, unsigned short xradius, unsigned short yradius, short startAngle, short stopAngle, unsigned long color)
{
	Ellipsed(x, y, xradius, yradius, startAngle * M_PI / 180, stopAngle * M_PI / 180, color, false, EllipseType::Chord);
}

void DrawPoly(Point * points, unsigned short count, unsigned long color)
{
	Poly(points, count, color, PolyType::ptLoop);
}

void DrawPolyLine(Point * points, unsigned short count, unsigned long color)
{
	Poly(points, count, color, PolyType::ptLine);
}

void FillEllipseSector(short x, short y, unsigned short xradius, unsigned short yradius, short startAngle, short stopAngle, unsigned long color)
{
	Ellipsed(x, y, xradius, yradius, startAngle * M_PI / 180, stopAngle * M_PI / 180, color, true, EllipseType::Sector);
}

void FillEllipseChord(short x, short y, unsigned short xradius, unsigned short yradius, short startAngle, short stopAngle, unsigned long color)
{
	Ellipsed(x, y, xradius, yradius, startAngle * M_PI / 180, stopAngle * M_PI / 180, color, true, EllipseType::Chord);
}

void DrawRectangle(short x1, short y1, short x2, short y2, unsigned long color)
{
	Rectangled(x1, y1, x2, y2, color, false);
}

void FillPoly(Point * points, unsigned short count, unsigned long color)
{
	Poly(points, count, color, PolyType::ptPolygon);
}

void SetLineWidth(float thickness)
{
	if( !g_GraphEnabled ) return;

	if( thickness <= 0 ) return;

	glLineWidth(thickness);
}


void FillRectangle(short x1, short y1, short x2, short y2, unsigned long color)
{
	Rectangled(x1, y1, x2, y2, color, true);
}

void ClearDevice(unsigned long color)
{
	if( !g_GraphEnabled ) return;

	ColorType tmpColor;
	tmpColor.dwColor = color;

	glClearColor(
		tmpColor.rgb.r / 255.0f,
		tmpColor.rgb.g / 255.0f,
		tmpColor.rgb.b / 255.0f,
		1.0
	);

	glClear(GL_COLOR_BUFFER_BIT);
}

void SwapBuffers()
{
	if( !g_GraphEnabled ) return;

	glfwSwapBuffers(g_GraphWindow);
}

#define ang( a ) a * M_PI / 180

double FontWidth = 4;
double FontHeight = 8;
double FontSpacing = 2;
unsigned long FontColor = Color::White;

void UnknownSymbol(double x, double y)
{
	Rectangled(x, y-FontHeight, x+FontWidth, y, FontColor, false);
}

void UpperA(double x, double y)
{
	Lined(x,y,(x+FontWidth/2), y-FontHeight, FontColor);
	Lined((x+FontWidth/2), y-FontHeight,x+FontWidth, y, FontColor);

	Lined((x+FontWidth/4), (y-FontHeight/2), (x+3*FontWidth/4), (y-FontHeight/2), FontColor);
}

void UpperB(double x, double y)
{
	double xradius, yradius;


	xradius = FontWidth / 2;
	yradius = FontHeight / 4;

	Lined(x, y, x, y- FontHeight, FontColor);

	Ellipsed(x, y - FontHeight + yradius, xradius, yradius,ang( 0),ang( 90), FontColor, false, EllipseType::Arc);
	Ellipsed(x, y - FontHeight + yradius, xradius, yradius,ang( 270),ang( 360), FontColor, false, EllipseType::Arc);

	Ellipsed(x, y - yradius, FontWidth, yradius,ang( 0),ang( 90), FontColor, false, EllipseType::Arc);
	Ellipsed(x, y - yradius, FontWidth, yradius,ang( 270),ang( 360), FontColor, false, EllipseType::Arc);
}

void UpperC(double x, double y)
{
	Ellipsed(x+FontWidth / 2, y-FontHeight/2, FontWidth / 2, FontHeight / 2,ang( 50),ang( 310), FontColor, false, EllipseType::Arc);
}

void UpperD(double x, double y)
{
	Ellipsed(x+FontWidth / 2, y-FontHeight / 2, FontWidth / 2, FontHeight / 2,ang( 0),ang( 90), FontColor, false, EllipseType::Arc);
	Ellipsed(x+FontWidth / 2, y-FontHeight / 2, FontWidth / 2, FontHeight / 2,ang( 270),ang( 360), FontColor, false, EllipseType::Arc);
	
	Lined(x+FontWidth / 2, y,x,y, FontColor);

	Lined(x,y,x,y-FontHeight, FontColor);
	Lined(x,y-FontHeight, x+FontWidth / 2,y-FontHeight, FontColor);
}

void UpperE(double x, double y)
{
	Lined(x+FontWidth, y - FontHeight,x, y - FontHeight, FontColor);
	Lined(x, y - FontHeight,x, y, FontColor);
	Lined(x, y,x+FontWidth, y, FontColor);

	Lined(x+FontWidth, y-(FontHeight / 2),x, y-(FontHeight / 2), FontColor);
}

void UpperF(double x, double y)
{
	Lined(x,y,x, y-FontHeight, FontColor);
	Lined(x, y-FontHeight,x+FontWidth, y-FontHeight, FontColor);
	Lined(x, y-FontHeight / 2, x + FontWidth - FontWidth / 4, y-FontHeight / 2, FontColor);
}

void UpperG(double x, double y)
{
	Ellipsed(x+FontWidth / 2, y-FontHeight / 2, FontWidth / 2, FontHeight / 2,ang( 50),ang( 270), FontColor, false, EllipseType::Arc);
	
	Lined(x + FontWidth / 2, y,x + FontWidth, y, FontColor);
	Lined(x + FontWidth, y,x + FontWidth, y-FontHeight / 3, FontColor);
	Lined(x + FontWidth, y-FontHeight / 3,x + FontWidth/2, y-FontHeight / 3, FontColor);
}

//Lined(,, FontColor);

void UpperH(double x, double y)
{
	Lined(x, y, x, y-FontHeight, FontColor);
	Lined(x + FontWidth, y, x+ FontWidth, y-FontHeight, FontColor);
	Lined(x, y-FontHeight / 2, x + FontWidth, y-FontHeight / 2, FontColor);
}

void UpperI(double x, double y)
{
	Lined(x + FontWidth / 4, y-FontHeight, x + FontWidth - FontWidth / 4, y-FontHeight, FontColor);
	Lined(x + FontWidth / 4, y, x + FontWidth - FontWidth / 4, y, FontColor);

	Lined(x+ FontWidth / 2, y, x+FontWidth / 2, y-FontHeight, FontColor);
}

void UpperJ(double x, double y)
{
	Lined(x+FontWidth, y-FontHeight, x+FontWidth / 2, y-FontHeight, FontColor);
	Lined(x+FontWidth, y-FontHeight, x+FontWidth, y-FontWidth / 2, FontColor);

	Ellipsed(x+FontWidth / 2, y-FontHeight / 2, FontWidth / 2, FontWidth / 2,ang( 180),ang( 360), FontColor, false, EllipseType::Arc);
}

void UpperK(double x, double y)
{
	Lined(x,y, x, y-FontHeight, FontColor);
	Lined(x+FontWidth, y-FontHeight,x, y-FontHeight / 2, FontColor);
	Lined(x, y-FontHeight / 2,x+FontWidth, y, FontColor);
}

void UpperL(double x, double y)
{
	Lined(x, y-FontHeight,x,y, FontColor);
	Lined(x,y,x+FontWidth, y, FontColor);
}

void UpperM(double x, double y)
{	
	Lined(x, y,x + FontWidth / 4, y-FontHeight, FontColor);
	Lined(x + FontWidth / 4, y-FontHeight,x+ FontWidth / 2, y-FontHeight / 2, FontColor);
	Lined(x+ FontWidth / 2, y-FontHeight / 2,x + FontWidth - FontWidth / 4, y - FontHeight, FontColor);
	Lined(x + FontWidth - FontWidth / 4, y - FontHeight,x + FontWidth, y, FontColor);
}

void UpperN(double x, double y)
{
	Lined(x,y,x,y-FontHeight, FontColor);
	Lined(x,y-FontHeight,x+FontWidth,y, FontColor);
	Lined(x+FontWidth,y,x+FontWidth,y -FontHeight, FontColor);
}

void UpperO(double x, double y)
{
	Ellipsed(x+FontWidth / 2, y-FontHeight / 2, FontWidth / 2, FontHeight / 2,ang( 0),ang( 360), FontColor, false, EllipseType::Arc);
}

void UpperP(double x, double y)
{
	double h3;

	h3 = FontHeight/3;

	Lined(x,y,x,y-FontHeight, FontColor);
	Ellipsed(x, y - FontHeight + h3, FontWidth, h3,ang( 0),ang( 90), FontColor, false, EllipseType::Arc);
	Ellipsed(x, y - FontHeight + h3, FontWidth, h3,ang( 270),ang( 360), FontColor, false, EllipseType::Arc);
}

void UpperQ(double x, double y)
{	
	Ellipsed(x+FontWidth / 2, y-FontHeight / 2, FontWidth / 2, FontHeight / 2,ang( 0),ang( 360), FontColor, false, EllipseType::Arc);
	Lined(x+FontWidth / 2, y-FontHeight / 4, x+FontWidth, y+FontHeight / 4, FontColor);
}

void UpperR(double x, double y)
{
	double h3;

	h3 = (FontHeight/4);

	Lined(x,y,x,y-FontHeight, FontColor);
	Ellipsed(x, y - FontHeight + h3, FontWidth, h3,ang( 0),ang( 90), FontColor, false, EllipseType::Arc);
	Ellipsed(x, y - FontHeight + h3, FontWidth, h3,ang( 270),ang( 360), FontColor, false, EllipseType::Arc);

	Lined(x, y - FontHeight + 2*h3, x + FontWidth, y, FontColor);
}

void UpperS(double x, double y)	
{
	double xradius, yradius;


	xradius = FontWidth / 2;
	yradius = FontHeight / 4;

	Ellipsed(x + xradius, y - FontHeight + yradius, xradius, yradius,ang( 30),ang( 280), FontColor, false, EllipseType::Arc);
	Ellipsed(x + xradius, y - yradius, xradius, yradius,ang( 0),ang( 100), FontColor, false, EllipseType::Arc);
	Ellipsed(x + xradius, y - yradius, xradius, yradius,ang( 210),ang( 360), FontColor, false, EllipseType::Arc);
}

void UpperT(double x, double y)
{
	Lined(x, y-FontHeight, x + FontWidth, y-FontHeight, FontColor);

	Lined(x+ FontWidth / 2, y, x+FontWidth / 2, y-FontHeight, FontColor);
}

void UpperU(double x, double y)
{
	Lined(x, y-FontHeight, x, y-FontWidth / 2, FontColor);
	Lined(x+FontWidth, y-FontHeight, x+FontWidth, y-FontWidth / 2, FontColor);

	Ellipsed(x+FontWidth / 2, y - FontWidth / 2, FontWidth / 2, FontWidth / 2,ang( 180),ang( 360), FontColor, false, EllipseType::Arc);
}


void UpperV(double x, double y)
{
	Lined(x,y-FontHeight,(x+FontWidth/2), y, FontColor);
	Lined((x+FontWidth/2), y,x+FontWidth, y-FontHeight, FontColor);
}

void UpperW(double x, double y)	
{
	Lined(x, y - FontHeight,x + FontWidth / 4, y, FontColor);
	Lined(x + FontWidth / 4, y,x+ FontWidth / 2, y-FontHeight / 2, FontColor);
	Lined(x+ FontWidth / 2, y-FontHeight / 2,x + FontWidth - FontWidth / 4, y, FontColor);
	Lined(x + FontWidth - FontWidth / 4, y,x + FontWidth, y - FontHeight, FontColor);
}

void UpperX(double x, double y)
{
	Lined(x,y, x+ FontWidth, y-FontHeight, FontColor);
	Lined(x,y-FontHeight, x+ FontWidth, y, FontColor);
}

void UpperY(double x, double y)
{
	Lined(x, y-FontHeight,x + FontWidth / 2, y - FontHeight / 2, FontColor);
	Lined(x + FontWidth / 2, y - FontHeight / 2,x + FontWidth, y - FontHeight, FontColor);

	Lined(x+FontWidth / 2, y-FontHeight / 2,x+FontWidth / 2, y, FontColor);
}

void LetterSpace(double x, double y)
{	
}

void UpperZ(double x, double y)
{
	Lined(x, y - FontHeight,x+FontWidth, y - FontHeight, FontColor);
	Lined(x+FontWidth, y - FontHeight, x, y, FontColor);
	Lined(x, y,x+FontWidth, y, FontColor);
}

void Symbol0(double x, double y)
{
	UpperO(x,y);

	Lined(x,y,x+FontWidth,y-FontHeight, FontColor);
}

void Symbol1(double x, double y)
{
	Lined(x + FontWidth / 4, y, x + FontWidth - FontWidth / 4, y, FontColor);

	Lined(x + FontWidth / 2, y,x + FontWidth / 2, y-FontHeight, FontColor);
	Lined(x + FontWidth / 2, y-FontHeight,x, y-FontHeight + FontHeight / 4, FontColor);
}

void Symbol2(double x, double y)
{
	double yradius;

	yradius = FontHeight / 4;

	Ellipsed(x + FontWidth - FontWidth / 2, y - FontHeight + yradius, FontWidth / 2, yradius,ang( 0),ang( 180), FontColor, false, EllipseType::Arc);

	Lined(x + FontWidth, y - FontHeight + yradius,x,y, FontColor);
	Lined(x,y,x+FontWidth,y, FontColor);
}

void Symbol3(double x, double y)
{
	double yradius;

	yradius = FontHeight / 4;
	Ellipsed(x + FontWidth / 2, y - FontHeight + yradius, FontWidth / 2, yradius,ang( 0),ang( 120), FontColor, false, EllipseType::Arc);
	Ellipsed(x + FontWidth / 2, y - FontHeight + yradius, FontWidth / 2, yradius,ang( 260),ang( 360), FontColor, false, EllipseType::Arc);

	Ellipsed(x + FontWidth / 2, y - yradius, FontWidth / 2, yradius,ang( 0),ang( 100), FontColor, false, EllipseType::Arc);
	Ellipsed(x + FontWidth / 2, y - yradius, FontWidth / 2, yradius,ang( 240),ang( 360), FontColor, false, EllipseType::Arc);
}

void Symbol4(double x, double y)
{
	Lined(x + ((FontWidth * 3) / 4), y,x + ((FontWidth * 3) / 4), y-FontHeight, FontColor);
	Lined(x + ((FontWidth * 3) / 4), y-FontHeight,x, y-FontHeight / 3, FontColor);
	Lined(x, y-FontHeight / 3,x+FontWidth, y-FontHeight / 3, FontColor);
}

void Symbol5(double x, double y)
{
	double yradius;

	yradius = (FontHeight + 3) / 4;

	Ellipsed(x, y - yradius, FontWidth, yradius,ang( 0),ang( 90), FontColor, false, EllipseType::Arc);
	Ellipsed(x, y - yradius, FontWidth, yradius,ang( 270),ang( 360), FontColor, false, EllipseType::Arc);

	Lined(x, y - 2*yradius,x, y - FontHeight, FontColor);
	Lined(x, y - FontHeight,x + FontWidth, y - FontHeight, FontColor);
}

void Symbol6(double x, double y)
{
	double yradius;

	yradius = FontHeight / 4;
	Ellipsed(x + FontWidth / 2, y - FontHeight / 2, FontWidth / 2, FontHeight / 2,ang( 90),ang( 270), FontColor, false, EllipseType::Arc);
	Ellipsed(x + FontWidth / 2, y - yradius, FontWidth / 2, yradius,ang( 0),ang( 360), FontColor, false, EllipseType::Arc);
}

void Symbol7(double x, double y)
{
	Lined(x, y - FontHeight, x + FontWidth, y - FontHeight, FontColor);
	Lined(x + FontWidth, y - FontHeight, x, y, FontColor);
}

void Symbol8(double x, double y)
{
	double yradius;

	yradius = FontHeight / 4;
	Ellipsed(x + FontWidth / 2, y - FontHeight + yradius, FontWidth / 2, yradius,ang( 0),ang( 360), FontColor, false, EllipseType::Arc);
	Ellipsed(x + FontWidth / 2, y - yradius, FontWidth / 2, yradius,ang( 0),ang( 360), FontColor, false, EllipseType::Arc);
}

void Symbol9(double x, double y)
{
	double yradius;

	yradius = FontHeight / 4;

	Ellipsed(x + FontWidth / 2, y - FontHeight / 2, FontWidth / 2, FontHeight / 2,ang( 0),ang( 90), FontColor, false, EllipseType::Arc);
	Ellipsed(x + FontWidth / 2, y - FontHeight / 2, FontWidth / 2, FontHeight / 2,ang( 270),ang( 360), FontColor, false, EllipseType::Arc);
	
	Ellipsed(x + FontWidth / 2, y - FontHeight + yradius, FontWidth / 2, yradius, ang(0), ang(360), FontColor, false, EllipseType::Arc);
}

void OutChar(double x, double y, char ch)
{
	switch (ch)
		{
		case ' ': LetterSpace(x,y); break;
		case 'a': case 'A': UpperA(x,y); break;
		case 'b': case 'B': UpperB(x,y); break;
		case 'c': case 'C': UpperC(x,y); break;
		case 'd': case 'D': UpperD(x,y); break;
		case 'e': case 'E': UpperE(x,y); break;
		case 'f': case 'F': UpperF(x,y); break;
		case 'g': case 'G': UpperG(x,y); break;
		case 'h': case 'H': UpperH(x,y); break;
		case 'i': case 'I': UpperI(x,y); break;
		case 'j': case 'J': UpperJ(x,y); break;
		case 'k': case 'K': UpperK(x,y); break;
		case 'l': case 'L': UpperL(x,y); break;		
		case 'm': case 'M': UpperM(x,y); break;
		case 'n': case 'N': UpperN(x,y); break;
		case 'o': case 'O': UpperO(x,y); break;
		case 'p': case 'P': UpperP(x,y); break;
		case 'q': case 'Q': UpperQ(x,y); break;
		case 'r': case 'R': UpperR(x,y); break;
		case 's': case 'S': UpperS(x,y); break;
		case 't': case 'T': UpperT(x,y); break;
		case 'u': case 'U': UpperU(x,y); break;
		case 'v': case 'V': UpperV(x,y); break;
		case 'w': case 'W': UpperW(x,y); break;
		case 'x': case 'X': UpperX(x,y); break;
		case 'y': case 'Y': UpperY(x,y); break;
		case 'z': case 'Z': UpperZ(x,y); break;
		case '1': Symbol1(x,y); break;
		case '2': Symbol2(x,y); break;
		case '3': Symbol3(x,y); break;
		case '4': Symbol4(x,y); break;
		case '5': Symbol5(x,y); break;
		case '6': Symbol6(x,y); break;
		case '7': Symbol7(x,y); break;
		case '8': Symbol8(x,y); break;
		case '9': Symbol9(x,y); break;
		case '0': Symbol0(x,y); break;
		default:
			UnknownSymbol(x, y);
		}
}

void OutText(short startx, short starty, char text, unsigned long color, unsigned short size)
{
	FontWidth = size;
	FontHeight = 2*size;
	FontSpacing = size/2;
	FontColor = color;

	double x = startx;
	double y = starty;

	OutChar(x,y, text);
}

void OutText(short startx, short starty, const std::string &text, unsigned long color, unsigned short size)
{
	OutText(startx, starty, text.c_str(), color, size);
}

void OutText(short startx, short starty, const char * text, unsigned long color, unsigned short size)
{
	DBG_PRINT("OutText %d %d %s %08X %d\n", startx, starty, text, color, size);

	FontWidth = size;
	FontHeight = 2*size;
	FontSpacing = size/2;
	FontColor = color;

	double x = startx;
	double y = starty;

	for (int i = 0; text[i]; ++i )
	{		
		OutChar(x,y,text[i]);

		x += FontWidth + FontSpacing;
	}
}

#pragma pack(push,1)
typedef struct tagBMPHEADER{
	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;
}BMPHEADER;

typedef struct tagBMPINFOHEADER{
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t  biCompression; 
	uint32_t  biSizeImage; 
	uint32_t  biXPelsPerMeter; 
	uint32_t  biYPelsPerMeter; 
	uint32_t  biClrUsed; 
	uint32_t  biClrImportant; 
}BMPINFOHEADER;

typedef struct tagRGBAPALE{
	unsigned char Red; 
	unsigned char Green; 
	unsigned char Blue; 
	unsigned char Alpha; 
}RGBAPALE;

typedef struct tagRGBDATA{
	unsigned char Red; 
	unsigned char Green; 
	unsigned char Blue; 
	// BYTE Alpha;
}RGBA;
#pragma pack(pop)

bool ValidateInfoHeader(const BMPINFOHEADER &bmiHeader, const char * filename)
{
	DBG_PRINT("BMPINFOHEADER for [%s]:\n"
			"\t biSize = %u\n"
			"\t biWidth = %u\n"
			"\t biHeight = %u\n"
			"\t biPlanes = %u\n"
			"\t biBitCount = %u\n"
			"\t biCompression = %u\n"
			"\t biSizeImage = %u\n"
			"\t biXPelsPerMeter = %u\n"
			"\t biYPelsPerMeter = %u\n"
			"\t biClrUsed = %u\n"
			"\t biClrImportant = %u\n",
			filename,			
			bmiHeader.biSize,
			bmiHeader.biWidth,
			bmiHeader.biHeight,
			bmiHeader.biPlanes,
			bmiHeader.biBitCount,
			bmiHeader.biCompression, 
			bmiHeader.biSizeImage, 
			bmiHeader.biXPelsPerMeter, 
			bmiHeader.biYPelsPerMeter, 
			bmiHeader.biClrUsed, 
			bmiHeader.biClrImportant
	);

	if (bmiHeader.biPlanes != 1)
	{
		printf("Error. File [%s] has invalid format. Planes is not 1\n", filename);
		return false;		
	}

	if (bmiHeader.biCompression != 0) //BI_RGB
	{
		printf("Error. File [%s]. Compressed BMP is not supported\n", filename);
		return false;		
	}

	if (bmiHeader.biBitCount != 24)
	{
		printf("Error. File [%s]. Only 24-bit BMP is supported\n", filename);
		return false;		
	}

	if (bmiHeader.biSizeImage == 0)
	{
		printf("Error. File [%s] has invalid format. Image size is 0\n", filename);
		return false;		
	}

	if (bmiHeader.biWidth == 0 || bmiHeader.biHeight == 0)
	{
		printf("Error. File [%s] has invalid format. Image dimensions are %d,%d\n", filename, bmiHeader.biWidth, bmiHeader.biHeight);
		return false;		
	}

	return true;
}

Image::Image()
	: m_Texture(0)
	, m_Initialized(false)
	, m_Width(0)
	, m_Height(0)
{	
}

Image::~Image()
{
	if (m_Initialized && m_Texture != 0)
	{
		 glDeleteTextures(1, &m_Texture);
		 m_Texture = 0;
	}	
}

bool Image::LoadBMP(const char * filename, bool transparent)
{
	DBG_PRINT("Image::LoadBMP [%s] %d", filename, transparent);	

	std::ifstream in(filename, std::ios::binary | std::ios::in);

	if (!in.is_open() || !in.good())
	{
		printf("Error [%d] opening file [%s]\n", errno, filename);
		return false;
	}

	BMPHEADER bmHeader;
	memset(&bmHeader,0,sizeof(BMPHEADER));

	DBG_PRINT("Reading %u bytes (BMPHEADER) from [%s]\n", sizeof(BMPHEADER), filename);	
	in.read((char*)&bmHeader,sizeof(BMPHEADER));
	if (in.bad())
	{
		printf("Error [%d] reading file [%s]", errno, filename);
		return false;		
	}

	if (bmHeader.bfType != 0x4d42)
	{
		printf("Error. File [%s] is not a BMP file\n", filename);
		return false;
	}

	BMPINFOHEADER bmiHeader;
	memset(&bmiHeader,0,sizeof(BMPINFOHEADER));
	
	DBG_PRINT("Reading %u bytes (BMPINFOHEADER) from [%s]\n", sizeof(BMPINFOHEADER), filename);

	in.read((char*)&bmiHeader,sizeof(BMPINFOHEADER));
	if (in.bad())
	{
		printf("Error [%d] reading file [%s]", errno, filename);
		return false;		
	}

	if (!ValidateInfoHeader(bmiHeader, filename))
	{
		return false;
	}

	DBG_PRINT("Read file [%s]\n", filename);

	unsigned int width = bmiHeader.biWidth;
	unsigned int height = bmiHeader.biHeight;

	uint8_t * bmpPixels = new uint8_t[bmiHeader.biSizeImage];

	in.seekg(bmHeader.bfOffBits);
	in.read((char*)bmpPixels, bmiHeader.biSizeImage);

	if (in.bad())
	{
		printf("Error [%d] reading file [%s]", errno, filename);
		delete [] bmpPixels;
		return false;
	}

	const int rowWidth = width*4;

	uint8_t * pixels = new uint8_t[height*rowWidth];

	const int bmpRowWidth = (width*3 + 3) & (~3);

	uint8_t transColor[3] = {
		bmpPixels[(height-1)*bmpRowWidth + 2],
		bmpPixels[(height-1)*bmpRowWidth + 1],
		bmpPixels[(height-1)*bmpRowWidth + 0]
	};

	for (unsigned int row = 0; row < height; ++row)
	{
		for (unsigned int col = 0; col < width; ++col)
		{
			pixels[row * rowWidth + col*4 + 0] = bmpPixels[(height-row-1)*bmpRowWidth + col*3 + 2];
			pixels[row * rowWidth + col*4 + 1] = bmpPixels[(height-row-1)*bmpRowWidth + col*3 + 1];
			pixels[row * rowWidth + col*4 + 2] = bmpPixels[(height-row-1)*bmpRowWidth + col*3 + 0];

			if (transparent &&
					pixels[row * rowWidth + col*4 + 0] == transColor[0] &&
					pixels[row * rowWidth + col*4 + 1] == transColor[1] &&
					pixels[row * rowWidth + col*4 + 2] == transColor[2]
					)
			{
				pixels[row * rowWidth + col*4 + 3] = 0;
			}
			else
			{
				pixels[row * rowWidth + col*4 + 3] = 0xff;
			}
		}
	}

	delete [] bmpPixels;

	/*******************GENERATING TEXTURES*******************/
	GLuint texture;

	glGenTextures(1, &texture);             // Generate a texture
	glBindTexture(GL_TEXTURE_2D, texture); // Bind that texture temporarily

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	 
	// Create the texture. We get the offsets from the image, then we use it with the image's
	// pixel data to create it.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	// Output a successful message
	DBG_PRINT("Texture from [%s] (%u x %u) successfully loaded (tex %u).\n", filename, width, height, texture);

	delete [] pixels;

	m_Texture = texture;
	m_Initialized = true;
	m_Width = width;
	m_Height = height;

	return true;
}

void Image::Draw(double x, double y) const
{
	DrawTilted(x+m_Width/2, y+m_Height/2, m_Width, m_Height, 0);
}

void Image::DrawTilted(double x, double y, double width, double height, double angle) const
{
	if( !g_GraphEnabled ) return;

	if( !m_Initialized ) return;

	DBG_PRINT("Image::DrawTilted (tex %u, size %u, %u) %.1f %.1f %.1f %.1f %.1f\n", m_Texture, m_Width, m_Height, x, y, width, height, angle);

	glBindTexture(GL_TEXTURE_2D, m_Texture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();

	glTranslated(x,y,0.0);
	glRotated(angle, 0.0, 0.0, -1.0);

	glBegin(GL_POLYGON);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	glTexCoord2f(0.0f, 0.0f);
	glVertex2d(-width/2, -height/2);

	glTexCoord2f(1.0f, 0.0f);
	glVertex2d(width/2, -height/2);

	glTexCoord2f(1.0f, 1.0f);
	glVertex2d(width/2, height/2);

	glTexCoord2f(0.0f, 1.0f);
	glVertex2d(-width/2, height/2);

	glEnd();

	glPopMatrix();

	glDisable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void DrawImage(const Image& image, short x, short y)
{
	image.Draw(x,y);
}

void DrawImageTilted(const Image& image, short x, short y, short width, short height, short angle)
{
	image.DrawTilted(x,y, width, height, angle);
}

bool LoadBMPImage(Image& image, const char* filename)
{
	return image.LoadBMP(filename, false);
}

bool LoadBMPImageTransparent(Image& image, const char* filename)
{
	return image.LoadBMP(filename, true);
}

} // namespace Graph