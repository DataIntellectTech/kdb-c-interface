# Interfacing KDB+ with C

This project demonstrates how to marshall data between kdb+ and C using the Kx provided interface. A shared
object is built as the output and a q script is provided that will show how to load the C functions into kdb+
dynamically.

The PDF documentation for this resource can be found [here][gitpdfdoc] and also on the [AquaQ Analytics][aquaqresources]
website.

Installation & Setup
--------------------

### Extra Resources
You will need to download the `k.h` header file on all platforms

This project uses CMake 2.6+ to build across multiple platforms. It has been tested on Linux and
Windows. Execute the following commands on all platforms to create platform appropriate build
files within the `build` directory.

```sh
mkdir build; cd build; cmake ..
```

### <img src="docs/icons/linux.png" height="16px"> Building on Linux

On Linux, you just need to run make install to complete the build process
and find the binary output in the `../bin` directory.

```sh
make install && cd ../bin
```

### <img src="docs/icons/windows.png" height="16px"> Building on Windows

On Windows platforms you will need to have the msbuild.exe available on your path. CMake creates
two Visual Studio projects that need to be built. The `INSTALL` project will not modify any of the
code and will just move the binaries from the `../build` directory to the `../bin` directory. An
extra `libqtoc.lib` file will be produced on Windows, which can be ignored after the build process.

```bat
msbuild ./ALL_BUILD.vcxproj /p:Configuration=Release
msbuild ./INSTALL.vcxproj /p:Configuration=Release
cd ../bin
```

Running the Examples
--------------------

The resulting directory after running a build should look like this:

    bin/                    -- contains the libqtoc.[dll/so] library and makeprint.q
    build/                  -- contains the makefile/visual studio projects
    src/                    -- contains the source code
    CMakeLists.txt

Once the build is complete, navigate to the `bin` directory and execute:

    q makeprint.q

This will load the C shared object and bind the functions to names (*make* and *print*). Instructions
and example commands should be displayed as soon as the the makeprint.q script loads.

```apl
##
# makeprint.q
#
# Example q script that loads in two functions from the libqtoc shared object
# and makes them available as 'make' and 'print'.
#
# AquaQ Analytics
# kdb+ consultancy, training and support
#
# For questions, comments, requests or bug reports, please contact us
# w: www.aquaq.co.uk
# e: support@aquaq.co.uk
#
# examples:
#       print[1b] to print an atom
#       print[101b] to print a list
#       print[`a`b`c!1 2 3] to print a dictionary]
#       print[([] a:til 5; b:reverse `char$65+til 5)] to print a table
#
#       make[`b] to create a atom
#       make[`B] to create a list
#       make[`dictionary] to create a dictionary
#       make[`table] to create a table
q) print[`a`b`c!1 2 3]
a   | 1
b   | 2
c   | 3
q) make[`M]
2011.12 2014.05 2019.05 2018.07 2012.09m
```

Other Resources
---------------

This resource is intended to suppliment the existing Kx Wiki sections on interfacting with C
and provide some concrete examples. Readers should look at the following pages on the Kx Wiki:

* [Interfacing With C][kxwikiinterface]
* [Extending With C][kxwikiextend]

[aquaqwebsite]: http://www.aquaq.co.uk  "AquaQ Analytics Website"
[aquaqresources]: http://www.aquaq.co.uk/resources "AquaQ Analytics Website Resources"
[gitpdfdoc]: https://github.com/markrooney/kdb-c-interface/blob/master/docs/InterfacingWithC.pdf
[kxwikiinterface]: http://code.kx.com/wiki/Cookbook/InterfacingWithC "Kx Wiki Interfacing with C"
[kxwikiextend]: http://code.kx.com/wiki/Cookbook/ExtendingWithC "Kx Wiki Extending with C"