"""Interface for the pub-sub-system used for the elastic-AI protocol."""

from abc import ABCMeta, abstractmethod
from typing import Callable


class PubSubInterface(metaclass=ABCMeta):
    """Interface for the pub-sub-system used for the elastic-AI protocol."""

    @classmethod
    def __subclasshook__(cls, subclass: type) -> bool:
        """Subclass hook."""
        return (
            hasattr(subclass, "get_client_id")
            and callable(subclass.get_client_id)
            and hasattr(subclass, "publish")
            and callable(subclass.publish)
            and hasattr(subclass, "subscribe")
            and callable(subclass.subscribe)
            and hasattr(subclass, "unsubscribe")
            and callable(subclass.unsubscribe)
        ) or NotImplemented

    @abstractmethod
    def get_client_id(self) -> str:
        """Get the client ID."""
        raise NotImplementedError

    @abstractmethod
    def publish(self, topic: str, message: str, retain: bool = False) -> None:
        """Publish a message to the defined topic.

        Args:
            topic(str): topic to publish to
            message(str): message to publish
            retain (bool): if message is retained

        Returns:
            None
        """
        raise NotImplementedError

    @abstractmethod
    def subscribe(self, topic: str, handler: Callable[[str, str], None]) -> None:
        """Subscribe a message to the defined topic.

        Args:
            topic(str): topic to subscribe to
            handler(Callable): handler function

        Returns:
            None
        """
        raise NotImplementedError

    @abstractmethod
    def unsubscribe(self, topic: str) -> None:
        """Unsubscribe a message to the defined topic.

        Args:
            topic(str): topic to unsubscribe to

        Returns:
            None
        """
        raise NotImplementedError
