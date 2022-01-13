#include "apicall.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <algorithm>
#include <numeric>
#include <Windows.h>
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
#elif __linux__

#include <cstdio>
#include <cstdlib>
#include <iostream>

std::string joongwon::getOpenFileName(const char *filter) {
    auto f = popen("zenity --file-selection", "r");
    char filename[1024];
    fgets(filename, 1024, f);
    int ret = pclose(f);
    std::cout << "filename: " << filename << std::endl;
    if (ret == 0)
        return filename;
    else
        return "";
}

std::string joongwon::getSaveFileName(const char *filter) {
    auto f = popen("zenity --file-selection --save", "r");
    char filename[1024];
    fgets(filename, 1024, f);
    int ret = pclose(f);
    std::cout << "filename: " << filename << std::endl;
    if (ret == 0)
        return filename;
    else
        return "";
}

void joongwon::showErrorMessageBox(const std::string &message) {
    auto command = "zenity --error --text \"" + message + "\"";
    std::system(command.c_str());
}

#else
#error "unknown compiler"
#endif