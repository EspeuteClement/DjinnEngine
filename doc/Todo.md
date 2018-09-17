# TODO/Feature list

## Important
[X] Live code reload with TCC
[X] Free memory if size/layout has changed
[X] SDL init
	*Open a sdl context from the back-end*
	*Open a opengl context and gives you the api*
	[X] Open a window from the engine
	[X] Opengl loading

[X] Game loop
	*Game loop that is controlled by the engine*
	*Use a delta-timed update system, but is designed to run in a constant frame-rate loop*
	*User provides various callback functions that the engine will call*
	*If no function is given, then default ones will be used*
	*These default one will function as a base for simple code*
	[X] init user function
	[X] loop user function
	[X] render user function

[.] Resources system :
	*The idea is to create a system that is akin to Gamemaker resources*
	*Each resource (sprites, audio ...) will be accessible by typing it's name without the extension*
	*This is archived by creating a unique header res.h that contains all these resources as constants id*
	*The engine will then use those ids as identifiers for all of its functions*
	*Example : draw_sprite(sprite_id, ...)*
	[ ] Preprocessor that create unique ID from resources
	[ ] Automatic spritesheet batching
	[ ] Sprite animation system akin to gamemaker, but : **sprite** is a **single image**, **anim** is a collection of **sprites**. Used via specific api.
	[.] Manager that handle the loading and unloading of game resources
		*This manager will load textures in memory as needed, like gamemaker*
		[X] Load and unload textures if needed
		[ ] Don't reload each texture at each game reload
			*Not needed at the moment, resource loading is instant*

[.] Custom static memory allocators (forbid the use of malloc in game code)
[X] Find a simple way to send the game api symbols without linking to it, so game context stays intact



## For the future
[ ] Add "frontend" with global keyboard hook
[ ] signal handling so game never crashes
	*This seems very unlikely. Signals seems very fragile*
[X] cImGui support
	*The port is a little buggy right now thanks to the minors incompatibilities between Visual and libtcc. Oh well ...*
[ ] Save/Load game via callback so progress isn't lost if memory is cleared

## Sidesteps
[ ] "Unified" Build System
	*Allow the building of the engine _minus_ the reload code in other compilers*
	*This will require the code to not rely on features from the TCC compiler*
	*This also means that the tools can run in a standalone mode*
	*Maybe use a single file compilation to ease the deployment to other platforms*
	[ ] Visual Studio target for debug
		*Thanks to some careful programming, using visual studio as a debugging platform should only be used in last resort*
	[ ] Emscripten target for web
	[ ] Linux build