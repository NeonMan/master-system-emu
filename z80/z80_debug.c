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

#include "z80_debug.h"
#include "z80.h"
#include "z80_macros.h"
#include <assert.h>

extern struct z80_s z80;
static void(*_hook_callback)(void) = 0;

void z80dbg_register_hooh(void(*callback)(void)) {
	_hook_callback = callback;
}

void z80dbg_hook_entry() {
	/* op, arg1 and arg2 are on the stack */
	if (_hook_callback) {
		_hook_callback();
	}
	else {
		/**@ToDo Make a default hook handler?*/
	}
}
