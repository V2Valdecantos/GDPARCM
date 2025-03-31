#pragma once

#include "UIScreen.h"
#include <vector>
#include <string>

namespace GDEngine
{
    class LoggerScreen : public UIScreen
    {
    public:
        LoggerScreen();
        ~LoggerScreen();

        static void addLog(const std::string& message);
        static void addError(const std::string& message);
        static void clearLogs();

        void draw() override;

    private:
        static std::vector<std::string> logs;
    };
}
