add_library(eai_protocol STATIC
        Protocol.c
        Parser.c
        include/private/eaip/protocol/Parser.h
)
target_link_libraries(eai_protocol PUBLIC
    eaip_communicationEndpoint
)
target_include_directories(eai_protocol PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include/private
)
target_include_directories(eai_protocol PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include/public
)
