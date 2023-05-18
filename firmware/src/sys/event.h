#ifndef _SYS_EVENT_H
#define _SYS_EVENT_H

typedef enum {
  event_none = 0,

  event_surface_create,
  event_surface_present,
  event_surface_destroy,

  event_button_down,
  event_button_up,
  event_button_hold,
  event_button_hold_click,
  event_button_release,

  event_widget_paint,
  event_widget_clicked,
  event_widget_get_value,

} event_t;

typedef uint32_t param_t;

#endif /* _SYS_EVENT_H */
