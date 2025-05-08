#include <stdlib.h>
#include <string.h>

#include "eaip/brokerMock/Broker.h"
#include "eaip/protocol/Protocol.h"
#include "unity.h"

#define BASE_URL "eaip://local-net"
#define DEVICE_ID "test-dev"

/* region TEST RUNTIME */
eaiProtocol_t config = {
    .subscribe = &subscribe,
    .unsubscribe = &unsubscribe,
    .publish = &publish,
    .baseUrl = BASE_URL,
    .deviceId = DEVICE_ID,
};

char *receivedTopic = NULL;
char *receivedData = NULL;
void validateTopic(char *topic, char *data) {
    receivedTopic = calloc(strlen(topic) + 1, sizeof(char));
    strcpy(receivedTopic, topic);
    receivedData = calloc(strlen(data) + 1, sizeof(char));
    strcpy(receivedData, data);
}
/* endregion TEST RUNTIME */

void test_publishStatusSuccessful() {
    eaipDeviceState_t state = {.deviceState = ONLINE, .deviceType = NODE};
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipPublishStatus(config, state));
}
void test_publishStatusTopicCorrect() {
    char expectedTopic[] = BASE_URL "/" DEVICE_ID "/STATUS";
    subscribe(expectedTopic, &validateTopic);

    eaipDeviceState_t state = {.deviceState = ONLINE, .deviceType = NODE};
    eaipPublishStatus(config, state);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedTopic, receivedTopic, strlen(expectedTopic));
}
void test_publishStandardStatusCorrect() {
    char expectedTopic[] = BASE_URL "/" DEVICE_ID "/STATUS";
    char expectedMessage[] = "ID:" DEVICE_ID ";TYPE:enV5;STATE:ONLINE;";
    subscribe(expectedTopic, &validateTopic);

    eaipDeviceState_t state = {.deviceState = ONLINE, .deviceType = NODE};
    eaipPublishStatus(config, state);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedMessage, receivedData, strlen(expectedMessage));
}
void test_publishExtendedStatusCorrect() {
    char expectedTopic[] = BASE_URL "/" DEVICE_ID "/STATUS";
    char expectedMessage[] =
        "ID:" DEVICE_ID ";TYPE:enV5;STATE:ONLINE;TEST F2:TEST DATA 02;TEST F1:TEST DATA 01;";
    subscribe(expectedTopic, &validateTopic);

    eaipStateDataField_t additionalField1 = {.id = "TEST F1", .data = "TEST DATA 01", .next = NULL};
    eaipStateDataField_t additionalField2 = {
        .id = "TEST F2", .data = "TEST DATA 02", .next = &additionalField1};
    eaipDeviceState_t state = {
        .deviceState = ONLINE, .deviceType = NODE, .additionalFields = &additionalField2};
    eaipPublishStatus(config, state);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedMessage, receivedData, strlen(expectedMessage));
}

void test_publishDataSuccessful() {
    eaipPubRequest_t data = {.dataId = "test-top", .data = "DATA"};
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipPublishData(config, data));
}
void test_publishDataTopicCorrect() {
    char expectedTopic[] = BASE_URL "/" DEVICE_ID "/DATA/test-top";
    subscribe(expectedTopic, &validateTopic);

    eaipPubRequest_t data = {.dataId = "test-top", .data = "DATA"};
    eaipPublishData(config, data);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedTopic, receivedTopic, strlen(expectedTopic));
}
void test_publishDataDataCorrect() {
    char expectedTopic[] = BASE_URL "/" DEVICE_ID "/DATA/test-top";
    char expectedMessage[] = "0.231F";
    subscribe(expectedTopic, &validateTopic);

    eaipPubRequest_t data = {.dataId = "test-top", .data = "0.231F"};
    eaipPublishData(config, data);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedMessage, receivedData, strlen(expectedMessage));
}

void test_publishStartRequestSuccessful() {
    eaipPubRequest_t data = {.deviceId = "test-receiver", .dataId = "test-top"};
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipPublishStart(config, data));
}
void test_publishStartRequestTopicCorrect() {
    char expectedTopic[] = BASE_URL "/test-receiver/START/test-top";
    subscribe(expectedTopic, &validateTopic);

    eaipPubRequest_t data = {.deviceId = "test-receiver", .dataId = "test-top"};
    eaipPublishStart(config, data);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedTopic, receivedTopic, strlen(expectedTopic));
}
void test_publishStartRequestDataCorrect() {
    char expectedTopic[] = BASE_URL "/test-receiver/START/test-top";
    char expectedMessage[] = BASE_URL "/" DEVICE_ID;
    subscribe(expectedTopic, &validateTopic);

    eaipPubRequest_t data = {.deviceId = "test-receiver", .dataId = "test-top"};
    eaipPublishStart(config, data);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedMessage, receivedData, strlen(expectedMessage));
}

