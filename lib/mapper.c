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
 * @file         fetch_mapper.c
 */

#include <sqsh_mapper_private.h>

#include <emscripten/fetch.h>
#include <sqsh_data.h>
#include <sqsh_error.h>

/* clang-format off */
EM_JS(int, fetch_page,
		(const char *url, int range_begin, int range_end), {
	return Asyncify.handleSleep(async function(wakeUp) {
		const request = new Request(url, {
			method: 'GET',
			headers: {
				"Content-Range": "bytes " + range_begin + "-" + range_end + "/*"
			}
		});
		const response = await fetch(url);
		wakeUp(text);
	});
})
/* clang-format on */

static int
sqsh_mapper_fetch_init(
		struct SqshMapper *mapper, const void *input, size_t *size) {
	(void)mapper;
	(void)input;
	(void)size;
	return 0;
}

static int
sqsh_mapper_fetch_map(struct SqshMapSlice *mapping) {
	(void)mapping;
	return 0;
}

static int
sqsh_mapper_fetch_cleanup(struct SqshMapper *mapper) {
	(void)mapper;
	return 0;
}

static int
sqsh_mapping_fetch_unmap(struct SqshMapSlice *mapping) {
	(void)mapping;
	return 0;
}

static const uint8_t *
sqsh_mapping_fetch_data(const struct SqshMapSlice *mapping) {
	(void)mapping;
	return 0;
}

static const struct SqshMemoryMapperImpl impl = {
		// 40kb
		.block_size_hint = 40 * 1024,
		.init = sqsh_mapper_fetch_init,
		.map = sqsh_mapper_fetch_map,
		.cleanup = sqsh_mapper_fetch_cleanup,
		.map_data = sqsh_mapping_fetch_data,
		.unmap = sqsh_mapping_fetch_unmap,
};
const struct SqshMemoryMapperImpl *const sqsh_mapper_impl_fetch = &impl;
