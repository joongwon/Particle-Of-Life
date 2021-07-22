#include "myutil.h"
#include <Windows.h>

void joongwon::showErrorMessageBox(const std::string & message)
{
    MessageBoxA(NULL, message.c_str(), "Error", MB_OK | MB_ICONERROR);
}