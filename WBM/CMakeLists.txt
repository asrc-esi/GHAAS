FILE(GLOB sources src/*.c)
add_executable(WBM30 ${sources})

if(${CMAKE_HOST_APPLE})
    target_link_libraries(WBM30 MF30 CM30 m)
else(${CMAKE_HOST_APPLE})
    target_link_libraries(WBM30 MF30 CM30 m -pthread)
endif(${CMAKE_HOST_APPLE})

target_include_directories(WBM30 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
install (TARGETS WBM30 RUNTIME DESTINATION ghaas/bin)
