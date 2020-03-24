#include <stdlib.h>
#include <stdio.h>

#include <infiniband/verbs.h>

#include "dis_send_receive.h"
#include "dis_utest.h"

int send_receive(struct send_receive_ctx *ctx)
{
    int i, num_devices;
    struct ibv_device **ibv_dev_list;
    struct dev_ctx *dev;
    printf_debug(DIS_STATUS_START);

    printf_debug("Getting Device List.\n");
    num_devices = 0;
    ibv_dev_list = ibv_get_device_list(&num_devices);
	if (!num_devices) {
        printf_debug(DIS_STATUS_FAIL);
        return -42;
    }

    for (i = 0; i < num_devices; i++) {
        printf_debug("Found device: %s\n", ibv_dev_list[i]->name);
    }

    dev = &ctx->dev;
    dev->port_num = 1;
    dev->ibv_dev = ibv_dev_list[0];
    printf_debug(DIS_STATUS_COMPLETE);
    return 0;
}

void send_receive_exit(struct send_receive_ctx *ctx)
{
    printf_debug(DIS_STATUS_START);

    printf_debug(DIS_STATUS_COMPLETE);
}