void test_publishStopRequestSuccessful() {
    eaipPubRequest_t data = {.deviceId = "test-receiver", .dataId = "test-top"};
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipPublishStop(config, data));
}
void test_publishStopRequestTopicCorrect() {
    char expectedTopic[] = BASE_URL "/test-receiver/STOP/test-top";
    subscribe(expectedTopic, &validateTopic);

    eaipPubRequest_t data = {.deviceId = "test-receiver", .dataId = "test-top"};
    eaipPublishStop(config, data);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedTopic, receivedTopic, strlen(expectedTopic));
}
void test_publishStopRequestDataCorrect() {
    char expectedTopic[] = BASE_URL "/test-receiver/STOP/test-top";
    char expectedMessage[] = BASE_URL "/" DEVICE_ID;
    subscribe(expectedTopic, &validateTopic);

    eaipPubRequest_t data = {.deviceId = "test-receiver", .dataId = "test-top"};
    eaipPublishStop(config, data);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedMessage, receivedData, strlen(expectedMessage));
}

void test_publishDoRequestSuccessful() {
    eaipPubRequest_t data = {.deviceId = "test-receiver", .dataId = "test-com", .data = "settings"};
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipPublishDo(config, data));
}
void test_publishDoRequestTopicCorrect() {
    char expectedTopic[] = BASE_URL "/test-receiver/DO/test-com";
    subscribe(expectedTopic, &validateTopic);

    eaipPubRequest_t data = {.deviceId = "test-receiver", .dataId = "test-com", .data = "settings"};
    eaipPublishDo(config, data);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedTopic, receivedTopic, strlen(expectedTopic));
}
void test_publishDoRequestDataCorrect() {
    char expectedTopic[] = BASE_URL "/test-receiver/DO/test-com";
    char expectedMessage[] = "settings";
    subscribe(expectedTopic, &validateTopic);

    eaipPubRequest_t data = {.deviceId = "test-receiver", .dataId = "test-com", .data = "settings"};
    eaipPublishDo(config, data);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedMessage, receivedData, strlen(expectedMessage));
}

void test_publishDoneMessageSuccessful() {
    eaipPubRequest_t data = {.dataId = "test-com", .data = "result"};
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipPublishDone(config, data));
}
void test_publishDoneMessageTopicCorrect() {
    char expectedTopic[] = BASE_URL "/" DEVICE_ID "/DONE/test-com";
    subscribe(expectedTopic, &validateTopic);

    eaipPubRequest_t data = {.dataId = "test-com", .data = "result"};
    eaipPublishDone(config, data);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedTopic, receivedTopic, strlen(expectedTopic));
}
void test_publishDoneMessageDataCorrect() {
    char expectedTopic[] = BASE_URL "/" DEVICE_ID "/DONE/test-com";
    char expectedMessage[] = "result";
    subscribe(expectedTopic, &validateTopic);

    eaipPubRequest_t data = {.dataId = "test-com", .data = "result"};
    eaipPublishDone(config, data);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedMessage, receivedData, strlen(expectedMessage));
}

void test_subscribeStatusSuccessful() {
    eaipSubRequest_t request = {.targetId = "test-device", .handler = NULL};
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipSubscribeStatus(config, request));
}
void test_subscribeStatusTopicCorrect() {
    char expectedTopic[] = BASE_URL "/test-device/STATUS";

    eaipSubRequest_t request = {.targetId = "test-device", .handler = NULL};
    eaipSubscribeStatus(config, request);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedTopic, subscriptions->subscription->topic,
                                 strlen(expectedTopic));
}
void test_subscribeStatusHandlerCorrect() {
    eaipSubRequest_t request = {.targetId = "test-device", .handler = &validateTopic};
    eaipSubscribeStatus(config, request);

    TEST_ASSERT_EQUAL_PTR(&validateTopic, subscriptions->subscription->handle);
}

void test_subscribeDataSuccessful() {
    eaipSubRequest_t request = {.targetId = "test-device", .dataId = "test-data", .handler = NULL};
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipSubscribeData(config, request));
}
void test_subscribeDataTopicCorrect() {
    char expectedTopic[] = BASE_URL "/test-device/DATA/test-data";

    eaipSubRequest_t request = {.targetId = "test-device", .dataId = "test-data", .handler = NULL};
    eaipSubscribeData(config, request);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedTopic, subscriptions->subscription->topic,
                                 strlen(expectedTopic));
}
void test_subscribeDataHandlerCorrect() {
    eaipSubRequest_t request = {
        .targetId = "test-device", .dataId = "test-data", .handler = &validateTopic};
    eaipSubscribeData(config, request);

    TEST_ASSERT_EQUAL_PTR(&validateTopic, subscriptions->subscription->handle);
}

