###########################################################
# PlainIrbis project
# Alexey Mironov, 2020
###########################################################

# turn Caps Lock key off
project(capsoff C)

set(CFiles
    src/main.c
)

add_executable(${PROJECT_NAME}
    ${CFiles}
)

target_link_libraries(${PROJECT_NAME} magna)

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS})
