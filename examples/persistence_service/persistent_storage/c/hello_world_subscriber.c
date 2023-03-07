/*******************************************************************************
 (c) 2005-2014 Copyright, Real-Time Innovations, Inc.  All rights reserved.
 RTI grants Licensee a license to use, modify, compile, and create derivative
 works of the Software.  Licensee has the right to distribute object form only
 for use with RTI products.  The Software is provided "as is", with no warranty
 of any type, including any warranty for fitness for any purpose. RTI is under
 no obligation to maintain or support the Software.  RTI shall not be liable for
 any incidental or consequential damages arising out of the use or inability to
 use the software.
 ******************************************************************************/
/* hello_world_subscriber.c

   A subscription example

   This file is derived from code automatically generated by the rtiddsgen
   command:

   rtiddsgen -language C -example <arch> hello_world.idl

   Example subscription of type hello_world automatically generated by
   'rtiddsgen'. To test them, follow these steps:

   (1) Compile this file and the example publication.

   (2) Start the subscription with the command
       objs/<arch>/hello_world_subscriber <domain_id> <sample_count>

   (3) Start the publication with the command
       objs/<arch>/hello_world_publisher <domain_id> <sample_count>

   (4) [Optional] Specify the list of discovery initial peers and
       multicast receive addresses via an environment variable or a file
       (in the current working directory) called NDDS_DISCOVERY_PEERS.

   You can run any number of publishers and subscribers programs, and can
   add and remove them dynamically from the domain.


   Example:

       To run the example application on domain <domain_id>:

       On UNIX systems:

       objs/<arch>/hello_world_publisher <domain_id>
       objs/<arch>/hello_world_subscriber <domain_id>

       On Windows systems:

       objs\<arch>\hello_world_publisher <domain_id>
       objs\<arch>\hello_world_subscriber <domain_id>

*/

#include "hello_world.h"
#include "hello_worldSupport.h"
#include "ndds/ndds_c.h"
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
  Macros to read input parameters
******************************************************************************/

#define READ_INTEGER_PARAM(parameter, outValue) \
    if (!strcmp(argv[i], parameter)) {          \
        if (i + 1 >= argc) {                    \
            printf("%s", syntax);               \
            return 1;                           \
        }                                       \
        i++;                                    \
        outValue = atoi(argv[i]);               \
        i++;                                    \
        continue;                               \
    }

#define READ_STRING_PARAM(parameter, outValue) \
    if (!strcmp(argv[i], parameter)) {         \
        if (i + 1 >= argc) {                   \
            printf("%s", syntax);              \
            return 1;                          \
        }                                      \
        i++;                                   \
        outValue = argv[i];                    \
        i++;                                   \
        continue;                              \
    }

void hello_worldListener_on_requested_deadline_missed(
        void *listener_data,
        DDS_DataReader *reader,
        const struct DDS_RequestedDeadlineMissedStatus *status)
{
}

void hello_worldListener_on_requested_incompatible_qos(
        void *listener_data,
        DDS_DataReader *reader,
        const struct DDS_RequestedIncompatibleQosStatus *status)
{
}

void hello_worldListener_on_sample_rejected(
        void *listener_data,
        DDS_DataReader *reader,
        const struct DDS_SampleRejectedStatus *status)
{
}

void hello_worldListener_on_liveliness_changed(
        void *listener_data,
        DDS_DataReader *reader,
        const struct DDS_LivelinessChangedStatus *status)
{
}

void hello_worldListener_on_sample_lost(
        void *listener_data,
        DDS_DataReader *reader,
        const struct DDS_SampleLostStatus *status)
{
}

void hello_worldListener_on_subscription_matched(
        void *listener_data,
        DDS_DataReader *reader,
        const struct DDS_SubscriptionMatchedStatus *status)
{
}

