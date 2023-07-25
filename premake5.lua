include "Dependencies.lua"

workspace "RayKitect"
    architecture "x64"
    startproject "RayKitect"

    configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "RayKitect/vendor/GLFW"
	include "RayKitect/vendor/glad"
	include "RayKitect/vendor/imgui"
group ""

include "RayKitect"