#pragma once

#include <Windows.h>
#include <string>
#include <iostream>
#include <exception>
#include <stdexcept>
#include "LoggerScreen.h"

#define DEBUG_LOGS true
#define ERROR_LOGS true
#define SUCCESS_LOGS false

namespace GDEngine
{
    class Logger
    {
    public:
        template <class T>
        static void log(T* sender, const std::string& message)
        {
            if (!DEBUG_LOGS)
                return;

            const std::string logMessage = "[" + getType(sender) + "]: " + message;
            LoggerScreen::addLog(logMessage);
            std::cout << logMessage << std::endl;
        }

        static void log(const std::string& message)
        {
            if (!DEBUG_LOGS)
                return;

            LoggerScreen::addLog("[DEBUG]: " + message);
            std::cout << "[DEBUG]: " << message << std::endl;
        }

        static void log(const std::wstring& message)
        {
            if (!DEBUG_LOGS)
                return;

            std::string convertedMessage = wstringToString(message);
            LoggerScreen::addLog("[DEBUG]: " + convertedMessage);
            std::cout << convertedMessage << std::endl;
        }

        template <class T>
        static bool log(T* sender, const HRESULT result)
        {
            if (FAILED(result))
            {
                const std::string message = std::system_category().message(result);
                log(sender, message);
                return false;
            }

            if (SUCCESS_LOGS)
                log(sender, "Operation was successful.");

            return true;
        }

        template <class T>
        static void error(T* sender, const std::string& msg)
        {
            if (!ERROR_LOGS)
                return;

            const std::string errorMessage = "[" + getType(sender) + " ERROR]: " + msg;
            LoggerScreen::addError(errorMessage);
            std::cout << errorMessage << std::endl;
        }

        static void throw_exception(const std::string& msg)
        {
            LoggerScreen::addError("[EXCEPTION]: " + msg);
            std::cout << "[EXCEPTION]: " << msg << std::endl;
            throw std::exception(msg.c_str());
        }

        static void out_of_range(const std::string& msg)
        {
            LoggerScreen::addError("[OUT OF RANGE]: " + msg);
            std::cout << "[OUT OF RANGE]: " << msg << std::endl;
            throw std::out_of_range(msg.c_str());
        }

    private:
        static std::string wstringToString(const std::wstring& wstr)
        {
            if (wstr.empty()) return std::string();

            int sizeRequired = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
            if (sizeRequired <= 0) return std::string();

            std::string str(sizeRequired - 1, '\0'); // Exclude the null terminator
            WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], sizeRequired, nullptr, nullptr);

            return str;
        }

        template <class T>
        static std::string getType(T* type)
        {
            return typeid(*type).name();
        }
    };
}
