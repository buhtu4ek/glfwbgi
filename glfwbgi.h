#pragma once

#include <string>

namespace Graph
{
// Повертає колір, зібраний з трьох компонент RGB.
// Кожна компонента може бути у діапазоні [0..255]
unsigned long GetColor(unsigned char r, unsigned char g, unsigned char b);

// структура для опису точок для малювання багатокутників
typedef struct  
{
	int x;
	int y;
} Point;

class Image;

//
// Функції для роботи з графічним вікном
//

// Створює графічне вікно розміром width х height, заголовок вікна - title
// повертає true якщо все добре
// якщо не вийшло - повертає false
bool InitGraph(int width, int height, const char * title);

// Повертає, чи була натиснена кнопка закриття вікна (хрестик)
bool ShouldClose();

// Коректно закриває графічне вікно
void CloseGraph();

//
// Процедуры для роботи з графічними буферами
//

// Очищує буфер малювання та заливає його заданимм кольором
void ClearDevice(unsigned long color);

// Міняє місцями буфер малювання та буфер екрану (виводить намальоване на екран)
void SwapBuffers();

//
// Процедури для роботи з клавіатурою
//

// Повертає, чи натиснена клавіша із заданим кодом (коди нижче) 
bool IsKeyPressed(unsigned short vkey);

// Повертає, чи натиснена будь-яка клавіша
bool KeyPressed();

// Чекає натиснення клавіші і повертає натиснений символ
// (не реагує на спец. клавіші типи "Ctrl" "Shift")
char ReadKey();

// Призупиняє виконання програми на задану кількість мілісекунд
void Delay(long ms);

//
// Процедуры малювання
// (все малювання відбувається на буфері малювання і одразу на екран не виводиться)
//

// Встановлює товщину лінії (у пікселях)
// для всіх наступних процедур малювання ліній
void SetLineWidth(float thickness);

// Малює лінію з координат (x1,y1) у координати (x2,y2) кольором color
void DrawLine(short x1, short y1, short x2, short y2, unsigned long color);

// Малює контур бгатокутника кольором color
// Координати вершин лежать в массиві points, кількість - count
void DrawPoly(Point * points, unsigned short count, unsigned long color);

// Малює ламану лінію кольором color
// Координати точок лежать в массиві points, кількість - count
void DrawPolyLine(Point * points, unsigned short count, unsigned long color);

// Малює дугу эліпсу кольором color
// Координати центру - (x,y) горизонтальний радіус - xradius, вертикальний радіус - yradius
// початковий кут дуги - startAngle, кінцевий кут - stopAngle
// (кути в градусах, рахуються проти годинникової стрілки, 0 градусів - це рівно праворуч)
void DrawEllipseArc(short x, short y, unsigned short xradius, unsigned short yradius, short startAngle, short stopAngle, unsigned long color);

// Малює контур сектору еліпсу кольором color
// Координати центру - (x,y) горизонтальний радіус - xradius, вертикальний радіус - yradius
// початковий кут дуги - startAngle, кінцевий кут - stopAngle
// (кути в градусах, рахуються проти годинникової стрілки, 0 градусів - це рівно праворуч)
void DrawEllipseSector(short x, short y, unsigned short xradius, unsigned short yradius, short startAngle, short stopAngle, unsigned long color);

// Малює контур сегменту еліпсу кольором color
// Координати центру - (x,y) горизонтальний радіус - xradius, вертикальний радіус - yradius
// початковий кут дуги - startAngle, кінцевий кут - stopAngle
// (кути в градусах, рахуються проти годинникової стрілки, 0 градусів - це рівно праворуч)
void DrawEllipseChord(short x, short y, unsigned short xradius, unsigned short yradius, short startAngle, short stopAngle, unsigned long color);

// Малює контур прямокутника кольором color
// Верхній лівий кут - (x1,y1), нижній правий кут - (x2,y2)
void DrawRectangle(short x1, short y1, short x2, short y2, unsigned long color);

// Малює багатокутник залитий кольором color
// Координати вершин лежать в массиві points, кількість - count
void FillPoly(Point * points, unsigned short count, unsigned long color);

// Малює сектор еліпсу залитий кольором color
// Координати центру - (x,y) горизонтальний радіус - xradius, вертикальний радіус - yradius
// початковий кут дуги - startAngle, кінцевий кут - stopAngle
// (кути в градусах, рахуються проти годинникової стрілки, 0 градусів - це рівно праворуч)
void FillEllipseSector(short x, short y, unsigned short xradius, unsigned short yradius, short startAngle, short stopAngle, unsigned long color);

// Малює сегмент еліпсу залитий кольором color
// Координати центру - (x,y) горизонтальний радіус - xradius, вертикальний радіус - yradius
// початковий кут дуги - startAngle, кінцевий кут - stopAngle
// (кути в градусах, рахуються проти годинникової стрілки, 0 градусів - це рівно праворуч)
void FillEllipseChord(short x, short y, unsigned short xradius, unsigned short yradius, short startAngle, short stopAngle, unsigned long color);

// Малює прямокутник залитий кольором color
// Верхній лівий кут - (x1,y1), нижній правий кут - (x2,y2)
void FillRectangle(short x1, short y1, short x2, short y2, unsigned long color);

bool LoadBMPImage(Image& image, const char* filename);

bool LoadBMPImageTransparent(Image& image, const char* filename);

void DrawImage(const Image& image, short x, short y);

void DrawImageTilted(const Image& image, short x, short y, short width, short height, short angle);

class Image
{
public:
	Image();
	~Image();

