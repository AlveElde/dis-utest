#include <stdlib.h>
#include <stdio.h>

#include <infiniband/verbs.h>

#include "dis_send_receive.h"
#include "dis_utest.h"
#include "dis_common.h"

int send_receive(struct send_receive_ctx *ctx)
{
    int i, ret;
    struct dev_ctx *dev;
    struct pd_ctx *pd;
    struct cq_ctx *cq;
    struct qp_ctx *qp;
    struct ibv_context *ibv_ctx_safe;
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
    dev->ibv_dev    = dev->ibv_dev_list[0];
    dev->port_num   = 1;

    dev->ibv_ctx = ibv_open_device(dev->ibv_dev);
    if (!dev->ibv_ctx) {
        printf_debug(DIS_STATUS_FAIL);
        return -42;
    }
    
    printf_debug("Creating Protection Domain: %d\n", ctx->pd_c);
    pd = &ctx->pd[ctx->pd_c];
    // pd->ibv_ctx = dev->ibv_ctx;

    pd->ibv_pd = ibv_alloc_pd(dev->ibv_ctx);
    if (!pd->ibv_pd) {
        printf_debug(DIS_STATUS_FAIL);
		return -42;
    }
    ctx->pd_c++;

    printf_debug("Creating Completion Queue: %d\n", ctx->cq_c);
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

    printf_debug("Creating Queue Pair: %d\n", ctx->qp_c);
    qp = &ctx->qp[ctx->qp_c];
    qp->ibv_pd  = pd->ibv_pd;
    qp->send_cq = cq;
    qp->recv_cq = cq;

    qp->init_attr.qp_context    = NULL;
    qp->init_attr.send_cq       = cq->ibv_cq;
    qp->init_attr.recv_cq       = cq->ibv_cq;
    qp->init_attr.srq           = NULL;
    qp->init_attr.qp_type       = IBV_QPT_RC;

    qp->init_attr.cap.max_send_wr       = DIS_MAX_SQE + 10;
    qp->init_attr.cap.max_recv_wr       = DIS_MAX_RQE + 10;
	qp->init_attr.cap.max_send_sge      = DIS_MAX_SGE;
	qp->init_attr.cap.max_recv_sge      = DIS_MAX_SGE;
	qp->init_attr.cap.max_inline_data   = 0;

    qp->ibv_qp = ibv_create_qp(qp->ibv_pd, &qp->init_attr);
    if (!qp->ibv_qp) {
        printf_debug(DIS_STATUS_FAIL);
        return -42;
    }
    ctx->qp_c++;

    printf_debug("Transitioning Queue Pair %d to INIT state", ctx->qp_c - 1);
    qp->attr.qp_state           = IBV_QPS_INIT;
    qp->attr.qp_access_flags    = 0;
    // qp->attr.qp_access_flags    = IBV_ACCESS_REMOTE_WRITE;
    // qp->attr.qp_access_flags    |= IBV_ACCESS_REMOTE_READ;
    // qp->attr.qp_access_flags    |= IBV_ACCESS_LOCAL_WRITE;
    qp->attr.pkey_index         = 0;
    qp->attr.port_num           = dev->port_num;
    qp->ibv_qp->context = dev->ibv_ctx;

    qp->attr_mask = IBV_QP_STATE;
    qp->attr_mask |= IBV_QP_ACCESS_FLAGS;
    qp->attr_mask |= IBV_QP_PKEY_INDEX;
    qp->attr_mask |= IBV_QP_PORT;

    printf_debug("Attr mask: %d\n", qp->attr_mask);

    ret = ibv_modify_qp(qp->ibv_qp, &qp->attr, qp->attr_mask);
    if (ret) {
        printf_debug("Error: %d\n", ret);
        printf_debug(DIS_STATUS_FAIL);
        return -42;
    }

    printf_debug("Transitioning Queue Pair %d to RTR state", ctx->qp_c - 1);
    qp->attr.qp_state               = IBV_QPS_RTR;
    qp->attr.path_mtu               = IBV_MTU_4096;
    qp->attr.dest_qp_num            = 100;
    qp->attr.rq_psn                 = 10;
    qp->attr.max_dest_rd_atomic     = 1;
    qp->attr.min_rnr_timer          = 1;

    qp->attr.ah_attr.sl             = 0;
    qp->attr.ah_attr.static_rate    = 1;
    qp->attr.ah_attr.port_num       = dev->port_num;

    qp->attr.ah_attr.grh.hop_limit  = 1;
    qp->attr.ah_attr.grh.sgid_index = 1;

    qp->attr_mask = IBV_QP_STATE;
    qp->attr_mask |= IBV_QP_AV;
    qp->attr_mask |= IBV_QP_PATH_MTU;
    qp->attr_mask |= IBV_QP_DEST_QPN;
    qp->attr_mask |= IBV_QP_RQ_PSN;
    qp->attr_mask |= IBV_QP_MAX_DEST_RD_ATOMIC;
    qp->attr_mask |= IBV_QP_MIN_RNR_TIMER;

    ret = ibv_modify_qp(qp->ibv_qp, &qp->attr, qp->attr_mask);
    if (ret) {
        printf_debug(DIS_STATUS_FAIL);
        return -42;
    }

    // printf_debug("Transitioing Queue Pair %d to RTS state", ctx->qp_c - 1);
    // qp->attr.qp_state       = IBV_QPS_RTS;
    // qp->attr.timeout        = 10;
    // qp->attr.retry_cnt      = 10;
    // qp->attr.rnr_retry      = 10;
    // qp->attr.sq_psn         = 10;
    // qp->attr.max_rd_atomic  = 1;

    // qp->attr_mask = IBV_QP_STATE;
    // qp->attr_mask |= IBV_QP_TIMEOUT;
    // qp->attr_mask |= IBV_QP_RETRY_CNT;
    // qp->attr_mask |= IBV_QP_RNR_RETRY;
    // qp->attr_mask |= IBV_QP_SQ_PSN;
    // qp->attr_mask |= IBV_QP_MAX_QP_RD_ATOMIC;

    // ret = ibv_modify_qp(qp->ibv_qp, &qp->attr, qp->attr_mask);
    // if (ret) {
    //     printf_debug(DIS_STATUS_FAIL);
    //     return -42;
    // }

    printf_debug(DIS_STATUS_COMPLETE);
    return 0;
}

void send_receive_exit(struct send_receive_ctx *ctx)
{
    int i, ret;
    printf_debug(DIS_STATUS_START);

    printf_debug("Cleaning up context\n");
    for (i = 0; i < ctx->qp_c; i++) {
        ibv_destroy_qp(ctx->qp[i].ibv_qp);
        printf_debug("Destroy QP %d: " DIS_STATUS_COMPLETE, i);
    }

    for (i = 0; i < ctx->cq_c; i++) {
        ibv_destroy_cq(ctx->cq[i].ibv_cq);
        printf_debug("Destroy CQ %d: " DIS_STATUS_COMPLETE, i);
    }

    for (i = 0; i < ctx->pd_c; i++) {
        printf_debug("Destroy PD %d: " DIS_STATUS_COMPLETE, i);
        ibv_dealloc_pd(ctx->pd[i].ibv_pd);
    }

    ret = ibv_close_device(ctx->dev.ibv_ctx);
    if (ret) {
        printf_debug(DIS_STATUS_FAIL);
    }
    printf_debug("Close device: " DIS_STATUS_COMPLETE);

    ibv_free_device_list(ctx->dev.ibv_dev_list);
    printf_debug("Free device list: " DIS_STATUS_COMPLETE);

    printf_debug(DIS_STATUS_COMPLETE);
}