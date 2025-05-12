"""MQTT Client implementation."""

import re as regex
from typing import Any, Callable

import paho.mqtt.client as paho_client
import paho.mqtt.enums as paho_enums
from elasticai.protocol.client_interface import PubSubInterface


class PublishError(Exception):
    """Publishing to topic failed!"""

    pass


class SubscribeError(Exception):
    """Subscribing to topic failed!"""

    pass


class UnsubscribeError(Exception):
    """Unsubscribing topic failed!"""

    pass


class MQTTClient(PubSubInterface):
    """Exclipse Paho backed MQTT client for the elastic-AI protocol."""

    def __init__(
        self,
        client_id: str,
        clean_session: bool = False,
        mqtt_version: paho_client.MQTTProtocolVersion = (
            paho_client.MQTTProtocolVersion.MQTTv311
        ),
        auto_reconnect: bool = True,
    ) -> None:
        """Initialize the client.

        IMPORTANT: call `connect()` to establish connection with a Broker!

        Args:
            client_id (str):
                The client ID to be associated with the device.
            clean_session (bool):
                Whether to start a clean session on connect.
            mqtt_version (paho_client.MQTTProtocolVersion):
                The version of the MQTT protocol.
            auto_reconnect (bool):
                Whether to automatically reconnect after losing a connection.

        Returns:
            None
        """
        self.__client_id: str = client_id
        self.__subscriptions: dict[str, Callable[[str, str], None]] = {}
        self.__paho_client: paho_client.Client = paho_client.Client(
            callback_api_version=paho_enums.CallbackAPIVersion.VERSION2,
            client_id=client_id,
            clean_session=clean_session,
            protocol=mqtt_version,
            reconnect_on_failure=auto_reconnect,
        )
        self.__paho_client.on_message = self.__on_message
        self.__paho_client.on_connect = self.__on_connect
        self.__paho_client.on_connect_fail = self.__on_connect_fail
        self.__paho_client.loop_start()

    def __del__(self) -> None:
        """Close MQTT connection on class destruction."""
        self.disconnect()

    def connect(self, mqtt_broker_address: str, mqtt_broker_port: int) -> None:
        """Connect to the MQTT broker.

        Args:
            mqtt_broker_address (str): The address of the MQTT broker.
            mqtt_broker_port (int): The port of the MQTT broker.

        Returns:
            None
        """
        self.__connected = False
        self.__paho_client.connect(host=mqtt_broker_address, port=mqtt_broker_port)
        while not self.__connected:
            pass

    def disconnect(self) -> None:
        """Disconnect from the MQTT broker."""
        self.__paho_client.disconnect()

    def get_client_id(self) -> str:
        """Get the client ID.

        Raises:
            BrokerNotConnectedError: If the client is not connected

        Returns:
            client ID (str)
        """
        return self.__client_id

    def publish(self, topic: str, message: str, retain: bool = False) -> None:
        """Publish Message.

        Args:
            topic (str): The topic to publish.
            message (str): The message to publish.
            retain (bool): If messaged should be retained.

        Raises:
            ValueError: If the Payload is too long
            PublishError: If publish failed.

        Returns:
            None
        """
        status, _ = self.__paho_client.publish(
            topic=topic, payload=message, retain=retain
        )
        if status != paho_client.MQTT_ERR_SUCCESS:
            raise PublishError(status)

    def subscribe(self, topic: str, handler: Callable[[str, str], None]) -> None:
        """Subscribe to topic.

        Args:
            topic (str): The topic to subscribe.
            handler (Callable[[str, str], None]):
                The handler function for received messages

        Raises:
            ValueError: If the topic is invalid
            SubscribeError: If subscription failed

        Returns:
            None
        """
        status, _ = self.__paho_client.subscribe(topic=topic)
        if status == paho_client.MQTT_ERR_SUCCESS:
            self.__subscriptions[topic] = handler
        else:
            raise SubscribeError(status)

    def unsubscribe(self, topic: str) -> None:
        """Unsubscribe from topic.

        Args:
            topic (str): The topic to unsubscribe.

        Raises:
            ValueError: If the topic is invalid
            KeyError: If topic was not subscribed

        Returns:
            None
        """
        status, _ = self.__paho_client.unsubscribe(topic=topic)
        if status == paho_client.MQTT_ERR_SUCCESS:
            self.__subscriptions.pop(topic)
        else:
            raise UnsubscribeError(status)

    @staticmethod
    def __replace(level: str) -> str:
        WILDCARD_PLUS: str = "[A-Za-z0-9]*"
        WILDCARD_HASH: str = "[A-Za-z0-9/]*"
        if level == "+":
            return WILDCARD_PLUS
        elif level == "#":
            return WILDCARD_HASH
        else:
            return level

    @staticmethod
    def __check_topic_equal(topic1: str, topic2: str) -> bool:
        """Check if topic2 matches topic1.

        IMPORTANT: Only topic1 is allowed to contain wildcards

        Args:
            topic1 (str): subscribed topic
            topic2 (str): topic to evaluate

        Returns:
            match (bool): if topic1 matches topic2
        """
        pattern: str = (
            "^" + "/".join(list(map(MQTTClient.__replace, topic1.split("/")))) + "$"
        )
        compiled_pattern: regex.Pattern = regex.compile(pattern)
        return True if compiled_pattern.search(topic2) is not None else False

    def __on_message(
        self, client: paho_client.Client, userdata: None, msg: paho_client.MQTTMessage
    ) -> None:
        for topic, handler in self.__subscriptions.items():
            if MQTTClient.__check_topic_equal(topic, msg.topic):
                handler(msg.topic, str(msg.payload))

    def __on_connect(
        self,
        client: paho_client.Client,
        userdata: Any,
        connect_flags: paho_client.ConnectFlags,
        reason_code: paho_client.ReasonCode,
        properties: paho_client.Properties | None,
    ) -> None:
        """Callback for connect."""
        if reason_code == paho_client.MQTT_ERR_SUCCESS:
            self.__connected = True

    def __on_connect_fail(self, client: paho_client.Client, userdata: Any) -> None:
        """Callback for auto reconnect failed."""
        self.__connected = False

    def __on_disconnect(
        self,
        client: paho_client.Client,
        userdata: Any,
        diconnect_flags: paho_client.DisconnectFlags,
        reason_code: paho_client.ReasonCode,
        properties: paho_client.Properties | None,
    ) -> None:
        self.__connected = False
