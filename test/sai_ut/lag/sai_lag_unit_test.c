#include <stdio.h>
#include "sai.h"

const char* test_profile_get_value(
    _In_ sai_switch_profile_id_t profile_id,
    _In_ const char* variable)
{
    return 0;
}

int test_profile_get_next_value(
    _In_ sai_switch_profile_id_t profile_id,
    _Out_ const char** variable,
    _Out_ const char** value)
{
    return -1;
}

const service_method_table_t test_services = {
    test_profile_get_value,
    test_profile_get_next_value
};

int main()
{
    sai_status_t              status;
    sai_lag_api_t            *lag_api;
    sai_switch_api_t         *switch_api;
    sai_object_id_t           lag_oid[2];
    sai_object_id_t           lag_member_oid[4];
    sai_attribute_t           attrs[2];
    sai_switch_notification_t notifications;
    sai_object_id_t           port_list[64];

    status = sai_api_initialize(0, &test_services);

    status = sai_api_query(SAI_API_SWITCH, (void**)&switch_api);
    status = switch_api->initialize_switch(0, "HW_ID", 0, &notifications);
    attrs[0].id = SAI_SWITCH_ATTR_PORT_LIST;
    attrs[0].value.objlist.list = port_list;
    attrs[0].value.objlist.count = 64;
    status = switch_api->get_switch_attribute(1, attrs);
    for (int32_t ii = 0; ii < attrs[0].value.objlist.count; ii++) {
        printf("Port #%d OID: 0x%lX\n", ii, attrs[0].value.objlist.list[ii]);
        port_list[ii] = attrs[0].value.objlist.list[ii];
    }

    status = sai_api_query(SAI_API_LAG, (void**)&lag_api);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to query LAG API, status=%d\n", status);
        return 1;
    }
    // LAG#1
    status = lag_api->create_lag(&lag_oid[0], 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create a LAG, status=%d\n", status);
        return 1;
    }


    attrs[0].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    attrs[0].value.oid = lag_oid[0];
    attrs[1].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    attrs[1].value.oid = port_list[0];
    
    status = lag_api->create_lag_member(&lag_member_oid[0], 2, attrs);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create a LAG, status=%d\n", status);
        return 1;
    }

    attrs[0].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    attrs[0].value.oid = lag_oid[0];
    attrs[1].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    attrs[1].value.oid = port_list[1];
    
    status = lag_api->create_lag_member(&lag_member_oid[1], 2, attrs);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create a LAG, status=%d\n", status);
        return 1;
    }

    // LAG#2

    status = lag_api->create_lag(&lag_oid[1], 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create a LAG, status=%d\n", status);
        return 1;
    }


    attrs[0].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    attrs[0].value.oid = lag_oid[1];
    attrs[1].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    attrs[1].value.oid = port_list[2];
    
    status = lag_api->create_lag_member(&lag_member_oid[2], 2, attrs);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create a LAG, status=%d\n", status);
        return 1;
    }

    attrs[0].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    attrs[0].value.oid = lag_oid[1];
    attrs[1].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    attrs[1].value.oid = port_list[3];
    
    status = lag_api->create_lag_member(&lag_member_oid[3], 2, attrs);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create a LAG, status=%d\n", status);
        return 1;
    }

    // GET LAG
    sai_object_id_t lag_port_list[64];
    attrs[0].id = SAI_LAG_ATTR_PORT_LIST;
    attrs[0].value.objlist.list = lag_port_list;
    attrs[0].value.objlist.count = 64;
    status = lag_api->get_lag_attribute(lag_oid[0], 1, attrs);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get a LAG_ATTRIBUTE, status=%d\n", status);
        return 1;
    }

    status = lag_api->get_lag_attribute(lag_oid[1], 1, attrs);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get a LAG_ATTRIBUTE, status=%d\n", status);
        return 1;
    }

    // GET LAG MEMBER
    attrs[0].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    status = lag_api->get_lag_member_attribute(lag_member_oid[0], 2, attrs);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get a LAG_MEMBER_ATTRIBUTE, status=%d\n", status);
        return 1;
    }

    attrs[0].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    status = lag_api->get_lag_member_attribute(lag_member_oid[2], 2, attrs);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get a LAG_MEMBER_ATTRIBUTE, status=%d\n", status);
        return 1;
    }

    // REMOVE LAG MEMBER

    status = lag_api->remove_lag_member(lag_member_oid[1]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove a LAG_MEMBER, status=%d\n", status);
        return 1;
    }

    attrs[0].id = SAI_LAG_ATTR_PORT_LIST;
    attrs[0].value.objlist.list = lag_port_list;
    attrs[0].value.objlist.count = 64;
    status = lag_api->get_lag_attribute(lag_oid[0], 1, attrs);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get a LAG_ATTRIBUTE, status=%d\n", status);
        return 1;
    }

    status = lag_api->remove_lag_member(lag_member_oid[2]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove a LAG_MEMBER, status=%d\n", status);
        return 1;
    }
    
    attrs[0].id = SAI_LAG_ATTR_PORT_LIST;
    attrs[0].value.objlist.list = lag_port_list;
    attrs[0].value.objlist.count = 64;
    status = lag_api->get_lag_attribute(lag_oid[1], 1, attrs);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get a LAG_ATTRIBUTE, status=%d\n", status);
        return 1;
    }

    status = lag_api->remove_lag_member(lag_member_oid[0]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove a LAG_MEMBER, status=%d\n", status);
        return 1;
    }

     status = lag_api->remove_lag_member(lag_member_oid[3]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove a LAG_MEMBER, status=%d\n", status);
        return 1;
    }

    status = lag_api->remove_lag(lag_oid[0]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove a LAG_MEMBER, status=%d\n", status);
        return 1;
    }

        status = lag_api->remove_lag(lag_oid[1]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove a LAG_MEMBER, status=%d\n", status);
        return 1;
    }

    
    switch_api->shutdown_switch(0);
    status = sai_api_uninitialize();

    return 0;
}
