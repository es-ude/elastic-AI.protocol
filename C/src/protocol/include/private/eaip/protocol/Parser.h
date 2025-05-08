#ifndef EAI_PROTOCOL_TOPICPARSER_HEADER
#define EAI_PROTOCOL_TOPICPARSER_HEADER

#include "eaip/protocol/Protocol.h"

typedef enum topic { STATUS, START, STOP, DATA, DO, DONE } topic_t;

size_t getTopicLength(topic_t topic, char *baseUrl, char *deviceId, char *dataId);
void parseTopic(char *topicBuffer, topic_t topic, char *baseUrl, char *deviceId, char *dataId);

size_t getStatusLength(char *deviceId, eaipDeviceState_t status);
void parseStatus(char *statusBuffer, char *deviceId, eaipDeviceState_t status);

#endif // EAI_PROTOCOL_TOPICPARSER_HEADER
