project(vrdlib.common)

aux_source_directory(source PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_link_libraries(${PROJECT_NAME} 
	vrdlib.api 
	vrdlib.canon 
	exiv2::exiv2
	log4cxx
)

