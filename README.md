# Interfacing KDB+ with C

The PDF documentation for this resource can be found on the [AquaQ Analytics][aquaqresources]
website.

# Installation & Setup

This project uses CMake 2.6+ to build across multiple platforms. It has been tested on Linux,
Windows and Mac OSX. Execute the following commands on all platforms to create platform
appropriate build files within the `build` directory.

    mkdir build; cd build; cmake ..

On Linux and Mac OSX platforms, you just need to run make install to complete the build process
and find the binary output in the `bin` directory.

        make install

# Running the Examples
Once the build is complete, navigate to the `bin` directory and execute:

    q makeprint.q

This will load the C shared object and bind the functions to names (*make* and *print*). Instructions
and example commands should be displayed as soon as the the makeprint.q script loads.

# Other Resources

This resource is intended to suppliment the existing Kx Wiki sections:

+ [Interfacing With C][kxwikiinterface]
+ [Extending With C][kxwikiextend]

[aquaqwebsite]: http://www.aquaq.co.uk
[aquaqresources]: http://www.aquaq.co.uk/resources
[kxwikiinterface]: http://code.kx.com/wiki/Cookbook/InterfacingWithC
[kxwikiextend]: http://code.kx.com/wiki/Cookbook/ExtendingWithC