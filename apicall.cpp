#include "apicall.h"
#include <Windows.h>
#include <algorithm>
#include <numeric>

void joongwon::showErrorMessageBox(const std::string & message)
{
    MessageBoxA(NULL, message.c_str(), "Error", MB_OK | MB_ICONERROR);
}

static void merge_extensions(const std::vector<std::string> &extension, char **output_, int *size) {
    *size = 0;
    for (auto &s : extension)
        *size += s.length();
    char *output = *output_ = new char[*size + extension.size() + 1];
    for (auto &s : extension) {
        std::copy(s.begin(), s.end(), output);
        output += s.length();
        *(output++) = '\0';
    }
    *output = '\0';
}

std::string joongwon::getSaveFileName(const char *filter)
{
    OPENFILENAMEA ofn;
    char file_name[256] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = file_name;
    ofn.nMaxFile = sizeof(file_name);
    ofn.lpstrFilter = filter;
    if (GetSaveFileNameA(&ofn) == TRUE)
        return file_name;
    else
        return "";
}

std::string joongwon::getOpenFileName(const char *filter)
{
    OPENFILENAMEA ofn;
    char file_name[256] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = file_name;
    ofn.nMaxFile = sizeof(file_name);
    ofn.lpstrFilter = filter;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetOpenFileNameA(&ofn) == TRUE)
        return file_name;
    else
        return "";
}
