"""Class to simplify the process of requesting data from another participant."""

from elasticai.protocol.base import DeviceState, Protocol
from elasticai.protocol.exceptions import DeviceNotAvailableError


class DataRequester:
    """Data Request tool."""

    def __init__(self, protocol: Protocol, target_device: str, data_id: str) -> None:
        """Initialize DataRequester.

        Args:
            protocol (Protocol): instance of the elastic-AI protocol
            target_device (str): id of the device to request data from
            data_id (str): id of the data field to request

        Returns:
            None
        """
        self.__protocol = protocol
        self.__device_id = target_device
        self.__data_id = data_id

        self.__data_buffer: list[str] = []

        self.__protocol.subscribe_status(self.__device_id, self.__status_handler)
        self.__protocol.subscribe_data(
            self.__device_id, self.__data_id, self.__data_handler
        )

    def __status_handler(self, topic: str, message: str) -> None:
        status: dict[str, str] = Protocol.parse_status(message)
        if "STATE" in status and status["STATE"] == DeviceState.ONLINE:
            self.__device_state = DeviceState.ONLINE
        else:
            self.__device_state = DeviceState.OFFLINE

    def __data_handler(self, topic: str, message: str) -> None:
        self.__data_buffer.append(message)

    def start(self) -> None:
        """Start requesting data.

        Raises:
            DeviceNotAvailableError if target device is OFFLINE

        Returns:
            None
        """
        if self.__device_state == DeviceState.OFFLINE:
            raise DeviceNotAvailableError()
        self.__protocol.publish_start(self.__device_id, self.__data_id)

    def stop(self) -> None:
        """Stop requesting data.

        Raises:
            DeviceNotAvailableError if target device is OFFLINE

        Returns:
            None
        """
        if self.__device_state == DeviceState.OFFLINE:
            raise DeviceNotAvailableError()
        self.__protocol.publish_stop(self.__device_id, self.__data_id)

    def get_data(self) -> str | None:
        """Get data from buffer.

        Returns:
            str: if data is present
            None: else
        """
        latest_val: str | None
        try:
            latest_val = self.__data_buffer.pop(0)
        except IndexError:
            latest_val = None

        return latest_val
