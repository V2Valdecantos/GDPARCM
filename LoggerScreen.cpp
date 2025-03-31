#include "LoggerScreen.h"
#include "imgui.h"

namespace GDEngine
{
    std::vector<std::string> LoggerScreen::logs;

    LoggerScreen::LoggerScreen() : UIScreen("LoggerScreen")
    {
    }

    LoggerScreen::~LoggerScreen()
    {
    }

    void LoggerScreen::addLog(const std::string& message)
    {
        logs.push_back(message);
    }

    void LoggerScreen::addError(const std::string& message)
    {
        logs.push_back(message);
    }

    void LoggerScreen::clearLogs()
    {
        logs.clear();
    }

    void LoggerScreen::draw()
    {
        ImGui::Begin("Logger", &isActive);

        if (ImGui::Button("Clear Logs"))
        {
            clearLogs();
        }

        ImGui::Separator();

        // Display logs
        ImGui::BeginChild("LogDisplay", ImVec2(0, -ImGui::GetTextLineHeightWithSpacing()), true);
        for (const std::string& log : logs)
        {
            ImGui::TextWrapped("%s", log.c_str());
        }
        ImGui::EndChild();

        ImGui::End();
    }
}
