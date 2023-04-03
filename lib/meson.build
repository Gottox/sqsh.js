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
	'mapper/fetch_mapper.c',
	'config.c',
]

sqsh_js_sources_ts = [
]

sqsh_js_dependencies = [
	libsqsh,
	emscripten,
]

sqsh_js_link_args = [
	'-sEXPORTED_FUNCTIONS=' + ','.join(exported_functions)
]

executable('sqsh', sqsh_js_sources_c,
	dependencies: sqsh_js_dependencies,
	link_args: sqsh_js_link_args
)
