#pragma once

enum class CompileMode : int
{
    Debug,
    DebugOptOn,
    Release
};

#ifdef YOLO_MODE_DEBUG
static constexpr CompileMode sCompileMode = CompileMode::Debug;

#elif defined YOLO_MODE_DEBUGOPTON
static constexpr CompileMode sCompileMode = CompileMode::DebugOptOn;

#elif defined YOLO_MODE_RELEASE
static constexpr CompileMode sCompileMode = CompileMode::Release;

#else
    #error Unknown compile mode!
#endif
