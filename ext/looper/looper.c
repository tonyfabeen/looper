#include<ruby.h>
#include "uv.h"

static VALUE mLooper;
static VALUE cEventLoop;

static VALUE eventloop_start(VALUE eventloop_class){
  uv_loop_t *loop = uv_loop_new();
  if(!loop){
    fprintf(stderr, "[Looper Ext] Error on start LibUV\n");
  }

  VALUE eventloop_data = Data_Wrap_Struct(eventloop_class, 0, free, loop);
  rb_obj_call_init(eventloop_data, 0,NULL);

  return eventloop_data;
}

static VALUE eventloop_init(VALUE self){
  return self;
}


void Init_looper(){
  mLooper = rb_define_module("Looper");
  cEventLoop = rb_define_class_under(mLooper, "EventLoop", rb_cObject);

  rb_define_singleton_method(cEventLoop, "start", eventloop_start,0);
  rb_define_method(cEventLoop, "initialize", eventloop_init, 0);
}

