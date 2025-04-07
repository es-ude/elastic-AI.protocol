#ifndef EAI_PROTOCOL_BROKERMOCK_HEADER
#define EAI_PROTOCOL_BROKERMOCK_HEADER

#include "eaip/endpoint/CommunicationEndpoint.h"

typedef struct subscription subscription_t;
struct subscription {
    char *topic;
    void (*handle)(char *topic, char *message);
};

typedef struct subscriptions subscriptions_t;
struct subscriptions {
    subscription_t *subscription;
    subscriptions_t *next;
};

extern subscriptions_t *subscriptions;

void resetSubscriptions(void);

#endif /* EAI_PROTOCOL_BROKERMOCK_HEADER */
