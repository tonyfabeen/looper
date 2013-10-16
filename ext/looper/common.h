#ifndef COMMON_H
#define COMMON_H

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

#define LOG_INT(msg,param)                                          \
  fprintf(stdout, "[Looper Ext] %s : %i\n", msg, param);


#endif

