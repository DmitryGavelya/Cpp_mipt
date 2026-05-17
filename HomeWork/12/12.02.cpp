#include <cstdio>

int main() {
  const char* code = R"QUINE(#include <cstdio>

int main() {
    const char* code = R"QUINE(%s)QUINE";
  printf(code, code);
  return 0;
}
)QUINE";
    printf(code, code);
return 0;
}
