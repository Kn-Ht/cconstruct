#include "../cconstruct.h"

int main(void) {
    char* test_1 = "test_1hello, world!";
    char* hello = cc_strremstart(test_1, "test_1");
    printf("%s\n", hello);

    char* world = "hello worldworld";
    char* helloworld = cc_strremend(world, "world");
    printf("%s\n", helloworld);
    return 0;
}