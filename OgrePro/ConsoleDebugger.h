#pragma once

// ����̨������
namespace ConsoleDebugger
{
	void OGREPRO_API allocConsole();
	void OGREPRO_API freeConsole();
	void OGREPRO_API print(const char *fmt, ...);
	void OGREPRO_API printStr(const char *str);
};
