#pragma once
#include <string>

namespace Graph
{
	class Font
	{
	public:
		Font(const std::string& path);
		Font(Font&&);
		~Font();

		const void* operator()() const;

		void DrawText(unsigned int font_size, float x, float y, const std::string& text, unsigned long color) const;
		void DrawText(unsigned int font_size, float x, float y, const std::wstring& text, unsigned long color) const;

		static bool Init();

	private:
		Font() = delete;
		Font(const Font&) = delete;
		
		Font& operator=(const Font&) = delete;
		Font& operator=(Font&&) = delete;

	private:
		void* handle;

		static void* libhandle;
	};


}


