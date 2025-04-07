#ifndef EAI_PROTOCOL_PROTOCOL_HEADER
#define EAI_PROTOCOL_PROTOCOL_HEADER

/*!
 * Public API for the elastic-AI protocol library
 *
 * This library assumes the implementation of the functions described in the
 * "eaip/protocol/CommunicationEndpoint.h" header to be implemented by the user.
 * These functions provide the required interface to the pub-sub broker used for
 * communication.
 */

#include <stdbool.h>

#include "eaip/endpoint/CommunicationEndpoint.h"

/* region CONFIGURATION */

/*!
 * @brief struct providing elastic-AI protocol configuration
 *
 * @param deviceId[char *] unique device identifier
 * @param baseUrl[char *] base url of the elastic-AI endpoint
 * @param publish function to handle publish requests
 * @param subscribe function to handle subscribe requests
 * @param unsubscribe function to handle unsubscribe requests
 *
 * IMPORTANT: The memory for the `deviceId` and `basUrl` field should be allocated on the heap with
 * `calloc`.
 */
typedef struct eaiProtocol {
    char *deviceId;
    char *baseUrl;
    eaipCommunicationErrorCodes (*publish)(char *topic, char *message, bool retain);
    eaipCommunicationErrorCodes (*subscribe)(char *topic,
                                             void (*handle)(char *topic, char *message));
    eaipCommunicationErrorCodes (*unsubscribe)(char *topic);
} eaiProtocol_t;

/* endregion CONFIGURATION */

/* region STATUS */

typedef enum deviceState { OFFLINE, ONLINE } deviceState_t;

typedef enum deviceType { APPLICATION, NODE } deviceType_t;

typedef struct eaipStateDataField eaipStateDataField_t;
struct eaipStateDataField {
    char *id;
    char *data;
    eaipStateDataField_t *next;
};

/*!
 * @brief A struct providing information for a status posting
 *
 * @param deviceType[deviceType_t] type of the device
 * @param deviceState[deviceState_t] state of the device
 * @param additionalFields[dataField_t *] additional Fields to add to the status
 *
 * IMPORTANT: The memory for the `id` and `data` field in the `additionalFields` struct must be
 *            allocated on the heap with `calloc`.
 *            The memory will be freed after the status is published successfully!
 */
typedef struct eaipDeviceState {
    deviceType_t deviceType;
    deviceState_t deviceState;
    eaipStateDataField_t *additionalFields;
} eaipDeviceState_t;

/* endregion STATUS */

/* region Requests */

typedef struct eaipPubRequest {
    char *deviceId;
    char *dataId;
    char *data;
} eaipPubRequest_t;

/*!
 * @brief function pointer for handler to process received posting
 *
 * @param topic[char*] topic of received message
 * @param data[char *] received message
 */
typedef void (*messageHandler)(char *topic, char *message);

typedef struct eaipSubRequest {
    char *targetId;
    char *dataId;
    messageHandler handler;
} eaipSubRequest_t;

/* endregion Requests */

/* region PUBLISH */

/*!
 * @brief publish state
 *
 * @param config[eaiProtocol_t] configuration
 * @param status[status_t] status to publish
 *
 * @return 0 if no error occurred
 */
eaipCommunicationErrorCodes eaipPublishStatus(eaiProtocol_t config, eaipDeviceState_t status);

/*!
 * @brief publish data
 *
 * @param config[eaiProtocol_t] configuration
 * @param request[eaipPubReuest_t] request
 *                                 deviceId -> unused
 *                                 dataId -> data-ID to publish for
 *                                 data -> data to publish
 *
 * @return 0 if no error occurred
 */
eaipCommunicationErrorCodes eaipPublishData(eaiProtocol_t config, eaipPubRequest_t request);

/*!
 * @brief publish data start request
 *
 * @param config[eaiProtocol_t] configuration
 * @param request[eaipPubReuest_t] request
 *                                 deviceId -> device-ID to start requesting data from
 *                                 dataId -> name of the data field to start requesting data
 *                                 data -> unused
 *
 * @return 0 if no error occurred
 */
eaipCommunicationErrorCodes eaipPublishStart(eaiProtocol_t config, eaipPubRequest_t request);

/*!
 * @brief publish data stop request
 *
 * @param config[eaiProtocol_t] configuration
 * @param request[eaipPubReuest_t] request
 *                                 deviceId -> device-ID to stop requesting data from
 *                                 dataId -> name of the data field to stop requesting data
 *                                 data -> unused
 *
 * @return 0 if no error occurred
 */
eaipCommunicationErrorCodes eaipPublishStop(eaiProtocol_t config, eaipPubRequest_t request);

/*!
 * @brief publish command
 *
 * @param config[eaiProtocol_t] configuration
 * @param request[eaipPubReuest_t] request
 *                                 deviceId -> device-ID to publish command for
 *                                 dataId -> command to publish
 *                                 data -> settings to give with command
 *
 * @return 0 if no error occurred
 */
eaipCommunicationErrorCodes eaipPublishDo(eaiProtocol_t config, eaipPubRequest_t request);

/*!
 * @brief publish command execution result
 *
 * @param config[eaiProtocol_t] configuration
 * @param request[eaipPubReuest_t] request
 *                                 deviceId -> unused
 *                                 dataId -> command to publish execution result for
 *                                 data -> status of command execution
 *
 * @return 0 if no error occurred
 */
eaipCommunicationErrorCodes eaipPublishDone(eaiProtocol_t config, eaipPubRequest_t request);

/* endregion PUBLISH */

