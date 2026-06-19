#include "vd_io.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

VD_IO_CALLBACK(on_tcp_listener_accept)
{
    printf("Accepted\n");
}

int main(int argc, char const *argv[])
{
    VdIoInitInfo init_info = vd_io_init_info_default();
    init_info.op_memory_size = vd_io_init_info_mem_size(&init_info);
    init_info.op_memory = malloc(init_info.op_memory_size);

    VdIo io;
    VdIoErr err = vd_io_init(&io, &init_info);
    assert(err == VD_IO_ERR_OK);

    VdIoHn tcp_listener;
    vd_io_make_socket(&io, "127.0.0.1", 8080, VD_IO_NET_TCP, &tcp_listener);

    vd_io_hn_accept(&io, tcp_listener, on_tcp_listener_accept, 0);

    while (1) {
        vd_io_wait(&io);
    }

    return 0;
}

#define VD_IO_IMPL
#include "vd_io.h"
