// Copyright 2015 Juan Luis Álvarez Martínez
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "debug/sms_debug.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <assert.h>
static void platform_assert(const char* exp, const char* file, unsigned int line) {
    assert(0);
}
#else
static void platform_assert(const char* exp, const char* file, unsigned int line) {
    exit(-1);
}
#endif

void sms_assert(const char* exp, const char* file, unsigned int line) {
    fprintf(stderr, "[%s @%d] Assert failed: %s\r\n", file, line, exp);
    //Call inner assert
    platform_assert(exp, file, line);
}
