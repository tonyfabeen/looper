#include "looper.h"
#include "common.h"
#include "tcp_server.h"
#include <signal.h>


//Handle Ctrl-C Signal
void ctrlc_handler(int signal_number){
  LOG("Bye Bye.. ");
  exit(signal_number);
}

void Init_looper(){

  signal(SIGINT, ctrlc_handler);

  mLooper = rb_define_module("Looper");
  mTCPServer = rb_define_module_under(mLooper, "TCPServer");

  //Initialize TCPServer Module
  rb_define_singleton_method(mTCPServer, "new", tcp_server_new, 0);
  rb_define_method(mTCPServer, "initialize", tcp_server_init, 1);
  rb_define_method(mTCPServer, "start", tcp_server_start, 0);
}

