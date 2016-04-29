
LibOVRWrapper
=============

This is a library that wraps calls from applications that user older Oculus VR SDK versions to a supported version (1.3). Currently only 0.8->1.3 wrapping is supported but more versions will be added.

Help is welcome. My c++ skills are rusty and I don't have time to support this very well.

Tested to be working in:

* Most of the SDK samples
* Surge 1.2.2

*To developers:* Please do not just use this to bring 1.3 support to your program. Native 1.3 support is always better.

Installation
------------

Extract to a folder. Copy the dll files to where the wanted application sees them. 

To install globally:

Copy LibOVRRT32*.dll to c:\windows\syswow64
AND
Copy LibOVRRT64*.dll to c:\windows\system32

Or run the install .bat file as administrator to do it for you.

To-Do List, in decreasing priority
----------------------------------

* 0.7 support
* 0.6 support
* 0.5 support
* OpenGL support

Limitations
-----------

* (Small) performance hit due to SDK changes that need workarounds
* Probably no extended mode support ever
* Highly likely no dx9 support ever
* Smaller SDK details that are not possible anymore:
 * Maximum layer count is 16 instead of 32
 * Some layer types are not supported anymore
 
Building
--------

Open LibOVRWrapper.sln in Visual Studio 2015

License
-------

The Oculus VR SDK is licensed under the Oculus VR, LLC Software Development Kit License Agreement. See the latest version at https://developer.oculus.com/licenses/sdk-3.4/.
