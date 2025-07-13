#pragma once
#include <iostream>
#include <jni.h>
#include <string>
#include <dlfcn.h>
#include <sys/mman.h>
#include <android/log.h>
#include <dobby.h>
#include <gbk2utf16.hpp>

#if defined(__i386__)
#define call_type __cdecl
#elif defined(__x86_64__)
#define call_type __fastcall
#else
#define call_type
#endif
#define TAG "amatsutsumihook"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)

struct hooker
{
    template<auto fun>
    inline static decltype(fun) call;

    template<auto fun>
    inline static auto add_hook(void* target) -> int
    {
        auto _out{reinterpret_cast<void**>(&hooker::call<fun>)};
        auto _fun{ reinterpret_cast<void*>(fun) };
        return ::DobbyHook(target, _fun, _out);
    }

    template<auto fun>
    inline static auto add_hook(decltype(fun) target) -> int
    {
        return hooker::add_hook<fun>(reinterpret_cast<void*>(target));
    }

};
