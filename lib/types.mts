export type SqshObj = number;
export type SqshResult = 0 | -1;
interface CTypes {
	"string": string;
	"number": number;
	"SqshObj": SqshObj;
	"SqshResult": SqshResult;
	"Uint8Array": Uint8Array;
}
export type CTypeId = keyof CTypes;
export type MappedCType<T> = T extends keyof CTypes ? CTypes[T] : never;
export type MappedCTypeTuple<T> = T extends (keyof CTypes)[] ? {
	[K in keyof T]: MappedCType<T[K]>;
} : never;

export type CFunction<A, R> = A extends [...(infer A)] ? (...a: A) => R : never;
