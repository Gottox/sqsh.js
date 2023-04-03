project(
	'libsqsh.js',
	['c'],
	default_options : [
		'c_std=c99',
		'optimization=3',
		'warning_level=3',
		'default_library=static',

		'libsqsh:tools=false',
		'libsqsh:threads=disabled',
		'libsqsh:curl=disabled',
		'libsqsh:lzo=disabled',
		'libsqsh:fuse=disabled',
		'libsqsh:zstd=disabled', # broken

		'libsqsh:lzma=enabled',
		'libsqsh:zlib=enabled',
		'libsqsh:lz4=enabled',
	],
	version: '0.2.0',
	meson_version: '>=0.57',
	license: 'BSD-2-Clause',
)
project_description = 'a fresh squashfs implementation. BSD-2 licensed.'
project_author = 'Enno Boland <g@s01.de>'

npm = find_program('npm')

package_json_conf = configuration_data({
	'PACKAGE_NAME': meson.project_name(),
	'VERSION': meson.project_version(),
	'DESCRIPTION': project_description,
	'AUTHOR': project_author,
	'LICENSE': ', '.join([] + meson.project_license()),
})

package_json = configure_file(
	input: [ 'package.json.in' ],
	output: 'package.json',
	configuration: package_json_conf,
)

libsqsh_proj = subproject('libsqsh')

libsqsh = libsqsh_proj.get_variable('libsqsh_dependency')

emscripten = declare_dependency(
	include_directories: join_paths(
		get_option('emsdk'),
		'upstream/emscripten/cache/sysroot/include'
	)
)

subdir('lib')
