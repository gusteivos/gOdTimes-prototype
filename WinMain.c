#include "main.h"


#ifdef _WIN32

#ifndef SDL_MAIN_HANDLED

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);
    int main_return = main(__argc, __argv);
    return main_return;
}

#endif

#endif

__attribute((unused))
static void WinMain_stub(void)
{
    return;
}
