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
 * @file         mapper.c
 */

#include <sqsh_mapper_private.h>

#include <emscripten/fetch.h>
#include <sqsh_data.h>
#include <sqsh_error.h>

/* clang-format off */
EM_ASYNC_JS(int, fetch_download,
		(const char *url, int offset, uint8_t *data, size_t size), {
	url = UTF8ToString(url);
	const range_regex = new RegExp("^bytes ([0-9]+)-([0-9]+)/([0-9]+)$");
	const end_offset = offset + size - 1;
	const req_opt = {
		method: 'GET',
		headers: {
			"Content-Range": "bytes=" + offset + "-" + end_offset,
			"Accept-Encoding": "identity"
		}
	};
	try {
		const res = await fetch(url, req_opt);
		const content_range = range_regex.exec(res.headers.get("Content-Range"));
		if (!content_range || Number(content_range[1]) !== offset) {
			return -1;
		}
		const file_size = Number(content_range[3]);
		const res_data = new Uint8Array(await res.arrayBuffer());
		const target = HEAPU8.subarray(data, data + size);
		
		target.set(res_data);
		return file_size;
	} catch(e) {
		console.log(e);
		return -1;
	}
})
/* clang-format on */

static int
sqsh_mapper_fetch_init(
		struct SqshMapper *mapper, const void *input, size_t *size) {
	(void)size;
	int rv = 0;

	const char *url = input;
	mapper->data.cl.url = url;

	const size_t block_size = sqsh__mapper_block_size(mapper);

	uint8_t *header = calloc(block_size, sizeof(uint8_t));
	if (header == NULL) {
		rv = -SQSH_ERROR_MAPPER_INIT;
		goto out;
	}
	mapper->data.cl.header_cache = header;
	rv = fetch_download(url, 0, header, block_size);
	if (rv < 0) {
		goto out;
	}
	*size = rv;
	rv = 0;

out:
	return rv;
}

static int
sqsh_mapper_fetch_map(struct SqshMapSlice *mapping) {
	const sqsh_index_t offset = mapping->offset;
	const size_t size = mapping->size;
	int rv = 0;
	uint64_t file_size = 0;
	uint64_t file_time = 0;

	const char *url = mapping->mapper->data.cl.url;
	if (offset == 0 && mapping->mapper->data.cl.header_cache != NULL) {
		mapping->data = mapping->mapper->data.cl.header_cache;
		mapping->mapper->data.cl.header_cache = NULL;
	} else {
		uint8_t *data = calloc(size, sizeof(uint8_t));
		rv = fetch_download(
				url, offset, data, file_size);
		if (rv < 0) {
			goto out;
		}
		mapping->data = data;
		file_size = rv;
		rv = 0;

		if (file_time != mapping->mapper->data.cl.expected_time) {
			rv = -SQSH_ERROR_MAPPER_MAP;
			goto out;
		}

		if (file_size != sqsh__mapper_size(mapping->mapper)) {
			rv = -SQSH_ERROR_MAPPER_MAP;
			goto out;
		}
	}

out:
	if (rv < 0) {
		sqsh__map_slice_cleanup(mapping);
	}
	return rv;
}

static int
sqsh_mapper_fetch_cleanup(struct SqshMapper *mapper) {
	(void)mapper;
	return 0;
}

static int
sqsh_mapping_fetch_unmap(struct SqshMapSlice *mapping) {
	free(mapping->data);
	return 0;
}

static const uint8_t *
sqsh_mapping_fetch_data(const struct SqshMapSlice *mapping) {
	return mapping->data;
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
