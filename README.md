# sqsh.js

squashfs is an open and free compressed read-only filesystem. It is used in embedded 
devices, live-CDs, or in packaging. It was original introduced into the kernel, 
but there are multiple user space implementations.

*sqsh.js* bundles a javascript API with the power of [libsqsh](https://github.com/Gottox/libsqsh)
to provide an interface to read squashfs archives from within the browser.

## State of development

sqsh.js is currently at a tech demo level and may never will get stable.

## How to build

install the emscripten sdk as described [here](https://emscripten.org/docs/getting_started/downloads.html)

```bash
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Download and install the latest SDK tools.
./emsdk install latest

# Make the "latest" SDK "active" for the current user. (writes .emscripten file)
./emsdk activate latest

# Activate PATH and other environment variables in the current terminal
source ./emsdk_env.sh

cd ..
```

Checkout and build the *sqsh.js* source:

```bash
git clone https://github.com/Gottox/sqsh.js.git

meson setup --cross-file emscripten.cross -Demsdk="../emsdk" ./build_dir

ninja -C ./build_dir compile
```
