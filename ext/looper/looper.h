#ifndef LOOPER_H
#define LOOPER_H

#include <ruby.h>
#include "uv.h"

//General
#define ASSERT(expr)                                      \
 do {                                                     \
  if (!(expr)) {                                          \
    fprintf(stderr,                                       \
            "[Looper Ext] Assertion failed in %s on line %d: %s\n",    \
            __FILE__,                                     \
            __LINE__,                                     \
            #expr);                                       \
    abort();                                              \
  }                                                       \
 } while (0)


#define LOG_ERROR(msg)                                    \
  fprintf(stderr, "[Looper Ext] %s\n", msg);

#define LOG_UV_ERROR(msg, uv_error)                       \
  fprintf(stderr, "[Looper Ext] %s : %s \n", msg, uv_error);

#define LOG(msg)                                          \
  fprintf(stdout, "[Looper Ext] %s\n", msg);

#define DEFAULT_TCP_PORT 8265

//


//LibUV Stuff
static uv_loop_t *loop;
static uv_handle_t *server;
static uv_tcp_t tcpServer;

static void on_connection(uv_stream_t*, int status);

static void alloc_buffer(uv_handle_t *handle,
    size_t suggested_size,
    uv_buf_t *buffer);

static void read_buffer(uv_stream_t *handle,
    ssize_t nread,
    const uv_buf_t *buf);

static void after_shutdown(uv_shutdown_t *req, int status);
static void on_close(uv_handle_t *peer);
static void on_server_close(uv_handle_t *handle);
static void emit_error(char* msg);

///Ruby Stuff
static VALUE mLooper;
static VALUE mTCPServer;
static VALUE cTarget;

#endif
