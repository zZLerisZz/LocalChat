#include "Application.h"
#include <windows.h>

int main( int argc, char* args[] ) {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    Application *app = new Application();
    app->Update();
    delete app;
    return 0;
}