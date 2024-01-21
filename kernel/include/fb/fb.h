#ifndef FB_H
#define FB_H

/*
 * Copyright 2021 - 2023 NSG650
 * Copyright 2021 - 2023 Neptune
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

#include "../fb/terminal/flanterm.h"
#include <fs/devtmpfs.h>
#include <stddef.h>
#include <stdint.h>

struct framebuffer {
	uint32_t *address;
	uint32_t *back_address;
	size_t pitch, bpp;
	uint16_t width, height;
	size_t tex_x, tex_y;
	uint16_t tex_width, tex_height;
	uint32_t tex_color;
	uint32_t bg_color;
};

extern struct framebuffer framebuff;

void framebuffer_init(struct framebuffer *fb);
void framebuffer_set_callback(void (*callback)(struct flanterm_context *,
											   uint64_t, uint64_t, uint64_t,
											   uint64_t));
void framebuffer_clear(uint32_t foreground, uint32_t background);
void framebuffer_putchar(char c);
void framebuffer_puts(char *string);
struct framebuffer *framebuffer_info(void);

void fbdev_init(void);

#endif
