#ifndef CCONSTRUCT_ARRAC_C
#define CCONSTRUCT_ARRAC_C

//! array.c: provides utilities for working with dynamic arrays
//! NOTE: define CC_ARRAY_UNSCOPE to 'unscope' the macros: CC_DEF_ARRAY -> DEF_ARRAY
//! NOTE: Also provides _SAFE alternatives that accept an `OK` parameter to check if allocation failed.

#include <stdbool.h>
#include <stdio.h>

struct CC_MockArray {
    size_t capacity; size_t len; void* items;
};

/// Defines a custom array of type T, usage: DEF_ARRAY(int); -> int_array_t
#define CC_DEF_ARRAY(T) typedef struct {size_t capacity; size_t len; T* items;} T##_array_t

/// Create new array of type T_array_t with initial capacity INIT_CAP
#define CC_ARRAY_NEW(T, INIT_CAP) \
    (T##_array_t){.capacity = INIT_CAP, .len = 0, .items = (T*)malloc(sizeof(T) * INIT_CAP)}

/// Frees the underlying buffer and sets all items to 0
#define CC_ARRAY_FREE(A) do {\
        free((A).items);  \
        (A).items = NULL; \
        (A).capacity = 0; \
        (A).len = 0;      \
    } while (0) 

/// 'Clears' the array (sets the len to 0)
#define CC_ARRAY_CLEAR(A) do {(A).len = 0;} while (0)

/// Reallocate array to capacity `NEW_CAP`.
/// NOTE: Items is not validated
#define CC_ARRAY_REALLOC(A, NEW_CAP) do { \
    (A).capacity = NEW_CAP;    \
    if ((A).capacity != NEW_CAP) \
        (A).items = realloc((A).items, (A).capacity * sizeof(*(A).items)); \
    } while(0)

/// Reallocate array to capacity `NEW_CAP`.
/// NOTE: check if it failed by providing `bool` variable `OK`
#define CC_ARRAY_REALLOC_SAFE(A, NEW_CAP, OK) do { \
    OK = true;                \
    (A).capacity = NEW_CAP;    \
    if ((A).capacity != NEW_CAP) \
        (A).items = realloc((A).items, (A).capacity * sizeof(*(A).items)); \
    if (!(A).items) OK = false; \
    } while(0)

/// Push `ITEM` to `A`, reallocating if needed.
/// NOTE: check reallocation failed by providing a `bool` as `OK`
#define CC_ARRAY_PUSH_SAFE(A, ITEM, OK) \
    do { \
        if ((A).len == (A).capacity) { \
            (A).capacity *= 2; \
            (A).items = realloc((A).items, (A).capacity * sizeof(*(A).items)); \
            if (!(A).items) OK = false; \
        } \
        if (OK) (A).items[(A).len++] = (ITEM); \
    } while (0)

/// Push `ITEM` to `A`, reallocating if needed.
/// NOTE: realloc() result not checked! If it should be, use CC_ARRAY_PUSH_SAFE
#define CC_ARRAY_PUSH(A, ITEM) \
    do { \
        if ((A).len == (A).capacity) { \
            (A).capacity *= 2; \
            (A).items = realloc((A).items, (A).capacity * sizeof(*(A).items)); \
        } \
        (A).items[(A).len++] = (ITEM); \
    } while (0)

/// Reverse Array, provide `bool` parameter `OK` to check if allocation failed.
#define CC_ARRAY_REVERSE_SAFE(A, ITEM_T, OK) do { \
        OK = true; \
        ITEM_T* temp_items = (ITEM_T*)malloc((A).capacity * sizeof(ITEM_T)); \
        if (temp_items) { \
            for (size_t i = 0; i < (A).len; i++) temp_items[i] = (A).items[(A).len - 1 - i]; \
            free((A).items); \
            (A).items = temp_items; \
        } else OK = false; \
    } while (0)

/// Reverse Array, provide `bool` parameter `OK` to check if allocation failed.
#define CC_ARRAY_REVERSE(A, ITEM_T) do { \
        ITEM_T* temp_items = (ITEM_T*)malloc((A).capacity * sizeof(ITEM_T)); \
        for (size_t i = 0; i < (A).len; i++) temp_items[i] = (A).items[(A).len - 1 - i]; \
        free((A).items); \
        (A).items = temp_items; \
    } while (0)

#ifdef CC_ARRAY_UNSCOPE
#define DEF_ARRAY CC_DEF_ARRAY 
#define ARRAY_NEW CC_ARRAY_NEW 
#define ARRAY_FREE CC_ARRAY_FREE 
#define ARRAY_PUSH CC_ARRAY_PUSH 
#define ARRAY_PUSH_SAFE CC_ARRAY_PUSH_SAFE
#define ARRAY_REALLOC CC_ARRAY_REALLOC
#define ARRAY_REALLOC_SAFE CC_ARRAY_REALLOC_SAFE
#define ARRAY_CLEAR CC_ARRAY_CLEAR
#define ARRAY_REVERSE CC_ARRAY_REVERSE
#define ARRAY_REVERSE_SAFE CC_ARRAY_REVERSE_SAFE
#endif  

#endif // CCONSTRUCT_ARRAC_C