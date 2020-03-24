#include <stdlib.h>
#include <stdio.h>

#include <infiniband/verbs.h>

#include "dis_send_receive.h"
#include "dis_utest.h"
#include "dis_common.h"

int send_receive(struct send_receive_ctx *ctx)
{
    int i;
    struct dev_ctx *dev;
    struct pd_ctx *pd;
    struct cq_ctx *cq;
    printf_debug(DIS_STATUS_START);

    printf_debug("Getting Device List.\n");
    dev = &ctx->dev;

    dev->ibv_dev_list = ibv_get_device_list(&dev->num_devices);
	if (!dev->num_devices) {
        printf_debug(DIS_STATUS_FAIL);
        return -42;
    }

    for (i = 0; i < dev->num_devices; i++) {
        printf_debug("Found device: %s\n", dev->ibv_dev_list[i]->name);
    }

    printf_debug("Opening Device.\n");
    dev->ibv_dev = dev->ibv_dev_list[0];

    dev->ibv_ctx = ibv_open_device(dev->ibv_dev);
    if (!dev->ibv_ctx) {
        printf_debug(DIS_STATUS_FAIL);
        return -42;
    }
    
    printf_debug("Creating Protection Domain: %d\n", ctx->pd_c);
    pd = &ctx->pd[ctx->pd_c];
    pd->ibv_ctx = dev->ibv_ctx;

    pd->ibv_pd = ibv_alloc_pd(pd->ibv_ctx);
    if (!pd->ibv_pd) {
        printf_debug(DIS_STATUS_FAIL);
		return -42;
    }
    ctx->pd_c++;


    printf_debug("Creating Completion Queue: %d", ctx->cq_c);
    cq = &ctx->cq[ctx->cq_c];
    cq->ibv_ctx     = dev->ibv_ctx;
    cq->cqe_max     = DIS_MAX_CQE;
    cq->ctx         = NULL;
    cq->comp_ch     = NULL;
    cq->comp_vec    = 0;

    cq->ibv_cq = ibv_create_cq(cq->ibv_ctx, 
                                cq->cqe_max, 
                                cq->ctx, 
                                cq->comp_ch,
                                cq->comp_vec);
    if (!cq->ibv_cq) {
        printf_debug(DIS_STATUS_FAIL);
        return -42;
    }
    ctx->cq_c++;

    printf_debug(DIS_STATUS_COMPLETE);
    return 0;
}

void send_receive_exit(struct send_receive_ctx *ctx)
{
    int i;
    printf_debug(DIS_STATUS_START);

    for (i = 0; i < ctx->cq_c; i++) {
        ibv_destroy_cq(ctx->cq[i].ibv_cq);
        printf_debug("Destroy CQ %d: " DIS_STATUS_COMPLETE, i);
    }

     for (i = 0; i < ctx->pd_c; i++) {
        ibv_dealloc_pd(ctx->pd[i].ibv_pd);
        printf_debug("Destroy PD %d: " DIS_STATUS_COMPLETE, i);
    }

    printf_debug(DIS_STATUS_COMPLETE);
}