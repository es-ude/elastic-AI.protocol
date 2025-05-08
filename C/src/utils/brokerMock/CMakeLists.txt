add_library(eaip_utils_brokerMock STATIC
        Broker.c
)
target_link_libraries(eaip_utils_brokerMock PUBLIC
        eaip_communicationEndpoint
)
target_include_directories(eaip_utils_brokerMock PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include/public
)
