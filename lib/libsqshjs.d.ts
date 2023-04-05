export function load(...args: any): Promise<Module>;
export class Module {
	cwrap(func: string, ret: string, args: string[], options?: Object): (...args: any) => any;
}
