######################################################################
# @author      : Enno Boland (mail@eboland.de)
# @file        : Makefile
# @created     : Sunday Dec 12, 2021 09:18:43 CET
######################################################################

NINJA := ninja
MESON := meson

NINJA_TARGETS := all test

BUILD_DIR = build_dir

$(NINJA_TARGETS): $(BUILD_DIR)
	$W $(NINJA) -C $< $@

update-db:
	meson wrap update-db

$(BUILD_DIR): meson.build Makefile emscripten.cross
	[ -d "$@" ] && rm -rf "$@" || true
	$W CC=$(CC) $(MESON) setup --cross-file emscripten.cross $(MESON_FLAGS) "$@"

.PHONY: clean

clean:
	rm -rf "$(BUILD_DIR)"
