#include <native-lib.hpp>
#include <amatsutsumi.hpp>
#include <gbk2utf16.hpp>

namespace amatsutsumi
{
    static std::string game_data_path{};

    auto c_main_frame::init::func(c_main_frame *m_this, void *unk_ptr) -> void *
    {
        char buffer[1024]{};
        hooker::call<c_main_frame::get_run_path_name::func>(m_this, buffer);
        amatsutsumi::game_data_path.assign(buffer);
        if(!amatsutsumi::game_data_path.empty())
        {
            if(!amatsutsumi::game_data_path.ends_with('/'))
            {
                amatsutsumi::game_data_path.append("/");
            }
        }
        return hooker::call<c_main_frame::init::func>(m_this, unk_ptr);
    }

    auto c_main_frame::get_run_path_name::func(c_main_frame *m_this, void *buffer) -> void*
    {
        return hooker::call<c_main_frame::get_run_path_name::func>(m_this, buffer);
    }

    auto c_font_list::get_font_image::func(ft_face_helper_t *helper, font_image_output_t *font_image_output) -> size_t
    {
        return hooker::call<c_font_list::get_font_image::func>(helper, font_image_output);
    }

    auto c_font_buffer::get_font_image_cache::func(c_font_buffer *m_this, uint16_t uchar, int size) -> uint8_t *
    {
        if (m_this->cache_entry != nullptr)
        {
            cache_entry_t *previous_entry{ nullptr };
            cache_entry_t *current_entry{ m_this->cache_entry };
            do
            {
                if (current_entry->uchar == uchar && current_entry->size == size)
                {
                    if (previous_entry != nullptr)
                    {
                        previous_entry->next = current_entry->next;
                        current_entry->next = m_this->cache_entry;
                        m_this->cache_entry = current_entry;
                    }
                    if (current_entry->buffer != nullptr)
                    {
                        return current_entry->buffer;
                    }
                }
                previous_entry = current_entry;
                current_entry = previous_entry->next;
            } while (current_entry != nullptr);
        }
        else
        {
            m_this->cache_count = 0;
        }
        auto buffer{new uint8_t[size * size]{}};
        font_image_output_t font_image_output
        {
            .uchar = static_cast<uint32_t>(uchar),
            .pixel_size = static_cast<uint32_t>(size),
            .load_flag = static_cast<size_t>(1)
        };
        hooker::call<c_font_list::get_font_image::func>(m_this->face_helper, &font_image_output);
        if (font_image_output.width != 0 && font_image_output.rows != 0 && font_image_output.bitmap_buffer != nullptr)
        {
            uint8_t *src_buffer{font_image_output.bitmap_buffer};
            const int32_t src_width{font_image_output.width};
            const int32_t src_rows{font_image_output.rows};
            const int32_t src_pitch{font_image_output.pitch};

            const int32_t dest_start_x{std::max(0, font_image_output.bearing_x)};
            const int32_t dest_start_y{std::max(0, font_image_output.bearing_y)};
            const int32_t copy_width{std::min(src_width, size - dest_start_x)};
            const int32_t copy_height{std::min(src_rows, size - dest_start_y)};

            uint8_t *src_ptr{src_buffer};
            uint8_t *dest_ptr{buffer + (dest_start_y * size) + dest_start_x};
            for (int i{0}; i < copy_height; ++i)
            {
                std::memcpy(dest_ptr, src_ptr, copy_width);
                dest_ptr += size;
                src_ptr += src_pitch;
            }
        }

        auto new_entry = new cache_entry_t
        {
            .next{ m_this->cache_entry },
            .buffer{ buffer },
            .uchar{ uchar },
            .size{ static_cast<uint16_t>(size) }
        };
        m_this->cache_entry = new_entry;
        if (m_this->cache_count >= 0x400 && new_entry->next != nullptr)
        {
            auto current_entry{ new_entry->next };
            auto previous_entry{ static_cast<cache_entry_t*>(nullptr) };
            do
            {
                previous_entry = current_entry;
                current_entry = previous_entry->next;
            } while (current_entry->next != nullptr);

            delete[] current_entry->buffer;
            delete current_entry;

            previous_entry->next = nullptr;
        }
        else
        {
            m_this->cache_count++;
        }
        return buffer;
    }

    auto ft_new_face::func(void *library, const char *filepathname, long face_index, void *face) -> int
    {
        if(!amatsutsumi::game_data_path.empty())
        {
            std::string target_font_path{};
            auto cmvs_cfg_file { amatsutsumi::game_data_path + "cmvs.cfg" };
            {
                char buffer[256]{}, unk_abyte{};
                hooker::call<api_get_private_profile_string::func>(
                        "USE_DEFAULT_FONT", "PATH", &unk_abyte, buffer,
                        sizeof(buffer), cmvs_cfg_file.c_str());
                std::string_view path{ buffer };
                if(!path.empty() && path.size() > 1)
                {
                    if (path[0] == '.' && ( path[1] == '/' || path[1] == '\\'))
                    {
                        path = path.substr(2);
                        target_font_path = amatsutsumi::game_data_path + path.data();
                    }
                    else if(path[0] != '/' && path[0] != '\\')
                    {
                        target_font_path = amatsutsumi::game_data_path + path.data();
                    }
                    else
                    {
                        target_font_path.assign(path.data());
                    }
                }
            }

            // logd("target_font_path -> %s", target_font_path.data());
            if(!target_font_path.empty())
            {
                auto error { hooker::call<ft_new_face::func>(library, target_font_path.data(), face_index, face) };
                if(error == 0)
                {
                    return 0;
                }
            }

        }
        return hooker::call<ft_new_face::func>(library, filepathname, face_index, face);
    }

    auto mbstowcs_one::func(uint16_t chars) -> wchar_t
    {
        bool is_sjis_chars
        {
            /* ♪・～－ 在GBK中没有的SJIS特殊符号 */
            chars == 0xF481 || chars == 0x4581 ||
            chars == 0x6081 || chars == 0x7C81
        };
        if(!is_sjis_chars)
        {
            auto u16char { gbk2utf16::query((chars & 0xFF) << 8 | (chars >> 8)) };
            if(u16char != 0xFFFF)
            {
                return u16char;
            }
        }
        return hooker::call<mbstowcs_one::func>(chars);
    }

    auto api_sjis_check::func(uint8_t achar) -> int
    {
        return static_cast<int>(achar > 0x7F);
    }

    auto api_get_private_profile_string::func(const char *top_name, const char *name, char *unk_abyte, char *buffer, uint32_t buffer_max, const char *file) -> void *
    {
        return hooker::call<api_get_private_profile_string::func>(top_name, name, unk_abyte, buffer, buffer_max, file);
    }
}