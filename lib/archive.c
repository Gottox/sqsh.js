#include <emscripten.h>
#include <sqsh_archive.h>
#include <sqsh_mapper.h>
#include <stdlib.h>

extern const struct SqshMemoryMapperImpl *const sqsh_mapper_impl_fetch;

EMSCRIPTEN_KEEPALIVE
struct SqshConfig *
sqshjs_config_new_memory(size_t size) {
	struct SqshConfig *config = calloc(1, sizeof(struct SqshConfig));
	if (config == NULL) {
		return NULL;
	}
	config->source_size = size;
	config->source_mapper = sqsh_mapper_impl_static;
	return config;
}

EMSCRIPTEN_KEEPALIVE
struct SqshConfig *
sqshjs_config_new_url(void) {
	struct SqshConfig *config = calloc(1, sizeof(struct SqshConfig));
	if (config == NULL) {
		return NULL;
	}
	config->source_mapper = sqsh_mapper_impl_fetch;
	return config;
}

EMSCRIPTEN_KEEPALIVE
void
sqshjs_config_free(struct SqshConfig *config) {
	free(config);
}
