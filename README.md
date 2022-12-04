# Ginuh Engine v0.1
A new open source 3D Game Engine made by two students at CITM: 

David Boces ([VoZeS](https://github.com/VoZeS))

Erik Martin ([eriik1212](https://github.com/eriik1212))

## Controls
Unity-like camera controls:
* While Right clicking, “WASD” fps-like movement and free look around must be enabled.
* Alt+Left click to orbit the object.
* F look at the selected game object.
* Holding SHIFT duplicates movement speed.

## Engine Functionality
### V0.1
* Street enviroment scene is automatically loaded.
* Engine accepts drag and drop of fbx files from anywhere in the hard drive. It accepts fbx, png.
* Console: the window LOG the geometry loading process from ASSIMP and the
external libraries initialization process.
* The engine has a screen to create basic shapes.
* Engine has a configuration screen that allows user to:
- See a graph for the frames per second and modify them.
- Configurate all variables on each module (renderer, window, input, textures).
- See information of output of memory consumption, hardware detection and software
versions (SDL, OpenGL, DevIL).
* Engine has an about screen.

### v0.2
* User can modify an object usimg inspector:
- Hierarchy: delete, reparent, create empty and create children
- Transform (translate, scale and rotate).
- Change the game object texture.
- Camera as a component that can be modified.
* GameObjects can be picked from the world using the mouse.
* All meshes use a bounding volume (AABB).
* The editor has 2 windows one with the scene(editor view) and another captured from a
gameobject with a camera component(main game camera).


## Third party Libraries Used:
* [SDL](https://github.com/libsdl-org/SDLS) 2.24.0.0
* [Glew](https://github.com/nigels-com/glew) 2.1.0.0
* [ImGui](https://github.com/ocornut/imgui) 1.88
* [JSON Parser](https://github.com/json-parser/json-parser) 1.1.0
* [MathGeoLib](https://github.com/juj/MathGeoLib) 1.15
* [Assimp](https://github.com/assimp/assimp) 3.1.1
* [DevIL](https://github.com/DentonW/DevIL) 1.8.0
* [Physfs](https://github.com/devkitPro/physfs) 3.0.2

## MIT License

Copyright (c) 2022 Erik Martín Garzón & David Boces Obis

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
