import { SqshObj, SqshResult } from './types.mjs';
import { CWrap } from './base.mjs';


export class ErrorHandler extends CWrap {
	#obj: SqshObj;

	#new = this._wrap('sqshjs_error_handler_new', 'SqshObj');
	#rv_addr = this._wrap('sqshjs_error_handler_rv_addr', 'SqshObj', 'SqshObj');
	#rv = this._wrap('sqshjs_error_handler_rv', 'SqshResult', 'SqshObj');
	#free = this._wrap('sqshjs_error_handler_free', 'SqshResult', 'SqshObj');
	#str = this._wrap('sqsh_error_str', 'string', 'SqshResult');

	constructor(parent: CWrap) {
		super(parent);

		this.#obj = this.#new();
		if (!this.#obj) {
			throw new Error('Failed to create error handler');
		}
	}

	get probe(): SqshObj {
		return this.#rv_addr(this.#obj);
	}

	success(): boolean {
		return this.#rv(this.#obj) == 0;
	}

	throw_if_err(rv: SqshResult = 0) {
		if (rv === 0 && this.success()) {
			return;
		}
		const err_str = this.#str(this.#rv(this.#obj));

		throw new Error(err_str);
	}

	cleanup() {
		this.#free(this.#obj);
	}
}
