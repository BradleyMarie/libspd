# libSPD

[![Test Status](https://github.com/BradleyMarie/libspd/actions/workflows/c-cpp.yml/badge.svg?branch=main)](https://github.com/BradleyMarie/libspd/actions/workflows/c-cpp.yml)
[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://github.com/BradleyMarie/libspd/master/LICENSE)

A zero-dependency SPD file reader for C++23. While there is no formal definition
of the SPD format, informal documentation can be found in the source code of the
[PBRT renderer](https://github.com/mmp/pbrt-v4/blob/39e01e61f8de07b99859df04b271a02a53d9aeb2/src/pbrt/util/spectrum.cpp#L106)
as well as in the [SPD files](https://github.com/mmp/pbrt-v4-scenes/blob/30cf4a0346ae5a80a2d7a530a3ef7d0fa4f70572/killeroos/spds/Au.k.spd#L4) in its example scenes.

At a high level the format can be described as an ASCII file containing a list
of whitespace separated floating point numbers where the numbers alternate in
significance between representing a wavelength and representing a spectral
power.

## Getting Started

libSPD uses Bazel as its build system. If you are using Bazel as well, you can
import libSPD into your workspace by adding a snippet like the following into
your `WORKSPACE` file.

```
http_archive(
    name = "libspd",
    sha256 = "9c906a7764db5747ff19642ecdaab6c181da1d81c5db6691bc5cc5ef311c0db7",
    strip_prefix = "libspd-9d970474acae30a6bb0efc28d8b995b204ca30e2",
    url = "https://github.com/BradleyMarie/libspd/archive/9d970474acae30a6bb0efc28d8b995b204ca30e2.zip",
)
```

Note: You should update `url` and `strip_prefix` to point to the latest commit
on the main branch and should also update `sha256` with the checksum from that
snapshot.

libSPD code is structured with the core modules residing in the `libspd`
directory. `spd_reader` contains the parent `SpdReader` class that contains the
logic for parsing out the contents of an SPD file. This class does very little
validation of its own other than ensuring that tokens are parsable into floats,
that there are an even number of tokens, and that line endings are consistent
(which is required in order to properly delineate where a comment ends).

The `SpdReader` class is designed for extension and exposes a small public API
as well as a small protected API that derived classes must implement.

Also inside the `libspd` directory is the `readers` directory. This directory
contains pre-implemented readers for SPD files that do more validation than the
base `SpdReader` class and reduce the amount of code clients would need to
implement.

Currently, this extra validation is provided by `ValidatingSpdReader` which
layers on top of `SpdReader` class and validates that wavelengths are 
finite/non-zero/non-negative, that spectral power values are finite/non-negative,
and that no wavelength has been sampled more than once. The class also handles
converting samples into the client's desired precision since `SpdReader` always
parses values into the maximum precision supported by the system. 

Also residing in the `readers` directory are the library functions 
`ReadEmissiveSpdFrom` and `ReadReflectiveSpdFrom`. These functions are built
using `ValidatingSpdReader` and provide the simplest interface for reading an
SPD file taking as inputs a binary-stream and returning back an ordered map from
wavelength to spectral power of the samples from the file in the client's
desired precision. The only difference between the two functions is that
`ReadEmissiveSpdFrom` will successfully spectral powers greater than one while
`ReadReflectiveSpdFrom` will return an error.

## Examples

Currently, there is no example code written for libSPD; however, since
libSPD has good unit test coverage you can use the test code as a reference
for how to work with the library.

Additionally, the code in the `readers` directory can be used as a reference
for working with the `SpdReader` class directly.

It is expected that most clients will simply use either `ReadEmissiveSpdFrom` or
`ReadReflectiveSpdFrom` and will avoid the added complexity of working with
`SpdReader` or `ValidatingSpdReader` directly.

## Versioning

libSPD currently is not strongly versioned and it is recommended that users
update to the latest commit from the main branch as often as is reasonably
possible.

The public API of `SpdReader` and `ValidatingSpdReader` should be stable at this
point; however, it is possible that future commits may introduce breaking API
changes.