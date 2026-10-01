#pragma once

#include <stdlib.h>

void *mollock(size_t size);
void mfree(void *ptr);
