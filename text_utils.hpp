#pragma once

#include <string>
#include <vector>
#include <map>

namespace text_utils
{

std::vector<int> apply_xor(std::string message, std::string key)
{
    std::vector<int> result;
    result.reserve(message.size());
    for (auto i = 0; i < message.size(); ++i) {
        result.emplace_back(message[i] ^ key[i % key.size()]);
    }

    return result;
}

std::string apply_xor(std::vector<int> buffer, std::string key)
{
    std::string result;
    result.reserve(buffer.size());
    for (auto i = 0; i < buffer.size(); ++i) {
        result.push_back(buffer[i] ^ key[i % key.size()]);
    }

    return result;
}

std::string apply_variables(std::string source, std::map<std::string, std::string> variables)
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
