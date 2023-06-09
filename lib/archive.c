/******************************************************************************
 *                                                                            *
 * Copyright (c) 2023, Enno Boland <g@s01.de>                                 *
 *                                                                            *
 * Redistribution and use in source and binary forms, with or without         *
 * modification, are permitted provided that the following conditions are     *
 * met:                                                                       *
 *                                                                            *
 * * Redistributions of source code must retain the above copyright notice,   *
 *   this list of conditions and the following disclaimer.                    *
 * * Redistributions in binary form must reproduce the above copyright        *
 *   notice, this list of conditions and the following disclaimer in the      *
 *   documentation and/or other materials provided with the distribution.     *
 *                                                                            *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS    *
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,  *
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR     *
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR          *
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,      *
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,        *
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR         *
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF     *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING       *
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         *
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               *
 *                                                                            *
 ******************************************************************************/

/**
 * @author       Enno Boland (mail@eboland.de)
 * @file         archive.c
 */

#include <emscripten.h>
#include <sqsh_archive.h>
#include <sqsh_mapper.h>
#include <stdlib.h>
#include <string.h>

extern const struct SqshMemoryMapperImpl *const sqsh_mapper_impl_fetch;

EMSCRIPTEN_KEEPALIVE
struct SqshConfig *
sqshjs_config_new(void) {
	struct SqshConfig *config = calloc(1, sizeof(struct SqshConfig));
	if (config == NULL) {
		return NULL;
	}
	return config;
}

EMSCRIPTEN_KEEPALIVE
int
sqshjs_config_set_source_mapper(struct SqshConfig *config, char *mapper) {
#define OPTION(o) \
	if (strcmp(#o, mapper) == 0) { \
		config->source_mapper = sqsh_mapper_impl_ ## o; \
		return 0; \
	}
	OPTION(static);
	OPTION(mmap);
	OPTION(fetch);

	return -1;
#undef OPTION
}

EMSCRIPTEN_KEEPALIVE
int
sqshjs_config_set(struct SqshConfig *config, char *name, uintptr_t value) {
#define OPTION(c, o) \
	if (strcmp(name, #o) == 0) { \
		config->o = (c)value; \
		return 0; \
	}

	OPTION(void *, source_mapper)
	OPTION(int, mapper_lru_size)
	OPTION(int, mapper_block_size)
	OPTION(int, compression_lru_size)

	return -1;
#undef OPTION
}

EMSCRIPTEN_KEEPALIVE
void
sqshjs_config_free(struct SqshConfig *config) {
	free(config);
}
