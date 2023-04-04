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

		'libsqsh:lzma=disabled',
		'libsqsh:zlib=disabled',
		'libsqsh:lz4=disabled',

		'lzma:threads=disabled',
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

npm_install_target = custom_target('npm_install',
	output : 'node_modules',
	input : package_json,
	command : [npm, 'install'],
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

webpack_config = configure_file(
	input: [ 'webpack.config.js.in' ],
	output: 'webpack.config.js',
	configuration: {
		'MESON_BUILD_ROOT': meson.current_build_dir(),
		'WEBPACK_MODE': get_option('debug') ? 'development' : 'production'
	},
)

sqsh_js = custom_target('sqsh.js',
	input : tsc_target,
	command : [
		npm, 'run',
		'webpack', '--', 'build',
		'-c', webpack_config,
	],
	output : 'sqsh.js',
	build_by_default: true
)

configure_file(
	input: [ 'index.html.in' ],
	output: 'index.html',
	configuration: { },
)
