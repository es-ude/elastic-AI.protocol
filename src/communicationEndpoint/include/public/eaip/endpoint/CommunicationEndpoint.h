#ifndef EAI_PROTOCOL_COMMUNICATION_ENDPOINT_HEADER
#define EAI_PROTOCOL_COMMUNICATION_ENDPOINT_HEADER

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    EAIP_COM_NO_ERROR = 0x00,
    EAIP_COM_GENERIC_ERROR = 0x01,
    EAIP_COM_BROKER_NOT_REACHABLE = 0x02,
    EAIP_COM_TOPIC_TO_LONG = 0x11,
    EAIP_COM_INVALID_TOPIC = 0x12,
    EAIP_COM_TOPIC_ALREADY_SUBSCRIBED = 0x13,
} eaipCommunicationErrorCodes;

eaipCommunicationErrorCodes publish(char *topic, char *data,
                                    __attribute__((unused)) __attribute__((unused)) bool retain);
eaipCommunicationErrorCodes subscribe(char *topic, void (*handle)(char *topic, char *message));
eaipCommunicationErrorCodes unsubscribe(char *topic);

#endif /* EAI_PROTOCOL_COMMUNICATION_ENDPOINT_HEADER */
