# Packaging google-cloud-cpp-spanner

<!-- This is an automatically generated file -->
<!-- Make changes in ci/test-markdown/generate-packaging.sh -->

This document is intended for package maintainers or anyone else who might like
to "install" the google-cloud-cpp-spanner library.

There are two primary ways of obtaining `google-cloud-cpp-spanner`. You can use git:

```bash
git clone https://github.com/googleapis/google-cloud-cpp-spanner.git $HOME/google-cloud-cpp-spanner
```

Or obtain the tarball release:

```bash
mkdir -p $HOME/google-cloud-cpp-spanner
wget -q https://github.com/googleapis/google-cloud-cpp-spanner/archive/v0.8.0.tar.gz
tar -xf v0.8.0.tar.gz -C $HOME/google-cloud-cpp-spanner --strip=1
```

# Installing google-cloud-cpp-spanner

When built with Bazel or as a CMake super build `google-cloud-cpp-spanner`
downloads and compiles all its dependencies ([see below](#required-libraries)
for a complete list). This makes it easier for users to "take the library for
a spin", and works well for users that "Live at Head", but does not work for
package maintainers or users that prefer to compile their dependencies once
and install them in `/usr/local/` or a similar directory.

This document provides instructions to install the dependencies of
`google-cloud-cpp-spanner`.

**If** all the dependencies of `google-cloud-cpp-spanner` are installed and
provide CMake support files, then compiling and installing the libraries
requires two commands:

```bash
cmake -H. -Bcmake-out
cmake --build cmake-out --target install
```

You may choose to parallelize the build by appending `-- -j ${NCPU}` to the
build command, where `NCPU` is an environment variable set to the number of
processors on your system. On Linux, you can obtain this information using the
`nproc` command or `sysctl -n hw.physicalcpu` on macOS.

Unfortunately getting your system to this state may require multiple steps,
the following sections describe how to install `google-cloud-cpp-spanner` on
several platforms.

## Required Libraries

`google-cloud-cpp-spanner` directly depends on the following libraries:

| Library | Minimum version | Description |
| ------- | --------------: | ----------- |
| gRPC    | 1.16.x | gRPC++ |

Note that these libraries may also depend on other libraries. The following
instructions include steps to install these indirect dependencies too.

When possible, the instructions below prefer to use pre-packaged versions of
these libraries and their dependencies. In some cases the packages do not exist,
or the package versions are too old to support `google-cloud-cpp`. If this is
the case, the instructions describe how you can manually download and install
these dependencies.

## Table of Contents

- [Fedora 30](#fedora-30)
- [openSUSE (Leap)](#opensuse-leap)
- [Ubuntu (18.04 - Bionic Beaver)](#ubuntu-1804---bionic-beaver)
- [Ubuntu (16.04 - Xenial Xerus)](#ubuntu-1604---xenial-xerus)
- [Debian (10 Buster)](#debian-10---buster)
- [Debian (9 Stretch)](#debian-9---stretch)
- [CentOS 8](#centos-8)
- [CentOS 7](#centos-7)

### Fedora (30)

Install the minimal development tools:

```bash
sudo dnf makecache && \
sudo dnf install -y cmake gcc-c++ git make openssl-devel pkgconfig \
        zlib-devel
```

Fedora 30 includes packages for gRPC, libcurl, and OpenSSL that are recent
enough for the project. Install these packages and additional development
tools to compile the dependencies:

```bash
sudo dnf makecache && \
sudo dnf install -y grpc-devel grpc-plugins \
        libcurl-devel protobuf-compiler tar wget zlib-devel
```

The following steps will install libraries and tools in `/usr/local`. By
default pkg-config does not search in these directories.

```bash
export PKG_CONFIG_PATH=/usr/local/lib64/pkgconfig
```

#### googleapis

We need a recent version of the Google Cloud Platform proto C++ libraries:

```bash
cd $HOME/Downloads
wget -q https://github.com/googleapis/cpp-cmakefiles/archive/v0.6.0.tar.gz && \
    tar -xf v0.6.0.tar.gz && \
    cd cpp-cmakefiles-0.6.0 && \
    cmake -DBUILD_SHARED_LIBS=YES -H. -Bcmake-out && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### google-cloud-cpp-common

The project also depends on google-cloud-cpp-common, the libraries shared by
all the Google Cloud C++ client libraries:

```bash
cd $HOME/Downloads
wget -q https://github.com/googleapis/google-cloud-cpp-common/archive/v0.24.0.tar.gz && \
    tar -xf v0.24.0.tar.gz && \
    cd google-cloud-cpp-common-0.24.0 && \
    cmake -H. -Bcmake-out -DBUILD_TESTING=OFF && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### Compile and install the main project

We can now compile, test, and install `google-cloud-cpp-spanner`.

```bash
cd $HOME/project
cmake -H. -Bcmake-out -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-out -- -j "${NCPU:-4}"
sudo cmake --build cmake-out --target install
```


### openSUSE (Leap)

Install the minimal development tools, libcurl and OpenSSL. The gRPC Makefile
uses `which` to determine whether the compiler is available. Install this
command for the extremely rare case where it may be missing from your
workstation or build server.

```bash
sudo zypper refresh && \
sudo zypper install --allow-downgrade -y automake cmake gcc gcc-c++ git gzip \
        libcurl-devel libopenssl-devel libtool make tar wget which
```

The following steps will install libraries and tools in `/usr/local`. openSUSE
does not search for shared libraries in these directories by default, there
are multiple ways to solve this problem, the following steps are one solution:

```bash
(echo "/usr/local/lib" ; echo "/usr/local/lib64") | \
sudo tee /etc/ld.so.conf.d/usrlocal.conf
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:/usr/local/lib64/pkgconfig
export PATH=/usr/local/bin:${PATH}
```

#### Protobuf

We need to install a version of Protobuf that is recent enough to support the
Google Cloud Platform proto files:

```bash
cd $HOME/Downloads
wget -q https://github.com/google/protobuf/archive/v3.11.3.tar.gz && \
    tar -xf v3.11.3.tar.gz && \
    cd protobuf-3.11.3/cmake && \
    cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=yes \
        -Dprotobuf_BUILD_TESTS=OFF \
        -H. -Bcmake-out && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### c-ares

Recent versions of gRPC require c-ares >= 1.11, while openSUSE/Leap
distributes c-ares-1.9. Manually install a newer version:

```bash
cd $HOME/Downloads
wget -q https://github.com/c-ares/c-ares/archive/cares-1_14_0.tar.gz && \
    tar -xf cares-1_14_0.tar.gz && \
    cd c-ares-cares-1_14_0 && \
    ./buildconf && ./configure && make -j ${NCPU:-4} && \
sudo make install && \
sudo ldconfig
```

#### gRPC

We also need a version of gRPC that is recent enough to support the Google
Cloud Platform proto files. We manually install it using:

```bash
cd $HOME/Downloads
wget -q https://github.com/grpc/grpc/archive/78ace4cd5dfcc1f2eced44d22d752f103f377e7b.tar.gz && \
    tar -xf 78ace4cd5dfcc1f2eced44d22d752f103f377e7b.tar.gz && \
    cd grpc-78ace4cd5dfcc1f2eced44d22d752f103f377e7b && \
    make -j ${NCPU:-4} && \
sudo make install && \
sudo ldconfig
```

#### googleapis

We need a recent version of the Google Cloud Platform proto C++ libraries:

```bash
cd $HOME/Downloads
wget -q https://github.com/googleapis/cpp-cmakefiles/archive/v0.6.0.tar.gz && \
    tar -xf v0.6.0.tar.gz && \
    cd cpp-cmakefiles-0.6.0 && \
    cmake -DBUILD_SHARED_LIBS=YES -H. -Bcmake-out && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### google-cloud-cpp-common

The project also depends on google-cloud-cpp-common, the libraries shared by
all the Google Cloud C++ client libraries:

```bash
cd $HOME/Downloads
wget -q https://github.com/googleapis/google-cloud-cpp-common/archive/v0.24.0.tar.gz && \
    tar -xf v0.24.0.tar.gz && \
    cd google-cloud-cpp-common-0.24.0 && \
    cmake -H. -Bcmake-out -DBUILD_TESTING=OFF && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### Compile and install the main project

We can now compile, test, and install `google-cloud-cpp-spanner`.

```bash
cd $HOME/project
cmake -H. -Bcmake-out -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-out -- -j "${NCPU:-4}"
sudo cmake --build cmake-out --target install
```


### Ubuntu (18.04 - Bionic Beaver)

Install the minimal development tools, libcurl, OpenSSL and libc-ares:

```bash
apt-get update && \
    apt-get --no-install-recommends install -y apt-transport-https apt-utils \
        automake build-essential cmake ca-certificates git gcc g++ cmake \
        libc-ares-dev libc-ares2 libcurl4-openssl-dev libssl-dev m4 make \
        pkg-config tar wget zlib1g-dev
```

#### Protobuf

We need to install a version of Protobuf that is recent enough to support the
Google Cloud Platform proto files:

```bash
cd $HOME/Downloads
wget -q https://github.com/google/protobuf/archive/v3.11.3.tar.gz && \
    tar -xf v3.11.3.tar.gz && \
    cd protobuf-3.11.3/cmake && \
    cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=yes \
        -Dprotobuf_BUILD_TESTS=OFF \
        -H. -Bcmake-out && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### gRPC

We also need a version of gRPC that is recent enough to support the Google
Cloud Platform proto files. We install it using:

```bash
cd $HOME/Downloads
wget -q https://github.com/grpc/grpc/archive/78ace4cd5dfcc1f2eced44d22d752f103f377e7b.tar.gz && \
    tar -xf 78ace4cd5dfcc1f2eced44d22d752f103f377e7b.tar.gz && \
    cd grpc-78ace4cd5dfcc1f2eced44d22d752f103f377e7b && \
    make -j ${NCPU:-4} && \
sudo make install && \
sudo ldconfig
```

#### googleapis

We need a recent version of the Google Cloud Platform proto C++ libraries:

```bash
cd $HOME/Downloads
wget -q https://github.com/googleapis/cpp-cmakefiles/archive/v0.6.0.tar.gz && \
    tar -xf v0.6.0.tar.gz && \
    cd cpp-cmakefiles-0.6.0 && \
    cmake -DBUILD_SHARED_LIBS=YES -H. -Bcmake-out && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### google-cloud-cpp-common

The project also depends on google-cloud-cpp-common, the libraries shared by
all the Google Cloud C++ client libraries:

```bash
cd $HOME/Downloads
wget -q https://github.com/googleapis/google-cloud-cpp-common/archive/v0.24.0.tar.gz && \
    tar -xf v0.24.0.tar.gz && \
    cd google-cloud-cpp-common-0.24.0 && \
    cmake -H. -Bcmake-out -DBUILD_TESTING=OFF && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### Compile and install the main project

We can now compile, test, and install `google-cloud-cpp-spanner`.

```bash
cd $HOME/project
cmake -H. -Bcmake-out -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-out -- -j "${NCPU:-4}"
sudo cmake --build cmake-out --target install
```


### Ubuntu (16.04 - Xenial Xerus)

Install the minimal development tools, OpenSSL and libcurl:

```bash
apt-get update && \
    apt-get --no-install-recommends install -y apt-transport-https apt-utils \
        automake build-essential cmake ca-certificates git gcc g++ cmake \
        libcurl4-openssl-dev libssl-dev libtool m4 make \
        pkg-config tar wget zlib1g-dev
```

#### Protobuf

We need to install a version of Protobuf that is recent enough to support the
Google Cloud Platform proto files:

```bash
cd $HOME/Downloads
wget -q https://github.com/google/protobuf/archive/v3.11.3.tar.gz && \
    tar -xf v3.11.3.tar.gz && \
    cd protobuf-3.11.3/cmake && \
    cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=yes \
        -Dprotobuf_BUILD_TESTS=OFF \
        -H. -Bcmake-out && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### c-ares

Recent versions of gRPC require c-ares >= 1.11, while Ubuntu-16.04
distributes c-ares-1.10. Manually install a newer version:

```bash
cd $HOME/Downloads
wget -q https://github.com/c-ares/c-ares/archive/cares-1_14_0.tar.gz && \
    tar -xf cares-1_14_0.tar.gz && \
    cd c-ares-cares-1_14_0 && \
    ./buildconf && ./configure && make -j ${NCPU:-4} && \
sudo make install && \
sudo ldconfig
```

#### gRPC

We also need a version of gRPC that is recent enough to support the Google
Cloud Platform proto files. We can install gRPC from source using:

```bash
cd $HOME/Downloads
wget -q https://github.com/grpc/grpc/archive/78ace4cd5dfcc1f2eced44d22d752f103f377e7b.tar.gz && \
    tar -xf 78ace4cd5dfcc1f2eced44d22d752f103f377e7b.tar.gz && \
    cd grpc-78ace4cd5dfcc1f2eced44d22d752f103f377e7b && \
    make -j ${NCPU:-4} && \
sudo make install && \
sudo ldconfig
```

#### googleapis

We need a recent version of the Google Cloud Platform proto C++ libraries:

```bash
cd $HOME/Downloads
wget -q https://github.com/googleapis/cpp-cmakefiles/archive/v0.6.0.tar.gz && \
    tar -xf v0.6.0.tar.gz && \
    cd cpp-cmakefiles-0.6.0 && \
    cmake -DBUILD_SHARED_LIBS=YES -H. -Bcmake-out && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### google-cloud-cpp-common

The project also depends on google-cloud-cpp-common, the libraries shared by
all the Google Cloud C++ client libraries:

```bash
cd $HOME/Downloads
wget -q https://github.com/googleapis/google-cloud-cpp-common/archive/v0.24.0.tar.gz && \
    tar -xf v0.24.0.tar.gz && \
    cd google-cloud-cpp-common-0.24.0 && \
    cmake -H. -Bcmake-out -DBUILD_TESTING=OFF && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### Compile and install the main project

We can now compile, test, and install `google-cloud-cpp-spanner`.

```bash
cd $HOME/project
cmake -H. -Bcmake-out -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-out -- -j "${NCPU:-4}"
sudo cmake --build cmake-out --target install
```


### Debian (10 - Buster)

Install the minimal development tools, libcurl, and OpenSSL:

```bash
apt-get update && \
    apt-get --no-install-recommends install -y apt-transport-https apt-utils \
        automake build-essential ca-certificates cmake git gcc g++ cmake \
        libc-ares-dev libc-ares2 libcurl4-openssl-dev libssl-dev m4 make \
        pkg-config tar wget zlib1g-dev
```

Debian 10 includes versions of gRPC and Protobuf that support the
Google Cloud Platform proto files. We simply install these pre-built versions:

```bash
apt-get update && \
    apt-get --no-install-recommends install -y libgrpc++-dev libprotobuf-dev libc-ares-dev \
        protobuf-compiler protobuf-compiler-grpc
```

#### googleapis

We need a recent version of the Google Cloud Platform proto C++ libraries:

```bash
cd $HOME/Downloads
wget -q https://github.com/googleapis/cpp-cmakefiles/archive/v0.6.0.tar.gz && \
    tar -xf v0.6.0.tar.gz && \
    cd cpp-cmakefiles-0.6.0 && \
    cmake -DBUILD_SHARED_LIBS=YES -H. -Bcmake-out && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### google-cloud-cpp-common

The project also depends on google-cloud-cpp-common, the libraries shared by
all the Google Cloud C++ client libraries:

```bash
cd $HOME/Downloads
wget -q https://github.com/googleapis/google-cloud-cpp-common/archive/v0.24.0.tar.gz && \
    tar -xf v0.24.0.tar.gz && \
    cd google-cloud-cpp-common-0.24.0 && \
    cmake -H. -Bcmake-out -DBUILD_TESTING=OFF && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### Compile and install the main project

We can now compile, test, and install `google-cloud-cpp-spanner`.

```bash
cd $HOME/project
cmake -H. -Bcmake-out -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-out -- -j "${NCPU:-4}"
sudo cmake --build cmake-out --target install
```


### Debian (9 - Stretch)

First install the development tools and libcurl.
On Debian 9, libcurl links against openssl-1.0.2, and one must link
against the same version or risk an inconsistent configuration of the library.
This is especially important for multi-threaded applications, as openssl-1.0.2
requires explicitly setting locking callbacks. Therefore, to use libcurl one
must link against openssl-1.0.2. To do so, we need to install libssl1.0-dev.
Note that this removes libssl-dev if you have it installed already, and would
prevent you from compiling against openssl-1.1.0.

```bash
apt-get update && \
    apt-get --no-install-recommends install -y apt-transport-https apt-utils \
        automake build-essential cmake ca-certificates git gcc g++ cmake libc-ares-dev \
        libc-ares2 libcurl4-openssl-dev libssl1.0-dev make m4 pkg-config tar \
        wget zlib1g-dev
```

#### Protobuf

We need to install a version of Protobuf that is recent enough to support the
Google Cloud Platform proto files:

```bash
cd $HOME/Downloads
wget -q https://github.com/google/protobuf/archive/v3.11.3.tar.gz && \
    tar -xf v3.11.3.tar.gz && \
    cd protobuf-3.11.3/cmake && \
    cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=yes \
        -Dprotobuf_BUILD_TESTS=OFF \
        -H. -Bcmake-out && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### gRPC

To install gRPC we first need to configure pkg-config to find the version of
Protobuf we just installed in `/usr/local`:

```bash
cd $HOME/Downloads
wget -q https://github.com/grpc/grpc/archive/78ace4cd5dfcc1f2eced44d22d752f103f377e7b.tar.gz && \
    tar -xf 78ace4cd5dfcc1f2eced44d22d752f103f377e7b.tar.gz && \
    cd grpc-78ace4cd5dfcc1f2eced44d22d752f103f377e7b && \
    make -j ${NCPU:-4} && \
sudo make install && \
sudo ldconfig
```

#### googleapis

We need a recent version of the Google Cloud Platform proto C++ libraries:

```bash
cd $HOME/Downloads
wget -q https://github.com/googleapis/cpp-cmakefiles/archive/v0.6.0.tar.gz && \
    tar -xf v0.6.0.tar.gz && \
    cd cpp-cmakefiles-0.6.0 && \
    cmake -DBUILD_SHARED_LIBS=YES -H. -Bcmake-out && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### google-cloud-cpp-common

The project also depends on google-cloud-cpp-common, the libraries shared by
all the Google Cloud C++ client libraries:

```bash
cd $HOME/Downloads
wget -q https://github.com/googleapis/google-cloud-cpp-common/archive/v0.24.0.tar.gz && \
    tar -xf v0.24.0.tar.gz && \
    cd google-cloud-cpp-common-0.24.0 && \
    cmake -H. -Bcmake-out -DBUILD_TESTING=OFF && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### Compile and install the main project

We can now compile, test, and install `google-cloud-cpp-spanner`.

```bash
cd $HOME/project
cmake -H. -Bcmake-out -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-out -- -j "${NCPU:-4}"
sudo cmake --build cmake-out --target install
```


### CentOS (8)

Install the minimal development tools, libcurl, OpenSSL, and the c-ares
library (required by gRPC):

```bash
sudo dnf makecache && \
sudo dnf install -y cmake gcc-c++ git make openssl-devel pkgconfig \
        zlib-devel libcurl-devel c-ares-devel tar wget which
```

The following steps will install libraries and tools in `/usr/local`. By
default CentOS-8 does not search for shared libraries in these directories,
there are multiple ways to solve this problem, the following steps are one
solution:

```bash
(echo "/usr/local/lib" ; echo "/usr/local/lib64") | \
sudo tee /etc/ld.so.conf.d/usrlocal.conf
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:/usr/local/lib64/pkgconfig
export PATH=/usr/local/bin:${PATH}
```

#### Protobuf

We need to install a version of Protobuf that is recent enough to support the
Google Cloud Platform proto files:

```bash
cd $HOME/Downloads
wget -q https://github.com/google/protobuf/archive/v3.11.3.tar.gz && \
    tar -xf v3.11.3.tar.gz && \
    cd protobuf-3.11.3/cmake && \
    cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=yes \
        -Dprotobuf_BUILD_TESTS=OFF \
        -H. -Bcmake-out && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### gRPC

We also need a version of gRPC that is recent enough to support the Google
Cloud Platform proto files. We manually install it using:

```bash
cd $HOME/Downloads
wget -q https://github.com/grpc/grpc/archive/78ace4cd5dfcc1f2eced44d22d752f103f377e7b.tar.gz && \
    tar -xf 78ace4cd5dfcc1f2eced44d22d752f103f377e7b.tar.gz && \
    cd grpc-78ace4cd5dfcc1f2eced44d22d752f103f377e7b && \
    make -j ${NCPU:-4} && \
sudo make install && \
sudo ldconfig
```

#### googleapis

We need a recent version of the Google Cloud Platform proto C++ libraries:

```bash
cd $HOME/Downloads
wget -q https://github.com/googleapis/cpp-cmakefiles/archive/v0.6.0.tar.gz && \
    tar -xf v0.6.0.tar.gz && \
    cd cpp-cmakefiles-0.6.0 && \
    cmake -DBUILD_SHARED_LIBS=YES -H. -Bcmake-out && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### google-cloud-cpp-common

The project also depends on google-cloud-cpp-common, the libraries shared by
all the Google Cloud C++ client libraries:

```bash
cd $HOME/Downloads
wget -q https://github.com/googleapis/google-cloud-cpp-common/archive/v0.24.0.tar.gz && \
    tar -xf v0.24.0.tar.gz && \
    cd google-cloud-cpp-common-0.24.0 && \
    cmake -H. -Bcmake-out -DBUILD_TESTING=OFF && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### Compile and install the main project

We can now compile, test, and install `google-cloud-cpp-spanner`.

```bash
cd $HOME/project
cmake -H. -Bcmake-out -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-out -- -j "${NCPU:-4}"
sudo cmake --build cmake-out --target install
```


### CentOS (7)

First install the development tools and OpenSSL. The development tools
distributed with CentOS 7 (notably CMake) are too old to build
the project. In these instructions, we use `cmake3` obtained from
[Software Collections](https://www.softwarecollections.org/).

```bash
sudo rpm -Uvh https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
sudo yum install -y centos-release-scl yum-utils
sudo yum-config-manager --enable rhel-server-rhscl-7-rpms
sudo yum makecache && \
sudo yum install -y automake cmake3 curl-devel gcc gcc-c++ git libtool \
        make openssl-devel pkgconfig tar wget which zlib-devel
sudo ln -sf /usr/bin/cmake3 /usr/bin/cmake && sudo ln -sf /usr/bin/ctest3 /usr/bin/ctest
```

The following steps will install libraries and tools in `/usr/local`. By
default CentOS-7 does not search for shared libraries in these directories,
there are multiple ways to solve this problem, the following steps are one
solution:

```bash
(echo "/usr/local/lib" ; echo "/usr/local/lib64") | \
sudo tee /etc/ld.so.conf.d/usrlocal.conf
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:/usr/local/lib64/pkgconfig
export PATH=/usr/local/bin:${PATH}
```

#### Protobuf

We need to install a version of Protobuf that is recent enough to support the
Google Cloud Platform proto files:

```bash
cd $HOME/Downloads
wget -q https://github.com/google/protobuf/archive/v3.11.3.tar.gz && \
    tar -xf v3.11.3.tar.gz && \
    cd protobuf-3.11.3/cmake && \
    cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=yes \
        -Dprotobuf_BUILD_TESTS=OFF \
        -H. -Bcmake-out && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### c-ares

Recent versions of gRPC require c-ares >= 1.11, while CentOS-7
distributes c-ares-1.10. Manually install a newer version:

```bash
cd $HOME/Downloads
wget -q https://github.com/c-ares/c-ares/archive/cares-1_14_0.tar.gz && \
    tar -xf cares-1_14_0.tar.gz && \
    cd c-ares-cares-1_14_0 && \
    ./buildconf && ./configure && make -j ${NCPU:-4} && \
sudo make install && \
sudo ldconfig
```

#### gRPC

We also need a version of gRPC that is recent enough to support the Google
Cloud Platform proto files. We manually install it using:

```bash
cd $HOME/Downloads
wget -q https://github.com/grpc/grpc/archive/78ace4cd5dfcc1f2eced44d22d752f103f377e7b.tar.gz && \
    tar -xf 78ace4cd5dfcc1f2eced44d22d752f103f377e7b.tar.gz && \
    cd grpc-78ace4cd5dfcc1f2eced44d22d752f103f377e7b && \
    make -j ${NCPU:-4} && \
sudo make install && \
sudo ldconfig
```

#### googleapis

We need a recent version of the Google Cloud Platform proto C++ libraries:

```bash
cd $HOME/Downloads
wget -q https://github.com/googleapis/cpp-cmakefiles/archive/v0.6.0.tar.gz && \
    tar -xf v0.6.0.tar.gz && \
    cd cpp-cmakefiles-0.6.0 && \
    cmake -DBUILD_SHARED_LIBS=YES -H. -Bcmake-out && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### google-cloud-cpp-common

The project also depends on google-cloud-cpp-common, the libraries shared by
all the Google Cloud C++ client libraries:

```bash
cd $HOME/Downloads
wget -q https://github.com/googleapis/google-cloud-cpp-common/archive/v0.24.0.tar.gz && \
    tar -xf v0.24.0.tar.gz && \
    cd google-cloud-cpp-common-0.24.0 && \
    cmake -H. -Bcmake-out -DBUILD_TESTING=OFF && \
    cmake --build cmake-out -- -j ${NCPU:-4} && \
sudo cmake --build cmake-out --target install -- -j ${NCPU:-4} && \
sudo ldconfig
```

#### Compile and install the main project

We can now compile, test, and install `google-cloud-cpp-spanner`.

```bash
cd $HOME/project
cmake -H. -Bcmake-out -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-out -- -j "${NCPU:-4}"
sudo cmake --build cmake-out --target install
```

