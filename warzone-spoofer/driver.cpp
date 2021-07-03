#include "driver.h"
#pragma warning (disable : 6001)

NTSTATUS driver::driver_entry()
{
    print_success_message("driver loaded");
    init_hook();

    return STATUS_SUCCESS;
}

NTSTATUS driver::init_hook()
{
    NTSTATUS status = STATUS_SUCCESS;
    UNICODE_STRING driver_name{};
    RtlInitUnicodeString(&driver_name, L"\\Driver\\disk");
    PDRIVER_OBJECT disk_driver_object = 0;

    status = utility::find_driver_object(&disk_driver_object, &driver_name);

    if (!NT_SUCCESS(status) || !disk_driver_object)
    {
        print_error_message("could not find disk driver object -> %p", status);
        return STATUS_UNSUCCESSFUL;
    }

    print_success_message("found disk driver object -> %p", disk_driver_object);

    hook_handler::original_dispatch = disk_driver_object->MajorFunction[IRP_MJ_DEVICE_CONTROL];
    disk_driver_object->MajorFunction[IRP_MJ_DEVICE_CONTROL] = (PDRIVER_DISPATCH)hook_handler::hooked_device_control;

    print_success_message("stored original dispatch -> %p", hook_handler::original_dispatch);
    print_success_message("swapped device control dispatch: %p -> %p", hook_handler::original_dispatch, hook_handler::hooked_device_control);

    return STATUS_SUCCESS;
}
