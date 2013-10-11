#include<ruby.h>
#include "uv.h"

static VALUE mLooper;
static VALUE cEventLoop;
static VALUE mTCPServer;

//Initialize TCPServer module
static VALUE tcp_server_new(VALUE tcp_server_class){
  uv_tcp_t *server = malloc(sizeof(uv_tcp_t));
  uv_tcp_init(uv_default_loop(), server);

  VALUE tcp_server_data = Data_Wrap_Struct(tcp_server_class, 0, free, server);
  rb_obj_call_init(tcp_server_data, 0, NULL);

  return tcp_server_data;
}

//TCPServer.initialize implementation
static VALUE tcp_serve_init(VALUE self){
  fprintf(stdout, "[Looper Ext] TCP Server initialized\n");
  return self;
}

//Initialize EventLoop class
static VALUE eventloop_start(VALUE eventloop_class){
  //uv_loop_t *loop = uv_loop_new();
  uv_loop_t *loop = uv_default_loop();

  if(!loop){
    fprintf(stderr, "[Looper Ext] Error on start LibUV\n");
  }

  VALUE eventloop_data = Data_Wrap_Struct(eventloop_class, 0, NULL, loop);
  rb_obj_call_init(eventloop_data, 0,NULL);


  return eventloop_data;
}

//EventLoop.initialize implementation
static VALUE eventloop_init(VALUE self){
  return self;
}


void Init_looper(){
  mLooper = rb_define_module("Looper");
  mTCPServer = rb_define_module_under(mLooper, "TCPServer");
  cEventLoop = rb_define_class_under(mLooper, "EventLoop", rb_cObject);

  //Initialize EventLoop class
  rb_define_singleton_method(cEventLoop, "start", eventloop_start,0);
  rb_define_method(cEventLoop, "initialize", eventloop_init, 0);

  //Initialize TCPServer Module
  rb_define_singleton_method(mTCPServer, "new", tcp_server_new, 0);
  rb_define_method(mTCPServer, "initialize", tcp_serve_init, 0);
}

