project(vrdlib.test)

aux_source_directory(source PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE} ${PROJECT_INCLUDES}) 
target_link_libraries(${PROJECT_NAME} 
	vrdlib.api 
	vrdlib.common 
	vrdlib.canon 
	vrdlib.utility 
	exiv2::exiv2
	gtest::gtest
)

