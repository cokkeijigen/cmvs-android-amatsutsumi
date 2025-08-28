#include <native-lib.hpp>
#include <amatsutsumi.hpp>

extern "C"
{
    JNIEXPORT auto _Patch_By_iTsukezigen_(void) -> const char*
    {
        return "https://github.com/cokkeijigen/amatsutsumi_android";
    }

    JNIEXPORT jstring JNICALL
    Java_ss_colytitse_amatsutsumi_Amatsutsumi_stringFromJNI(JNIEnv *env, jclass)
    {
        return env->NewStringUTF("Patch By iTsukezigen.");
    }




    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
    {
        hooker libamatsutsumi{ "libamatsutsumi.so" };
        libamatsutsumi.hook<amatsutsumi::ft_open_face>();
        libamatsutsumi.hook<amatsutsumi::mbstowcs_one>();
        libamatsutsumi.hook<amatsutsumi::api_sjis_check>();
        libamatsutsumi.hook<amatsutsumi::c_font_buffer::get_font_image_cache>();
        libamatsutsumi.hook<amatsutsumi::c_font_list::get_font_image>();
        libamatsutsumi.hook<amatsutsumi::c_main_frame::init>();
        libamatsutsumi.hook<amatsutsumi::c_main_frame::get_run_path_name>();
        libamatsutsumi.hook<amatsutsumi::api_get_private_profile_string>();
        return JNI_VERSION_1_6;
    }
}
