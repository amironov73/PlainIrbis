###########################################################
# PlainIrbis project
# Alexey Mironov, 2020
###########################################################

# simple example
project(hello C)

set(CFiles
    src/main.c
)

add_executable(${PROJECT_NAME}
    ${CFiles}
)

target_link_libraries(${PROJECT_NAME} magna irbis)

if(MSVC)
    target_link_libraries(${PROJECT_NAME} Ws2_32.lib)
endif()

if(MINGW)
    target_link_libraries(${PROJECT_NAME} libws2_32.a)
endif(MINGW)

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS})
