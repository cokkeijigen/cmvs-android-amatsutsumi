#pragma once
#include <iostream>
#include <jni.h>
#include <string>
#include <dlfcn.h>
#include <sys/mman.h>
#include <android/log.h>
#include <dobby.h>

#define TAG "amatsutsumihook"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define logd(...) LOGD(__VA_ARGS__)

template <class T>
concept hooker_target = requires(T t)
{
    T::sign;
    T::func;
};

struct hooker
{
    template<auto fun>
    inline static decltype(fun) call;

    template<auto fun>
    inline static auto add(void* target) -> int
    {
        auto _out{ reinterpret_cast<void**>(&hooker::call<fun>) };
        auto _fun{ reinterpret_cast<void*>(fun) };
        return ::DobbyHook(target, _fun, _out);
    }

    template<auto fun>
    inline static auto add(decltype(fun) target) -> int
    {
        return hooker::add<fun>(reinterpret_cast<void*>(target));
    }

    const void* handle;
    inline hooker(const char* name): handle{ ::dlopen(name, RTLD_NOW) } {}
    inline hooker(const void* handle): handle{ handle } {}

    template<class target>
    requires hooker_target<target>
    inline auto hook() -> int
    {
        auto raw{ ::dlsym(const_cast<void *>(this->handle), target::sign) };
        if(raw == nullptr)
        {
            return 0;
        }
        return hooker::add<target::func>(raw);
    }
};

