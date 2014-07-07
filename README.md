Land of Dreams
===

Simple attempt of drawing big terrains in OpenGL with CDLOD. The project also involves character animation with GPU skinning. It uses my c++ OpenGL wrapper: oglwrap. It aims Unix platforms (it doesn't use anything platform specific, but it only has build tools for Unix).

**This is just an educational project.** I don't own the rights for every media in the application.

Some screenshots (might not be up to date):
![screenshot](screenshot.png)

Dependencies:
-------------
* C++11 compiler.
* OpenGL 2.1, Shader model 120
* GLEW - for loading OpenGL extensions
* [oglwrap](https://github.com/Tomius/oglwrap) - my C++ OpenGL wrapper.
* GLFW3 - for window handling
* Magick++ - for image loading.
* Assimp - for loading 3D models.
* FreeType2 - for text rendering (via freetype-gl).
* Bullet - for physics (isn't fully integrated yet).


How to build (Debian/Ubuntu):
--------------------
```
mkdir LandOfDreams && cd LandOfDreams && sudo apt-get install libmagick++-dev libglew-dev libassimp-dev libbullet-dev && git clone https://github.com/glfw/glfw.git && cd glfw && cmake . && sudo make -j4 install && cd .. && git clone --recursive https://github.com/Tomius/LoD.git && cd LoD && make -j4 && ./LoD
```

How to build (linux):
---------------------
* get the external dependencies: libmagick++-dev libglew-dev libassimp-dev libbullet-dev [libglfw3-dev](https://github.com/glfw/glfw)
* initialize the oglwrap submodule: git submodule init && git submodule update
* build with make (uses clang++), run with ./LoD

Acknowledgements
================
* *Darkz* for useful feedback and help with the fixing of several bugs.
* *Kristóf Marussy (kris7t)* for a lot of useful advices and catching many implementation-dependent bugs.

Inspirations, resources and tools:
---------------------------
* [World Machine](http://www.world-machine.com) - Used for heightmap generation
* [X-blades](http://www.x-blades.com/) - Borrowed Ayumi's character design from this game
* [Autodesk Maya](http://www.autodesk.com/products/autodesk-maya/overview) && [Blender](http://www.blender.org/) - Used for modelling and animating 3D assets.
* [Oglplus sky example](http://oglplus.org/oglplus/html/oglplus_2023_sky_8cpp-example.html) - My skybox is based on this one.
* [Konserian highlands](http://opengameart.org/content/konserian-highlands-set) - Used the trees from this 3D art pack
* [Gamasutra fast blur artice](http://www.gamasutra.com/view/feature/3102/four_tricks_for_fast_blurring_in_.php) - The mipmap based DoF was inspired by this.
* [Filip Strugar's CDLOD paper](http://www.vertexasylum.com/downloads/cdlod/cdlod_latest.pdf) - Implemented the current Level of Detail algorithm for the terrain using this paper (with some changes).

----------------------
If you have any problem, please post in the issues tab or mail me at icyplusplus@gmail.com. Any feedback would be appreciated.
