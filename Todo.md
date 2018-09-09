# TODO/Feature list

## Important
[X] Live code reload with TCC
[X] Free memory if size/layout has changed
[ ] SDL init
	*Open a sdl context from the backend*
	*Open a opengl context and gives you the api*
	[ ] Open a window from the engine
	[ ] Opengl loading

[ ] Game loop
	*Game loop that is controlled by the engine*
	*Use a delta-timed update system, but is designed to run in a constant framerate loop*
	*User provides various callback functions that the engine will call*
	*If no function is given, then default ones will be used*
	*These default one will function as a base for simple code*
	[ ] init user function
	[ ] loop user function
	[ ] render user function

[ ] Ressources system :
	*The idea is to create a system that is akin to Gamemaker resources*
	*Each resource (sprites, audio ...) will be accesible by typing it's name without the extention*
	*This is archieved by creating a unique header res.h that contains all these resources as constants id*
	*The engine will then use thoese ids as identifiers for all of its functions*
	*Example : draw_sprite(sprite_id, ...)*
	[ ] Preprocessor that create unique ID from resources
	[ ] Automatic spritesheet batching
	[ ] Sprite animation system akin to gamemaker, but : **sprite** is a **single image**, **anim** is a collection of **sprites**. Used via specific api.

[ ] Custom static memory allocators (forbid the use of malloc in game code)
[ ] Find a simple way to send the game api symbols without linking to it, so game context stays intact



## For the future
[ ] Add "frontend" with global keyboard hook
[ ] signal handling so game never crashes
[ ] cImGui support
[ ] Save/Load game via callback so progress isn't lost if memory is cleared

## Sidesteps
[ ] Visual Studio target for debug
[ ] Emscripten target for web
[ ] Linux build