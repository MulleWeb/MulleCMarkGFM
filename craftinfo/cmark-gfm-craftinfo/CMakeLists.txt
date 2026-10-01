cmake_minimum_required( VERSION 3.13...99.99)

project( cmark-gfm-craftinfo LANGUAGES NONE)


# install build script, if any into "${DEPENDENCY_DIR}/bin"

file( GLOB SCRIPTS
   bin/cmark-gfm-build
   bin/cmark-gfm-build.*
)

list( FILTER SCRIPTS EXCLUDE REGEX ".*[.]example")

if( SCRIPTS)
	install( FILES ${SCRIPTS} DESTINATION "bin"
	         PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
	                     GROUP_EXECUTE GROUP_READ
	                     WORLD_EXECUTE WORLD_READ
	)
endif()

# install craftinfos

file( GLOB DEFINITION_DIRS
   definition*
)

if( DEFINITION_DIRS)
	foreach( DIR ${DEFINITION_DIRS} )
	   install( DIRECTORY "${DIR}" DESTINATION "share/mulle-craft/cmark-gfm"
	            USE_SOURCE_PERMISSIONS
	            PATTERN "*._DS_Store" EXCLUDE
	   )
	endforeach()
else()
   message( SEND_ERROR "Remove craftinfo \"cmark-gfm\" as it is empty with:\n   mulle-sourcetree remove craftinfo/cmark-gfm\n   rm -rf craftinfo/cmark-gfm")
endif()
