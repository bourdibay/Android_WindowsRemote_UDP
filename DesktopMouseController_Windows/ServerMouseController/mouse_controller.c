
#include <Windows.h>
#include <stddef.h>
#include <stdio.h>

#include "error_handling.h"
#include "json_msg_parser.h"
#include "mouse_controller.h"

static int __calculate_distance_x(point_t const *start, point_t const *end)
{
    return end->x - start->x;
}

static int __calculate_distance_y(point_t const *start, point_t const *end)
{
    return end->y - start->y;
}

static void __move_cursor(point_t const *final_pos)
{
    const BOOL ret = SetCursorPos(final_pos->x, final_pos->y);
    if (!ret) {
        print_error(GetLastError());
    }
}
void move_mouse(point_t const *previous_point, point_t const *new_point)
{
    const int dx = __calculate_distance_x(previous_point, new_point);
    const int dy = __calculate_distance_y(previous_point, new_point);
    if (dx != 0 || dy != 0) {
        point_t final_pos;
        POINT point;
        BOOL ret = GetCursorPos(&point);
        if (!ret) {
            print_error(GetLastError());
        }
        final_pos.x = point.x + dx;
        final_pos.y = point.y + dy;
        __move_cursor(&final_pos);
        ret = GetCursorPos(&point);
        if (!ret) {
            print_error(GetLastError());
        }
    }
}

void left_down()
{
    INPUT Input = { 0 };
    ZeroMemory(&Input, sizeof(INPUT));
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &Input, sizeof(INPUT));
}

void left_up()
{
    INPUT Input = { 0 };
    ZeroMemory(&Input, sizeof(INPUT));
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &Input, sizeof(INPUT));

}

void right_down()
{
    INPUT Input = { 0 };
    ZeroMemory(&Input, sizeof(INPUT));
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    SendInput(1, &Input, sizeof(INPUT));
}

void right_up()
{
    INPUT Input = { 0 };
    ZeroMemory(&Input, sizeof(INPUT));
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    SendInput(1, &Input, sizeof(INPUT));

}
