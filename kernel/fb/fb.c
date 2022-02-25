#include <fb/fb.h>
#include <klibc/mem.h>
#include <locks/spinlock.h>
#include <mem/liballoc.h>

/*
 * Copyright 2021, 2022 NSG650
 * Copyright 2021, 2022 Sebastian
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

uint8_t framebuffer_font[4096] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00,
	0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x12, 0x12, 0x12, 0x7e, 0x24, 0x24, 0x7e, 0x48, 0x48, 0x48, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x3e, 0x49, 0x48, 0x38, 0x0e, 0x09, 0x49,
	0x3e, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x4a, 0x4a, 0x34,
	0x08, 0x08, 0x16, 0x29, 0x29, 0x46, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x1c, 0x22, 0x22, 0x14, 0x18, 0x29, 0x45, 0x42, 0x46, 0x39, 0x00, 0x00,
	0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 0x08, 0x10, 0x10,
	0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x20,
	0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x10, 0x20, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x49, 0x2a, 0x1c, 0x2a, 0x49,
	0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08,
	0x08, 0x7f, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x08, 0x08, 0x10,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x02, 0x04, 0x08, 0x08, 0x10, 0x10, 0x20, 0x40, 0x40, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x46, 0x4a, 0x52, 0x62, 0x42,
	0x24, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x18, 0x28, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x3c, 0x42, 0x42, 0x02, 0x0c, 0x10, 0x20, 0x40, 0x40, 0x7e, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x02, 0x1c, 0x02, 0x02, 0x42,
	0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0c, 0x14, 0x24,
	0x44, 0x44, 0x7e, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x7e, 0x40, 0x40, 0x40, 0x7c, 0x02, 0x02, 0x02, 0x42, 0x3c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x1c, 0x20, 0x40, 0x40, 0x7c, 0x42, 0x42, 0x42,
	0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x02, 0x02, 0x04,
	0x04, 0x04, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x3c, 0x42, 0x42, 0x42, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x3e, 0x02, 0x02, 0x02,
	0x04, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18,
	0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x08, 0x08, 0x10, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x10, 0x08,
	0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e,
	0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x02, 0x04, 0x08, 0x08, 0x00,
	0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x22, 0x4a, 0x56,
	0x52, 0x52, 0x52, 0x4e, 0x20, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x18, 0x24, 0x24, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x7c, 0x42, 0x42, 0x42, 0x7c, 0x42, 0x42, 0x42,
	0x42, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x40,
	0x40, 0x40, 0x40, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x78, 0x44, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x44, 0x78, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x7e, 0x40, 0x40, 0x40, 0x7c, 0x40, 0x40, 0x40,
	0x40, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x40, 0x40, 0x40,
	0x7c, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x3c, 0x42, 0x42, 0x40, 0x40, 0x4e, 0x42, 0x42, 0x46, 0x3a, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x42,
	0x42, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x1f, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x44, 0x44, 0x38, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x42, 0x44, 0x48, 0x50, 0x60, 0x60, 0x50, 0x48,
	0x44, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40,
	0x40, 0x40, 0x40, 0x40, 0x40, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x42, 0x42, 0x66, 0x66, 0x5a, 0x5a, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x42, 0x62, 0x62, 0x52, 0x52, 0x4a, 0x4a, 0x46,
	0x46, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42,
	0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x7c, 0x42, 0x42, 0x42, 0x7c, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x5a,
	0x66, 0x3c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x42, 0x42, 0x42,
	0x7c, 0x48, 0x44, 0x44, 0x42, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x3c, 0x42, 0x42, 0x40, 0x30, 0x0c, 0x02, 0x42, 0x42, 0x3c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x7f, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42,
	0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x41, 0x41, 0x41, 0x22, 0x22, 0x22, 0x14, 0x14, 0x08, 0x08, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x5a, 0x5a, 0x66, 0x66,
	0x42, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x24, 0x24,
	0x18, 0x18, 0x24, 0x24, 0x42, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x41, 0x41, 0x22, 0x22, 0x14, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x7e, 0x02, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40,
	0x40, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x40, 0x40, 0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x02, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
	0x10, 0x10, 0x70, 0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00,
	0x00, 0x20, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42,
	0x02, 0x3e, 0x42, 0x42, 0x46, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
	0x40, 0x40, 0x5c, 0x62, 0x42, 0x42, 0x42, 0x42, 0x62, 0x5c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x40, 0x40, 0x40, 0x40,
	0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x3a, 0x46,
	0x42, 0x42, 0x42, 0x42, 0x46, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3c, 0x42, 0x42, 0x7e, 0x40, 0x40, 0x42, 0x3c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x0c, 0x10, 0x10, 0x10, 0x7c, 0x10, 0x10, 0x10, 0x10,
	0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x3a, 0x44,
	0x44, 0x44, 0x38, 0x20, 0x3c, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x00, 0x40,
	0x40, 0x40, 0x5c, 0x62, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x08, 0x08, 0x00, 0x18, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x00, 0x0c, 0x04,
	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x48, 0x30, 0x00, 0x00, 0x00, 0x40,
	0x40, 0x40, 0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44, 0x42, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x49,
	0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x5c, 0x62, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42,
	0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5c, 0x62,
	0x42, 0x42, 0x42, 0x42, 0x62, 0x5c, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3a, 0x46, 0x42, 0x42, 0x42, 0x42, 0x46, 0x3a, 0x02, 0x02,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5c, 0x62, 0x42, 0x40, 0x40, 0x40,
	0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42,
	0x40, 0x30, 0x0c, 0x02, 0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x10, 0x10, 0x10, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,
	0x46, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42,
	0x42, 0x24, 0x24, 0x24, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x41, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x24, 0x18, 0x18, 0x24,
	0x42, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42,
	0x42, 0x42, 0x42, 0x26, 0x1a, 0x02, 0x02, 0x3c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7e, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x7e, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x0c, 0x10, 0x10, 0x08, 0x08, 0x10, 0x20, 0x10, 0x08,
	0x08, 0x10, 0x10, 0x0c, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x30,
	0x08, 0x08, 0x10, 0x10, 0x08, 0x04, 0x08, 0x10, 0x10, 0x08, 0x08, 0x30,
	0x00, 0x00, 0x00, 0x31, 0x49, 0x46, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xaa, 0x00, 0x80, 0x00, 0x80, 0x73, 0xca, 0x4b,
	0xca, 0x73, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55, 0xaa, 0x00, 0x80, 0x00,
	0x80, 0x71, 0xca, 0x73, 0xc2, 0x42, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55,
	0xaa, 0x00, 0x80, 0x00, 0x80, 0x49, 0xca, 0x7a, 0xca, 0x49, 0x80, 0x00,
	0x80, 0x00, 0x80, 0x55, 0xaa, 0x00, 0x80, 0x00, 0x80, 0x73, 0xca, 0x73,
	0xca, 0x72, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55, 0xaa, 0x00, 0x80, 0x00,
	0x80, 0x4b, 0xea, 0x5b, 0xca, 0x4b, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55,
	0xaa, 0x00, 0x80, 0x00, 0x80, 0x74, 0xa6, 0x25, 0xa4, 0x74, 0x80, 0x00,
	0x80, 0x00, 0x80, 0x55, 0xaa, 0x00, 0x80, 0x00, 0x80, 0x4b, 0xea, 0x5b,
	0xca, 0x4b, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55, 0xaa, 0x00, 0x80, 0x00,
	0x80, 0x39, 0xc2, 0x31, 0x88, 0x73, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55,
	0xaa, 0x00, 0x80, 0x00, 0x80, 0x79, 0xc2, 0x79, 0xc0, 0x7b, 0x80, 0x00,
	0x80, 0x00, 0x80, 0x55, 0xaa, 0x00, 0x80, 0x00, 0x80, 0x4b, 0xc9, 0x79,
	0xc9, 0x49, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55, 0xaa, 0x00, 0x80, 0x00,
	0x80, 0x25, 0xa4, 0x3c, 0xa4, 0x24, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55,
	0xaa, 0x00, 0x80, 0x00, 0x80, 0x45, 0xc4, 0x44, 0xa8, 0x10, 0x80, 0x00,
	0x80, 0x00, 0x80, 0x55, 0xaa, 0x00, 0x80, 0x00, 0x80, 0x72, 0xca, 0x72,
	0xc2, 0x43, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55, 0xaa, 0x00, 0x80, 0x00,
	0x80, 0x72, 0xca, 0x72, 0xc2, 0x43, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55,
	0xaa, 0x00, 0x80, 0x00, 0x80, 0x0e, 0x89, 0x0e, 0x8a, 0x09, 0x80, 0x00,
	0x80, 0x00, 0x80, 0x55, 0xaa, 0x00, 0x80, 0x00, 0x80, 0x39, 0xc2, 0x31,
	0x88, 0x73, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55, 0xaa, 0x00, 0x80, 0x00,
	0x80, 0x39, 0xc2, 0x31, 0x88, 0x73, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55,
	0xaa, 0x00, 0x80, 0x00, 0x80, 0x71, 0xca, 0x4a, 0xca, 0x71, 0x80, 0x00,
	0x80, 0x00, 0x80, 0x55, 0xaa, 0x00, 0x80, 0x00, 0x80, 0x72, 0xca, 0x72,
	0xc2, 0x41, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55, 0xaa, 0x00, 0x80, 0x00,
	0x80, 0x72, 0xca, 0x72, 0xc2, 0x41, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55,
	0xaa, 0x00, 0x80, 0x00, 0x80, 0x3b, 0xc1, 0x31, 0x89, 0x71, 0x80, 0x00,
	0x80, 0x00, 0x80, 0x55, 0xaa, 0x00, 0x80, 0x00, 0x80, 0x39, 0xc2, 0x42,
	0xc2, 0x39, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55, 0xaa, 0x00, 0x80, 0x00,
	0x80, 0x22, 0xb6, 0x2a, 0xa2, 0x22, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55,
	0xaa, 0x00, 0x80, 0x00, 0x80, 0x3b, 0xc2, 0x33, 0x8a, 0x72, 0x80, 0x00,
	0x80, 0x00, 0x80, 0x55, 0xaa, 0x00, 0x80, 0x00, 0x80, 0x7b, 0xc2, 0x7b,
	0xc2, 0x7a, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55, 0xaa, 0x00, 0x80, 0x00,
	0x80, 0x39, 0xc2, 0x32, 0x8a, 0x71, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55,
	0xaa, 0x00, 0x80, 0x00, 0x80, 0x33, 0xc4, 0x25, 0x94, 0x63, 0x80, 0x00,
	0x80, 0x00, 0x80, 0x55, 0xaa, 0x00, 0x80, 0x00, 0x80, 0x39, 0xc2, 0x32,
	0x8a, 0x71, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55, 0xaa, 0x00, 0x80, 0x00,
	0x80, 0x39, 0xc2, 0x41, 0xc0, 0x3b, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55,
	0xaa, 0x00, 0x80, 0x00, 0x80, 0x0e, 0x90, 0x0c, 0x82, 0x1c, 0x80, 0x00,
	0x80, 0x00, 0x80, 0x55, 0xaa, 0x00, 0x80, 0x00, 0x80, 0x31, 0xca, 0x49,
	0xc8, 0x33, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55, 0xaa, 0x00, 0x80, 0x00,
	0x80, 0x1c, 0x92, 0x1c, 0x90, 0x10, 0x80, 0x00, 0x80, 0x00, 0x80, 0x55,
	0xaa, 0x00, 0x80, 0x00, 0x80, 0x33, 0xca, 0x7b, 0xca, 0x4a, 0x80, 0x00,
	0x80, 0x00, 0x80, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x08, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x3e, 0x49, 0x48, 0x48, 0x49, 0x3e,
	0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x10, 0x10, 0x10,
	0x7c, 0x10, 0x10, 0x10, 0x3e, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x42, 0x3c, 0x24, 0x42, 0x42, 0x24, 0x3c, 0x42, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x41, 0x22, 0x14, 0x08, 0x7f, 0x08, 0x7f, 0x08,
	0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08,
	0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x3c, 0x42, 0x40, 0x3c, 0x42, 0x42, 0x3c, 0x02, 0x42, 0x3c, 0x00, 0x00,
	0x24, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x99, 0xa5,
	0xa1, 0xa1, 0xa5, 0x99, 0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x02,
	0x1e, 0x22, 0x1e, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x12, 0x24, 0x24, 0x48, 0x24, 0x24,
	0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7e, 0x02, 0x02, 0x02, 0x00, 0x00, 0xaa, 0x00, 0x80, 0x3a,
	0xc2, 0x33, 0x8a, 0x72, 0x80, 0x00, 0x80, 0x03, 0x80, 0x00, 0x80, 0x55,
	0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0xb9, 0xa5, 0xa5, 0xb9, 0xa9, 0xa5,
	0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x18, 0x24, 0x24, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x7f, 0x08, 0x08, 0x08, 0x00,
	0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x04, 0x18, 0x20,
	0x40, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38,
	0x44, 0x04, 0x38, 0x04, 0x44, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x04, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42,
	0x42, 0x42, 0x42, 0x42, 0x66, 0x59, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7a, 0x7a, 0x7a, 0x3a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x30, 0x00, 0x00, 0x00, 0x10,
	0x30, 0x50, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x1c, 0x22, 0x22, 0x22, 0x1c, 0x00, 0x3e, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x48, 0x24,
	0x24, 0x12, 0x24, 0x24, 0x48, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x22, 0x62, 0x24, 0x28, 0x28, 0x12, 0x16, 0x2a, 0x4e, 0x42, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x22, 0x62, 0x24, 0x28, 0x28, 0x14, 0x1a, 0x22,
	0x44, 0x4e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x62, 0x12, 0x24, 0x18,
	0x68, 0x12, 0x16, 0x2a, 0x4e, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x10, 0x10, 0x00, 0x10, 0x10, 0x20, 0x40, 0x42, 0x42, 0x3c, 0x00, 0x00,
	0x30, 0x0c, 0x00, 0x00, 0x18, 0x24, 0x24, 0x42, 0x42, 0x7e, 0x42, 0x42,
	0x42, 0x42, 0x00, 0x00, 0x0c, 0x30, 0x00, 0x00, 0x18, 0x24, 0x24, 0x42,
	0x42, 0x7e, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00, 0x18, 0x24, 0x00, 0x00,
	0x18, 0x24, 0x24, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00,
	0x32, 0x4c, 0x00, 0x00, 0x18, 0x24, 0x24, 0x42, 0x42, 0x7e, 0x42, 0x42,
	0x42, 0x42, 0x00, 0x00, 0x24, 0x24, 0x00, 0x00, 0x18, 0x24, 0x24, 0x42,
	0x42, 0x7e, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00, 0x18, 0x24, 0x18, 0x00,
	0x18, 0x24, 0x24, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x1f, 0x28, 0x48, 0x48, 0x7f, 0x48, 0x48, 0x48,
	0x48, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x40,
	0x40, 0x40, 0x40, 0x42, 0x42, 0x3c, 0x08, 0x30, 0x30, 0x0c, 0x00, 0x00,
	0x7e, 0x40, 0x40, 0x40, 0x7c, 0x40, 0x40, 0x40, 0x40, 0x7e, 0x00, 0x00,
	0x0c, 0x30, 0x00, 0x00, 0x7e, 0x40, 0x40, 0x40, 0x7c, 0x40, 0x40, 0x40,
	0x40, 0x7e, 0x00, 0x00, 0x18, 0x24, 0x00, 0x00, 0x7e, 0x40, 0x40, 0x40,
	0x7c, 0x40, 0x40, 0x40, 0x40, 0x7e, 0x00, 0x00, 0x24, 0x24, 0x00, 0x00,
	0x7e, 0x40, 0x40, 0x40, 0x7c, 0x40, 0x40, 0x40, 0x40, 0x7e, 0x00, 0x00,
	0x18, 0x06, 0x00, 0x00, 0x3e, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x3e, 0x00, 0x00, 0x0c, 0x30, 0x00, 0x00, 0x3e, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x3e, 0x00, 0x00, 0x18, 0x24, 0x00, 0x00,
	0x3e, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3e, 0x00, 0x00,
	0x24, 0x24, 0x00, 0x00, 0x3e, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x44, 0x42, 0x42,
	0xf2, 0x42, 0x42, 0x42, 0x44, 0x78, 0x00, 0x00, 0x32, 0x4c, 0x00, 0x00,
	0x42, 0x62, 0x62, 0x52, 0x52, 0x4a, 0x4a, 0x46, 0x46, 0x42, 0x00, 0x00,
	0x30, 0x0c, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,
	0x42, 0x3c, 0x00, 0x00, 0x0c, 0x30, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42,
	0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x18, 0x24, 0x00, 0x00,
	0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00,
	0x32, 0x4c, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,
	0x42, 0x3c, 0x00, 0x00, 0x24, 0x24, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42,
	0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x42, 0x24, 0x18, 0x24, 0x42, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x3a, 0x44, 0x46, 0x4a, 0x4a, 0x52, 0x52, 0x62,
	0x22, 0x5c, 0x40, 0x00, 0x30, 0x0c, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42,
	0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x0c, 0x30, 0x00, 0x00,
	0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00,
	0x18, 0x24, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,
	0x42, 0x3c, 0x00, 0x00, 0x24, 0x24, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42,
	0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x0c, 0x30, 0x00, 0x00,
	0x41, 0x41, 0x22, 0x22, 0x14, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x40, 0x40, 0x78, 0x44, 0x42, 0x42, 0x44, 0x78, 0x40,
	0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x48,
	0x58, 0x44, 0x42, 0x42, 0x52, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0c,
	0x00, 0x00, 0x3c, 0x42, 0x02, 0x3e, 0x42, 0x42, 0x46, 0x3a, 0x00, 0x00,
	0x00, 0x00, 0x0c, 0x30, 0x00, 0x00, 0x3c, 0x42, 0x02, 0x3e, 0x42, 0x42,
	0x46, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x00, 0x00, 0x3c, 0x42,
	0x02, 0x3e, 0x42, 0x42, 0x46, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x32, 0x4c,
	0x00, 0x00, 0x3c, 0x42, 0x02, 0x3e, 0x42, 0x42, 0x46, 0x3a, 0x00, 0x00,
	0x00, 0x00, 0x24, 0x24, 0x00, 0x00, 0x3c, 0x42, 0x02, 0x3e, 0x42, 0x42,
	0x46, 0x3a, 0x00, 0x00, 0x00, 0x18, 0x24, 0x18, 0x00, 0x00, 0x3c, 0x42,
	0x02, 0x3e, 0x42, 0x42, 0x46, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3e, 0x49, 0x09, 0x3f, 0x48, 0x48, 0x49, 0x3e, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x40, 0x40, 0x40, 0x40,
	0x42, 0x3c, 0x08, 0x30, 0x00, 0x00, 0x30, 0x0c, 0x00, 0x00, 0x3c, 0x42,
	0x42, 0x7e, 0x40, 0x40, 0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x30,
	0x00, 0x00, 0x3c, 0x42, 0x42, 0x7e, 0x40, 0x40, 0x42, 0x3c, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x24, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x7e, 0x40, 0x40,
	0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x24, 0x24, 0x00, 0x00, 0x3c, 0x42,
	0x42, 0x7e, 0x40, 0x40, 0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0c,
	0x00, 0x00, 0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3e, 0x00, 0x00,
	0x00, 0x00, 0x0c, 0x30, 0x00, 0x00, 0x18, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x00, 0x00, 0x18, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x24, 0x24,
	0x00, 0x00, 0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3e, 0x00, 0x00,
	0x00, 0x00, 0x32, 0x0c, 0x14, 0x22, 0x02, 0x3e, 0x42, 0x42, 0x42, 0x42,
	0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x32, 0x4c, 0x00, 0x00, 0x5c, 0x62,
	0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0c,
	0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00,
	0x00, 0x00, 0x0c, 0x30, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42,
	0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x00, 0x00, 0x3c, 0x42,
	0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x32, 0x4c,
	0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00,
	0x00, 0x00, 0x24, 0x24, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42,
	0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00,
	0x00, 0x7e, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x02, 0x3c, 0x46, 0x4a, 0x4a, 0x52, 0x52, 0x62, 0x3c, 0x40, 0x00,
	0x00, 0x00, 0x30, 0x0c, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,
	0x46, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x30, 0x00, 0x00, 0x42, 0x42,
	0x42, 0x42, 0x42, 0x42, 0x46, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x18, 0x24,
	0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x46, 0x3a, 0x00, 0x00,
	0x00, 0x00, 0x24, 0x24, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,
	0x46, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x30, 0x00, 0x00, 0x42, 0x42,
	0x42, 0x42, 0x42, 0x26, 0x1a, 0x02, 0x02, 0x3c, 0x00, 0x00, 0x00, 0x40,
	0x40, 0x40, 0x5c, 0x62, 0x42, 0x42, 0x42, 0x42, 0x62, 0x5c, 0x40, 0x40,
	0x00, 0x00, 0x24, 0x24, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x26,
	0x1a, 0x02, 0x02, 0x3c,
};

uint8_t framebuffer_initialised = 0;
lock_t framebuff_lock;
lock_t swap_lock;

#define ISO_CHAR_MIN 0x00
#define ISO_CHAR_MAX 0xFF
#define ISO_CHAR_WIDTH 8
#define ISO_CHAR_HEIGHT 16

struct framebuffer framebuff;

void framebuffer_init(struct framebuffer *fb) {
	framebuff.address = fb->address;
	framebuff.pitch = fb->pitch;
	framebuff.bpp = fb->bpp;
	framebuff.width = fb->width;
	framebuff.height = fb->height;
	framebuff.tex_x = fb->tex_x + 1;
	framebuff.tex_y = fb->tex_y;
	framebuff.tex_color = fb->tex_color;
	framebuff.back_address = kmalloc(framebuff.pitch * framebuff.height);
	framebuff.bg_color = fb->bg_color;
	framebuffer_initialised = 1;
}

void framebuffer_putpx(int x, int y, uint32_t color, bool fob) {
	spinlock_acquire(framebuff_lock);
	if (!framebuffer_initialised)
		return;
	if (fob)
		framebuff.address[y * (framebuff.pitch / sizeof(uint32_t)) + x] = color;
	else
		framebuff.back_address[y * (framebuff.pitch / sizeof(uint32_t)) + x] =
			color;
	spinlock_drop(framebuff_lock);
}

void framebuffer_swap(struct framebuffer *fb) {
	spinlock_acquire(swap_lock);
	uint32_t *front = fb->address;
	uint32_t *back = fb->back_address;
	for (size_t i = 0;
		 i < framebuff.pitch * framebuff.height / sizeof(uint32_t); i++) {
		front[i] = back[i];
	}
	spinlock_drop(swap_lock);
}

void framebuffer_clear(uint32_t color) {
	framebuff.bg_color = color;
	for (size_t i = 0;
		 i < framebuff.height * framebuff.pitch / sizeof(uint32_t); i++)
		framebuff.back_address[i] = color;
	framebuff.tex_x = 0;
	framebuff.tex_y = 0;
	framebuffer_swap(&framebuff);
}

void framebuffer_scroll(void) {
	framebuff.tex_y--;
	size_t row_size = framebuff.pitch * ISO_CHAR_HEIGHT / sizeof(uint32_t);
	size_t screen_size = framebuff.pitch * ISO_CHAR_HEIGHT *
						 (framebuff.height / ISO_CHAR_HEIGHT) /
						 sizeof(uint32_t);
	for (size_t i = 0; i < screen_size - row_size; i++) {
		framebuff.address[i] = framebuff.address[i + row_size];
		framebuff.address[i + row_size] = framebuff.bg_color;
	}
}

static void framebuffer_putc(char c, int x, int y) {
	x *= 8;
	y *= 16;
	uint8_t line;
	for (uint32_t x_bit = 0; x_bit < ISO_CHAR_HEIGHT; x_bit++) {
		line = framebuffer_font[c * 16 + x_bit];
		for (uint32_t y_bit = 0; y_bit <= 8; y_bit++) {
			if (line & (1 << (8 - y_bit - 1))) {
				if (framebuff.width * framebuff.height >
					(y + x_bit) * framebuff.width + x + y_bit)
					framebuffer_putpx(x + y_bit, y + x_bit, framebuff.tex_color,
									  true);
			}
		}
	}
}

void framebuffer_putchar(char c) {
	switch (c) {
		case '\n':
			framebuff.tex_x = 0;
			framebuff.tex_y++;
			if (framebuff.tex_y > framebuff.height / ISO_CHAR_HEIGHT - 1) {
				framebuffer_scroll();
			}
			return;
		case '\r':
			framebuff.tex_x = 0;
			return;
		case '\b':
			framebuff.tex_x--;
			return;
		case '\t':
			framebuff.tex_x += 5;
			return;
	}
	if (framebuff.tex_x * ISO_CHAR_WIDTH >= framebuff.width) {
		framebuff.tex_x = 0;
		framebuff.tex_y++;
	}
	framebuffer_putc(c, framebuff.tex_x, framebuff.tex_y);
	framebuff.tex_x++;
}

void framebuffer_puts(char *string) {
	while (*string != '\0') {
		framebuffer_putchar(*string++);
	}
}
