#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "eaip/brokerMock/Broker.h"
#include "unity.h"

void cleanSubscriptions() {
  subscriptions_t *current = subscriptions;
  while (current != NULL) {
    subscriptions = current->next;
    free(current->subscription->topic);
    free(current->subscription);
    free(current);
    current = subscriptions;
  }
}

char expectedTopicPublish[] = "eaip://test-dev/topic";
char expectedTopicSubscribe[] = "eaip://+/topic";
void validateTopic(char *topic, __attribute__((unused)) char *data) {
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedTopicPublish, topic, strlen(expectedTopicPublish));
}
char expectedData[] = "Catch me if you can!";
void validateData(__attribute__((unused)) char *topic, char *data) {
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedData, data, strlen(expectedData));
}

void test_publishFailsWithTopicToLong() {
  char topic[] = "/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/"
                 "test/test/test/test/test/test/test/test/test/test/test";
  TEST_ASSERT_EQUAL_UINT(EAIP_COM_TOPIC_TO_LONG, publish(topic, NULL, false));
}
void test_publishFailsWithInvalidTopic() {
  TEST_ASSERT_EQUAL_UINT(EAIP_COM_INVALID_TOPIC, publish(expectedTopicSubscribe, NULL, false));
}
void test_publishSuccessful() {
  TEST_ASSERT_EQUAL_UINT(EAIP_COM_NO_ERROR, publish(expectedTopicPublish, NULL, false));
}
void test_publishTopicCorrect() {
  subscription_t *newSub = calloc(1, sizeof(subscription_t));
  newSub->topic = calloc(1, strlen(expectedTopicSubscribe) + 1);
  strcpy(newSub->topic, expectedTopicSubscribe);
  newSub->handle = &validateTopic;
  subscriptions = calloc(1, sizeof(subscription_t));
  subscriptions->subscription = newSub;

  publish(expectedTopicPublish, NULL, false);
}
void test_publishDataCorrect() {
  subscription_t *newSub = calloc(1, sizeof(subscription_t));
  newSub->topic = calloc(1, strlen(expectedTopicSubscribe) + 1);
  strcpy(newSub->topic, expectedTopicSubscribe);
  newSub->handle = &validateData;
  subscriptions = calloc(1, sizeof(subscription_t));
  subscriptions->subscription = newSub;

  publish(expectedTopicPublish, expectedData, false);
}

void test_subscribeWithTopicToLongFails() {
  char topic[] = "/test/test/test/test/test/test/test/test/test/test/test/test/test/test/test/"
                 "test/test/test/test/test/test/test/test/test/test/test";
  TEST_ASSERT_EQUAL_UINT(EAIP_COM_TOPIC_TO_LONG, subscribe(topic, NULL));
}
void test_subscribeFirstTopicSuccessful() {
  TEST_ASSERT_NULL(subscriptions);
  TEST_ASSERT_EQUAL_UINT(EAIP_COM_NO_ERROR, subscribe(expectedTopicSubscribe, NULL));
  TEST_ASSERT_NOT_NULL(subscriptions);
}
void test_subscribeFailsForSameTopic() {
  TEST_ASSERT_EQUAL_UINT(EAIP_COM_NO_ERROR, subscribe(expectedTopicSubscribe, NULL));
  TEST_ASSERT_EQUAL_UINT(EAIP_COM_TOPIC_ALREADY_SUBSCRIBED,
                         subscribe(expectedTopicSubscribe, NULL));
}
void test_subscribeMultipleTopicsSuccessful() {
  TEST_ASSERT_NULL(subscriptions);
  TEST_ASSERT_EQUAL_UINT(EAIP_COM_NO_ERROR, subscribe(expectedTopicSubscribe, NULL));
  TEST_ASSERT_EQUAL_UINT(EAIP_COM_NO_ERROR, subscribe(expectedTopicPublish, NULL));
  TEST_ASSERT_NOT_NULL(subscriptions->next);
}
void test_subscribeTopicCorrect() {
  subscribe(expectedTopicSubscribe, NULL);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(subscriptions->subscription->topic, expectedTopicSubscribe,
                               strlen(expectedTopicSubscribe));
}
void test_subscribeHandleCorrect() {
  subscribe(expectedTopicSubscribe, &validateTopic);
  TEST_ASSERT_EQUAL_PTR(subscriptions->subscription->handle, &validateTopic);
}

void test_unsubscribeWithNoSubscriptions() {
  TEST_ASSERT_EQUAL_UINT(EAIP_COM_NO_ERROR, unsubscribe(expectedTopicSubscribe));
}
void test_unsubscribeFromNonSubscribedTopic() {
  subscribe(expectedTopicSubscribe, NULL);
  TEST_ASSERT_EQUAL_UINT(EAIP_COM_NO_ERROR, unsubscribe(expectedTopicPublish));
}
void test_unsubscribeFromSubscribedTopicSuccessful() {
  subscribe(expectedTopicSubscribe, NULL);
  TEST_ASSERT_NOT_NULL(subscriptions);
  unsubscribe(expectedTopicSubscribe);
  TEST_ASSERT_NULL(subscriptions);
}
void test_unsubscribeFromSubscribedMiddleTopicSuccessful() {
  subscribe(expectedTopicSubscribe, NULL);
  subscribe(expectedTopicPublish, NULL);
  TEST_ASSERT_NOT_NULL(subscriptions);
  unsubscribe(expectedTopicSubscribe);
  TEST_ASSERT_NOT_NULL(subscriptions);
}

void setUp(void) {}

void tearDown(void) { cleanSubscriptions(); }

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_publishFailsWithTopicToLong);
  RUN_TEST(test_publishFailsWithInvalidTopic);
  RUN_TEST(test_publishSuccessful);
  RUN_TEST(test_publishTopicCorrect);
  RUN_TEST(test_publishDataCorrect);

  RUN_TEST(test_subscribeWithTopicToLongFails);
  RUN_TEST(test_subscribeFirstTopicSuccessful);
  RUN_TEST(test_subscribeFailsForSameTopic);
  RUN_TEST(test_subscribeMultipleTopicsSuccessful);
  RUN_TEST(test_subscribeTopicCorrect);
  RUN_TEST(test_subscribeHandleCorrect);

  RUN_TEST(test_unsubscribeWithNoSubscriptions);
  RUN_TEST(test_unsubscribeFromNonSubscribedTopic);
  RUN_TEST(test_unsubscribeFromSubscribedTopicSuccessful);
  RUN_TEST(test_unsubscribeFromSubscribedMiddleTopicSuccessful);

  return UNITY_END();
}
