#include <iostream>
#include <windows.h>
#include "Application.h"

int main(int argv, char *args[]) {
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	Application app;
	app.Update();
	return 0;
}