#pragma once

#include <map>
#include <string>
#include <vector>

namespace text_utils {

static std::vector<unsigned long> decode_utf8(const std::string& text)
{
    std::vector<unsigned long> out;
    out.reserve(text.size()); // upper bound; ASCII-heavy text is 1:1

    size_t i = 0;
    const size_t n = text.size();
    while (i < n) {
        const unsigned char b0 = static_cast<unsigned char>(text[i]);
        unsigned long cp = 0;
        int extra = 0;

        if ((b0 & 0x80) == 0x00) { // 0xxxxxxx
            cp = b0;
            extra = 0;
        } else if ((b0 & 0xE0) == 0xC0) { // 110xxxxx
            cp = b0 & 0x1F;
            extra = 1;
        } else if ((b0 & 0xF0) == 0xE0) { // 1110xxxx
            cp = b0 & 0x0F;
            extra = 2;
        } else if ((b0 & 0xF8) == 0xF0) { // 11110xxx
            cp = b0 & 0x07;
            extra = 3;
        } else {
            // invalid leading byte — skip it (or substitute U+FFFD if preferred)
            ++i;
            continue;
        }

        if (i + extra >= n) {
            // truncated sequence at end of string — bail on this codepoint
            break;
        }

        bool valid = true;
        for (int k = 1; k <= extra; ++k) {
            const unsigned char bk = static_cast<unsigned char>(text[i + k]);
            if ((bk & 0xC0) != 0x80) { // must be 10xxxxxx
                valid = false;
                break;
            }
            cp = (cp << 6) | (bk & 0x3F);
        }

        if (!valid) {
            ++i; // resync by one byte
            continue;
        }

        out.push_back(cp);
        i += extra + 1;
    }

    return out;
}

static std::vector<int> apply_xor(std::string message, std::string key)
{
    std::vector<int> result;
    result.reserve(message.size());
    for (auto i = 0; i < message.size(); ++i) {
        result.emplace_back(message[i] ^ key[i % key.size()]);
    }

    return result;
}

static std::string apply_xor(std::vector<int> buffer, std::string key)
{
    std::string result;
    result.reserve(buffer.size());
    for (auto i = 0; i < buffer.size(); ++i) {
        result.push_back(buffer[i] ^ key[i % key.size()]);
    }

    return result;
}

static std::string apply_variables(std::string source, std::map<std::string, std::string> variables)
{
    while (true) {
        bool done = true;
        for (auto i : variables) {
            const auto target = "{{" + i.first + "}}";
            while (true) {
                const auto p = source.find(target);
                if (p == std::string::npos) {
                    break;
                }
                source.replace(p, target.size(), i.second);

                if (i.second.find(target) != std::string::npos) {
                    break;
                }

                done = false;
            }
        }

        if (done) {
            break;
        }
    }

    return source;
}

}
