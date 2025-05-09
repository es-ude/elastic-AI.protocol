"""Elastic-AI protocol implementation."""

from enum import Enum
from typing import Callable

from elasticai.protocol.client_interface import PubSubInterface


class DeviceType(str, Enum):
    """Type of the client."""

    APPLICATION = "APP"
    NODE = "NODE"


class DeviceState(str, Enum):
    """State of the client."""

    OFFLINE = "OFFLINE"
    ONLINE = "ONLINE"


class Protocol:
    """Elastic-AI protocol implementation."""

    __handler: PubSubInterface
    __device_id: str
    __device_type: DeviceType
    __base_url: str
    __topics: dict[str, str]

    def __init__(
        self,
        handler: PubSubInterface,
        device_type: DeviceType = DeviceType.APPLICATION,
        base_url: str = "eaip://uni-due.de",
    ) -> None:
        """Initialize the EAI protocol.

        Args:
            handler (IPubSubClient): Handler for pub-sub-system
            device_type (DeviceType): Type of the client
            base_url (str): Base URl for the communication

        Returns:
            None
        """
        self.__handler = handler
        self.__device_id = handler.get_client_id()
        self.__device_type = device_type
        self.__base_url = base_url.lstrip().removesuffix("/")
        self.__topics: dict[str, str] = {
            "status": "STATUS",
            "data": "DATA",
            "start": "START",
            "stop": "STOP",
            "do": "DO",
            "done": "DONE",
        }

    # region PUBLISH

    def publish_status(
        self,
        device_state: DeviceState,
        additional_information: dict[str, str] | None = None,
    ) -> None:
        """Publish the current status of the client.

        Args:
            device_state (DeviceState): Device state of the client.
            additional_information (dict[str, str]): Additional information
                                                     key -> Field Name
                                                     value -> Field Value

        Returns:
            None
        """
        topic: str = "{}/{}/{}".format(
            self.__base_url, self.__device_id, self.__topics["status"]
        )
        message: str = "ID:{};TYPE:{};STATE:{};".format(
            self.__device_id, self.__device_type.value, device_state.value
        )
        if additional_information is not None:
            for field, value in additional_information.items():
                message += "{}:{};".format(field, value)
        self.__handler.publish(topic, message, retain=True)

    def publish_data(self, data_id: str, data: str) -> None:
        """Publish datat of the client.

        Args:
            data_id (str): id for the data
            data (str): data to publish

        Returns:
            None
        """
        topic: str = "{}/{}/{}/{}".format(
            self.__base_url,
            self.__device_id,
            self.__topics["data"],
            data_id.removeprefix("/").removesuffix("/"),
        )
        self.__handler.publish(topic, data)

    def publish_start(self, device_id: str, data_id: str) -> None:
        """Publish data start request.

        Args:
            device_id (str): device ID of the client to start requesting data from
            data_id (str): id of the data to stop requesting

        Returns:
            None
        """
        topic: str = "{}/{}/{}/{}".format(
            self.__base_url,
            device_id,
            self.__topics["start"],
            data_id.removeprefix("/").removesuffix("/"),
        )
        message: str = "{}/{}".format(self.__base_url, self.__device_id)
        self.__handler.publish(topic, message)

    def publish_stop(self, device_id: str, data_id: str) -> None:
        """Publish data stop request.

        Args:
            device_id (str): device ID of the client to stop requesting data from
            data_id (str): id of the data to stop requesting

        Returns:
            None
        """
        topic: str = "{}/{}/{}/{}".format(
            self.__base_url,
            device_id,
            self.__topics["stop"],
            data_id.removeprefix("/").removesuffix("/"),
        )
        message: str = "{}/{}".format(self.__base_url, self.__device_id)
        self.__handler.publish(topic, message)

    def publish_do(
        self, device_id: str, command: str, settings: str | None = None
    ) -> None:
        """Publish a command.

        Args:
            device_id (str): device ID of the client to send command to
            command (str): command to send
            settings (str): additional settings to send

        Returns:
            None
        """
        topic: str = "{}/{}/{}/{}".format(
            self.__base_url,
            device_id,
            self.__topics["do"],
            command.removeprefix("/").removesuffix("/"),
        )
        message: str = settings if settings is not None else ""
        self.__handler.publish(topic, message)

    def publish_done(self, device_id: str, command: str, result: str) -> None:
        """Publish a command response.

        Args:
            device_id (str): device ID of the client to send command to
            command (str): command to send
            result (str): result of command execution

        Returns:
            None
        """
        topic: str = "{}/{}/{}/{}".format(
            self.__base_url,
            device_id,
            self.__topics["done"],
            command.removeprefix("/").removesuffix("/"),
        )
        self.__handler.publish(topic, result)

    # endregion PUBLISH

    # region SUBSCRIBE

    def subscribe_status(
        self, device_id: str, handler: Callable[[str, str], None]
    ) -> None:
        """Subscribe to the status of a client.

        Args:
            device_id (str): device ID of the client to subscribe
            handler (Callable[[str, str], None]): handler function to process
                                                  received status

        Returns:
            None
        """
        topic: str = "{}/{}/{}".format(
            self.__base_url,
            device_id.removeprefix("/").removesuffix("/"),
            self.__topics["status"],
        )
        self.__handler.subscribe(topic, handler)

    def subscribe_data(
        self, device_id: str, data_id: str, handler: Callable[[str, str], None]
    ) -> None:
        """Subscribe to the data of a client.

        Args:
            device_id (str): device ID of the client to subscribe
            data_id (str): ID of the data to subscribe
            handler (Callable[[str, str], None]): handler function to process
                                                  received status

        Returns:
            None
        """
        topic: str = "{}/{}/{}/{}".format(
            self.__base_url,
            device_id.removeprefix("/").removesuffix("/"),
            self.__topics["data"],
            data_id.removeprefix("/").removesuffix("/"),
        )
        self.__handler.subscribe(topic, handler)

    def subscribe_start(
        self, data_id: str, handler: Callable[[str, str], None]
    ) -> None:
        """Subscribe to start requests of a data topic.

        Args:
            data_id (str): ID of the data to subscribe for start requests
            handler (Callable[[str, str], None]): handler function to process
                                                  received status

        Returns:
            None
        """
        topic: str = "{}/{}/{}/{}".format(
            self.__base_url,
            self.__device_id,
            self.__topics["start"],
            data_id.removeprefix("/").removesuffix("/"),
        )
        self.__handler.subscribe(topic, handler)

    def subscribe_stop(self, data_id: str, handler: Callable[[str, str], None]) -> None:
        """Subscribe to stop requests of a data topic.

        Args:
            data_id (str): ID of the data to subscribe for start requests
            handler (Callable[[str, str], None]): handler function to process
                                                  received status

        Returns:
            None
        """
        topic: str = "{}/{}/{}/{}".format(
            self.__base_url,
            self.__device_id,
            self.__topics["stop"],
            data_id.removeprefix("/").removesuffix("/"),
        )
        self.__handler.subscribe(topic, handler)

    def subscribe_do(self, command: str, handler: Callable[[str, str], None]) -> None:
        """Subscribe to command.

        Args:
            command (str): command to subscribe for execution requests
            handler (Callable[[str, str], None]): handler function to process
                                                  received status

        Returns:
            None
        """
        topic: str = "{}/{}/{}/{}".format(
            self.__base_url,
            self.__device_id,
            self.__topics["do"],
            command.removeprefix("/").removesuffix("/"),
        )
        self.__handler.subscribe(topic, handler)

    def subscribe_done(
        self, device_id: str, command: str, handler: Callable[[str, str], None]
    ) -> None:
        """Subscribe to command execution result.

        Args:
            device_id (str): device ID of the client to subscribe
            command (str): command to subscribe for execution requests
            handler (Callable[[str, str], None]): handler function to process
                                                  received status

        Returns:
            None
        """
        topic: str = "{}/{}/{}/{}".format(
            self.__base_url,
            device_id.removeprefix("/").removesuffix("/"),
            self.__topics["done"],
            command.removeprefix("/").removesuffix("/"),
        )
        self.__handler.subscribe(topic, handler)

    # endregion SUBSCRIBE

    # region UNSUBSCRIBE

    def unsubscribe_status(self, device_id: str) -> None:
        """Unsubscribe from status of a client.

        Args:
            device_id (str): device ID of the client to unsubscribe from

        Returns:
            None
        """
        topic: str = "{}/{}/{}".format(
            self.__base_url,
            device_id.removeprefix("/").removesuffix("/"),
            self.__topics["status"],
        )
        self.__handler.unsubscribe(topic)

    def unsubscribe_data(self, device_id: str, data_id: str) -> None:
        """Unsubscribe from status of a client.

        Args:
            device_id (str): device ID of the client to unsubscribe from
            data_id(str): ID of the data to unsubscribe

        Returns:
            None
        """
        topic: str = "{}/{}/{}/{}".format(
            self.__base_url,
            device_id.removeprefix("/").removesuffix("/"),
            self.__topics["data"],
            data_id.removeprefix("/").removesuffix("/"),
        )
        self.__handler.unsubscribe(topic)

    def unsubscribe_start(self, data_id: str) -> None:
        """Unsubscribe from data start requests.

        Args:
            data_id(str): ID of the data to unsubscribe

        Returns:
            None
        """
        topic: str = "{}/{}/{}/{}".format(
            self.__base_url,
            self.__device_id,
            self.__topics["start"],
            data_id.removeprefix("/").removesuffix("/"),
        )
        self.__handler.unsubscribe(topic)

    def unsubscribe_stop(self, data_id: str) -> None:
        """Unsubscribe from data stop requests.

        Args:
            data_id(str): ID of the data to unsubscribe

        Returns:
            None
        """
        topic: str = "{}/{}/{}/{}".format(
            self.__base_url,
            self.__device_id,
            self.__topics["stop"],
            data_id.removeprefix("/").removesuffix("/"),
        )
        self.__handler.unsubscribe(topic)

    def unsubscribe_do(self, command: str) -> None:
        """Unsubscribe from command execution requests.

        Args:
            command(str): command to unsubscribe

        Returns:
            None
        """
        topic: str = "{}/{}/{}/{}".format(
            self.__base_url,
            self.__device_id,
            self.__topics["do"],
            command.removeprefix("/").removesuffix("/"),
        )
        self.__handler.unsubscribe(topic)

    def unsubscribe_done(self, device_id: str, command: str) -> None:
        """Unsubscribe from command execution results.

        Args:
            device_id (str): ID of the device to unsubscribe from
            command(str): command to unsubscribe

        Returns:
            None
        """
        topic: str = "{}/{}/{}/{}".format(
            self.__base_url,
            device_id.removeprefix("/").removesuffix("/"),
            self.__topics["done"],
            command.removeprefix("/").removesuffix("/"),
        )
        self.__handler.unsubscribe(topic)

    # endregion UNSUBSCRIBE

    # region TOOLS

    @staticmethod
    def parse_status(status: str) -> dict[str, str]:
        """Parse Status Message into dictionary."""
        return dict(
            map(
                lambda field: (field[0], field[2]),
                map(
                    lambda field: field.partition(":"),
                    [field for field in status.split(";") if field.strip()],
                ),
            )
        )

    # endregion TOOLS
