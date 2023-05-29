#ifndef _WIDGETS_STACK_H
#define _WIDGETS_STACK_H

#include <stdint.h>
#include <stdbool.h>
#include <sys/event.h>

typedef struct stack_ctx stack_ctx_t;
typedef struct stack_node stack_node_t;
typedef struct window window_t;
typedef struct window_def window_def_t;

typedef uint32_t (* cb_wndproc)(stack_ctx_t *ctx, event_t event, param_t lp, param_t wp);

struct stack_node {
  stack_node_t* next;
  stack_node_t* last;
  window_t* window;
};

struct stack_ctx {
  uint8_t deepth;
  stack_node_t *entry;
  stack_node_t *final;
};

struct window {
  void *userdata;
  const window_def_t *def;
};

typedef enum {
  wndtype_normal     = 0,
  wndtype_fullscreen = 1,
  wndtype_popup      = 2
} window_type_t;

struct window_def {
  const char** title;
  window_type_t type;
  cb_wndproc cb_wndproc;
};

// present counter
#define present_counter(ctx) ++(*((uint32_t *)(&(((window_t *)ctx)->userdata))))

// present_frame_scale(ctx, 60) means frame / 60.
#define present_frame_scale(ctx, scale) ((*((uint32_t *)(&(((window_t *)ctx)->userdata)))) % scale == 0)

/**
 * @brief create a window stack
 * @return if success return a stack pointer
 */
stack_ctx_t* stack_create();

/**
 * @brief push window to the front
 * @param ctx stack context
 * @param window window def
 * @return return the stack deepth
 */
uint8_t stack_push_window(stack_ctx_t* ctx, const window_def_t *window);

/**
 * @brief replace the front window
 * @param ctx stack context
 * @param window window def
 * @return the stack deepth
 */
uint8_t stack_set_window(stack_ctx_t* ctx, window_def_t *window);

/**
 * @brief stack back
 * @param ctx stack context
 * @return the stack deepth
 */
uint8_t stack_back(stack_ctx_t* ctx);

/**
 * @brief get the deepth of the stack
 * @param ctx stack context
 * @return the stack deepth
 */
uint8_t stack_get_deepth(stack_ctx_t* ctx);

/**
 * @brief call event to the window
 * @param ctx stack context
 * @param event event. @ref event_t
 * @param lp lparam
 * @param wp wparam
 * @return result
 */
int8_t stack_call_event(stack_ctx_t* ctx, event_t event, param_t lp, param_t wp);

#endif /* _WIDGETS_STACK_H */
