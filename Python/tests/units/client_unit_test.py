import pytest
from mqtt.client import MQTTClient, PublishError, SubscribeError, UnsubscribeError

CLIENT_ID = "test-client"
BROKER_HOST = "localhost"
BROKER_PORT = 1883


@pytest.fixture
def mqtt_client() -> MQTTClient:
    mqtt_client = MQTTClient(CLIENT_ID)
    return mqtt_client


def test_get_client_id(mqtt_client):
    assert mqtt_client.get_client_id() == CLIENT_ID


def test_mqtt_publish_PublishError(mocker):
    with pytest.raises(PublishError):
        mqtt_client = MQTTClient(CLIENT_ID)

        paho_mock = mocker.Mock()
        mocker.patch.object(mqtt_client, "_MQTTClient__paho_client", paho_mock)
        paho_mock.publish.return_value = (4, 0)

        mqtt_client.publish("Test", "Test")


def test_mqtt_publish_successful(mqtt_client, mocker):
    test_topic: str = "TEST/TOP"
    test_payload: str = "test data"

    def paho_publish_mock(topic: str, payload: str, retain: bool) -> tuple[int, int]:
        assert test_topic == topic
        assert test_payload == payload
        assert not retain
        return 0, 1

    paho_mock = mocker.Mock()
    mocker.patch.object(mqtt_client, "_MQTTClient__paho_client", paho_mock)
    paho_mock.MQTT_ERR_SUCCESS.value = 0
    paho_mock.publish = paho_publish_mock

    mqtt_client.publish(test_topic, test_payload)


def test_mqtt_subscribe_SubscribeError(mqtt_client, mocker):
    paho_mock = mocker.Mock()
    mocker.patch.object(mqtt_client, "_MQTTClient__paho_client", paho_mock)
    paho_mock.MQTT_ERR_SUCCESS.value = 0
    paho_mock.subscribe.return_value = (4, 1)

    with pytest.raises(SubscribeError):
        mqtt_client.subscribe("test/top", None)


def test_mqtt_subscribe(mqtt_client, mocker):
    test_topic1: str = "TEST/TOP/1"
    test_topic2: str = "TEST/TOP/2"

    test_topic = ""

    def paho_subscribe_mock(topic: str) -> tuple[int, int]:
        assert test_topic == topic
        return 0, 1

    subscriptions = {}
    mocker.patch.object(mqtt_client, "_MQTTClient__subscriptions", subscriptions)
    paho_mock = mocker.Mock()
    mocker.patch.object(mqtt_client, "_MQTTClient__paho_client", paho_mock)
    paho_mock.MQTT_ERR_SUCCESS.value = 0
    paho_mock.subscribe = paho_subscribe_mock

    assert len(subscriptions) == 0
    test_topic = test_topic1
    mqtt_client.subscribe(test_topic1, None)
    assert len(subscriptions) == 1
    assert list(subscriptions.keys()) == [test_topic1]
    test_topic = test_topic2
    mqtt_client.subscribe(test_topic2, None)
    assert len(subscriptions) == 2
    assert list(subscriptions.keys()) == [test_topic1, test_topic2]


def test_mqtt_unsubscribe_UnsubscribeError(mqtt_client, mocker):
    paho_mock = mocker.Mock()
    mocker.patch.object(mqtt_client, "_MQTTClient__paho_client", paho_mock)
    paho_mock.MQTT_ERR_SUCCESS.value = 0
    paho_mock.unsubscribe.return_value = (4, 1)

    with pytest.raises(UnsubscribeError):
        mqtt_client.unsubscribe("test/top")


def test_mqtt_unsubscribe(mqtt_client, mocker):
    test_topic1: str = "TEST/TOP/1"
    test_topic2: str = "TEST/TOP/2"

    test_topic = ""

    def paho_unsubscribe_mock(topic: str) -> tuple[int, int]:
        assert test_topic == topic
        return 0, 1

    subscriptions = {}
    mocker.patch.object(mqtt_client, "_MQTTClient__subscriptions", subscriptions)
    paho_mock = mocker.Mock()
    mocker.patch.object(mqtt_client, "_MQTTClient__paho_client", paho_mock)
    paho_mock.MQTT_ERR_SUCCESS.value = 0
    paho_mock.subscribe.return_value = (0, 1)
    paho_mock.unsubscribe = paho_unsubscribe_mock

    assert len(subscriptions) == 0
    mqtt_client.subscribe(test_topic1, None)
    mqtt_client.subscribe(test_topic2, None)
    assert len(subscriptions) == 2
    test_topic = test_topic1
    mqtt_client.unsubscribe(test_topic1)
    assert len(subscriptions) == 1
    assert list(subscriptions.keys()) == [test_topic2]
    test_topic = test_topic2
    mqtt_client.unsubscribe(test_topic2)
    assert len(subscriptions) == 0


def test_mqtt_unsubscribe_KeyError(mqtt_client, mocker):
    test_topic1: str = "TEST/TOP/1"
    test_topic2: str = "TEST/TOP/2"

    subscriptions = {}
    mocker.patch.object(mqtt_client, "_MQTTClient__subscriptions", subscriptions)
    paho_mock = mocker.Mock()
    mocker.patch.object(mqtt_client, "_MQTTClient__paho_client", paho_mock)
    paho_mock.MQTT_ERR_SUCCESS.value = 0
    paho_mock.subscribe.return_value = (0, 1)
    paho_mock.unsubscribe.return_value = (0, 1)

    with pytest.raises(KeyError):
        mqtt_client.subscribe(test_topic1, None)
        mqtt_client.unsubscribe(test_topic2)


if __name__ == "__main__":
    pytest.main()
