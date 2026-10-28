#pragma once

#include <stdio.h>

#define TEST_ASSERT(label, expr) (fprintf(stderr,(char const *)label": %s\n", expr? "pass":"FAIL"))
