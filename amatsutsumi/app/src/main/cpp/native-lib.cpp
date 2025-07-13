#include <native-lib.hpp>

static auto call_type mbstowcs_one_hook(uint16_t uChar) -> wchar_t
{
    auto u16char { gbk2utf16::to_utf16( (uChar & 0xFF) << 8 | (uChar >> 8)) };
    if(u16char != 0xFFFF)
    {
        return u16char;
    }
    return hooker::call<mbstowcs_one_hook>(uChar);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {

    void* handle { ::dlopen("libamatsutsumi.so", RTLD_NOW) };
    if (handle != nullptr)
    {
        void* target_function_ptr { ::dlsym(handle, "_Z13_mbstowcs_onet")};
        if(target_function_ptr)
        {
            hooker::add_hook<mbstowcs_one_hook>(target_function_ptr);
        }
        else
        {
            LOGD("target_function_ptr is nullptr");
        }
    }
    else
    {
        LOGD("handle is nullptr");
    }

    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_ss_colytitse_amatsutsumi_Amatsutsumi_stringFromJNI(JNIEnv *env, jclass) {
    return env->NewStringUTF("Patch by iTsukezigen.");
}