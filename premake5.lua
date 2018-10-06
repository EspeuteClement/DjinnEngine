-- Build this file for standalone non reloading game.
-- Use build.bat for reloading engine

workspace "Djinn_Engine"
	configurations { "Debug32", "Release32", "Debug64", "Release64" }

project "Djinn_Engine"
	kind "ConsoleApp"
	language "C"
	targetdir "bin/%{cfg.buildcfg}"
	
	location ("projects/" .. _ACTION)

	filter "configurations:*32"
		architecture "x86"
		libdirs { "engine/sdl/lib/x86", os.findlib("SLD2")}


	filter "configurations:*64"
		architecture "x86_64"
		libdirs { "engine/sdl/lib/x64", os.findlib("SLD2")}
	
	filter {}

	files {"engine/djn_unity_build.c"}
	removefiles {"**/tcc/**"}

	includedirs { "engine/sdl/include", "engine" }

	defines {"DJN_NO_RELOAD", "DJN_NO_IMGUI", "SDL_MAIN_HANDLED"}

	links {"SDL2"}
	


	
	filter "configurations:Debug*"
	  defines { "DEBUG" }
	  debugdir "."
	  symbols "On"

	filter "configurations:Release*"
	  defines { "NDEBUG" }
	  optimize "On"

	filter {"action:gmake"}
		if os.execute("clang -v") == 0 then
			toolset "clang"
		end