void hello_worldListener_on_data_available(
        void *listener_data,
        DDS_DataReader *reader)
{
    hello_worldDataReader *hello_world_reader = NULL;
    struct hello_worldSeq data_seq = DDS_SEQUENCE_INITIALIZER;
    struct DDS_SampleInfoSeq info_seq = DDS_SEQUENCE_INITIALIZER;
    DDS_ReturnCode_t retcode;
    int i;

    hello_world_reader = hello_worldDataReader_narrow(reader);
    if (hello_world_reader == NULL) {
        printf("DataReader narrow error\n");
        return;
    }

    retcode = hello_worldDataReader_take(
            hello_world_reader,
            &data_seq,
            &info_seq,
            DDS_LENGTH_UNLIMITED,
            DDS_ANY_SAMPLE_STATE,
            DDS_ANY_VIEW_STATE,
            DDS_ANY_INSTANCE_STATE);
    if (retcode == DDS_RETCODE_NO_DATA) {
        return;
    } else if (retcode != DDS_RETCODE_OK) {
        printf("take error %d\n", retcode);
        return;
    }

    for (i = 0; i < hello_worldSeq_get_length(&data_seq); ++i) {
        if (DDS_SampleInfoSeq_get_reference(&info_seq, i)->valid_data) {
            hello_worldTypeSupport_print_data(
                    hello_worldSeq_get_reference(&data_seq, i));
        }
    }

    retcode = hello_worldDataReader_return_loan(
            hello_world_reader,
            &data_seq,
            &info_seq);
    if (retcode != DDS_RETCODE_OK) {
        printf("return loan error %d\n", retcode);
    }
}

/* Delete all entities */
static int subscriber_shutdown(DDS_DomainParticipant *participant)
{
    DDS_ReturnCode_t retcode;
    int status = 0;

    if (participant != NULL) {
        retcode = DDS_DomainParticipant_delete_contained_entities(participant);
        if (retcode != DDS_RETCODE_OK) {
            printf("delete_contained_entities error %d\n", retcode);
            status = -1;
        }

        retcode = DDS_DomainParticipantFactory_delete_participant(
                DDS_TheParticipantFactory,
                participant);
        if (retcode != DDS_RETCODE_OK) {
            printf("delete_participant error %d\n", retcode);
            status = -1;
        }
    }

    /* RTI Connext provides the finalize_instance() method on
       domain participant factory for users who want to release memory used
       by the participant factory. Uncomment the following block of code for
       clean destruction of the singleton. */
    /*
        retcode = DDS_DomainParticipantFactory_finalize_instance();
        if (retcode != DDS_RETCODE_OK) {
            printf("finalize_instance error %d\n", retcode);
            status = -1;
        }
    */

    return status;
}