	void Draw(double x, double y) const;

	void DrawTilted(double x, double y, double width, double height, double angle) const;

	bool LoadBMP(const char * filename, bool transparent = false);

private:

	unsigned int m_Texture;

	unsigned int m_Width;
	unsigned int m_Height;

	bool m_Initialized;
};

namespace Color
{
//
// Color constants
// Константи кольору
//
const unsigned long AliceBlue = 0xFFF8F0;
const unsigned long AlizarinCrimson = 0x3626E3;
const unsigned long Amber = 0x00BFFF;
const unsigned long Amethyst = 0xCC6699;
const unsigned long AntiqueWhite = 0xD7EBFA;
const unsigned long Aquamarine = 0xD4FF7F;
const unsigned long Asparagus = 0x5BA07B;
const unsigned long Azure = 0xFFFFF0;
const unsigned long Beige = 0xDCF5F5;
const unsigned long Bisque = 0xC4E4FF;
const unsigned long Bistre = 0x1F2B3D;
const unsigned long BitterLemon = 0x0DE0CA;
const unsigned long Black = 0x000000;
const unsigned long BlanchedAlmond = 0xCDEBFF;
const unsigned long BlazeOrange = 0x0099FF;
const unsigned long Blue = 0xFF0000;
const unsigned long BlueViolet = 0xE22B8A;
const unsigned long BondiBlue = 0xB69500;
const unsigned long Brass = 0x42A6B5;
const unsigned long BrightGreen = 0x00FF66;
const unsigned long BrightTurquoise = 0xDEE808;
const unsigned long BrightViolet = 0xCD00CD;
const unsigned long Bronze = 0x327FCD;
const unsigned long Brown = 0x2A2AA5;
const unsigned long Buff = 0x82DCF0;
const unsigned long Burgundy = 0x200090;
const unsigned long BurlyWood = 0x87B8DE;
const unsigned long BurntOrange = 0x0055CC;
const unsigned long BurntSienna = 0x5174E9;
const unsigned long BurntUmber = 0x24338A;
const unsigned long CadetBlue = 0xA09E5F;
const unsigned long CamouflageGreen = 0x6B8678;
const unsigned long Cardinal = 0x3A1EC4;
const unsigned long Carmine = 0x180096;
const unsigned long Carrot = 0x2191ED;
const unsigned long Casper = 0xD1BEAD;
const unsigned long Celadon = 0xAFE1AC;
const unsigned long Cerise = 0x6331DE;
const unsigned long Cerulean = 0xA77B00;
const unsigned long CeruleanBlue = 0xBE522A;
const unsigned long Chartreuse = 0x00FF7F;
const unsigned long Chocolate = 0x1E69D2;
const unsigned long Cinnamon = 0x003F7B;
const unsigned long Cobalt = 0xAB4700;
const unsigned long Copper = 0x3373B8;
const unsigned long Coral = 0x507FFF;
const unsigned long Corn = 0x5DECFB;
const unsigned long CornflowerBlue = 0xED9564;
const unsigned long Cornsilk = 0xDCF8FF;
const unsigned long Cream = 0xD0FDFF;
const unsigned long Crimson = 0x3C14DC;
const unsigned long Cyan = 0xFFFF00;
const unsigned long DarkBlue = 0x8B0000;
const unsigned long DarkBrown = 0x214365;
const unsigned long DarkCerulean = 0x7E4508;
const unsigned long DarkChestnut = 0x606998;
const unsigned long DarkCoral = 0x455BCD;
const unsigned long DarkCyan = 0x8B8B00;
const unsigned long DarkGoldenrod = 0x0B86B8;
const unsigned long DarkGray = 0x545454;
const unsigned long DarkGreen = 0x006400;
const unsigned long DarkIndigo = 0x620031;
const unsigned long DarkKhaki = 0x6BB7BD;
const unsigned long DarkMagenta = 0x8B008B;
const unsigned long DarkOlive = 0x326855;
const unsigned long DarkOliveGreen = 0x2F6B55;
const unsigned long DarkOrange = 0x008CFF;
const unsigned long DarkOrchid = 0xCC3299;
const unsigned long DarkPastelGreen = 0x3CC003;
const unsigned long DarkPink = 0x8054E7;
const unsigned long DarkRed = 0x00008B;
const unsigned long DarkSalmon = 0x7A96E9;
const unsigned long DarkScarlet = 0x190356;
const unsigned long DarkSeaGreen = 0x8FBC8F;
const unsigned long DarkSlateBlue = 0x8B3D48;
const unsigned long DarkSlateGray = 0x4F4F2F;
const unsigned long DarkSpringGreen = 0x457217;
const unsigned long DarkTan = 0x518191;
const unsigned long DarkTangerine = 0x12A8FF;
const unsigned long DarkTeaGreen = 0xADDBBA;
const unsigned long DarkTerraCotta = 0x5C4ECC;
const unsigned long DarkTurquoise = 0xD1CE00;
const unsigned long DarkViolet = 0xD30094;
const unsigned long DeepPink = 0x9314FF;
const unsigned long DeepSkyBlue = 0xFFBF00;
const unsigned long Denim = 0xBD6015;
const unsigned long DimGray = 0x696969;
const unsigned long DodgerBlue = 0xFF901E;
const unsigned long Emerald = 0x78C850;
const unsigned long Eggplant = 0x660099;
const unsigned long FernGreen = 0x42794F;
const unsigned long FireBrick = 0x2222B2;
const unsigned long Flax = 0x82DCEE;
const unsigned long FloralWhite = 0xF0FAFF;
const unsigned long ForestGreen = 0x228B22;
const unsigned long Fractal = 0x808080;
const unsigned long Fuchsia = 0xA100F4;
const unsigned long Gainsboro = 0xDCDCDC;
const unsigned long Gamboge = 0x0F9BE4;
const unsigned long GhostWhite = 0xFFF8F8;
const unsigned long Gold = 0x00D7FF;
const unsigned long Goldenrod = 0x20A5DA;
const unsigned long Gray = 0x7E7E7E;
const unsigned long GrayAsparagus = 0x455946;
const unsigned long GrayTeaGreen = 0xBADACA;
const unsigned long Green = 0x008000;
const unsigned long GreenYellow = 0x2FFFAD;
const unsigned long Heliotrope = 0xFF73DF;
const unsigned long Honeydew = 0xF0FFF0;
const unsigned long HotPink = 0xB469FF;
const unsigned long IndianRed = 0x5C5CCD;
const unsigned long Indigo = 0x82004B;
const unsigned long InternationalKleinBlue = 0xA72F00;
const unsigned long InternationalOrange = 0x004FFF;
const unsigned long Ivory = 0xF0FFFF;
const unsigned long Jade = 0x6BA800;
const unsigned long Khaki = 0x8CE6F0;
const unsigned long Lavender = 0xFAE6E6;
const unsigned long LavenderBlush = 0xF5F0FF;
const unsigned long LawnGreen = 0x00FC7C;
const unsigned long Lemon = 0x10E9FD;
const unsigned long LemonChiffon = 0xCDFAFF;
const unsigned long LightBlue = 0xE6D8AD;
const unsigned long LightBrown = 0x8CB4D2;
const unsigned long LightCoral = 0x8080F0;
const unsigned long LightCyan = 0xFFFFE0;
const unsigned long LightGoldenrodYellow = 0xD2FAFA;
const unsigned long LightGray = 0xA8A8A8;
const unsigned long LightGreen = 0x90EE90;
const unsigned long LightMagenta = 0xFF80FF;
const unsigned long LightPink = 0xC1B6FF;
const unsigned long LightRed = 0x8080FF;
const unsigned long LightSalmon = 0x7AA0FF;
const unsigned long LightSeaGreen = 0xAAB220;
const unsigned long LightSkyBlue = 0xFACE87;
const unsigned long LightSlateGray = 0x998877;
const unsigned long LightSteelBlue = 0xDEC4B0;
const unsigned long LightYellow = 0xE0FFFF;
const unsigned long Lilac = 0xC8A2C8;
const unsigned long Lime = 0x00FF00;
const unsigned long LimeGreen = 0x32CD32;
const unsigned long Linen = 0xE6F0FA;
const unsigned long Magenta = 0xFF00FF;
const unsigned long Malachite = 0x51DA0B;
const unsigned long Maroon = 0x000080;
const unsigned long Mauve = 0xFFB0E0;
const unsigned long MediumAquamarine = 0xAACD66;
const unsigned long MediumBlue = 0xCD0000;
const unsigned long MediumOrchid = 0xD355BA;
const unsigned long MediumPurple = 0xDB7093;
const unsigned long MediumSeaGreen = 0x71B33C;
const unsigned long MediumSlateBlue = 0xEE687B;
const unsigned long MediumSpringGreen = 0x9AFA00;
const unsigned long MediumTurquoise = 0xCCD148;
const unsigned long MediumVioletRed = 0x8515C7;
const unsigned long MidnightBlue = 0x701919;
const unsigned long MintCream = 0xFAFFF5;
const unsigned long MistyRose = 0xE1E4FF;
const unsigned long Moccasin = 0xB5E4FF;
const unsigned long MoneyGreen = 0xC0DCC0;
const unsigned long Monza = 0x1E03C7;
const unsigned long MossGreen = 0xADDFAD;
const unsigned long MountbattenPink = 0x8D7A99;
const unsigned long Mustard = 0x58DBFF;
const unsigned long NavajoWhite = 0xADDEFF;
const unsigned long Navy = 0x800000;
const unsigned long Ochre = 0x2277CC;
const unsigned long OldGold = 0x3BB5CF;
const unsigned long OldLace = 0xE6F5FD;
const unsigned long Olive = 0x008080;
const unsigned long OliveDrab = 0x238E6B;
const unsigned long Orange = 0x00A5FF;
const unsigned long OrangeRed = 0x0045FF;
const unsigned long Orchid = 0xD670DA;
const unsigned long PaleBrown = 0x547698;
const unsigned long PaleCarmine = 0x3540AF;
const unsigned long PaleChestnut = 0xAFADDD;
const unsigned long PaleCornflowerBlue = 0xEFCDAB;
const unsigned long PaleGoldenrod = 0xAAE8EE;
const unsigned long PaleGreen = 0x98FB98;
const unsigned long PaleMagenta = 0xE584F9;
const unsigned long PaleMauve = 0x666699;
const unsigned long PalePink = 0xDDDAFA;
const unsigned long PaleSandyBrown = 0xABBDDA;
const unsigned long PaleTurquoise = 0xEEEEAF;
const unsigned long PaleVioletRed = 0x9370DB;
const unsigned long PapayaWhip = 0xD5EFFF;
const unsigned long PastelGreen = 0x77DD77;
const unsigned long PastelPink = 0xDCD1FF;
const unsigned long Peach = 0xB4E5FF;
const unsigned long PeachOrange = 0x99CCFF;
const unsigned long PeachPuff = 0xB9DAFF;
const unsigned long PeachYellow = 0xADDFFA;
const unsigned long Pear = 0x31E2D1;
const unsigned long Periwinkle = 0xFFCCCC;
const unsigned long PersianBlue = 0xFF0066;
const unsigned long Peru = 0x3F85CD;
const unsigned long PineGreen = 0x6F7901;
const unsigned long Pink = 0xCBC0FF;
const unsigned long PinkOrange = 0x6699FF;
const unsigned long Plum = 0xDDA0DD;
const unsigned long PowderBlue = 0xE6E0B0;
const unsigned long PrussianBlue = 0x533100;
const unsigned long Puce = 0x9988CC;
const unsigned long Pumpkin = 0x1875FF;
const unsigned long Purple = 0x800080;
const unsigned long RawUmber = 0x124A73;
const unsigned long Red = 0x0000FF;
const unsigned long Reef = 0xA2FFC9;
const unsigned long RobinEggBlue = 0xCCCC00;
const unsigned long RosyBrown = 0x8F8FBC;
const unsigned long RoyalBlue = 0xE16941;
const unsigned long Russet = 0x1B4680;
const unsigned long Rust = 0x0E41B7;
const unsigned long SaddleBrown = 0x13458B;
const unsigned long Saffron = 0x30C4F4;
const unsigned long Salmon = 0x7280FA;
const unsigned long SandyBrown = 0x60A4F4;
const unsigned long Sangria = 0x0A0092;
const unsigned long Sapphire = 0x672508;
const unsigned long Scarlet = 0x0024FF;
const unsigned long SchoolBusYellow = 0x00D8FF;
const unsigned long SeaGreen = 0x578B2E;
const unsigned long SeaShell = 0xEEF5FF;
const unsigned long SelectiveYellow = 0x00BAFF;
const unsigned long Sepia = 0x144270;
const unsigned long Sienna = 0x2D52A0;
const unsigned long Silver = 0xC0C0C0;
const unsigned long SkyBlue = 0xEBCE87;
const unsigned long SlateBlue = 0xCD5A6A;
const unsigned long SlateGray = 0x908070;
const unsigned long Snow = 0xFAFAFF;
const unsigned long SpringGreen = 0x7FFF00;
const unsigned long SteelBlue = 0xB48246;
const unsigned long SwampGreen = 0x8EB7AC;
const unsigned long Taupe = 0x7E98BC;
const unsigned long Tangerine = 0x00CCFF;
const unsigned long Teal = 0x808000;
const unsigned long TeaGreen = 0xC0F0D0;
const unsigned long Tenne = 0x0057CD;
const unsigned long TerraCotta = 0x5B72E2;
const unsigned long Thistle = 0xD8BFD8;
const unsigned long Tomato = 0x4763FF;
const unsigned long Turquoise = 0xD0E040;
const unsigned long Ultramarine = 0x8F0A12;
const unsigned long Vermilion = 0x004DFF;
const unsigned long Violet = 0xEE82EE;
const unsigned long VioletEggplant = 0x991199;
const unsigned long Viridian = 0x6D8240;
const unsigned long Wheat = 0xB3DEF5;
const unsigned long White = 0xFFFFFF;
const unsigned long WhiteSmoke = 0xF5F5F5;
const unsigned long Wisteria = 0xDCA0C9;
const unsigned long Yellow = 0x00FFFF;
const unsigned long YellowGreen = 0x32CD9A;
const unsigned long Zinnwaldite = 0xAFC2EB;
} //namespace Color

// Виведення тексту
void OutText(
	short startx, short starty,
	const char * text,
	unsigned long color = Color::White,
	unsigned short size = 4
	);

void OutText(
	short startx, short starty,
	const std::string &text,
	unsigned long color = Color::White,
	unsigned short size = 4
	);

void OutText(
	short startx, short starty,
	char text,
	unsigned long color = Color::White,
	unsigned short size = 4
	);

//
// Key constants
//

// Printable keys
const unsigned long GLFW_KEY_SPACE = 32;
const unsigned long GLFW_KEY_APOSTROPHE = 39;
const unsigned long GLFW_KEY_COMMA = 44;
const unsigned long GLFW_KEY_MINUS = 45;
const unsigned long GLFW_KEY_PERIOD = 46;
const unsigned long GLFW_KEY_SLASH = 47;
const unsigned long GLFW_KEY_0 = 48;
const unsigned long GLFW_KEY_1 = 49;
const unsigned long GLFW_KEY_2 = 50;
const unsigned long GLFW_KEY_3 = 51;
const unsigned long GLFW_KEY_4 = 52;
const unsigned long GLFW_KEY_5 = 53;
const unsigned long GLFW_KEY_6 = 54;
const unsigned long GLFW_KEY_7 = 55;
const unsigned long GLFW_KEY_8 = 56;
const unsigned long GLFW_KEY_9 = 57;
const unsigned long GLFW_KEY_SEMICOLON = 59;
const unsigned long GLFW_KEY_EQUAL = 61;
const unsigned long GLFW_KEY_A = 65;
const unsigned long GLFW_KEY_B = 66;
const unsigned long GLFW_KEY_C = 67;
const unsigned long GLFW_KEY_D = 68;
const unsigned long GLFW_KEY_E = 69;
const unsigned long GLFW_KEY_F = 70;
const unsigned long GLFW_KEY_G = 71;
const unsigned long GLFW_KEY_H = 72;
const unsigned long GLFW_KEY_I = 73;
const unsigned long GLFW_KEY_J = 74;
const unsigned long GLFW_KEY_K = 75;
const unsigned long GLFW_KEY_L = 76;
const unsigned long GLFW_KEY_M = 77;
const unsigned long GLFW_KEY_N = 78;
const unsigned long GLFW_KEY_O = 79;
const unsigned long GLFW_KEY_P = 80;
const unsigned long GLFW_KEY_Q = 81;
const unsigned long GLFW_KEY_R = 82;
const unsigned long GLFW_KEY_S = 83;
const unsigned long GLFW_KEY_T = 84;
const unsigned long GLFW_KEY_U = 85;
const unsigned long GLFW_KEY_V = 86;
const unsigned long GLFW_KEY_W = 87;
const unsigned long GLFW_KEY_X = 88;
const unsigned long GLFW_KEY_Y = 89;
const unsigned long GLFW_KEY_Z = 90;
const unsigned long GLFW_KEY_LEFT_BRACKET = 91;
const unsigned long GLFW_KEY_BACKSLASH = 92;
const unsigned long GLFW_KEY_RIGHT_BRACKET = 93;
const unsigned long GLFW_KEY_GRAVE_ACCENT = 96;
const unsigned long GLFW_KEY_WORLD_1 = 161;
const unsigned long GLFW_KEY_WORLD_2 = 162;

// Function keys
const unsigned long GLFW_KEY_ESCAPE = 256;
const unsigned long GLFW_KEY_ENTER = 257;
const unsigned long GLFW_KEY_TAB = 258;
const unsigned long GLFW_KEY_BACKSPACE = 259;
const unsigned long GLFW_KEY_INSERT = 260;
const unsigned long GLFW_KEY_DELETE = 261;
const unsigned long GLFW_KEY_RIGHT = 262;
const unsigned long GLFW_KEY_LEFT = 263;
const unsigned long GLFW_KEY_DOWN = 264;
const unsigned long GLFW_KEY_UP = 265;
const unsigned long GLFW_KEY_PAGE_UP = 266;
const unsigned long GLFW_KEY_PAGE_DOWN = 267;
const unsigned long GLFW_KEY_HOME = 268;
const unsigned long GLFW_KEY_END = 269;
const unsigned long GLFW_KEY_CAPS_LOCK = 280;
const unsigned long GLFW_KEY_SCROLL_LOCK = 281;
const unsigned long GLFW_KEY_NUM_LOCK = 282;
const unsigned long GLFW_KEY_PRINT_SCREEN = 283;
const unsigned long GLFW_KEY_PAUSE = 284;
const unsigned long GLFW_KEY_F1 = 290;
const unsigned long GLFW_KEY_F2 = 291;
const unsigned long GLFW_KEY_F3 = 292;
const unsigned long GLFW_KEY_F4 = 293;
const unsigned long GLFW_KEY_F5 = 294;
const unsigned long GLFW_KEY_F6 = 295;
const unsigned long GLFW_KEY_F7 = 296;
const unsigned long GLFW_KEY_F8 = 297;
const unsigned long GLFW_KEY_F9 = 298;
const unsigned long GLFW_KEY_F10 = 299;
const unsigned long GLFW_KEY_F11 = 300;
const unsigned long GLFW_KEY_F12 = 301;
const unsigned long GLFW_KEY_F13 = 302;
const unsigned long GLFW_KEY_F14 = 303;
const unsigned long GLFW_KEY_F15 = 304;
const unsigned long GLFW_KEY_F16 = 305;
const unsigned long GLFW_KEY_F17 = 306;
const unsigned long GLFW_KEY_F18 = 307;
const unsigned long GLFW_KEY_F19 = 308;
const unsigned long GLFW_KEY_F20 = 309;
const unsigned long GLFW_KEY_F21 = 310;
const unsigned long GLFW_KEY_F22 = 311;
const unsigned long GLFW_KEY_F23 = 312;
const unsigned long GLFW_KEY_F24 = 313;
const unsigned long GLFW_KEY_F25 = 314;
const unsigned long GLFW_KEY_KP_0 = 320;
const unsigned long GLFW_KEY_KP_1 = 321;
const unsigned long GLFW_KEY_KP_2 = 322;
const unsigned long GLFW_KEY_KP_3 = 323;
const unsigned long GLFW_KEY_KP_4 = 324;
const unsigned long GLFW_KEY_KP_5 = 325;
const unsigned long GLFW_KEY_KP_6 = 326;
const unsigned long GLFW_KEY_KP_7 = 327;
const unsigned long GLFW_KEY_KP_8 = 328;
const unsigned long GLFW_KEY_KP_9 = 329;
const unsigned long GLFW_KEY_KP_DECIMAL = 330;
const unsigned long GLFW_KEY_KP_DIVIDE = 331;
const unsigned long GLFW_KEY_KP_MULTIPLY = 332;
const unsigned long GLFW_KEY_KP_SUBTRACT = 333;
const unsigned long GLFW_KEY_KP_ADD = 334;
const unsigned long GLFW_KEY_KP_ENTER = 335;
const unsigned long GLFW_KEY_KP_EQUAL = 336;
const unsigned long GLFW_KEY_LEFT_SHIFT = 340;
const unsigned long GLFW_KEY_LEFT_CONTROL = 341;
const unsigned long GLFW_KEY_LEFT_ALT = 342;
const unsigned long GLFW_KEY_LEFT_SUPER = 343;
const unsigned long GLFW_KEY_RIGHT_SHIFT = 344;
const unsigned long GLFW_KEY_RIGHT_CONTROL = 345;
const unsigned long GLFW_KEY_RIGHT_ALT = 346;
const unsigned long GLFW_KEY_RIGHT_SUPER = 347;
const unsigned long GLFW_KEY_MENU = 348;

const unsigned long GLFW_KEY_LAST = GLFW_KEY_MENU;

//
// Windows virtual keys
// Коды виртуальных клавиш для процедуры IsKeyPressed()
//
const unsigned long VK_SPACE = GLFW_KEY_SPACE;
const unsigned long VK_APOSTROPHE = GLFW_KEY_APOSTROPHE;
const unsigned long VK_COMMA = GLFW_KEY_COMMA;
const unsigned long VK_MINUS = GLFW_KEY_MINUS;
const unsigned long VK_PERIOD = GLFW_KEY_PERIOD;
const unsigned long VK_SLASH = GLFW_KEY_SLASH;
const unsigned long VK_0 = GLFW_KEY_0;
const unsigned long VK_1 = GLFW_KEY_1;
const unsigned long VK_2 = GLFW_KEY_2;
const unsigned long VK_3 = GLFW_KEY_3;
const unsigned long VK_4 = GLFW_KEY_4;
const unsigned long VK_5 = GLFW_KEY_5;
const unsigned long VK_6 = GLFW_KEY_6;
const unsigned long VK_7 = GLFW_KEY_7;
const unsigned long VK_8 = GLFW_KEY_8;
const unsigned long VK_9 = GLFW_KEY_9;
const unsigned long VK_SEMICOLON = GLFW_KEY_SEMICOLON;
const unsigned long VK_EQUAL = GLFW_KEY_EQUAL;
const unsigned long VK_A = GLFW_KEY_A;
const unsigned long VK_B = GLFW_KEY_B;
const unsigned long VK_C = GLFW_KEY_C;
const unsigned long VK_D = GLFW_KEY_D;
const unsigned long VK_E = GLFW_KEY_E;
const unsigned long VK_F = GLFW_KEY_F;
const unsigned long VK_G = GLFW_KEY_G;
const unsigned long VK_H = GLFW_KEY_H;
const unsigned long VK_I = GLFW_KEY_I;
const unsigned long VK_J = GLFW_KEY_J;
const unsigned long VK_K = GLFW_KEY_K;
const unsigned long VK_L = GLFW_KEY_L;
const unsigned long VK_M = GLFW_KEY_M;
const unsigned long VK_N = GLFW_KEY_N;
const unsigned long VK_O = GLFW_KEY_O;
const unsigned long VK_P = GLFW_KEY_P;
const unsigned long VK_Q = GLFW_KEY_Q;
const unsigned long VK_R = GLFW_KEY_R;
const unsigned long VK_S = GLFW_KEY_S;
const unsigned long VK_T = GLFW_KEY_T;
const unsigned long VK_U = GLFW_KEY_U;
const unsigned long VK_V = GLFW_KEY_V;
const unsigned long VK_W = GLFW_KEY_W;
const unsigned long VK_X = GLFW_KEY_X;
const unsigned long VK_Y = GLFW_KEY_Y;
const unsigned long VK_Z = GLFW_KEY_Z;
const unsigned long VK_LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET;
const unsigned long VK_BACKSLASH = GLFW_KEY_BACKSLASH;
const unsigned long VK_RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET;
const unsigned long VK_GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT;
const unsigned long VK_WORLD_1 = GLFW_KEY_WORLD_1;
const unsigned long VK_WORLD_2 = GLFW_KEY_WORLD_2;
const unsigned long VK_ESCAPE = GLFW_KEY_ESCAPE;
const unsigned long VK_ENTER = GLFW_KEY_ENTER;
const unsigned long VK_TAB = GLFW_KEY_TAB;
const unsigned long VK_BACKSPACE = GLFW_KEY_BACKSPACE;
const unsigned long VK_INSERT = GLFW_KEY_INSERT;
const unsigned long VK_DELETE = GLFW_KEY_DELETE;
const unsigned long VK_RIGHT = GLFW_KEY_RIGHT;
const unsigned long VK_LEFT = GLFW_KEY_LEFT;
const unsigned long VK_DOWN = GLFW_KEY_DOWN;
const unsigned long VK_UP = GLFW_KEY_UP;
const unsigned long VK_PAGE_UP = GLFW_KEY_PAGE_UP;
const unsigned long VK_PAGE_DOWN = GLFW_KEY_PAGE_DOWN;
const unsigned long VK_HOME = GLFW_KEY_HOME;
const unsigned long VK_END = GLFW_KEY_END;
const unsigned long VK_CAPS_LOCK = GLFW_KEY_CAPS_LOCK;
const unsigned long VK_SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK;
const unsigned long VK_NUM_LOCK = GLFW_KEY_NUM_LOCK;
const unsigned long VK_PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN;
const unsigned long VK_PAUSE = GLFW_KEY_PAUSE;
const unsigned long VK_F1 = GLFW_KEY_F1;
const unsigned long VK_F2 = GLFW_KEY_F2;
const unsigned long VK_F3 = GLFW_KEY_F3;
const unsigned long VK_F4 = GLFW_KEY_F4;
const unsigned long VK_F5 = GLFW_KEY_F5;
const unsigned long VK_F6 = GLFW_KEY_F6;
const unsigned long VK_F7 = GLFW_KEY_F7;
const unsigned long VK_F8 = GLFW_KEY_F8;
const unsigned long VK_F9 = GLFW_KEY_F9;
const unsigned long VK_F10 = GLFW_KEY_F10;
const unsigned long VK_F11 = GLFW_KEY_F11;
const unsigned long VK_F12 = GLFW_KEY_F12;
const unsigned long VK_F13 = GLFW_KEY_F13;
const unsigned long VK_F14 = GLFW_KEY_F14;
const unsigned long VK_F15 = GLFW_KEY_F15;
const unsigned long VK_F16 = GLFW_KEY_F16;
const unsigned long VK_F17 = GLFW_KEY_F17;
const unsigned long VK_F18 = GLFW_KEY_F18;
const unsigned long VK_F19 = GLFW_KEY_F19;
const unsigned long VK_F20 = GLFW_KEY_F20;
const unsigned long VK_F21 = GLFW_KEY_F21;
const unsigned long VK_F22 = GLFW_KEY_F22;
const unsigned long VK_F23 = GLFW_KEY_F23;
const unsigned long VK_F24 = GLFW_KEY_F24;
const unsigned long VK_F25 = GLFW_KEY_F25;
const unsigned long VK_KP_0 = GLFW_KEY_KP_0;
const unsigned long VK_KP_1 = GLFW_KEY_KP_1;
const unsigned long VK_KP_2 = GLFW_KEY_KP_2;
const unsigned long VK_KP_3 = GLFW_KEY_KP_3;
const unsigned long VK_KP_4 = GLFW_KEY_KP_4;
const unsigned long VK_KP_5 = GLFW_KEY_KP_5;
const unsigned long VK_KP_6 = GLFW_KEY_KP_6;
const unsigned long VK_KP_7 = GLFW_KEY_KP_7;
const unsigned long VK_KP_8 = GLFW_KEY_KP_8;
const unsigned long VK_KP_9 = GLFW_KEY_KP_9;
const unsigned long VK_KP_DECIMAL = GLFW_KEY_KP_DECIMAL;
const unsigned long VK_KP_DIVIDE = GLFW_KEY_KP_DIVIDE;
const unsigned long VK_KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY;
const unsigned long VK_KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT;
const unsigned long VK_KP_ADD = GLFW_KEY_KP_ADD;
const unsigned long VK_KP_ENTER = GLFW_KEY_KP_ENTER;
const unsigned long VK_KP_EQUAL = GLFW_KEY_KP_EQUAL;
const unsigned long VK_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT;
const unsigned long VK_LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL;
const unsigned long VK_LEFT_ALT = GLFW_KEY_LEFT_ALT;
const unsigned long VK_LEFT_SUPER = GLFW_KEY_LEFT_SUPER;
const unsigned long VK_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT;
const unsigned long VK_RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL;
const unsigned long VK_RIGHT_ALT = GLFW_KEY_RIGHT_ALT;
const unsigned long VK_RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER;
const unsigned long VK_MENU = GLFW_KEY_MENU;


} // namespace Graph