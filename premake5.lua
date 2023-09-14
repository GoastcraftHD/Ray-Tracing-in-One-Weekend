include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Ray Tracing in One Weekend"
	architecture "x86_64"
	startproject "Ray Tracing in One Weekend"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["glm"] = "%{wks.location}/Ray Tracing in One Weekend/vendor/glm"

group "Dependencies"
	include "vendor/premake"
group ""

include "Ray Tracing in One Weekend"