void test_subscribeStartSuccessful() {
    eaipSubRequest_t request = {.dataId = "test-data", .handler = NULL};
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipSubscribeStart(config, request));
}
void test_subscribeStartTopicCorrect() {
    char expectedTopic[] = BASE_URL "/" DEVICE_ID "/START/test-data";

    eaipSubRequest_t request = {.dataId = "test-data", .handler = NULL};
    eaipSubscribeStart(config, request);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedTopic, subscriptions->subscription->topic,
                                 strlen(expectedTopic));
}
void test_subscribeStartHandlerCorrect() {
    eaipSubRequest_t request = {.dataId = "test-data", .handler = &validateTopic};
    eaipSubscribeStart(config, request);

    TEST_ASSERT_EQUAL_PTR(&validateTopic, subscriptions->subscription->handle);
}

void test_subscribeStopSuccessful() {
    eaipSubRequest_t request = {.dataId = "test-data", .handler = NULL};
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipSubscribeStop(config, request));
}
void test_subscribeStopTopicCorrect() {
    char expectedTopic[] = BASE_URL "/" DEVICE_ID "/STOP/test-data";

    eaipSubRequest_t request = {.dataId = "test-data", .handler = NULL};
    eaipSubscribeStop(config, request);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedTopic, subscriptions->subscription->topic,
                                 strlen(expectedTopic));
}
void test_subscribeStopHandlerCorrect() {
    eaipSubRequest_t request = {.dataId = "test-data", .handler = &validateTopic};
    eaipSubscribeStop(config, request);

    TEST_ASSERT_EQUAL_PTR(&validateTopic, subscriptions->subscription->handle);
}

void test_subscribeDoSuccessful() {
    eaipSubRequest_t request = {.dataId = "test-cmd", .handler = NULL};
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipSubscribeDo(config, request));
}
void test_subscribeDoTopicCorrect() {
    char expectedTopic[] = BASE_URL "/" DEVICE_ID "/DO/test-cmd";

    eaipSubRequest_t request = {.dataId = "test-cmd", .handler = NULL};
    eaipSubscribeDo(config, request);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedTopic, subscriptions->subscription->topic,
                                 strlen(expectedTopic));
}
void test_subscribeDoHandlerCorrect() {
    eaipSubRequest_t request = {.dataId = "test-cmd", .handler = &validateTopic};
    eaipSubscribeDo(config, request);

    TEST_ASSERT_EQUAL_PTR(&validateTopic, subscriptions->subscription->handle);
}

void test_subscribeDoneSuccessful() {
    eaipSubRequest_t request = {.targetId = "test-device", .dataId = "test-cmd", .handler = NULL};
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipSubscribeDone(config, request));
}
void test_subscribeDoneTopicCorrect() {
    char expectedTopic[] = BASE_URL "/test-device/DONE/test-cmd";

    eaipSubRequest_t request = {.targetId = "test-device", .dataId = "test-cmd", .handler = NULL};
    eaipSubscribeDone(config, request);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedTopic, subscriptions->subscription->topic,
                                 strlen(expectedTopic));
}
void test_subscribeDoneHandlerCorrect() {
    eaipSubRequest_t request = {
        .targetId = "test-device", .dataId = "test-cmd", .handler = &validateTopic};
    eaipSubscribeDone(config, request);

    TEST_ASSERT_EQUAL_PTR(&validateTopic, subscriptions->subscription->handle);
}

void test_unsubscribeStatusSuccessful() {
    eaipSubRequest_t request = {.targetId = "test-device", .handler = NULL};
    eaipSubscribeStatus(config, request);

    TEST_ASSERT_NOT_NULL(subscriptions);
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipUnsubscribeStatus(config, request));
    TEST_ASSERT_NULL(subscriptions);
}
void test_unsubscribeStatusCorrectTopic() {
    eaipSubRequest_t request1 = {.targetId = "test-device", .handler = NULL};
    eaipSubscribeStatus(config, request1);
    eaipSubRequest_t request2 = {.targetId = "test-device2", .handler = NULL};
    eaipSubscribeStatus(config, request2);

    TEST_ASSERT_NOT_NULL(subscriptions);
    TEST_ASSERT_NOT_NULL(subscriptions->next);
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipUnsubscribeStatus(config, request1));
    TEST_ASSERT_NOT_NULL(subscriptions);
    TEST_ASSERT_NULL(subscriptions->next);
}

