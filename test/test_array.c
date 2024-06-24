#define CC_ARRAY_UNSCOPE
#include "../cconstruct.h"

DEF_ARRAY(int);

void print_array(int_array_t* array) {
    printf("{capacity = %zu, len = %zu, items = {", array->capacity, array->len);
    for (size_t i = 0; i < array->len; i++) {
        printf("%d", array->items[i]);
        if (i < array->len - 1) printf(", ");
    }
    printf("}}\n");
}

void push(int_array_t* array, int num) {
    printf("Pushing %d...\n", num);
    ARRAY_PUSH(*array, num);
    printf("after push: ");
    print_array(array);
}

int main(void) {
    int_array_t array = ARRAY_NEW(int, 20);
    printf("Array:");
    print_array(&array);

    push(&array, 1);
    push(&array, 2);
    printf("Reversing array...\n");
    ARRAY_REVERSE(array, int);
    printf("reversed: ");
    print_array(&array);
    printf("Clearing array...\n");
    ARRAY_CLEAR(array);
    push(&array, 3);
    printf("Final array: ");
    print_array(&array);

    ARRAY_FREE(array);
}