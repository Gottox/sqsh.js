/**
 * @class       : config
 * @author      : Enno Boland (mail@eboland.de)
 * @created     : Tuesday Apr 04, 2023 11:56:59 CEST
 * @description : config
 */

import { ErrorHandler } from './error.mjs'
import { SqshObj } from './types.mjs'
import { CWrap } from './base.mjs'
import { load, Module } from './libsqshjs.js'
import wasm from './libsqshjs.wasm'

export interface Config {
	mapper_block_size?: number,
	mapper_lru_size?: number,
	compression_lru_size?: number,
}

export interface UrlConfig extends Config {
	url: string
}

export interface BufferConfig extends Config {
	data: Uint8Array
}

export async function open(config: BufferConfig | UrlConfig | string) {
	const mod = await load({
		locateFile(path: string) {
			if (path.endsWith(`.wasm`)) {
				return wasm;
			}
			return path;
		},
	});

	const archive = new ArchiveWrap(mod);
	return await archive.open(config);
}

class ConfigWrap extends CWrap {
	#obj: SqshObj = null;
	#source: Uint8Array;

	#new = this._wrap("sqshjs_config_new", 'SqshResult');

	#set = this._wrap("sqshjs_config_set", 'SqshResult', 'SqshObj', 'string', 'number');

	#set_source_mapper = this._wrap("sqshjs_config_set_source_mapper", 'SqshResult', 'SqshObj', 'string');

	#free = this._wrap("sqshjs_config_free", 'SqshResult', 'SqshObj');

	constructor(parent: CWrap, config: BufferConfig | UrlConfig | string) {
		super(parent);

		if (typeof config === 'string') {
			config = { url: config };
		}

		const c_config = this.#new();
		for (const name of Object.keys(config)) {
			this.#set(c_config, name, config[name]);
		}

		if ('url' in config) {
			this.#set_source_mapper(c_config, "fetch");
			this.#source = new TextEncoder().encode(`${config.url}\u0000`);
		} else {
			this.#set_source_mapper(c_config, "static");
			this.#source = config.data;
		}
		this.#set(c_config, "source_size", this.#source.length);

		this._gc(this.#free, this.#obj);
		this.#free(c_config);
	}

	get obj() {
		return this.#obj;
	}

	get source() {
		return this.#source
	}
}

export class ArchiveWrap extends CWrap {
	#obj: SqshObj = null;

	#new_fetch = this._wrap_async("sqsh_archive_new", 'number', 'string', 'number', 'number');
	#new_static = this._wrap_async("sqsh_archive_new", 'number', 'Uint8Array', 'number', 'number');

	#free = this._wrap("sqsh_archive_free", 'SqshResult', 'SqshObj');

	constructor(mod: Module) {
		super(mod);
	}

	async open(config: BufferConfig | UrlConfig | string) {
		const err = new ErrorHandler(this);

		const config_wrap = new ConfigWrap(this, config);

		if (config_wrap.source instanceof Uint8Array)
			this.#obj = await this.#new_static(config_wrap.source, config_wrap.obj, err.probe);
		else
			this.#obj = await this.#new_fetch(config_wrap.source, config_wrap.obj, err.probe);
		err.throw_if_err();
		this._gc(this.#free, this.#obj);
		return this;
	}
}
