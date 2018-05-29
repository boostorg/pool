Pool, part of collection of the [Boost C++ Libraries](http://github.com/boostorg), provides an efficient way to handle memory suballocation for fixed-size items.

### License

Distributed under the [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt).

### Properties

* C++03
* Requires linking against Boost.Thread, Boost.System if BOOST_POOL_NO_MT is not defined, otherwise header-only (see [trac-7085](https://svn.boost.org/trac10/ticket/7085))

### Build Status

Branch          | Travis | Appveyor | Coverity Scan | codecov.io | Deps | Docs | Tests |
:-------------: | ------ | -------- | ------------- | ---------- | ---- | ---- | ----- |
[`master`](https://github.com/boostorg/pool/tree/master) | [![Build Status](https://travis-ci.org/boostorg/pool.svg?branch=master)](https://travis-ci.org/boostorg/pool) | [![Build status](https://ci.appveyor.com/api/projects/status/64lm86703kct8cpu/branch/master?svg=true)](https://ci.appveyor.com/project/jeking3/pool-nof9u/branch/master) | [![Coverity Scan Build Status](https://scan.coverity.com/projects/15800/badge.svg)](https://scan.coverity.com/projects/boostorg-pool) | [![codecov](https://codecov.io/gh/boostorg/pool/branch/master/graph/badge.svg)](https://codecov.io/gh/boostorg/pool/branch/master)| [![Deps](https://img.shields.io/badge/deps-master-brightgreen.svg)](https://pdimov.github.io/boostdep-report/master/pool.html) | [![Documentation](https://img.shields.io/badge/docs-master-brightgreen.svg)](http://www.boost.org/doc/libs/master/doc/html/pool.html) | [![Enter the Matrix](https://img.shields.io/badge/matrix-master-brightgreen.svg)](http://www.boost.org/development/tests/master/developer/pool.html)
[`develop`](https://github.com/boostorg/pool/tree/develop) | [![Build Status](https://travis-ci.org/boostorg/pool.svg?branch=develop)](https://travis-ci.org/boostorg/pool) | [![Build status](https://ci.appveyor.com/api/projects/status/64lm86703kct8cpu/branch/develop?svg=true)](https://ci.appveyor.com/project/jeking3/pool-nof9u/branch/develop) | [![Coverity Scan Build Status](https://scan.coverity.com/projects/15800/badge.svg)](https://scan.coverity.com/projects/boostorg-pool) | [![codecov](https://codecov.io/gh/boostorg/pool/branch/develop/graph/badge.svg)](https://codecov.io/gh/boostorg/pool/branch/develop) | [![Deps](https://img.shields.io/badge/deps-develop-brightgreen.svg)](https://pdimov.github.io/boostdep-report/develop/pool.html) | [![Documentation](https://img.shields.io/badge/docs-develop-brightgreen.svg)](http://www.boost.org/doc/libs/develop/doc/html/pool.html) | [![Enter the Matrix](https://img.shields.io/badge/matrix-develop-brightgreen.svg)](http://www.boost.org/development/tests/develop/developer/pool.html)

### Directories

| Name        | Purpose                        |
| ----------- | ------------------------------ |
| `doc`       | documentation                  |
| `example`   | examples                       |
| `include`   | headers                        |
| `test`      | unit tests                     |

### More information

* [Ask questions](http://stackoverflow.com/questions/ask?tags=c%2B%2B,boost,boost-pool)
* [Report bugs](https://github.com/boostorg/pool/issues): Be sure to mention Boost version, platform and compiler you're using. A small compilable code sample to reproduce the problem is always good as well.
* Submit your patches as pull requests against **develop** branch. Note that by submitting patches you agree to license your modifications under the [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt).
* Discussions about the library are held on the [Boost developers mailing list](http://www.boost.org/community/groups.html#main). Be sure to read the [discussion policy](http://www.boost.org/community/policy.html) before posting and add the `[pool]` tag at the beginning of the subject line.

