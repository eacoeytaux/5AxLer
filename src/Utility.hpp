//
//  Utility.hpp
//  generator
//
//  Created by Ethan Coeytaux on 10/6/16.
//  Copyright © 2016 MAP MQP. All rights reserved.
//

#ifndef Utility_hpp
#define Utility_hpp

#define DEBUG_MODE
//#define PRINT_SEPERATE_LOGS
#define PRINT_LOGS_TO_CONSOLE

#define SETTINGS_JSON_FILE_PATH "./settings.json"

//hardware variables

#define THETA_MAX 0.785398163397448309616 //in radians (should be between 0-pi)

#define A_AXIS_RANGE_DEGREES 90.0
#define A_AXIS_PRECISION_DEGREES 0.1
#define A_AXIS_DISCRETE_POINTS (int)(A_AXIS_RANGE_DEGREES / A_AXIS_PRECISION_DEGREES)

#define B_AXIS_PRECISION_DEGREES 0.1
#define B_AXIS_RANGE_DEGREES 360.0
#define B_AXIS_DISCRETE_POINTS (int)(B_AXIS_RANGE_DEGREES / B_AXIS_PRECISION_DEGREES)

#define SLICE_THICKNESS 1

//end hardware variables

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <memory>
#include <string>

#include <sys/stat.h>

#include "../libs/rapidjson/document.h"

#include "Clock.hpp"

namespace mapmqp {
    enum MESSAGE_TYPE {
        INFO,
        WARNING,
        ERROR
    };

    inline bool doubleEquals(const double & d1, const double & d2, const double & tolerance = 1e-5) {
        return (fabs(d1 - d2) <= tolerance);
    }
    
    //TODO should these functions have an init function?
    
    //TODO for some stupid reason this won't link if it's in Utility.cpp
    //for the time being declaring it as inline...this should change eventually
    inline void writeLog(MESSAGE_TYPE type, const char * entry, ...) {
        static bool logInit = false;
        
        static FILE * logFile;
#ifdef PRINT_SEPERATE_LOGS
        static FILE * logInfoFile;
        static FILE * logWarningsFile;
        static FILE * logErrorsFile;
#endif
        
        if (!logInit) {
            mkdir("./logs", 0777);
            
            //open all log files
            std::string timeStr = Clock::wallTimeString("-", "_", "-");
            std::string logFilePath = "./logs/" + timeStr + "_mapmqp.log";
#ifdef PRINT_SEPERATE_LOGS
            std::string logInfoFilePath = "./logs/" + timeStr + "_mapmqp-info.log";
            std::string logWarningsFilePath = "./logs/" + timeStr + "_mapmqp-warnings.log";
            std::string logErrorsFilePath = "./logs/" + timeStr + "_mapmqp-errors.log";
#endif
            
            logFile = fopen(logFilePath.c_str(), "w+");
#ifdef PRINT_SEPERATE_LOGS
            logInfoFile = fopen(logInfoFilePath.c_str(), "w+");
            logWarningsFile = fopen(logWarningsFilePath.c_str(), "w+");
            logErrorsFile = fopen(logErrorsFilePath.c_str(), "w+");
#endif
            
            //TODO confirm this was successful
            
            logInit = true;
        }
        
        //extract string from arguments
        va_list args;
        va_list argsInfo;
        va_list argsWarning;
        va_list argsError;
        va_start(args, entry);
        va_start(argsInfo, entry);
        va_start(argsWarning, entry);
        va_start(argsError, entry);
        
#ifdef PRINT_LOGS_TO_CONSOLE
        va_list argsConsole;
        va_start(argsConsole, entry);
#endif
        
        switch (type) {
            case INFO: {
#ifdef PRINT_SEPERATE_LOGS
                fprintf(logInfoFile, "[INFO] ");
                vfprintf(logInfoFile, entry, argsInfo);
                fprintf(logInfoFile, "\n");
                fflush(logInfoFile);
                fprintf(logFile, "[INFO] ");
#endif
#ifdef PRINT_LOGS_TO_CONSOLE
                printf("[INFO] ");
#endif
                break;
            }
            case WARNING: {
#ifdef PRINT_SEPERATE_LOGS
                fprintf(logWarningsFile, "[WARNING] ");
                vfprintf(logWarningsFile, entry, argsWarning);
                fprintf(logWarningsFile, "\n");
                fflush(logWarningsFile);
                fprintf(logFile, "[WARNING] ");
#endif
#ifdef PRINT_LOGS_TO_CONSOLE
                printf("[WARNING] ");
#endif
                break;
            }
            case ERROR: {
#ifdef PRINT_SEPERATE_LOGS
                fprintf(logErrorsFile, "[ERROR] ");
                vfprintf(logErrorsFile, entry, argsError);
                fprintf(logErrorsFile, "\n");
                fflush(logErrorsFile);
                fprintf(logFile, "[ERROR] ");
#endif
#ifdef PRINT_LOGS_TO_CONSOLE
                printf("[ERROR] ");
#endif
                break;
            }
            default:
                break;
        }
        
        //print to main log no matter what message type
        vfprintf(logFile, entry, args);
        fprintf(logFile, "\n");
        fflush(logFile);
        
        va_end(args);
        va_end(argsInfo);
        va_end(argsWarning);
        va_end(argsError);
      
#ifdef PRINT_LOGS_TO_CONSOLE
        vprintf(entry, argsConsole);
        printf("\n");
        va_end(argsConsole);
#endif
    }
    
    inline const std::shared_ptr<rapidjson::Document> settingsDocument() {
        static bool settingsInit = false;
        static std::shared_ptr<rapidjson::Document> jsonDoc(new rapidjson::Document());
        
        if (!settingsInit) {
            writeLog(INFO, "reading settings json file from %s...", SETTINGS_JSON_FILE_PATH);
            
            //open settings json file
            std::ifstream jsonFile;
            jsonFile.open(SETTINGS_JSON_FILE_PATH);
            if (jsonFile.is_open()) {
                //place string value of json in jsonStr
                std::stringstream strStream;
                strStream << jsonFile.rdbuf();
                jsonFile.close();
                
                //move json string to buffer and parse into document
                std::string jsonStr = strStream.str();
                int jsonStrLen = strlen(jsonStr.c_str());
                char * buffer = new char[jsonStrLen - 1]; //TODO does this need to be deleted?
                std::memcpy(buffer, jsonStr.c_str(), jsonStrLen - 1);
                if (jsonDoc->ParseInsitu(buffer).HasParseError()) {
                    writeLog(ERROR, "error parsing settings json file");
                } else {
                    settingsInit = true;
                }
                writeLog(INFO, "%s:\n%s", SETTINGS_JSON_FILE_PATH, jsonStr.c_str());
            } else {
                writeLog(ERROR, "could not read settings json file from path %s", SETTINGS_JSON_FILE_PATH);
            }
        }
        
        return jsonDoc;
    }
}

#endif /* Utility_hpp */
