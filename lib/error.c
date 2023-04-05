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
 * @file         error.c
 */

#include "emscripten/em_macros.h"
#include <emscripten.h>
#include <sqsh_archive.h>
#include <sqsh_mapper.h>
#include <stdlib.h>
#include <string.h>

struct SqshErrorHandler {
	int rv;
};

EMSCRIPTEN_KEEPALIVE struct SqshErrorHandler *
sqshjs_error_handler_new(void) {
	return calloc(1, sizeof(struct SqshErrorHandler));
}

EMSCRIPTEN_KEEPALIVE
int *
sqshjs_error_handler_rv_addr(struct SqshErrorHandler *error_handler) {
	return &error_handler->rv;
}

EMSCRIPTEN_KEEPALIVE
int
sqshjs_error_handler_rv(struct SqshErrorHandler *error_handler) {
	return error_handler->rv;
}

EMSCRIPTEN_KEEPALIVE
int
sqshjs_error_handler_free(struct SqshErrorHandler *error_handler) {
	free(error_handler);
	return 0;
}
