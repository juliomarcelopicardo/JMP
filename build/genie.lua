-- GENIe solution to build the JMP Solution.
-- Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
-- 

PROJ_DIR = path.getabsolute("../")

solution "JMP_Solution"
  configurations {
    "Release",
    "Debug",
  }
  language "C++"
  platforms { "x32", "x64" }

  configuration "Debug"
    flags { "Symbols" }
  	targetsuffix "-d"
  	links {}
    defines { "_DEBUG", }

  configuration "Release"
    flags { "OptimizeSize" }
  	links {}
    defines { "NDEBUG" }

  project("JMP")
  kind "ConsoleApp" 

  includedirs {
    path.join(PROJ_DIR, "include/"),
  }

  files {
    path.join(PROJ_DIR, "src/*.*"),
    path.join(PROJ_DIR, "include/*.*"),
  }

  configuration "Debug"
    targetdir ("../bin/")
    links {}

  configuration "Release"
    targetdir ("../bin/")
    links {}
