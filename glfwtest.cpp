#include <stdio.h>

#include <random>
#include <time.h>

#include "glfwbgi.h"

#include "include/GLFW/glfw3.h"

using namespace Graph;

void CursorPosHandler(int xpos, int ypos)
{
	printf("Cursor is at (%d,%d)\n", xpos, ypos);
}

void CursorEnterHandler(bool entered)
{
	printf("Cursor %s the window\n", entered ? "entered" : "left");
}

void MouseButtonHandler(Mouse::Button button, Mouse::Action act)
{
	static const char* ButtonNames[] = { "Left", "Right", "Middle", "Additional1", "Additional2", "Additional3", "Additional4", "Additional5" };

	printf("%s mouse button %s\n",
		ButtonNames[(int)button],
		act == Mouse::Action::Pressed ? "pressed" : "released"
	);
}

void ScrollHandler (int xoffset, int yoffset)
{
	printf("Mouse scrolled by (%d, %d)\n", xoffset, yoffset);
}

int main()
{
	// Створення графічного вікна розміром 800 х 600 пікселей
	if (!Graph::InitGraph(800, 600, "Some Title: Press Q to quit"))
	{
		printf("Graphics could not be initialized");
		return 0;
	}

	// Очистка вікна і заливка заданим кольором
	ClearDevice(Color::Aquamarine);

	// Заповнений (суцільно) прямокутник оранжевого кольору
	FillRectangle(10, 20, 100, 350, Color::Orange);

	// Прямокутник (контур) червоного кольору
	DrawRectangle(10, 20, 100, 350, Color::Red);

	DrawRectangle(300, 300, 350, 350, Color::Olive);

	FillRectangle(400, 400, 450, 450, Color::Black);

	// Товщина ліний в пікселях (впливає лише на наступні команди)
	SetLineWidth(1.7f);

	// Еліпси (контури) зеленого кольору
	DrawEllipseArc(200, 120, 15, 10, 10, 250, Color::Green);
	DrawEllipseSector(200, 220, 15, 10, 10, 250, Color::Green);	
	DrawEllipseChord(200, 320, 15, 10, 10, 250, Color::Green);

	// Еліпси (залиті)
	FillEllipseChord(400, 120, 15, 10, 0, 360, Color::Lavender);
	FillEllipseSector(400, 220, 15, 10, 10, 250, Color::Lavender);
	FillEllipseChord(400, 320, 15, 10, 10, 250, Color::Lavender);

	// Підготовка масиву точок п'ятикутника
	Point polyPoints[5];
	polyPoints[0].x = 500;
	polyPoints[0].y = 100;
	polyPoints[1].x = 510;
	polyPoints[1].y = 90;
	polyPoints[2].x = 540;
	polyPoints[2].y = 100;
	polyPoints[3].x = 580;
	polyPoints[3].y = 120;
	polyPoints[4].x = 500;
	polyPoints[4].y = 130;

	// П'ятикутник залитий чорним
	FillPoly(polyPoints, 5, Color::Black);
	// Контур п'ятикутника жовтого кольору
	DrawPoly(polyPoints, 5, Color::Yellow);
	
	polyPoints[0].x = 500;
	polyPoints[0].y = 100 + 100;
	polyPoints[1].x = 510;
	polyPoints[1].y = 90 + 100;
	polyPoints[2].x = 540;
	polyPoints[2].y = 100 + 100;
	polyPoints[3].x = 580;
	polyPoints[3].y = 120 + 100;
	polyPoints[4].x = 500;
	polyPoints[4].y = 130 + 100;
	// Ламана лінія з 4 відрізків (5 точок)
	DrawPolyLine(polyPoints, 5, Color::Asparagus);

	// Лінія червоного кольору				   R   G  B
	DrawLine(550, 400, 570, 400, GetColor(255, 0, 0));

	// Лінія синього кольору
	DrawLine(580, 400, 580, 440, Color::Blue);

	// Лінія зеленого кольору
	DrawLine(580, 450, 540, 460, Color::Green);

	// Текст
	OutText(10, 400, "the quick brown fox jumps over the lazy dog 1234567890", Color::White);
	OutText(10, 500, "the quick brown fox jumps over the lazy dog 1234567890", Color::Red, 10);

	std::string s = "std test";
	OutText(10, 550, s, Color::Red, 5);

	OutText(10, 600, 'S', Color::Blue, 12);

	// Спрайт
	Graph::Image image1;
	if (LoadBMPImage(image1, "mario.bmp"))
	{
		DrawImage(image1, 240, 120);
	}

	// Псевдо-прозорий нахилений спрайт
	Graph::Image image2;
	if (LoadBMPImageTransparent(image2, "mario.bmp"))
	{
		DrawImageTilted(image2, 640,120, 40, 60, 30);

		DrawImageTilted(image2, 640,220, 40, 60, 70);
	}

	// Прозорий нахилений спрайт
	Graph::Image image3;
	if (image3.LoadPNG("mario.png"))
	{
		DrawImageTilted(image3, 750, 120, 40, 60, -30);

		DrawImageTilted(image3, 750, 220, 40, 60, -70);
	}

	// "зміна буферів"
	// Тільки після цієї команди все намальоване з'явиться на екрані
	SwapBuffers();

	// Реєстрація колбеків для миші
	Mouse::SetButtonCallback(MouseButtonHandler);
	Mouse::SetScrollButtonCallback(ScrollHandler);
	Mouse::SetCursorPosCallback(CursorPosHandler);
	Mouse::SetCursorEnterCallback(CursorEnterHandler);

	// Нормальний режим курсору
	Mouse::SetCursorMode(Mouse::CursorMode::Normal);

	// Цикл очікування натискання клавіші "Q"
	while (! Graph::IsKeyPressed( Graph::VK_Q))
	{
		// Перевірка натисненої клавіші
		if (Mouse::IsButtonPressed(Mouse::Button::Left))
		{
			printf("(poll) Left button pressed\n");
		}

		// Отримання поточних координат курсору
		auto pos = Mouse::GetCursorPos();

		// Зміна курсору в залежності від положення
		if (pos.x < 200 && pos.y < 200)
		{
			Mouse::SetCursor(Mouse::CursorType::Crosshair);
		}
		else
		{
			Mouse::SetCursor(Mouse::CursorType::Arrow);
		}

		// Затримка 10 мс (обов'язково використовувати саме її, а не системні функції)
		Graph::Delay(10);

		// Ця умова зпрацює якщо було натиснено хрестик на вікні
		if (Graph::ShouldClose())
			break;
	}

	// Коректне закриття графічного вікна
	Graph::CloseGraph();
	
	return 0;
}

