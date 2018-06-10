cmake_minimum_required(VERSION 2.8.11)
project(vrdextractor)

aux_source_directory(source PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")
file(GLOB PROJECT_ETC "etc/*")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE} ${PROJECT_INCLUDES} ${PROJECT_ETC})
target_link_libraries(${PROJECT_NAME} vrdlib.api vrdlib.common vrdlib.canon vrdlib.utility ${Boost_LIBRARIES})

add_custom_target(${PROJECT_NAME}.Copy ALL
   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/etc $<TARGET_FILE_DIR:${PROJECT_NAME}>/etc
   DEPENDS ${PROJECT_NAME}
   COMMENT "Copying configuration files")

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/etc/ 
        DESTINATION share/vrdextractor)

#DIRECTORY_PERMISSIONS OWNER_WRITE WORLD_EXECUTE WORLD_READ

