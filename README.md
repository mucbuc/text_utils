# text_utils
text utils

## interface

```
namespace text_utils
{
  std::vector<int> apply_xor(std::string message, std::string key);
  std::string apply_xor(std::vector<int> buffer, std::string key);
  std::string apply_variables(std::string source, std::map<std::string, std::string> variables);
}
```

## run tests
```
cd test
./build_and_run
```
=>
```
void test_apply_vars_recursive() passed!
void test_apply_vars() passed!
void test_xor_scramble() passed!
```
