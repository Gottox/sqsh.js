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
	'_sqsh_error_str',
]

sqsh_js_sources_c = [
	'mapper.c',
	'archive.c',
	'error.c'
]

sqsh_js_sources_ts = [
	'index.d.ts',
	'libsqshjs.d.ts',
	'base.mts',
	'types.mts',
	'archive.mts',
	'lib.mts',
	'error.mts',
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
		copy: true,
	)
	sqsh_js_build_sources_ts += '"' + source + '"' 
	sqsh_js_build_targets_js += source.split('.')[0] + '.mjs'
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

sqsh_js_c_args = [
]

sqsh_js_link_args = [
	'-sMODULARIZE',
	'-sEXPORT_NAME=load',
	'-sENVIRONMENT=web',
	'-sFILESYSTEM=0',
	'-sASYNCIFY',
	'-sASYNCIFY_ADVISE=1',
	'-sEXPORTED_FUNCTIONS=' + ','.join(exported_functions),
	'-sEXPORTED_RUNTIME_METHODS=cwrap',
]

if get_option('debug')
	sqsh_js_link_args += [
		'-gsource-map',
		'-sSAFE_HEAP=1',
		'-sSTACK_OVERFLOW_CHECK=1',
	]
	sqsh_js_c_args += [
		'-gsource-map'
	]
	# Hack to make source maps work
	foreach source : sqsh_js_sources_c
		configure_file(
			input: source,
			output: source,
			copy: true
		)
	endforeach
endif

libsqshjs = executable('libsqshjs', sqsh_js_sources_c,
	dependencies: [ sqsh_js_dependencies ],
	link_args: sqsh_js_link_args,
	c_args: sqsh_js_c_args
)

tsc_target = custom_target('tsc',
	input : [ npm_install_target, sqsh_js_sources_ts, tsconfig, libsqshjs ],
	command : [
		npm, 'run',
		'tsc', '--',
		'--project', meson.current_build_dir(),
		'--outDir', meson.current_build_dir()
	],
	output : sqsh_js_build_targets_js,
	build_by_default: true
)
