#pragma once

namespace amatsutsumi
{

    struct cache_entry_t
    {
        cache_entry_t* next;
        uint8_t* buffer;
        uint16_t uchar;
        uint16_t size;
    };

    struct ft_face_helper_t
    {
        void* unk_ptr;
        void* face;
    };

    struct font_image_output_t
    {
        uint32_t uchar;
        uint32_t pixel_size;
        size_t load_flag;
        uint8_t* bitmap_buffer;
        int32_t bearing_x;
        int32_t bearing_y;
        int32_t width;
        int32_t rows;
        int32_t pitch;
    };

    struct c_font_buffer
    {
        ft_face_helper_t* face_helper;
        cache_entry_t* cache_entry;
        size_t cache_count;

        struct get_font_image_cache    
        {
            inline static constexpr char sign[]{ "_ZN11CFontBuffer17GetFontImageCacheEtj" };
            static auto func(c_font_buffer* m_this, uint16_t achar, int size) -> uint8_t*;
        };

    };

    struct c_font_list
    {
        void* ft_library;
        void* ft_face;
        struct get_font_image
        {
            inline static constexpr char sign[]{ "_ZN9CFontList12GetFontImageEP17tag_FONT_ONE_DATA" };
            static auto func(ft_face_helper_t* helper, font_image_output_t* font_image_output) -> size_t;
        };

    };

    struct c_main_frame
    {
        struct init
        {
            inline static constexpr char sign[]{ "_ZN10CMainFrame4InitEP11android_app" };
            static auto func(c_main_frame *m_this, void* unk_ptr) -> void*;
        };

        struct get_run_path_name
        {
            inline static constexpr char sign[]{ "_ZN13CMainFrameWnd14GetRunPathNameEPc" };
            static auto func(c_main_frame *m_this, void* buffer) -> void*;
        };
    };

    struct ft_open_face
    {
        struct ft_open_args
        {
            unsigned int flags;
            unsigned char* memory_base;
            unsigned long memory_size;
            const char* pathname;
            const void* stream;
            const void* driver;
            signed int num_params;
            const void* params;
        };

        inline static constexpr char sign[]{ "FT_Open_Face" };
        static auto func(void* library, ft_open_args *args, long face_index, void *face) -> int;
    };
    struct mbstowcs_one
    {
        inline static constexpr char sign[]{ "_Z13_mbstowcs_onet" };
        static auto func(uint16_t chars) -> wchar_t;
    };
    
    struct api_sjis_check    
    {
        inline static constexpr char sign[]{ "_Z12ApiSjisCheckh" };
        static auto func(uint8_t achar) -> int;
    };

    struct api_get_private_profile_string
    {
        inline static constexpr char sign[]{ "_Z27_ApiGetPrivateProfileStringPcS_S_S_jS_" };
        static auto func(const char *top_name, const char *name, char* unk_abyte, char *buffer, uint32_t buffer_max, const char *file) -> void*;
    };
}