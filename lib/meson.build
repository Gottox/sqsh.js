exported_functions = [
	'_sqsh_archive_new',
	'_sqsh_archive_config',
	'_sqsh_archive_superblock',
	'_sqsh_archive_id_table',
	'_sqsh_archive_export_table',
	'_sqsh_archive_fragment_table',
	'_sqsh_archive_xattr_table',
	'_sqsh_archive_data_extractor',
	'_sqsh_archive_metablock_extractor',
	'_sqsh_archive_map_manager',
	'_sqsh_archive_free',
]

sqsh_js_sources_c = [
	'mapper.c',
	'archive.c',
]

sqsh_js_sources_ts = [
	'archive.ts',
	'lib.ts',
]

sqsh_js_dependencies = [
	libsqsh,
	emscripten,
]

sqsh_js_build_sources_ts = []
sqsh_js_build_targets_js = []
foreach source: sqsh_js_sources_ts
	configure_file(
		input: source,
		output: source,
		configuration: {},
	)
	sqsh_js_build_sources_ts += '"' + source + '"' 
	sqsh_js_build_targets_js += source.split('.')[0] + '.js'
endforeach

tsconfig_conf = configuration_data({
	'SQSH_JS_SOURCES': ','.join(sqsh_js_build_sources_ts),
	'MESON_BUILD_DIR': meson.current_build_dir(),
})

tsconfig = configure_file(
	input: [ 'tsconfig.json.in' ],
	output: 'tsconfig.json',
	configuration: tsconfig_conf,
)

sqsh_js_link_args = [
	'-sEXPORTED_FUNCTIONS=' + ','.join(exported_functions),
	'-sEXPORTED_RUNTIME_METHODS=ccall,cwrap',
]

tsc_target = custom_target('tsc',
	input : [ npm_install_target, sqsh_js_sources_ts, tsconfig ],
	command : [
		npm, 'run',
		'tsc', '--',
		'--project', meson.current_build_dir(),
		'--outDir', meson.current_build_dir()
	],
	output : sqsh_js_build_targets_js,
	build_by_default: true
)

executable('sqsh_wasm', sqsh_js_sources_c,
	dependencies: [ sqsh_js_dependencies ],
	link_args: sqsh_js_link_args
)