static int subscriber_main(int domain_id, int sample_count, int drs)
{
    DDS_DomainParticipant *participant = NULL;
    DDS_Subscriber *subscriber = NULL;
    DDS_Topic *topic = NULL;
    struct DDS_DataReaderListener reader_listener =
            DDS_DataReaderListener_INITIALIZER;
    struct DDS_DataReaderQos reader_qos;
    DDS_DataReader *reader = NULL;
    DDS_ReturnCode_t retcode;
    const char *type_name = NULL;
    int count = 0;
    struct DDS_Duration_t poll_period = { 4, 0 };

    /* To customize participant QoS, use
     the configuration file USER_QOS_PROFILES.xml */
    participant = DDS_DomainParticipantFactory_create_participant(
            DDS_TheParticipantFactory,
            domain_id,
            &DDS_PARTICIPANT_QOS_DEFAULT,
            NULL /* listener */,
            DDS_STATUS_MASK_NONE);
    if (participant == NULL) {
        printf("create_participant error\n");
        subscriber_shutdown(participant);
        return -1;
    }

    /* To customize subscriber QoS, use
     the configuration file USER_QOS_PROFILES.xml */
    subscriber = DDS_DomainParticipant_create_subscriber(
            participant,
            &DDS_SUBSCRIBER_QOS_DEFAULT,
            NULL /* listener */,
            DDS_STATUS_MASK_NONE);
    if (subscriber == NULL) {
        printf("create_subscriber error\n");
        subscriber_shutdown(participant);
        return -1;
    }

    /* Register the type before creating the topic */
    type_name = hello_worldTypeSupport_get_type_name();
    retcode = hello_worldTypeSupport_register_type(participant, type_name);
    if (retcode != DDS_RETCODE_OK) {
        printf("register_type error %d\n", retcode);
        subscriber_shutdown(participant);
        return -1;
    }

    /* To customize topic QoS, use
     the configuration file USER_QOS_PROFILES.xml */
    topic = DDS_DomainParticipant_create_topic(
            participant,
            "Example hello_world",
            type_name,
            &DDS_TOPIC_QOS_DEFAULT,
            NULL /* listener */,
            DDS_STATUS_MASK_NONE);
    if (topic == NULL) {
        printf("create_topic error\n");
        subscriber_shutdown(participant);
        return -1;
    }

    /* Set up a data reader listener */
    reader_listener.on_requested_deadline_missed =
            hello_worldListener_on_requested_deadline_missed;
    reader_listener.on_requested_incompatible_qos =
            hello_worldListener_on_requested_incompatible_qos;
    reader_listener.on_sample_rejected = hello_worldListener_on_sample_rejected;
    reader_listener.on_liveliness_changed =
            hello_worldListener_on_liveliness_changed;
    reader_listener.on_sample_lost = hello_worldListener_on_sample_lost;
    reader_listener.on_subscription_matched =
            hello_worldListener_on_subscription_matched;
    reader_listener.on_data_available = hello_worldListener_on_data_available;

    /* If you use Durable Reader State, you need to set up several properties.
     * In this example, we have modified them using a QoS XML profile. See
     * further details in USER_QOS_PROFILES.xml.
     */
    if (drs == 1) {
        reader = DDS_Subscriber_create_datareader_with_profile(
                subscriber,
                DDS_Topic_as_topicdescription(topic),
                "persistence_example_Library",
                "durable_reader_state_Profile",
                &reader_listener,
                DDS_STATUS_MASK_ALL);
        if (reader == NULL) {
            printf("create_datareader error\n");
            subscriber_shutdown(participant);
            return -1;
        }
    } else {
        reader = DDS_Subscriber_create_datareader_with_profile(
                subscriber,
                DDS_Topic_as_topicdescription(topic),
                "persistence_example_Library",
                "persistence_service_Profile",
                &reader_listener,
                DDS_STATUS_MASK_ALL);
        if (reader == NULL) {
            printf("create_datareader error\n");
            subscriber_shutdown(participant);
            return -1;
        }
    }

    /* Main loop */
    for (count = 0; (sample_count == 0) || (count < sample_count); ++count) {
        printf("hello_world subscriber sleeping for %d sec...\n",
               poll_period.sec);
        NDDS_Utility_sleep(&poll_period);
    }

    /* Cleanup and delete all entities */
    return subscriber_shutdown(participant);
}

int main(int argc, char *argv[])
{
    int domain_id = 0;
    int drs = 0;
    int i;
    char syntax[1024];
    int sample_count = 0; /* infinite loop */

    sprintf(syntax,
            "%s [options] \n\
    -domain_id <domain ID> (default: 0)\n\
    -sample_count <sample_count> (default: infinite => 0) \n\
    -drs <1|0> Enable/Disable durable reader state (default: 0)\n",
            argv[0]);

    for (i = 1; i < argc;) {
        READ_INTEGER_PARAM("-sample_count", sample_count);
        READ_INTEGER_PARAM("-domain_id", domain_id);
        READ_INTEGER_PARAM("-drs", drs);
        printf("%s", syntax);
        return 0;
    }

    /* Uncomment this to turn on additional logging
    NDDS_Config_Logger_set_verbosity_by_category(
        NDDS_Config_Logger_get_instance(),
        NDDS_CONFIG_LOG_CATEGORY_API,
        NDDS_CONFIG_LOG_VERBOSITY_STATUS_ALL);
    */

    return subscriber_main(domain_id, sample_count, drs);
}

#ifdef RTI_VX653
const unsigned char *__ctype = NULL;

void usrAppInit()
{
    #ifdef USER_APPL_INIT
    USER_APPL_INIT; /* for backwards compatibility */
    #endif

    /* add application specific code here */
    taskSpawn(
            "sub",
            RTI_OSAPI_THREAD_PRIORITY_NORMAL,
            0x8,
            0x150000,
            (FUNCPTR) subscriber_main,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0);
}
#endif