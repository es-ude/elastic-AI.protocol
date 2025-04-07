#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eaip/brokerMock/Broker.h"
#include "eaip/endpoint/CommunicationEndpoint.h"

/* region TOPIC VALIDATION */
#define MQTT_MAX_TOPIC_LENGTH 128 /*! As defined by the ESP32 AT commands */
#define REGEX_MQTT_SUBSCRIBE                                                                       \
  "^(\\/?[a-zA-Z0-9_\\!$&'()*+,.:;=?@%~-]*(\\/[a-zA-Z0-9_\\!$&'()*+,.:;=?@%~-]*)*(\\/"             \
  "\\#)?|\\+)$"
#define REGEX_MQTT_PUBLISH                                                                         \
  "^(\\/?[a-zA-Z0-9_\\!$&'()*,.:;=?@%~-]+(\\/[a-zA-Z0-9_\\!$&'()*,.:;=?@%~-]*)*\\/?$)"

typedef struct validationResult {
  int errorCode;
  bool isValidTopic;
} validationResult_t;
validationResult_t validateTopicForSubscribe(char *topic) {
  regex_t regex;
  validationResult_t validation = {.errorCode = 0};

  validation.errorCode = regcomp(&regex, REGEX_MQTT_SUBSCRIBE, REG_EXTENDED);
  if (validation.errorCode == 0) {
    int result = regexec(&regex, topic, 0, NULL, 0);
    if (result == 0) {
      validation.isValidTopic = true;
    } else if (result == REG_NOMATCH) {
      validation.isValidTopic = false;
    } else {
      validation.errorCode = result;
    }
  }
  regfree(&regex);
  return validation;
}
validationResult_t validateTopicForPublish(char *topic) {
  regex_t regex;
  validationResult_t validation = {.errorCode = 0};

  validation.errorCode = regcomp(&regex, REGEX_MQTT_PUBLISH, REG_EXTENDED);
  if (validation.errorCode == 0) {
    int result = regexec(&regex, topic, 0, NULL, 0);
    if (result == 0) {
      validation.isValidTopic = true;
    } else if (result == REG_NOMATCH) {
      validation.isValidTopic = false;
    } else {
      validation.errorCode = result;
    }
  }
  regfree(&regex);
  return validation;
}

bool topicIsTooLong(char *topic) {
  return (topic != NULL && strlen(topic) > MQTT_MAX_TOPIC_LENGTH);
}
bool topicIsAlreadySubscribed(char *topic) {
  if (subscriptions != NULL) {
    subscriptions_t *current = subscriptions;
    while (current != NULL) {
      if (0 == strcmp(current->subscription->topic, topic)) {
        return true;
      }
      current = current->next;
    }
  }
  return false;
}
bool subscribedTopicIsSame(char *subscription, char *topic) {
  while (*subscription && *topic) {
    if (*subscription == '#') {
      // match all remaining levels
      return 1;
    } else if (*subscription == '+') {
      // skip one level in the topic
      while (*topic && *topic != '/') {
        topic++;
      }
      subscription++;
    } else if (*subscription == *topic) {
      // level is equal
      subscription++;
      topic++;
    } else {
      // mismatch
      return 0;
    }

    if (*topic == '/' && *subscription != '/' && *subscription != '#') {
      // mismatch due to structural difference
      return 0;
    }
  }

  if (*subscription == '#') {
    // handle trailing `/` or `#`
    return 1;
  }

  return *subscription == '\0' && *topic == '\0';
}
/* endregion TOPIC VALIDATION */

/* region SUBSCRIPTION MANAGEMENT */
subscriptions_t *subscriptions = NULL;
void appendSubscription(subscription_t *subscription) {
  subscriptions_t *new = calloc(1, sizeof(subscriptions_t));
  new->subscription = subscription;
  new->next = NULL;

  if (subscriptions == NULL) {
    subscriptions = new;
    return;
  }

  subscriptions_t *current = subscriptions;
  while (current->next != NULL) {
    current = current->next;
  }
  current->next = new;
}
void freeSubscription(subscriptions_t *subscriptionItem) {
  free(subscriptionItem->subscription->topic);
  free(subscriptionItem->subscription);
  free(subscriptionItem);
}
void removeSubscription(char *topic) {
  subscriptions_t *last = NULL;
  subscriptions_t *current = subscriptions;

  if (current == NULL) {
    // Nothing to do here!
  } else if (0 == strcmp(current->subscription->topic, topic)) {
    subscriptions = current->next;
    freeSubscription(current);
  } else if (current->next != NULL) {
    last = current;
    current = current->next;
    while (0 != strcmp(current->subscription->topic, topic)) {
      last = current;
      current = current->next;
      if (current == NULL) {
        return;
      }
    }
    last->next = current->next;
    freeSubscription(current);
  }
}
/* endregion SUBSCRIPTION MANAGEMENT */

eaipCommunicationErrorCodes subscribe(char *topic, void (*handle)(char *topic, char *message)) {
  if (topicIsTooLong(topic)) {
    return EAIP_COM_TOPIC_TO_LONG;
  }

  validationResult_t validation = validateTopicForSubscribe(topic);
  if (0 != validation.errorCode) {
    return EAIP_COM_GENERIC_ERROR;
  } else if (!validation.isValidTopic) {
    return EAIP_COM_INVALID_TOPIC;
  }

  if (topicIsAlreadySubscribed(topic)) {
    return EAIP_COM_TOPIC_ALREADY_SUBSCRIBED;
  }

  subscription_t *newSubscription = calloc(1, sizeof(subscription_t));
  newSubscription->topic = calloc(1, strlen(topic) + 1);
  strcpy(newSubscription->topic, topic);
  newSubscription->handle = handle;
  appendSubscription(newSubscription);

  return EAIP_COM_NO_ERROR;
}

eaipCommunicationErrorCodes unsubscribe(char *topic) {
  removeSubscription(topic);
  return EAIP_COM_NO_ERROR;
}

eaipCommunicationErrorCodes publish(char *topic, char *data, __attribute__((unused)) bool retain) {
  if (topicIsTooLong(topic)) {
    return EAIP_COM_TOPIC_TO_LONG;
  }

  validationResult_t validation = validateTopicForPublish(topic);
  if (0 != validation.errorCode) {
    return EAIP_COM_GENERIC_ERROR;
  } else if (!validation.isValidTopic) {
    return EAIP_COM_INVALID_TOPIC;
  }

  subscriptions_t *current = subscriptions;
  while (current != NULL) {
    if (subscribedTopicIsSame(current->subscription->topic, topic)) {
      current->subscription->handle(topic, data);
    }
    current = current->next;
  }
  return EAIP_COM_NO_ERROR;
}

void resetSubscriptions() {
  subscriptions_t *current = subscriptions;
  while (current != NULL) {
    subscriptions_t *next = current->next;

    freeSubscription(current);
    current = next;
  }
  subscriptions = NULL;
}
