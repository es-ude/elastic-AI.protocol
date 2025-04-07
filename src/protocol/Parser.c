#include <stdio.h>
#include <string.h>

#include "eaip/protocol/Parser.h"
#include "eaip/protocol/Protocol.h"

/* region TOPIC */

#define TOPIC_STATUS "STATUS"
#define TOPIC_START "START"
#define TOPIC_STOP "STOP"
#define TOPIC_DATA "DATA"
#define TOPIC_DO "DO"
#define TOPIC_DONE "DONE"

size_t getTopicLength(topic_t topic, char *baseUrl, char *deviceId, char *dataId) {
    size_t length = strlen(baseUrl) + strlen(deviceId);

    if (dataId != NULL) {
        length += strlen(dataId);
    }

    switch (topic) {
    case STATUS:
        length += strlen(TOPIC_STATUS) + 3;
        break;
    case START:
        length += strlen(TOPIC_START) + 4;
        break;
    case STOP:
        length += strlen(TOPIC_STOP) + 4;
        break;
    case DATA:
        length += strlen(TOPIC_DATA) + 4;
        break;
    case DO:
        length += strlen(TOPIC_DO) + 4;
        break;
    case DONE:
        length += strlen(TOPIC_DONE) + 4;
        break;
    }

    return length;
}

void parseTopic(char *topicBuffer, topic_t topic, char *baseUrl, char *deviceId, char *dataId) {
    switch (topic) {
    case STATUS:
        sprintf(topicBuffer, "%s/%s/%s", baseUrl, deviceId, TOPIC_STATUS);
        break;
    case START:
        sprintf(topicBuffer, "%s/%s/%s/%s", baseUrl, deviceId, TOPIC_START, dataId);
        break;
    case STOP:
        sprintf(topicBuffer, "%s/%s/%s/%s", baseUrl, deviceId, TOPIC_STOP, dataId);
        break;
    case DATA:
        sprintf(topicBuffer, "%s/%s/%s/%s", baseUrl, deviceId, TOPIC_DATA, dataId);
        break;
    case DO:
        sprintf(topicBuffer, "%s/%s/%s/%s", baseUrl, deviceId, TOPIC_DO, dataId);
        break;
    case DONE:
        sprintf(topicBuffer, "%s/%s/%s/%s", baseUrl, deviceId, TOPIC_DONE, dataId);
        break;
    }
}

/* endregion TOPIC */

/* region STATUS */

#define FIELD_NAME_ID "ID"

#define FIELD_NAME_TYPE "TYPE"
#define TYPE_APPLICATION "APPLICATION"
#define TYPE_NODE "enV5"

#define FIELD_NAME_STATE "STATE"
#define STATE_ONLINE "ONLINE"
#define STATE_OFFLINE "OFFLINE"

size_t getStatusLength(char *deviceId, eaipDeviceState_t status) {
    size_t length = strlen(FIELD_NAME_ID) + strlen(deviceId) + strlen(FIELD_NAME_TYPE) +
                    strlen(FIELD_NAME_STATE) + 7;

    switch (status.deviceType) {
    case APPLICATION:
        length += strlen(TYPE_APPLICATION);
        break;
    case NODE:
        length += strlen(TYPE_NODE);
        break;
    }

    switch (status.deviceState) {
    case OFFLINE:
        length += strlen(STATE_OFFLINE);
        break;
    case ONLINE:
        length += strlen(STATE_ONLINE);
        break;
    }

    eaipStateDataField_t *next = status.additionalFields;
    while (next != NULL) {
        length += strlen(next->id) + strlen(next->data) + 2;
        next = next->next;
    }

    return length;
}

void parseStatus(char *statusBuffer, char *deviceId, eaipDeviceState_t status) {
    sprintf(statusBuffer, "%s:%s;", FIELD_NAME_ID, deviceId);

    strcat(statusBuffer, FIELD_NAME_TYPE ":");
    switch (status.deviceType) {
    case APPLICATION:
        strcat(statusBuffer, TYPE_APPLICATION);
        break;
    case NODE:
        strcat(statusBuffer, TYPE_NODE);
        break;
    }
    strcat(statusBuffer, ";");

    strcat(statusBuffer, FIELD_NAME_STATE ":");
    switch (status.deviceState) {
    case OFFLINE:
        strcat(statusBuffer, STATE_OFFLINE);
        break;
    case ONLINE:
        strcat(statusBuffer, STATE_ONLINE);
        break;
    }
    strcat(statusBuffer, ";");

    eaipStateDataField_t *next = status.additionalFields;
    while (next != NULL) {
        strcat(statusBuffer, next->id);
        strcat(statusBuffer, ":");
        strcat(statusBuffer, next->data);
        strcat(statusBuffer, ";");
        next = next->next;
    }
}

/* endregion STATUS */
