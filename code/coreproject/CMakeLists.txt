#-------------------------------------------------------------------------------
# coreproject
#-------------------------------------------------------------------------------

# glob the source files and add them to folders in visual studio
FILE(GLOB SRC "gamestates/*.cc" "gamestates/*.cpp")
FILE(GLOB HEAD "gamestates/*.h")
LIST(APPEND SRCSGAMESTATES ${SRC} ${HEAD})
SOURCE_GROUP("coreproject\\gamestates" FILES ${SRCSGAMESTATES})

FILE(GLOB SRC "*.cc" "*.cpp")
FILE(GLOB HEAD "*.h")
LIST(APPEND SRCSBASE ${SRC} ${HEAD})
SOURCE_GROUP("coreproject" FILES ${SRCSBASE})

# define precompiled headers
SET(PCH_files	
    ../stdneb.cc
)
SOURCE_GROUP("Precompiled" FILES ${PCH_files})

LIST(APPEND ALLSRCS ${SRCSBASE} ${SRCSGAMESTATES} ${PCH_files})

ADD_EXECUTABLE(coreproject ${ALLSRCS})
SET_TARGET_PROPERTIES(coreproject PROPERTIES FOLDER "Core-Project")

# link to application project, will pull in all dependencies and includes required
TARGET_LINK_LIBRARIES(coreproject application_features ui)
ADD_DEPENDENCIES(coreproject application_features ui)

# apply default compiler settings, has to be done to be consistent throughout
N_SET_COMPILER_DEFAULTS()
N_SET_EXE_LINKER_DEFAULTS(coreproject)
N_SET_EXE_SUBSYSTEM(coreproject Windows)

# setup precompiled headers if chosen at project level
IF(N_USE_PRECOMPILED_HEADERS)
	N_SET_PCH(coreproject  stdneb.h stdneb.cc ../)
ENDIF()