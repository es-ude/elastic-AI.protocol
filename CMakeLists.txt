cmake_minimum_required(VERSION 3.20...3.24)

if(NOT TARGET _eai_protocol_inclusion_marker)
    add_library(_eai_protocol_inclusion_marker INTERFACE)

    include(C/eai_protocol_init.cmake)

    project("elastic-AI.protocol" LANGUAGES C)
    set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
    eai_protocol_is_top_level_project(EAI_PROTOCOL_TOP_LEVEL_PROJECT)

    add_cexception()
    add_subdirectory(C/src/protocol)
    add_subdirectory(C/src/communicationEndpoint)

    if (EAI_PROTOCOL_TOP_LEVEL_PROJECT)
        if (DEBUG_OUTPUT)
            add_definitions(-DDEBUG_MODE)
        endif ()

        add_compile_options(-ftrapv) # abort execution on integer overflow

        add_ctest()
        add_unity()
        add_subdirectory(C/src/utils/brokerMock)
        add_subdirectory(C/test)
    endif ()
endif ()
