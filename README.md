wxKinectHelper
==============

Microsoft Kinect SDK Helper Library and Sample Application using wxWidgets ToolKit for C++

<h3>How to Build</h3>
<h4>Build under Windows</h4>
In order to build the project under Windows you need the following:
* [Visual Studio 2012](http://www.microsoft.com/visualstudio/rus/products/visual-studio-express-products)
* [wxWidgets library](http://www.wxwidgets.org/develop/svn.htm) - SVN HEAD is fine
* [CMake](http://www.cmake.org/cmake/resources/software.html)
* [Microsoft Kinect SDK](http://www.microsoft.com/en-us/kinectforwindowsdev/default.aspx)

After you install the Visual Stuio, CMake and Kinect SDK, you need to ensure that you have `KINECTSDK10_DIR` environment variable in your system. You can do this by executing `set` command in console.
This variable should point to the directory where you have the Kinect SDK installed (e.g. C:\Program Files\Microsoft SDKs\Kinect\v1.8)

After that you need to build the wxWidgets library from the source code. Ensure that you have statically linked CRT in all your wxWidgets libs (in Project properties -> C++ -> Code Generation -> Runtime library settings you should have `Multi-Threaded` or `Multi-Threaded Debug` without `DLL`).

When you get wxWidgets library (Debug and Release version) compiled, you can go to `<KINECT_HELPER_SOURCE_ROOT>/build` folder and execute `cm.bat`. This script should generate the Visual Studio solution in `<KINECT_HELPER_SOURCE_ROOT>/build/Win` folder.
