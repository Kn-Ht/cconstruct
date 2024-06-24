# CConstruct

A simple C utility library, designed to be simple and (hopefully) crossplatform.

---

## Notes

- There are `_safe` versions provided for most functions.

---

## How to use:

1. Clone the repo

```
git clone https://github.com/kn-ht/CConstructor
```

2. include either all the headers through `cconstruct.h` or a specific header through `src/`

```c
#include "cconstruct.h"

int main(void) {
	printf("%d\n", cc_file_exists(".gitignore"));
}
```
