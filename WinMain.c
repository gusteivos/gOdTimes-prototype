
#ifdef _WIN32

#ifndef SDL_MAIN_HANDLED

extern int main(int argc, char *argv[]);

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    return main(__argc, __argv);

}

#endif

#endif

__attribute((unused))
static void WinMain_stub(void)
{
    return;
}
