#include "check_text.hpp"

#include <cassert>
#include <cstdint>
#include <optional>
#include <string>

#include <log.hpp>

namespace dnd {

static consteval uint32_t utf8_code_to_bytes(uint32_t code_point) {
    if (code_point >= 0x110000) {
        return 0xff'ff'ff'ff;
    }
    if (code_point <= 0x80) {
        return code_point;
    } else if (code_point <= 0x800) {
        uint32_t x = (code_point & 0x0700) >> 8;
        uint32_t y = (code_point & 0x00f0) >> 4;
        uint32_t z = (code_point & 0x000f);
        uint32_t byte1 = (0b11 << 6) | (x << 2) | (y >> 2);
        uint32_t byte2 = (0b10 << 6) | ((y & 0b11) << 4) | z;
        return (byte1 << 8) + byte2;
    } else if (code_point <= 0x10000) {
        uint32_t w = (code_point & 0xf000) >> 12;
        uint32_t x = (code_point & 0x0f00) >> 8;
        uint32_t y = (code_point & 0x00f0) >> 4;
        uint32_t z = (code_point & 0x000f);
        uint32_t byte1 = (0b1110 << 4) | w;
        uint32_t byte2 = (0b10 << 6) | (x << 2) | (y >> 2);
        uint32_t byte3 = (0b10 << 6) | ((y & 0b11) << 4) | z;
        return (byte1 << 16) | (byte2 << 8) | byte3;
    } else {
        uint32_t u = (code_point & 0x100000) >> 20;
        uint32_t v = (code_point & 0x0f0000) >> 16;
        uint32_t w = (code_point & 0x00f000) >> 12;
        uint32_t x = (code_point & 0x000f00) >> 8;
        uint32_t y = (code_point & 0x0000f0) >> 4;
        uint32_t z = (code_point & 0x00000f);
        uint32_t byte1 = (0b11110 << 3) | (u << 2) | (v >> 2);
        uint32_t byte2 = (0b10 << 6) | ((v & 0b11) << 4) | w;
        uint32_t byte3 = (0b10 << 6) | (x << 2) | (y >> 2);
        uint32_t byte4 = (0b10 << 6) | ((y & 0b11) << 4) | z;
        return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
    }
}

#if DND_DEBUG_MODE
static constexpr uint32_t bytes_to_utf8_code(uint32_t concat_bytes) {
    if ((concat_bytes & 0xf8'c0'c0'c0) == 0xf0'80'80'80) {
        uint32_t byte1_data = (concat_bytes >> 24) & 0x7;
        uint32_t byte2_data = (concat_bytes >> 16) & 0x3f;
        uint32_t byte3_data = (concat_bytes >> 8) & 0x3f;
        uint32_t byte4_data = concat_bytes & 0x3f;
        return (byte1_data << 18) | (byte2_data << 12) | (byte3_data << 6) | byte4_data;
    } else if ((concat_bytes & 0xff'f0'c0'c0) == 0x00'e0'80'80) {
        uint32_t byte1_data = (concat_bytes >> 16) & 0xf;
        uint32_t byte2_data = (concat_bytes >> 8) & 0x3f;
        uint32_t byte3_data = concat_bytes & 0x3f;
        return (byte1_data << 12) | (byte2_data << 6) | byte3_data;
    } else if ((concat_bytes & 0xff'ff'e0'c0) == 0x00'00'c0'80) {
        uint32_t byte1_data = (concat_bytes >> 8) & 0x1f;
        uint32_t byte2_data = concat_bytes & 0x3f;
        return (byte1_data << 6) | byte2_data;
    } else if ((concat_bytes & 0xff'ff'ff'80) == 0x00'00'00'00) {
        return concat_bytes;
    } else {
        return 0xff'ff'ff'ff;
    }
}
#endif

static void push_back_bytes(std::string& str, const char* bytes, size_t byte_count) {
    for (size_t i = 0; i < byte_count; ++i) {
        str.push_back(bytes[i]);
    }
}

std::string checked_string(std::string&& str) {
    std::string out;

    std::optional<char> prev = std::nullopt;
    std::string::iterator cur = str.begin();
    std::string::iterator end = str.end();

    char utf8_bytes[5];
    while (cur != end) {
        unsigned char first_byte = utf8_bytes[0] = *cur;
        bool valid_code_point = true;
        size_t expected_byte_count;
        if ((first_byte >> 7) == 0) {
            expected_byte_count = 1;
        } else if ((first_byte >> 5) == 0b110) {
            expected_byte_count = 2;
        } else if ((first_byte >> 4) == 0b1110) {
            expected_byte_count = 3;
        } else if ((first_byte >> 3) == 0b11110) {
            expected_byte_count = 4;
        } else {
            expected_byte_count = 1;
            valid_code_point = false;
            LOGWARN("Invalid UTF-8 first byte: '{:#x}'\n  in '{}'", first_byte, str);
        }

        uint64_t concat_bytes = first_byte;
        size_t byte_count = 1;
        while (byte_count < expected_byte_count) {
            ++cur;
            if (cur == end) {
                LOGWARN("Expected more UTF-8 bytes, but string ended\n  string: '{}'", str);
                valid_code_point = false;
                break;
            }
            unsigned char new_byte = utf8_bytes[byte_count] = *cur;
            ++byte_count;
            if ((new_byte >> 6) != 0b10) {
                LOGWARN("Invalid extra UTF-8 byte: '{:#x}'\n  in '{}'", new_byte, str);
                valid_code_point = false;
                break;
            }
            concat_bytes = (concat_bytes << 8) | new_byte;
        }

        if (byte_count == 1 && std::isprint(first_byte)) {
            out.push_back(first_byte);
        } else if (valid_code_point) {
            switch (concat_bytes) {
                case utf8_code_to_bytes(0xbd): // 1/2
                {
                    if (prev.has_value() && std::isdigit(prev.value())) {
                        out.push_back(' ');
                    }
                    out.append("1/2");
                    break;
                }
                case utf8_code_to_bytes(0xd7): // times symbol
                {
                    out.push_back('x');
                    break;
                }
                case utf8_code_to_bytes(0x2013): // en dash
                case utf8_code_to_bytes(0x2014): // em dash
                case utf8_code_to_bytes(0x2212): // minus sign
                {
                    out.push_back('-');
                    break;
                }
                // accepted unicode symbols
                case utf8_code_to_bytes(0xc4): // A umlaut
                case utf8_code_to_bytes(0xd6): // O umlaut
                case utf8_code_to_bytes(0xdc): // U umlaut
                case utf8_code_to_bytes(0xdf): // sharp S
                case utf8_code_to_bytes(0xe4): // a umlaut
                case utf8_code_to_bytes(0xf6): // o umlaut
                case utf8_code_to_bytes(0xfc): // u umlaut
                case utf8_code_to_bytes(0xfb): // u with ^
                {
                    push_back_bytes(out, utf8_bytes, byte_count);
                    break;
                }
                default: {
                    utf8_bytes[byte_count] = '\0';
                    LOGWARN(
                        "Unknown or unhandled UTF-8 code point: '{}' ({:#x})\n  in '{}'", &utf8_bytes[0],
                        bytes_to_utf8_code(concat_bytes), str
                    );
                    push_back_bytes(out, utf8_bytes, byte_count);
                    break;
                }
            }
        } else {
            push_back_bytes(out, utf8_bytes, byte_count);
        }

        if (cur != end) {
            prev = *cur;
            ++cur;
        }
    }
    return out;
}

} // namespace dnd
