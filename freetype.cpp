#include "freetype.h"

#include "glfw\glfw3.h"
#include <gl\GL.h>

#include "ft2build.h"
#include FT_FREETYPE_H
//#include <freetype/freetype.h>


#include <vector>
#include <iostream>

namespace Graph
{
	void* Font::libhandle = nullptr;

	bool Font::Init()
	{
		int error = FT_Init_FreeType((FT_Library*)&Font::libhandle);
		if (error)
		{
			std::cerr << "Failed to init FTLib: " << error << std::endl;
			return false;
		}
		std::cout << "FreeTypeLib initialized\n";
		return true;
	}

	Font::Font(const std::string& path)
		: handle(nullptr)
	{
		FT_Face face;

		auto error = FT_New_Face((FT_Library)libhandle,
			path.c_str(),
			0,
			&face);
		if (error == FT_Err_Unknown_File_Format)
		{
			std::cerr << path << ": font format is unsupported: " << error << std::endl;
			throw std::runtime_error("font format is unsupported");
		}
		else if (error)
		{
			std::cerr << path << ": font file open error: " << error << std::endl;
			throw std::runtime_error("file open error");
		}
		
		std::cout << path << ": Font loaded" << std::endl;
		handle = (void*)face;
	}

	Font::Font(Font&& other)
	{
		this->handle = other.handle;
		other.handle = nullptr;
	}

	Font::~Font()
	{
		if (handle)
		{
			FT_Done_Face((FT_Face)handle);
		}
	}

	const void* Font::operator()() const
	{
		return handle;
	}

	void DrawChar(FT_Bitmap* bitmap, float x, float y, unsigned long color)
	{
		GLuint texture;

		glGenTextures(1, &texture);             // Generate a texture
		glBindTexture(GL_TEXTURE_2D, texture); // Bind that texture temporarily

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Create the texture. We get the offsets from the image, then we use it with the image's
		// pixel data to create it.

		const unsigned char r = (unsigned char)color;
		const unsigned char g = (unsigned char)(color >> 8);
		const unsigned char b = (unsigned char)(color >> 16);

		const auto total_pixels = bitmap->width * bitmap->rows;
		std::vector<unsigned char> pixels(total_pixels * 4);
		for (size_t i = 0; i < total_pixels; ++i)
		{
			pixels[4 * i + 0] = r;// -bitmap->buffer[i];
			pixels[4 * i + 1] = g;// -bitmap->buffer[i];
			pixels[4 * i + 2] = b;// -bitmap->buffer[i];
			pixels[4 * i + 3] = bitmap->buffer[i];
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap->width, bitmap->rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap->width, bitmap->rows, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, bitmap->buffer);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPushMatrix();

		glTranslatef(x, y, 0.0);
		//glRotated(angle, 0.0, 0.0, -1.0);

		glBegin(GL_POLYGON);

		glColor4f(1.0, 1.0, 1.0, 1.0);

		glTexCoord2f(0.0f, 0.0f);
		glVertex2d(0.0, 0.0);


		glTexCoord2f(1.0f, 0.0f);
		glVertex2d(bitmap->width, 0.0);

		glTexCoord2f(1.0f, 1.0f);
		glVertex2d(bitmap->width, bitmap->rows);

		glTexCoord2f(0.0f, 1.0f);
		glVertex2d(0, bitmap->rows);

		glEnd();

		glPopMatrix();

		glDisable(GL_BLEND);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Font::DrawText(unsigned int font_size,  float pen_x, float pen_y, const std::string& text, unsigned long color) const
	{
		const FT_Face face = (FT_Face)handle;

		int error = 0;

		error = FT_Set_Pixel_Sizes(
			face,        /* handle to face object */
			0,           /* pixel_width           */
			font_size);  /* pixel_height          */
		if (error)
		{
			std::cerr << "set pixel size error: " << error << std::endl;
			return;
		}

		const FT_GlyphSlot  slot = face->glyph;

		for (int n = 0; n < text.length(); n++)
		{
			/* load glyph image into the slot (erase previous one) */
			error = FT_Load_Char(face, text[n], FT_LOAD_RENDER);
			if (error)
			{
				std::cerr << "FT2: error loading char '" << text[n] << "' from font" << std::endl;
				continue;  /* ignore errors */
			}

			/* now, draw to our target surface */
			DrawChar(&slot->bitmap,
				pen_x + slot->bitmap_left,
				pen_y - slot->bitmap_top,
				color
				);

			/* increment pen position */
			pen_x += slot->advance.x >> 6;
		}
	}

	void Font::DrawText(unsigned int font_size, float pen_x, float pen_y, const std::wstring& text, unsigned long color) const
	{
		const FT_Face face = (FT_Face)handle;

		int error = 0;

		const FT_GlyphSlot  slot = face->glyph;

		for (int n = 0; n < text.length(); n++)
		{
			/* load glyph image into the slot (erase previous one) */
			error = FT_Load_Char(face, text[n], FT_LOAD_RENDER);
			if (error)
			{
				std::cerr << "FT2: error loading char '" << text[n] << "' from font" << std::endl;
				continue;  /* ignore errors */
			}

			/* now, draw to our target surface */
			DrawChar(&slot->bitmap,
				pen_x + slot->bitmap_left,
				pen_y - slot->bitmap_top,
				color);

			/* increment pen position */
			pen_x += slot->advance.x >> 6;
		}
	}
}
