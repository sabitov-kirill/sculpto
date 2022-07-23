/*!****************************************************************//**
 * \file   scene_scrtipts_system.h
 * \brief  Scene scripts (for objects behaviours) handling system class declaration module.
 * 
 * \author Sabitov Kirill
 * \date   21 July 2022
 *********************************************************************/

#pragma once

#include <RuntimeObjectSystem/RuntimeObjectSystem.h>
#include <RuntimeCompiler/ICompilerLogger.h>

#include "core/application/timer.h"
#include "utilities/logger/logger.h"

namespace scl
{
    /*! Internal logger system for logging script compilation class. */
    class script_log_system: public ICompilerLogger
    {
    private: /*! Logger system for logging script compilation data. */
        logger Logger { "Scripts" };

    public: /*! Logger system for logging script compilation methods. */
        /*! Log compilation error message function. */
        void LogError(const char *format, ...) override
        {
            va_list args;
            va_start(args, format);
            Logger.Error(format, args);
        }

        /*! Log compilation warning message function. */
        void LogWarning(const char *format, ...) override
        {
            va_list args;
            va_start(args, format);
            Logger.Warn(format, args);
        }

        /*! Log compilation info message function. */
        void LogInfo(const char *format, ...) override
        {
            va_list args;
            va_start(args, format);
            Logger.Info(format, args);
        }
    };

    /* Scene scripts (for objects behaviours) handling system class. */
    class scene_scrtipts_system
    {
    private:
        script_log_system    *ScriptsRuntimeCompilationLogger {}; /*! Scrips logger for displaying its compilation info. */
        IRuntimeObjectSystem *ScriptsRuntimeObjectSystem {};      /*! Scrips runtime object system for controlling dynamicly compiled sccript code. */

    public:
        scene_scrtipts_system()
        {
            ScriptsRuntimeObjectSystem = new RuntimeObjectSystem();
            ScriptsRuntimeCompilationLogger = new script_log_system();
            ScriptsRuntimeObjectSystem->Initialise(ScriptsRuntimeCompilationLogger, nullptr);
        }

        ~scene_scrtipts_system()
        {
            delete ScriptsRuntimeObjectSystem;
            delete ScriptsRuntimeCompilationLogger;
        }

        void UpdateScripts()
        {
            if (ScriptsRuntimeObjectSystem->GetIsCompiledComplete())
                ScriptsRuntimeObjectSystem->LoadCompiledModule();

            if (!ScriptsRuntimeObjectSystem->GetIsCompiling())
                ScriptsRuntimeObjectSystem->GetFileChangeNotifier()->Update(timer::GetDeltaTime());
        }

    };
}
