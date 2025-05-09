from unittest.mock import Mock

import pytest
from elasticai.protocol.base import DeviceState, DeviceType, Protocol

BASE_URL: str = "eaip://test"
DEVICE_ID: str = "test_device_01"
DEVICE_TYPE: DeviceType = DeviceType.APPLICATION


@pytest.fixture
def mqtt_client_mock(mocker: Mock) -> Mock:
    client_mock = mocker.Mock()
    client_mock.get_client_id.return_value = DEVICE_ID
    return client_mock


@pytest.fixture
def handler_mock(mocker: Mock) -> Mock:
    handler_mock = mocker.Mock()
    return handler_mock


@pytest.fixture
def protocol(mqtt_client_mock: Mock) -> Protocol:
    return Protocol(
        mqtt_client_mock,
        base_url=BASE_URL,
        device_type=DEVICE_TYPE,
    )


# region PUBLISH


def test_class_instance_created(mqtt_client_mock: Mock) -> None:
    protocol = Protocol(mqtt_client_mock)
    assert isinstance(protocol, Protocol)


def test_class_instance_values_set_correct(mqtt_client_mock: Mock) -> None:
    protocol = Protocol(mqtt_client_mock)
    assert protocol._Protocol__handler == mqtt_client_mock
    assert protocol._Protocol__device_id == DEVICE_ID
    assert protocol._Protocol__device_type == DEVICE_TYPE
    assert protocol._Protocol__base_url == "eaip://uni-due.de"


def test_instance_created_with_custom_base_url(mqtt_client_mock: Mock) -> None:
    protocol = Protocol(mqtt_client_mock, base_url="  " + BASE_URL + "/")
    assert protocol._Protocol__base_url == BASE_URL


def test_instance_created_with_custom_device_type(mqtt_client_mock: Mock) -> None:
    protocol = Protocol(mqtt_client_mock, device_type=DEVICE_TYPE)
    assert protocol._Protocol__device_type == DEVICE_TYPE


def test_publish_status(protocol: Protocol, mqtt_client_mock: Mock) -> None:
    protocol.publish_status(DeviceState.ONLINE)
    mqtt_client_mock.publish.assert_called_once()
    mqtt_client_mock.publish.assert_called_with(
        "eaip://test/test_device_01/STATUS",
        "ID:test_device_01;TYPE:APP;STATE:ONLINE;",
        retain=True,
    )


def test_publish_status_with_additional_fields(
    protocol: Protocol, mqtt_client_mock: Mock
) -> None:
    protocol.publish_status(
        DeviceState.ONLINE, additional_information={"f1": "test", "URL": "TEST ME"}
    )
    mqtt_client_mock.publish.assert_called_once()
    mqtt_client_mock.publish.assert_called_with(
        "eaip://test/test_device_01/STATUS",
        "ID:test_device_01;TYPE:APP;STATE:ONLINE;f1:test;URL:TEST ME;",
        retain=True,
    )


def test_publish_data(protocol: Protocol, mqtt_client_mock: Mock) -> None:
    protocol.publish_data("/test/data/", "test data")
    mqtt_client_mock.publish.assert_called_once()
    mqtt_client_mock.publish.assert_called_with(
        "eaip://test/test_device_01/DATA/test/data",
        "test data",
    )


def test_publish_start(protocol: Protocol, mqtt_client_mock: Mock) -> None:
    protocol.publish_start("test_device_02", "/test/data/")
    mqtt_client_mock.publish.assert_called_once()
    mqtt_client_mock.publish.assert_called_with(
        "eaip://test/test_device_02/START/test/data",
        "{}/{}".format(BASE_URL, DEVICE_ID),
    )


def test_publish_stop(protocol: Protocol, mqtt_client_mock: Mock) -> None:
    protocol.publish_stop("test_device_02", "/test/data/")
    mqtt_client_mock.publish.assert_called_once()
    mqtt_client_mock.publish.assert_called_with(
        "eaip://test/test_device_02/STOP/test/data",
        "{}/{}".format(BASE_URL, DEVICE_ID),
    )


def test_publish_do_without_settings(
    protocol: Protocol, mqtt_client_mock: Mock
) -> None:
    protocol.publish_do("test_device_02", "/test/cmd/")
    mqtt_client_mock.publish.assert_called_once()
    mqtt_client_mock.publish.assert_called_with(
        "eaip://test/test_device_02/DO/test/cmd",
        "",
    )


def test_publish_do_with_settings(protocol: Protocol, mqtt_client_mock: Mock) -> None:
    protocol.publish_do("test_device_02", "/test/cmd/", settings="val1=2;val3=23")
    mqtt_client_mock.publish.assert_called_once()
    mqtt_client_mock.publish.assert_called_with(
        "eaip://test/test_device_02/DO/test/cmd",
        "val1=2;val3=23",
    )


def test_publish_done(protocol: Protocol, mqtt_client_mock: Mock) -> None:
    protocol.publish_done("test_device_02", "/test/cmd/", result="SUCCESS")
    mqtt_client_mock.publish.assert_called_once()
    mqtt_client_mock.publish.assert_called_with(
        "eaip://test/test_device_02/DONE/test/cmd",
        "SUCCESS",
    )


