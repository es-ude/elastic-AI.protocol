add_executable(test_brokerMock
        test_brokerMock.c
)
target_link_libraries(test_brokerMock
        unity
        eaip_utils_brokerMock
)
add_test(test_brokerMock test_brokerMock)

add_executable(test_protocol
        test_protocol.c
)
target_link_libraries(test_protocol
        unity
        eaip_utils_brokerMock
        eai_protocol
)
add_test(test_protocol test_protocol)

