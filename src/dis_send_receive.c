#include <stdlib.h>
#include <stdio.h>

#include <infiniband/verbs.h>

#include "dis_send_receive.h"
#include "dis_utest.h"

int send_receive(struct send_receive_ctx *ctx)
{
    struct ibv_device **ibv_dev_list;

    printf_debug(DIS_STATUS_START);

    ibv_dev_list = ibv_get_device_list(NULL);
	if (!ibv_dev_list) {
        printf_debug(DIS_STATUS_FAIL);
        return -42;
    }

    printf_debug(DIS_STATUS_COMPLETE);
    return 0;
}

void send_receive_exit(struct send_receive_ctx *ctx)
{
    printf_debug(DIS_STATUS_START);

    printf_debug(DIS_STATUS_COMPLETE);
}