# endregion PUBLISH

# region SUBSCRIBE


def test_subscribe_status(
    protocol: Protocol, mqtt_client_mock: Mock, handler_mock: Mock
) -> None:
    protocol.subscribe_status("test_device_02", handler_mock)
    mqtt_client_mock.subscribe.assert_called_once()
    mqtt_client_mock.subscribe.assert_called_with(
        "eaip://test/test_device_02/STATUS", handler_mock
    )


def test_subscribe_data(
    protocol: Protocol, mqtt_client_mock: Mock, handler_mock: Mock
) -> None:
    protocol.subscribe_data("test_device_02", "/test/data/", handler_mock)
    mqtt_client_mock.subscribe.assert_called_once()
    mqtt_client_mock.subscribe.assert_called_with(
        "eaip://test/test_device_02/DATA/test/data", handler_mock
    )


def test_subscribe_start(
    protocol: Protocol, mqtt_client_mock: Mock, handler_mock: Mock
) -> None:
    protocol.subscribe_start("/test/data/", handler_mock)
    mqtt_client_mock.subscribe.assert_called_once()
    mqtt_client_mock.subscribe.assert_called_with(
        "eaip://test/test_device_01/START/test/data", handler_mock
    )


def test_subscribe_stop(
    protocol: Protocol, mqtt_client_mock: Mock, handler_mock: Mock
) -> None:
    protocol.subscribe_stop("/test/data/", handler_mock)
    mqtt_client_mock.subscribe.assert_called_once()
    mqtt_client_mock.subscribe.assert_called_with(
        "eaip://test/test_device_01/STOP/test/data", handler_mock
    )


def test_subscribe_do(
    protocol: Protocol, mqtt_client_mock: Mock, handler_mock: Mock
) -> None:
    protocol.subscribe_do("/test/cmd/", handler_mock)
    mqtt_client_mock.subscribe.assert_called_once()
    mqtt_client_mock.subscribe.assert_called_with(
        "eaip://test/test_device_01/DO/test/cmd", handler_mock
    )


def test_subscribe_done(
    protocol: Protocol, mqtt_client_mock: Mock, handler_mock: Mock
) -> None:
    protocol.subscribe_done("test_device_02", "/test/cmd/", handler_mock)
    mqtt_client_mock.subscribe.assert_called_once()
    mqtt_client_mock.subscribe.assert_called_with(
        "eaip://test/test_device_02/DONE/test/cmd", handler_mock
    )


# endregion SUBSCRIBE

# region UNSUBSCRIBE


def test_unsubscribe_status(protocol: Protocol, mqtt_client_mock: Mock) -> None:
    protocol.unsubscribe_status("test_device_02")
    mqtt_client_mock.unsubscribe.assert_called_once()
    mqtt_client_mock.unsubscribe.assert_called_with("eaip://test/test_device_02/STATUS")


def test_unsubscribe_data(protocol: Protocol, mqtt_client_mock: Mock) -> None:
    protocol.unsubscribe_data("test_device_02", "/test/data/")
    mqtt_client_mock.unsubscribe.assert_called_once()
    mqtt_client_mock.unsubscribe.assert_called_with(
        "eaip://test/test_device_02/DATA/test/data"
    )


def test_unsubscribe_start(protocol: Protocol, mqtt_client_mock: Mock) -> None:
    protocol.unsubscribe_start("/test/data/")
    mqtt_client_mock.unsubscribe.assert_called_once()
    mqtt_client_mock.unsubscribe.assert_called_with(
        "eaip://test/test_device_01/START/test/data"
    )


def test_unsubscribe_stop(protocol: Protocol, mqtt_client_mock: Mock) -> None:
    protocol.unsubscribe_stop("/test/data/")
    mqtt_client_mock.unsubscribe.assert_called_once()
    mqtt_client_mock.unsubscribe.assert_called_with(
        "eaip://test/test_device_01/STOP/test/data"
    )


def test_unsubscribe_do(protocol: Protocol, mqtt_client_mock: Mock) -> None:
    protocol.unsubscribe_do("/test/cmd/")
    mqtt_client_mock.unsubscribe.assert_called_once()
    mqtt_client_mock.unsubscribe.assert_called_with(
        "eaip://test/test_device_01/DO/test/cmd"
    )


def test_unsubscribe_done(protocol: Protocol, mqtt_client_mock: Mock) -> None:
    protocol.unsubscribe_done("test_device_02", "/test/cmd/")
    mqtt_client_mock.unsubscribe.assert_called_once()
    mqtt_client_mock.unsubscribe.assert_called_with(
        "eaip://test/test_device_02/DONE/test/cmd"
    )


# endregion UNSUBSCRIBE

# region TOOLS


def test_status_parsed_correct() -> None:
    status: str = "f1:test1;f2:test2;field3:value3;"
    status_dict: dict[str, str] = Protocol.parse_status(status)
    assert status_dict == {"f1": "test1", "f2": "test2", "field3": "value3"}


# endregion TOOLS

if __name__ == "__main__":
    pytest.main()
