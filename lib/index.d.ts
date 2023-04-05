declare module '*.wasm' {
	const value: any;
	export = value;
}

declare module '*libsqsh.js' {
	function load(...args: any): Module;
	class Module {
		cwrap(func: string, args: string[], ret: string): (...args: any) => any;
	}
}
