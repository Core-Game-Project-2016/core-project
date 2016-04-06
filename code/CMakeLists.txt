#-------------------------------------------------------------------------------
# Demos
#-------------------------------------------------------------------------------

OPTION(N_BUILD_CORE_PROJECT "Core Project" ON)

if(N_BUILD_CORE_PROJECT)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SUBDIRS(
coreproject
)
ENDIF()
