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

typedef struct {
  uv_write_t req;
  uv_buf_t buf;
} write_req_t;
int server_closed;

static void on_connection(uv_stream_t*, int status);

static void alloc_buffer(uv_handle_t *handle,
    size_t suggested_size,
    uv_buf_t *buffer);

static void read_buffer(uv_stream_t *handle,
    ssize_t nread,
    const uv_buf_t *buf);

static void write_buffer(uv_write_t *req, int status);
static void after_shutdown(uv_shutdown_t *req, int status);
static void on_close(uv_handle_t *peer);
static void on_server_close(uv_handle_t *handle);

///Ruby Stuff
static VALUE mLooper;
static VALUE cEventLoop;
static VALUE mTCPServer;
static VALUE cTarget;

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

  if(rb_respond_to(cTarget, rb_intern("on_connection"))){
    rb_funcall(cTarget, rb_intern("on_connection"),0,0);
  }

  r = uv_read_start(stream, alloc_buffer, read_buffer);
}

static void alloc_buffer(uv_handle_t *handle,
    size_t suggested_size,
    uv_buf_t *buffer){

  buffer->base = malloc(suggested_size);
  buffer->len  = suggested_size;
}

static void read_buffer(uv_stream_t *handle,
    ssize_t nread,
    const uv_buf_t *buf){

  int i;
  write_req_t *wr;
  uv_shutdown_t *req;

  if(nread < 0){
    ASSERT(nread == UV_EOF);

    if(buf->base){
      free(buf->base);
    }

    req = (uv_shutdown_t *) malloc(sizeof *req);
    uv_shutdown(req, handle, after_shutdown);

    return;
  }

  if(nread == 0){
    /* nothing to read */
    free(buf->base);
    return;
  }

  /*
   * 'Q' quits the server
   * 'QS' closes the stream
   */
  if(!server_closed){

    for(i = 0; i < nread; i++){
      if(buf->base[i] == 'Q'){
        if(i + 1 < nread && buf->base[i + 1] == 'S'){
          free(buf->base);
          uv_close((uv_handle_t *) handle, on_close);
          return;
        }else{
          uv_close(server, on_server_close);
          server_closed = 1;
        }
      }
    }
  }

  wr = (write_req_t *) malloc(sizeof *wr);
  wr->buf = uv_buf_init(buf->base, nread);

  if(rb_respond_to(cTarget, rb_intern("on_data"))){
    const char *buffer_data = buf->base;
    VALUE data = rb_str_new2(buffer_data);
    rb_funcall(cTarget, rb_intern("on_data"),1, data);
  }

  if(uv_write(&wr->req, handle, &wr->buf, 1, write_buffer)){
    LOG_ERROR("uv_write failed");
  }
}

static void write_buffer(uv_write_t *req, int status){
  write_req_t *wr;

  /*free read/write buffer and request*/
  wr = (write_req_t *) req;
  free(wr->buf.base);
  free(wr);

  if(status == 0)
    return;

  LOG_UV_ERROR("uv_write error: ", uv_strerror(status));

  if(status == UV_ECANCELED)
    return;

  ASSERT(status == UV_EPIPE);
  uv_close((uv_handle_t *) req->handle, on_close);
}


static void after_shutdown(uv_shutdown_t *req, int status){
  uv_close((uv_handle_t *) req->handle, on_close);
}

static void on_close(uv_handle_t *peer){
  free(peer);
}

static void on_server_close(uv_handle_t *handle){
  ASSERT(handle == server);
}


/*
 *
 */
static int server_start(){
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

  LOG("TCP Server started at port 8265 \n");

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
static VALUE tcp_server_init(VALUE self){
  cTarget = self;
  return self;
}

//TCPServer.start implementation
static VALUE tcp_server_start(VALUE self){
  loop = uv_default_loop();

  server_start();

  uv_run(loop, UV_RUN_DEFAULT);

  return self;
}


void Init_looper(){
  mLooper = rb_define_module("Looper");
  mTCPServer = rb_define_module_under(mLooper, "TCPServer");

  //Initialize TCPServer Module
  rb_define_singleton_method(mTCPServer, "new", tcp_server_new, 0);
  rb_define_method(mTCPServer, "initialize", tcp_server_init, 0);
  rb_define_method(mTCPServer, "start", tcp_server_start, 0);
}

