#include<ruby.h>
#include "uv.h"

//General
#define ASSERT(expr)                                      \
 do {                                                     \
  if (!(expr)) {                                          \
    fprintf(stderr,                                       \
            "Assertion failed in %s on line %d: %s\n",    \
            __FILE__,                                     \
            __LINE__,                                     \
            #expr);                                       \
    abort();                                              \
  }                                                       \
 } while (0)


#define LOG_ERROR(msg)                                    \
  fprintf(stderr, "%s\n", msg);

#define LOG_UV_ERROR(msg, uv_error)                       \
  fprintf(stderr, "%s : %s \n", msg, uv_error);

#define LOG(msg)                                          \
  fprintf(stdout, "%s\n", msg);

#define DEFAULT_TCP_PORT 8265

//


//LibUV Stuff
static uv_loop_t *loop;
static uv_handle_t *server;
static uv_tcp_t tcpServer;
typedef struct {
  uv_write_t req;
  uv_buf_t buf;
} write_req_t;
int server_closed;
static void on_connection(uv_stream_t*, int status);

//Ruby Stuff
static VALUE mLooper;
static VALUE cEventLoop;
static VALUE mTCPServer;

static void on_connection(uv_stream_t *server, int status){

  uv_stream_t *stream;
  int r;

  if(status != 0){
    LOG_UV_ERROR("Connect Error", uv_err_name(status));
  }
  ASSERT(status == 0);

  stream = malloc(sizeof(uv_tcp_t));
  ASSERT(stream != NULL);

  r = uv_tcp_init(loop, (uv_tcp_t *) stream);
  ASSERT(r == 0);

  stream->data = server;

  r = uv_accept(server, stream);
  ASSERT(r == 0);

  LOG("Connection Accepted !!!");

  //r = uv_read_start(stream, alloc_buffer, after_read);
}


/*
 *
 */
static int tcp_server_start(){
  struct sockaddr_in addr;
  int r;

  ASSERT(0 == uv_ip4_addr("0.0.0.0", DEFAULT_TCP_PORT, &addr));

  //server = (uv_handle_t *) &tcpServer;
  r = uv_tcp_init(loop, &tcpServer);
  if(r){
    LOG_ERROR("Socket Creation Error");
    return 1;
  }

  r = uv_tcp_bind(&tcpServer, (const struct sockaddr *) &addr);
  if(r){
    LOG_ERROR("Bind Error");
    return 1;
  }

  r = uv_listen((uv_stream_t *) &tcpServer, 128, on_connection);
  if(r){
    LOG_ERROR("Listen Error");
    return 1;
  }

  fprintf(stdout, "TCP Server started at port 8265 \n");

  return 0;
}


//Initialize TCPServer module
static VALUE tcp_server_new(VALUE tcp_server_class){

  server = (uv_handle_t *) &tcpServer;

  VALUE tcp_server_data = Data_Wrap_Struct(tcp_server_class, 0, free, server);
  rb_obj_call_init(tcp_server_data, 0, NULL);

  return tcp_server_class;
}

//TCPServer.initialize implementation
static VALUE tcp_serve_init(VALUE self){
  loop = uv_default_loop();

  tcp_server_start();

  return uv_run(loop, UV_RUN_DEFAULT);
}


void Init_looper(){
  mLooper = rb_define_module("Looper");
  mTCPServer = rb_define_module_under(mLooper, "TCPServer");

  //Initialize TCPServer Module
  rb_define_singleton_method(mTCPServer, "new", tcp_server_new, 0);
  rb_define_method(mTCPServer, "initialize", tcp_serve_init, 0);
}

