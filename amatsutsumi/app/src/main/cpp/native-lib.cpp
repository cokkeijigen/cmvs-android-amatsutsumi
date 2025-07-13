#include <native-lib.hpp>

static auto call_type mbstowcs_one_hook(uint16_t chr) -> wchar_t
{
    auto u16char { gbk2utf16::query((chr & 0xFF) << 8 | (chr >> 8)) };
    if(u16char != 0xFFFF)
    {
        return u16char;
    }
    return hooker::call<mbstowcs_one_hook>(chr);
}

static auto call_type api_sjis_check_hook(uint8_t chr) -> int
{
    return static_cast<int>(chr > 0x7F);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {

    void* handle { ::dlopen("libamatsutsumi.so", RTLD_NOW) };
    if (handle != nullptr)
    {
        void* mbstowcs_onet_ptr { ::dlsym(handle, "_Z13_mbstowcs_onet") };
        if(mbstowcs_onet_ptr != nullptr)
        {
            hooker::add_hook<mbstowcs_one_hook>(mbstowcs_onet_ptr);
        }
        else
        {
            LOGD("mbstowcs_onet_ptr is nullptr");
        }

        void* api_sjis_check_ptr { ::dlsym(handle, "_Z12ApiSjisCheckh")  };
        if(api_sjis_check_ptr != nullptr)
        {
            hooker::add_hook<api_sjis_check_hook>(api_sjis_check_ptr);
        }
        else
        {
            LOGD("api_sjis_check_ptr is nullptr");
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