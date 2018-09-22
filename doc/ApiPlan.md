# Plans for the API

## Introduction
All commands are prefixed by their category (draw, sound, ...), and prefixed by the parameter list type.

### Parameter list suffixes :
- `<nothing>` the simplest version of the function, that takes the least amount of paramters and which is the one most people will use
- `*_adv` an advanced version of the function, which takes more parameters
- `*_pro` the ultimate version of the function, that takes the most parameters

Certains categories can have other suffixes that add some behavior to the function

## Resources
- `res_*` is the prefix for all low level resource manipulation
	- `res_options_*`
		-`res_options_autoload`	set the handling of autoloading of resources if they are not loaded, forcing you to better manage your memory. 3 modes : load (load resources if they are not loaded), assert (crash if the resource is not loaded), silent (doesn't crash if resources aren't loaded). 
	- `res_texture_*` is the prefix for texture management
		- `res_texture_preload` load a texture onto ram and vram if it's not loaded already
		- `res_texture_unload` unload a texture
	- `res_sound_*` is the prefix for audio management
		- `res_sound_preload` load a sound into memory
		- `res_sound_unload` unload a previously loaded memory

## Graphical drawing
- `draw_*` is the prefix for all these calls
	- `draw_sprite` Simplest way to draw a sprite
		- `draw_sprite_rot` Draw a sprite with rotation
		- `draw_sprite_scl` Draw a sprite with scale x,y
		- `draw_sprite_col` Draw a sprite tinted
		- `draw_sprite_adv` Draw a sprite with rot and scale and color
	- `draw_anim(anim_id, x,y,frame*)`	Simplest way to draw an anmiation. Updates frame with the standard framerate
		- `draw_anim_adv(anim_id, x,y,frame*, timescale)` Draw an anim, and updates the graphics by timescale


## Animation
- `anim_` is the prefix for all that is linked to animation management
	- `anim_get_sprite(anim_id, frame)` Get the sprite corresponding to the frame of the anim
	- `anim_update(anim_id, frame*, timescale)` Updates frame by the amount given in timescale. Also return the previous frame of the anim
