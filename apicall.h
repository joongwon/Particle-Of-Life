#pragma once

#include <string>
#include <vector>

namespace joongwon {
    void showErrorMessageBox(const std::string &message);
    std::string getSaveFileName(const char *filter);
    std::string getOpenFileName(const char *filter);
}
