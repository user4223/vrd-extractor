project(vrdlib.test)

aux_source_directory(source PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE} ${PROJECT_INCLUDES}) 
target_link_libraries(${PROJECT_NAME} 
	vrdlib.api 
	vrdlib.common 
	vrdlib.canon 
	vrdlib.utility 
	CONAN_PKG::Exiv2
	CONAN_PKG::gtest
)

