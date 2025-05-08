if (NOT TARGET _eai_protocol_pre_init_marker)
    add_library(_eai_protocol_pre_init_marker INTERFACE)

    function(eai_protocol_is_top_level_project VAR)
        string(TOLOWER ${CMAKE_CURRENT_LIST_DIR} __list_dir)
        string(TOLOWER ${CMAKE_SOURCE_DIR} __source_dir)
        if (__source_dir STREQUAL __list_dir)
            set(${VAR} 1 PARENT_SCOPE)
        else ()
            set(${VAR} 0 PARENT_SCOPE)
        endif ()
    endfunction()

    if (NOT EAI_PROTOCOL_PATH)
        set(EAI_PROTOCOL_PATH ${CMAKE_CURRENT_LIST_DIR})
    endif ()

    get_filename_component(EAI_PROTOCOL_PATH "${EAI_PROTOCOL_PATH}" REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")

    set(EAI_PROTOCOL_PATH ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "Path to the EAI protocol" FORCE)

    list(APPEND CMAKE_MODULE_PATH ${EAI_PROTOCOL_PATH}/cmake)

    message("EAI protocol path is ${EAI_PROTOCOL_PATH}")

    include(third_party_deps)

    macro(eai_protocol_init)
        if (NOT CMAKE_PROJECT_NAME)
            message(WARNING "eai_protocol_init() should be called after the project is created (and languages added)")
        endif ()
        add_subdirectory(${EAI_PROTOCOL_PATH} eai-protocol)
    endmacro()
endif ()
