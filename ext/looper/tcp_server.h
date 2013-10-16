#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <ruby.h>
#include "uv.h"

//LibUV Stuff
uv_loop_t *loop;
uv_handle_t *server;
uv_tcp_t tcpServer;

void on_connection(uv_stream_t*, int status);

void alloc_buffer(uv_handle_t *handle,
    size_t suggested_size,
    uv_buf_t *buffer);

void read_buffer(uv_stream_t *handle,
    ssize_t nread,
    const uv_buf_t *buf);

void after_shutdown(uv_shutdown_t *req, int status);
void on_close(uv_handle_t *peer);
void on_server_close(uv_handle_t *handle);
void emit_error(char* msg);

///Ruby Stuff
VALUE mLooper;
VALUE mTCPServer;
VALUE cTarget;
int tcp_port;

VALUE tcp_server_new(VALUE tcp_server_class);
VALUE tcp_server_init(VALUE self, VALUE port);
VALUE tcp_server_start(VALUE self);

#endif
