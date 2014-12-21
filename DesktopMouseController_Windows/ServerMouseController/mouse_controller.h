
#ifndef __MOUSE_CONTROLLER_H__
#define __MOUSE_CONTROLLER_H__

typedef struct point_s point_t;

void move_mouse(point_t const *previous_point, point_t const *new_point);
void left_down();
void right_down();
void left_up();
void right_up();

#endif
