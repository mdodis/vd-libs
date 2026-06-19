#include "vd_io.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int G_Running = 1;

typedef struct {
    VdIoHn client;
    char buffer[1024];
} Connection;

VD_IO_CALLBACK(on_tcp_listener_send)
{
    if (evt->err != VD_IO_ERR_OK) {
        printf("Send Error\n");    
    }

    Connection *conn = (Connection*)usr;
    vd_io_hn_disconnect(io, conn->client);
    free(conn);
    G_Running = 0;
}

VD_IO_CALLBACK(on_tcp_listener_recv)
{
    Connection *conn = (Connection*)usr;
    if (evt->err != VD_IO_ERR_OK) {
        printf("Recv Error\n");    
        return;
    }

    if (evt->data.recv.buffer_written) {
        printf("Received %.*s\n", (int)evt->data.recv.buffer_written, (char*)evt->data.recv.buffer);
    }

    memcpy(conn->buffer, evt->data.recv.buffer, evt->data.recv.buffer_written);

    vd_io_hn_send(io, conn->client,
                  conn->buffer, evt->data.recv.buffer_written,
                  on_tcp_listener_send, (void*)conn);
}

VD_IO_CALLBACK(on_tcp_listener_accept)
{
    if (evt->err != VD_IO_ERR_OK) {
        printf("Accept Error\n");    
        return;
    }

    Connection *conn = (Connection*)calloc(1, sizeof(Connection));
    conn->client = evt->data.accept.socket;
    vd_io_hn_recv(io, conn->client, conn->buffer, sizeof(conn->buffer), on_tcp_listener_recv, (void*)conn);

    char bytes[16];
    vd_io_ipv4_str(evt->data.accept.remote_ip, bytes);

    printf("New Client connected! %s:%d\n", bytes, evt->data.accept.remote_port);
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
    vd_io_listen(&io, "127.0.0.1", 8080, VD_IO_NET_TCP, &tcp_listener);

    vd_io_hn_accept(&io, tcp_listener, on_tcp_listener_accept, 0);

    while (G_Running) {
        vd_io_wait(&io);
    }

    return 0;
}

#define VD_IO_IMPL
#include "vd_io.h"
