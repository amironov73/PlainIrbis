###########################################################
# PlainIrbis project
# Alexey Mironov, 2020
###########################################################

# self tests for tester
project(tester C)

set(CFiles
    src/main.c
)

add_executable(${PROJECT_NAME}
    ${CFiles}
    ../../include/magna/tester.h
)

target_link_libraries(${PROJECT_NAME} magna)
