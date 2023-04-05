import { SqshObj, SqshResult, CFunction, MappedCType, MappedCTypeTuple, CTypeId } from "./types.mjs";
import { Module } from "./libsqshjs.js";

function bailout(_: never): never {
	throw new Error("Should not happen");
}

type DTor = (obj: SqshObj) => SqshResult;
type DTorObj = { dtor: DTor, obj: SqshObj };
export abstract class CWrap {
	static #finalizer = new globalThis.FinalizationRegistry(({ dtor, obj }: DTorObj) => {
		dtor(obj);
	})

	_gc(dtor: DTor, obj: SqshObj) {
		const dtor_obj: DTorObj = { dtor, obj };
		CWrap.#finalizer.register(this, dtor_obj);
	}

	#mod: Module;
	#parent: CWrap;

	constructor(parent: CWrap | Module) {
		if ('_mod' in parent) {
			this.#parent = parent;
			this.#mod = parent._mod;
		} else {
			this.#parent = null;
			this.#mod = parent;
		}
	}

	get _mod(): Module {
		return this.#mod;
	}

	static #map_type(ctype: CTypeId) {
		switch (ctype) {
			case "string":
				return "string";
			case "number":
			case "SqshObj":
			case "SqshResult":
			case "Uint8Array":
				return "number";
			default:
				bailout(ctype);
		}
	}

	#wrap<A extends CTypeId[], R>
		(cfunc: string, ret: R & string, args: A, options: {}):
		CFunction<MappedCTypeTuple<A>, any> {

		const cArgs: CTypeId[] = args.map(x => CWrap.#map_type(x));
		return this._mod.cwrap(cfunc, ret, cArgs, options) as CFunction<MappedCTypeTuple<A>, any>;
	}

	_wrap<
		A extends CTypeId[],
		R extends CTypeId>
		(cfunc: string, ret: R, ...args: A):
		CFunction<MappedCTypeTuple<A>, MappedCType<R>> {
		return this.#wrap(cfunc, ret, args, { async: false });
	}

	_wrap_async<
		A extends CTypeId[],
		R extends CTypeId>
		(cfunc: string, ret: R, ...args: A):
		CFunction<MappedCTypeTuple<A>, Promise<MappedCType<R>>> {
		return this.#wrap(cfunc, ret, args, { async: true });
	}
}