void test_unsubscribeDataSuccessful() {
    eaipSubRequest_t request = {.targetId = "test-device", .dataId = "test-data", .handler = NULL};
    eaipSubscribeData(config, request);

    TEST_ASSERT_NOT_NULL(subscriptions);
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipUnsubscribeData(config, request));
    TEST_ASSERT_NULL(subscriptions);
}
void test_unsubscribeDataCorrectTopic() {
    eaipSubRequest_t request1 = {.targetId = "test-device", .dataId = "test-data", .handler = NULL};
    eaipSubscribeData(config, request1);
    eaipSubRequest_t request2 = {
        .targetId = "test-device2", .dataId = "test-data", .handler = NULL};
    eaipSubscribeStatus(config, request2);

    TEST_ASSERT_NOT_NULL(subscriptions);
    TEST_ASSERT_NOT_NULL(subscriptions->next);
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipUnsubscribeData(config, request1));
    TEST_ASSERT_NOT_NULL(subscriptions);
    TEST_ASSERT_NULL(subscriptions->next);
}

void test_unsubscribeStartSuccessful() {
    eaipSubRequest_t request = {.dataId = "test-data", .handler = NULL};
    eaipSubscribeStart(config, request);

    TEST_ASSERT_NOT_NULL(subscriptions);
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipUnsubscribeStart(config, request));
    TEST_ASSERT_NULL(subscriptions);
}
void test_unsubscribeStartCorrectTopic() {
    eaipSubRequest_t request1 = {.dataId = "test-data", .handler = NULL};
    eaipSubscribeStart(config, request1);
    eaipSubRequest_t request2 = {
        .targetId = "test-device2", .dataId = "test-data", .handler = NULL};
    eaipSubscribeStatus(config, request2);

    TEST_ASSERT_NOT_NULL(subscriptions);
    TEST_ASSERT_NOT_NULL(subscriptions->next);
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipUnsubscribeStart(config, request1));
    TEST_ASSERT_NOT_NULL(subscriptions);
    TEST_ASSERT_NULL(subscriptions->next);
}

void test_unsubscribeStopSuccessful() {
    eaipSubRequest_t request = {.dataId = "test-data", .handler = NULL};
    eaipSubscribeStop(config, request);

    TEST_ASSERT_NOT_NULL(subscriptions);
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipUnsubscribeStop(config, request));
    TEST_ASSERT_NULL(subscriptions);
}
void test_unsubscribeStopCorrectTopic() {
    eaipSubRequest_t request1 = {.dataId = "test-data", .handler = NULL};
    eaipSubscribeStop(config, request1);
    eaipSubRequest_t request2 = {
        .targetId = "test-device2", .dataId = "test-data", .handler = NULL};
    eaipSubscribeStatus(config, request2);

    TEST_ASSERT_NOT_NULL(subscriptions);
    TEST_ASSERT_NOT_NULL(subscriptions->next);
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipUnsubscribeStop(config, request1));
    TEST_ASSERT_NOT_NULL(subscriptions);
    TEST_ASSERT_NULL(subscriptions->next);
}

void test_unsubscribeDoSuccessful() {
    eaipSubRequest_t request = {.dataId = "test-data", .handler = NULL};
    eaipSubscribeDo(config, request);

    TEST_ASSERT_NOT_NULL(subscriptions);
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipUnsubscribeDo(config, request));
    TEST_ASSERT_NULL(subscriptions);
}
void test_unsubscribeDoCorrectTopic() {
    eaipSubRequest_t request1 = {.dataId = "test-data", .handler = NULL};
    eaipSubscribeDo(config, request1);
    eaipSubRequest_t request2 = {
        .targetId = "test-device2", .dataId = "test-data", .handler = NULL};
    eaipSubscribeStatus(config, request2);

    TEST_ASSERT_NOT_NULL(subscriptions);
    TEST_ASSERT_NOT_NULL(subscriptions->next);
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipUnsubscribeDo(config, request1));
    TEST_ASSERT_NOT_NULL(subscriptions);
    TEST_ASSERT_NULL(subscriptions->next);
}

