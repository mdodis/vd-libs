#include "vd_io.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int G_Running = 1;

static char Recv_Buf[1024];

VD_IO_CALLBACK(on_tcp_recv)
{
    if (evt->err != VD_IO_ERR_OK) {
        printf("Recv error\n");    
        return;
    }

    printf("Received: %.*s\n", (int)evt->data.recv.buffer_written, Recv_Buf);
}

VD_IO_CALLBACK(on_tcp_send)
{
    if (evt->err != VD_IO_ERR_OK) {
        printf("Send error\n");    
        return;
    }

    vd_io_hn_recv(io, evt->data.send.receiver, Recv_Buf, sizeof(Recv_Buf), on_tcp_recv, 0);
}


static char The_Msg[] = "Hello!";

VD_IO_CALLBACK(on_tcp_connect)
{
    if (evt->err != VD_IO_ERR_OK) {
        printf("Connection error\n");    
        return;
    }

    printf("Connected.\n");

    vd_io_hn_send(io, evt->data.connect.socket, The_Msg, sizeof(The_Msg) - 1, on_tcp_send, 0);
}

int main(int argc, char const *argv[])
{
    VdIoInitInfo init_info = vd_io_init_info_default();
    init_info.op_memory_size = vd_io_init_info_mem_size(&init_info);
    init_info.op_memory = malloc(init_info.op_memory_size);

    VdIo io;
    VdIoErr err = vd_io_init(&io, &init_info);
    assert(err == VD_IO_ERR_OK);

    VdIoHn socket;
    vd_io_connect(&io, "127.0.0.1", 8080, VD_IO_NET_TCP, &socket, on_tcp_connect, 0);

    while (G_Running) {
        vd_io_wait(&io);
    }

    return 0;
}

#define VD_IO_IMPL
#include "vd_io.h"
