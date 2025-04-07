#include <stdio.h>
#include <string.h>

#include "eaip/protocol/Parser.h"
#include "eaip/protocol/Protocol.h"

/* region PUBLISH */

eaipCommunicationErrorCodes eaipPublishStatus(eaiProtocol_t config, eaipDeviceState_t status) {
  char topic[getTopicLength(STATUS, config.baseUrl, config.deviceId, NULL)];
  parseTopic(topic, STATUS, config.baseUrl, config.deviceId, NULL);

  char data[getStatusLength(config.deviceId, status)];
  parseStatus(data, config.deviceId, status);

  return config.publish(topic, data, true);
}

eaipCommunicationErrorCodes eaipPublishData(eaiProtocol_t config, eaipPubRequest_t request) {
  char topic[getTopicLength(DATA, config.baseUrl, config.deviceId, request.dataId)];
  parseTopic(topic, DATA, config.baseUrl, config.deviceId, request.dataId);

  return config.publish(topic, request.data, false);
}

eaipCommunicationErrorCodes eaipPublishStart(eaiProtocol_t config, eaipPubRequest_t request) {
  char topic[getTopicLength(START, config.baseUrl, request.deviceId, request.dataId)];
  parseTopic(topic, START, config.baseUrl, request.deviceId, request.dataId);

  char data[strlen(config.baseUrl) + strlen(config.deviceId) + 2];
  sprintf(data, "%s/%s", config.baseUrl, config.deviceId);

  return config.publish(topic, data, false);
}

eaipCommunicationErrorCodes eaipPublishStop(eaiProtocol_t config, eaipPubRequest_t request) {
  char topic[getTopicLength(STOP, config.baseUrl, request.deviceId, request.dataId)];
  parseTopic(topic, STOP, config.baseUrl, request.deviceId, request.dataId);

  char data[strlen(config.baseUrl) + strlen(config.deviceId) + 2];
  sprintf(data, "%s/%s", config.baseUrl, config.deviceId);

  return config.publish(topic, data, false);
}

eaipCommunicationErrorCodes eaipPublishDo(eaiProtocol_t config, eaipPubRequest_t request) {
  char topic[getTopicLength(DO, config.baseUrl, request.deviceId, request.dataId)];
  parseTopic(topic, DO, config.baseUrl, request.deviceId, request.dataId);

  return config.publish(topic, request.data, false);
}

eaipCommunicationErrorCodes eaipPublishDone(eaiProtocol_t config, eaipPubRequest_t request) {
  char topic[getTopicLength(DONE, config.baseUrl, config.deviceId, request.dataId)];
  parseTopic(topic, DONE, config.baseUrl, config.deviceId, request.dataId);

  return config.publish(topic, request.data, false);
}

/* endregion PUBLISH */

/* region SUBSCRIBE */

eaipCommunicationErrorCodes eaipSubscribeStatus(eaiProtocol_t config, eaipSubRequest_t request) {
  char topic[getTopicLength(STATUS, config.baseUrl, request.targetId, NULL)];
  parseTopic(topic, STATUS, config.baseUrl, request.targetId, NULL);

  return config.subscribe(topic, request.handler);
}

eaipCommunicationErrorCodes eaipSubscribeData(eaiProtocol_t config, eaipSubRequest_t request) {
  char topic[getTopicLength(DATA, config.baseUrl, request.targetId, request.dataId)];
  parseTopic(topic, DATA, config.baseUrl, request.targetId, request.dataId);

  return config.subscribe(topic, request.handler);
}

eaipCommunicationErrorCodes eaipSubscribeStart(eaiProtocol_t config, eaipSubRequest_t request) {
  char topic[getTopicLength(START, config.baseUrl, config.deviceId, request.dataId)];
  parseTopic(topic, START, config.baseUrl, config.deviceId, request.dataId);

  return config.subscribe(topic, request.handler);
}

eaipCommunicationErrorCodes eaipSubscribeStop(eaiProtocol_t config, eaipSubRequest_t request) {
  char topic[getTopicLength(STOP, config.baseUrl, config.deviceId, request.dataId)];
  parseTopic(topic, STOP, config.baseUrl, config.deviceId, request.dataId);

  return config.subscribe(topic, request.handler);
}

eaipCommunicationErrorCodes eaipSubscribeDo(eaiProtocol_t config, eaipSubRequest_t request) {
  char topic[getTopicLength(DO, config.baseUrl, config.deviceId, request.dataId)];
  parseTopic(topic, DO, config.baseUrl, config.deviceId, request.dataId);

  return config.subscribe(topic, request.handler);
}

eaipCommunicationErrorCodes eaipSubscribeDone(eaiProtocol_t config, eaipSubRequest_t request) {
  char topic[getTopicLength(DONE, config.baseUrl, request.targetId, request.dataId)];
  parseTopic(topic, DONE, config.baseUrl, request.targetId, request.dataId);

  return config.subscribe(topic, request.handler);
}

/* endregion SUBSCRIBE */

/* region UNSUBSCRIBE */

eaipCommunicationErrorCodes eaipUnsubscribeStatus(eaiProtocol_t config, eaipSubRequest_t request) {
  char topic[getTopicLength(STATUS, config.baseUrl, request.targetId, NULL)];
  parseTopic(topic, STATUS, config.baseUrl, request.targetId, NULL);

  return config.unsubscribe(topic);
}

eaipCommunicationErrorCodes eaipUnsubscribeData(eaiProtocol_t config, eaipSubRequest_t request) {
  char topic[getTopicLength(DATA, config.baseUrl, request.targetId, request.dataId)];
  parseTopic(topic, DATA, config.baseUrl, request.targetId, request.dataId);

  return config.unsubscribe(topic);
}

eaipCommunicationErrorCodes eaipUnsubscribeStart(eaiProtocol_t config, eaipSubRequest_t request) {
  char topic[getTopicLength(START, config.baseUrl, config.deviceId, request.dataId)];
  parseTopic(topic, START, config.baseUrl, config.deviceId, request.dataId);

  return config.unsubscribe(topic);
}

eaipCommunicationErrorCodes eaipUnsubscribeStop(eaiProtocol_t config, eaipSubRequest_t request) {
  char topic[getTopicLength(STOP, config.baseUrl, config.deviceId, request.dataId)];
  parseTopic(topic, STOP, config.baseUrl, config.deviceId, request.dataId);

  return config.unsubscribe(topic);
}

eaipCommunicationErrorCodes eaipUnsubscribeDo(eaiProtocol_t config, eaipSubRequest_t request) {
  char topic[getTopicLength(DO, config.baseUrl, config.deviceId, request.dataId)];
  parseTopic(topic, DO, config.baseUrl, config.deviceId, request.dataId);

  return config.unsubscribe(topic);
}

eaipCommunicationErrorCodes eaipUnsubscribeDone(eaiProtocol_t config, eaipSubRequest_t request) {
  char topic[getTopicLength(DONE, config.baseUrl, request.targetId, request.dataId)];
  parseTopic(topic, DONE, config.baseUrl, request.targetId, request.dataId);

  return config.unsubscribe(topic);
}

/* endregion UNSUBSCRIBE */