/* region SUBSCRIBE */

/*!
 * @brief subscribe to state updates
 *
 * @param config[eaiProtocol_t] configuration
 * @param request[eaipSubRequest] request
 *                                targetId -> target device to subscribe states
 *                                dataId -> unused
 *                                handler -> function to handle received states
 *
 * @return 0 if no error occurred
 */
eaipCommunicationErrorCodes eaipSubscribeStatus(eaiProtocol_t config, eaipSubRequest_t request);

/*!
 * @brief subscribe to data
 *
 * @param config[eaiProtocol_t] configuration
 * @param request[eaipSubRequest] request
 *                                targetId -> target device to subscribe data
 *                                dataId -> id of data field to subscribe
 *                                handler -> function to handle received data
 *
 * @return 0 if no error occurred
 */
eaipCommunicationErrorCodes eaipSubscribeData(eaiProtocol_t config, eaipSubRequest_t request);

/*!
 * @brief subscribe to start requests
 *
 * @param config[eaiProtocol_t] configuration
 * @param request[eaipSubRequest] request
 *                                targetId -> unused
 *                                dataId -> id of data field to subscribe
 *                                handler -> function to handle received requests
 *
 * @return 0 if no error occurred
 */
eaipCommunicationErrorCodes eaipSubscribeStart(eaiProtocol_t config, eaipSubRequest_t request);

/*!
 * @brief subscribe to stop requests
 *
 * @param config[eaiProtocol_t] configuration
 * @param request[eaipSubRequest] request
 *                                targetId -> unused
 *                                dataId -> id of data field to subscribe
 *                                handler -> function to handle received requests
 *
 * @return 0 if no error occurred
 */
eaipCommunicationErrorCodes eaipSubscribeStop(eaiProtocol_t config, eaipSubRequest_t request);

/*!
 * @brief subscribe to command
 *
 * @param config[eaiProtocol_t] configuration
 * @param request[eaipSubRequest] request
 *                                targetId -> unused
 *                                dataId -> command to subscribe for
 *                                handler -> function to handle received requests
 *
 * @return 0 if no error occurred
 */
eaipCommunicationErrorCodes eaipSubscribeDo(eaiProtocol_t config, eaipSubRequest_t request);

/*!
 * @brief subscribe to command
 *
 * @param config[eaiProtocol_t] configuration
 * @param request[eaipSubRequest] request
 *                                targetId -> target device to subscribe data
 *                                dataId -> command to subscribe
 *                                handler -> function to handle received data
 *
 * @return 0 if no error occurred
 */
eaipCommunicationErrorCodes eaipSubscribeDone(eaiProtocol_t config, eaipSubRequest_t request);

/* endregion SUBSCRIBE */

/* region UNSUBSCRIBE */

/*!
 * @brief unsubscribe from state updates
 *
 * @param config[eaiProtocol_t] configuration
 * @param request[eaipSubRequest] request
 *                                deviceId -> target device to unsubscribe states
 *                                dataId -> unused
 *                                handler -> unused
 *
 * @return 0 if no error occurred
 */
eaipCommunicationErrorCodes eaipUnsubscribeStatus(eaiProtocol_t config, eaipSubRequest_t request);

/*!
 * @brief unsubscribe from data
 *
 * @param config[eaiProtocol_t] configuration
 * @param request[eaipSubRequest] request
 *                                deviceId -> target device to unsubscribe data
 *                                dataId -> data-id to unsubscribe
 *                                handler -> unused
 *
 * @return 0 if no error occurred
 */
eaipCommunicationErrorCodes eaipUnsubscribeData(eaiProtocol_t config, eaipSubRequest_t request);

/*!
 * @brief unsubscribe from data start requests
 *
 * @param config[eaiProtocol_t] configuration
 * @param request[eaipSubRequest] request
 *                                deviceId -> unused
 *                                dataId -> data-id to unsubscribe
 *                                handler -> unused
 *
 * @return 0 if no error occurred
 */
eaipCommunicationErrorCodes eaipUnsubscribeStart(eaiProtocol_t config, eaipSubRequest_t request);

/*!
 * @brief unsubscribe from data stop requests
 *
 * @param config[eaiProtocol_t] configuration
 * @param request[eaipSubRequest] request
 *                                deviceId -> unused
 *                                dataId -> data-id to unsubscribe
 *                                handler -> unused
 *
 * @return 0 if no error occurred
 */
eaipCommunicationErrorCodes eaipUnsubscribeStop(eaiProtocol_t config, eaipSubRequest_t request);

/*!
 * @brief unsubscribe from command requests
 *
 * @param config[eaiProtocol_t] configuration
 * @param request[eaipSubRequest] request
 *                                deviceId -> unused
 *                                dataId -> command to unsubscribe
 *                                handler -> unused
 *
 * @return 0 if no error occurred
 */
eaipCommunicationErrorCodes eaipUnsubscribeDo(eaiProtocol_t config, eaipSubRequest_t request);

/*!
 * @brief unsubscribe from command execution states
 *
 * @param config[eaiProtocol_t] configuration
 * @param request[eaipSubRequest] request
 *                                deviceId -> target device to unsubscribe
 *                                dataId -> command to unsubscribe
 *                                handler -> unused
 *
 * @return 0 if no error occurred
 */
eaipCommunicationErrorCodes eaipUnsubscribeDone(eaiProtocol_t config, eaipSubRequest_t request);

/* endregion UNSUBSCRIBE */

#endif /* EAI_PROTOCOL_PROTOCOL_HEADER */
