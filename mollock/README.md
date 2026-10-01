# mollock

A simple memory allocator that uses a 16KB array to store metadata.

## Usage

Drop-in replacement for standard `malloc` and `free`.

```c
#include "mollock.h"

void *ptr = mollock(size);
mfree(ptr);
```

> Note: mollock does not support `realloc`.

## Testing

```sh
gcc -std=c17 -Wall -Wextra -Wpedantic -g mollock.c mollock.c -o test
./test
```