void test_unsubscribeDoneSuccessful() {
    eaipSubRequest_t request = {.targetId = "test-device", .dataId = "test-data", .handler = NULL};
    eaipSubscribeDone(config, request);

    TEST_ASSERT_NOT_NULL(subscriptions);
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipUnsubscribeDone(config, request));
    TEST_ASSERT_NULL(subscriptions);
}
void test_unsubscribeDoneCorrectTopic() {
    eaipSubRequest_t request1 = {.targetId = "test-device", .dataId = "test-data", .handler = NULL};
    eaipSubscribeDone(config, request1);
    eaipSubRequest_t request2 = {
        .targetId = "test-device2", .dataId = "test-data", .handler = NULL};
    eaipSubscribeStatus(config, request2);

    TEST_ASSERT_NOT_NULL(subscriptions);
    TEST_ASSERT_NOT_NULL(subscriptions->next);
    TEST_ASSERT_EQUAL(EAIP_COM_NO_ERROR, eaipUnsubscribeDone(config, request1));
    TEST_ASSERT_NOT_NULL(subscriptions);
    TEST_ASSERT_NULL(subscriptions->next);
}

void setUp() {
    TEST_ASSERT_NULL(receivedTopic);
    TEST_ASSERT_NULL(receivedData);
    TEST_ASSERT_NULL(subscriptions);
}

void tearDown() {
    if (receivedTopic != NULL) {
        free(receivedTopic);
        receivedTopic = NULL;
    }

    if (receivedData != NULL) {
        free(receivedData);
        receivedData = NULL;
    }

    resetSubscriptions();
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_publishStatusSuccessful);
    RUN_TEST(test_publishStatusTopicCorrect);
    RUN_TEST(test_publishStandardStatusCorrect);
    RUN_TEST(test_publishExtendedStatusCorrect);

    RUN_TEST(test_publishDataSuccessful);
    RUN_TEST(test_publishDataTopicCorrect);
    RUN_TEST(test_publishDataDataCorrect);

    RUN_TEST(test_publishStartRequestSuccessful);
    RUN_TEST(test_publishStartRequestTopicCorrect);
    RUN_TEST(test_publishStartRequestDataCorrect);

    RUN_TEST(test_publishStopRequestSuccessful);
    RUN_TEST(test_publishStopRequestTopicCorrect);
    RUN_TEST(test_publishStopRequestDataCorrect);

    RUN_TEST(test_publishDoRequestSuccessful);
    RUN_TEST(test_publishDoRequestTopicCorrect);
    RUN_TEST(test_publishDoRequestDataCorrect);

    RUN_TEST(test_publishDoneMessageSuccessful);
    RUN_TEST(test_publishDoneMessageTopicCorrect);
    RUN_TEST(test_publishDoneMessageDataCorrect);

    RUN_TEST(test_subscribeStatusSuccessful);
    RUN_TEST(test_subscribeStatusTopicCorrect);
    RUN_TEST(test_subscribeStatusHandlerCorrect);

    RUN_TEST(test_subscribeDataSuccessful);
    RUN_TEST(test_subscribeDataTopicCorrect);
    RUN_TEST(test_subscribeDataHandlerCorrect);

    RUN_TEST(test_subscribeStartSuccessful);
    RUN_TEST(test_subscribeStartTopicCorrect);
    RUN_TEST(test_subscribeStartHandlerCorrect);

    RUN_TEST(test_subscribeStopSuccessful);
    RUN_TEST(test_subscribeStopTopicCorrect);
    RUN_TEST(test_subscribeStopHandlerCorrect);

    RUN_TEST(test_subscribeDoSuccessful);
    RUN_TEST(test_subscribeDoTopicCorrect);
    RUN_TEST(test_subscribeDoHandlerCorrect);

    RUN_TEST(test_subscribeDoneSuccessful);
    RUN_TEST(test_subscribeDoneTopicCorrect);
    RUN_TEST(test_subscribeDoneHandlerCorrect);

    RUN_TEST(test_unsubscribeStatusSuccessful);
    RUN_TEST(test_unsubscribeStatusCorrectTopic);

    RUN_TEST(test_unsubscribeDataSuccessful);
    RUN_TEST(test_unsubscribeDataCorrectTopic);

    RUN_TEST(test_unsubscribeStartSuccessful);
    RUN_TEST(test_unsubscribeStartCorrectTopic);

    RUN_TEST(test_unsubscribeStopSuccessful);
    RUN_TEST(test_unsubscribeStopCorrectTopic);

    RUN_TEST(test_unsubscribeDoSuccessful);
    RUN_TEST(test_unsubscribeDoCorrectTopic);

    RUN_TEST(test_unsubscribeDoneSuccessful);
    RUN_TEST(test_unsubscribeDoneCorrectTopic);

    return UNITY_END();
}
