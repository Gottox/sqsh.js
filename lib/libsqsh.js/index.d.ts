function load(...args: any): Module;
class Module {
	cwrap(func: string, args: string[], ret: string): (...args: any) => any;
}
