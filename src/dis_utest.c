#include <stdlib.h>

#include "dis_utest.h"
#include "dis_send_receive.h"

int main(int argc, char *argv[])
{
    struct send_receive_ctx ctx;
    printf_debug(DIS_STATUS_START);

    memset(&ctx, 0, sizeof(struct send_receive_ctx));
    send_receive(&ctx);
    send_receive_exit(&ctx);

    printf_debug(DIS_STATUS_COMPLETE);
    return 0;
}