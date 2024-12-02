
#include <iostream>
#include <cassert>
#define ASSERT(p) assert((p))

#define FOOTER std::cout << __PRETTY_FUNCTION__ << " passed!" << std::endl

#include <text_utils/text_utils.hpp>

using namespace std;
using namespace text_utils;

static void test_apply_vars_recursive()
{
    const string text = "{{name}} cares!";
    ASSERT( apply_variables( text, {{"name", "{{npc}}"}} ) == "{{npc}} cares!");
    ASSERT( apply_variables( text, {{"name", "{{npc}}"}, {"npc", "anonom"}} ) == "anonom cares!");

    FOOTER;
}

static void test_apply_vars()
{
    const string text = "{{name}} cares!";
    ASSERT( apply_variables( text, {{"name", "nobody"}} ) == "nobody cares!");

    FOOTER;
}

static void test_xor_scramble()
{
    const string text = "who cares?";
    const string key = "()*&*(&KJH))";
    const auto scrambled { apply_xor(text, key) };
    const auto decrypted = apply_xor(scrambled, key);
    ASSERT(decrypted == text);

    FOOTER;
}

int main()
{
    test_apply_vars_recursive();
    test_apply_vars();
    test_xor_scramble();
    return 0;
}
