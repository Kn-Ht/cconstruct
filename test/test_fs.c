#include "../cconstruct.h"

static const char* test_paths[] = {
    ".gitignore", // File,
    "test", // Directory (exists)
    "src", // Directory (exists)
    "fake/", // Directory (does not exist)
    "new.txt", // File (does not exists)
};

#define testb(F) \
    puts("##### [TESTING "#F"(str) -> bool] #####"); \
    for (int i = 0; i < sizeof(test_paths) / sizeof(test_paths[0]); i++) { \
        printf("  "#F"(%s) = %d\n", test_paths[i], F(test_paths[i]));    \
    } \
    putchar('\n');

#define tests(F) \
    puts("##### [TESTING "#F"(str) -> str] #####"); \
    for (int i = 0; i < sizeof(test_paths) / sizeof(test_paths[0]); i++) { \
        char* result = F(test_paths[i]); \
        printf("  "#F"(%s) = \"%s\"\n", test_paths[i], result? result : "NULL");    \
        free(result); \
    } \
    putchar('\n');

#define testse(F) \
    puts("##### [TESTING "#F"(str, &error) -> str] #####"); \
    for (int i = 0; i < sizeof(test_paths) / sizeof(test_paths[0]); i++) { \
        errno_t error = 0; \
        char* result = F(test_paths[i], &error); \
        printf("  "#F"(%s) = error: %d, content: \"%s\"\n", test_paths[i], error, result? result : "NULL");    \
        free(result); \
    } \
    putchar('\n');


int main(void) {
    testb(cc_dir_exists);
    testb(cc_dir_exists_safe);
    testb(cc_file_exists);
    testb(cc_file_exists_safe);
    testb(cc_file_create);
    testb(cc_file_create_safe);
    tests(cc_file_read);
    testse(cc_file_read_safe);
}