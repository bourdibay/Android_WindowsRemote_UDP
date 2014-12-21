
#ifndef __JSON_MSG_PARSER_H__
#define __JSON_MSG_PARSER_H__

#include "../Jsmn/jsmn.h"

typedef enum type
{
    UNKNOWN = 0,
    MOVE,
    LEFT_DOWN_CLICK,
    RIGHT_DOWN_CLICK,
    LEFT_UP_CLICK,
    RIGHT_UP_CLICK
} type_t;

typedef struct point_s {
    int x;
    int y;
} point_t;

typedef struct delta_point_s
{
    point_t from_point;
    point_t to_point;
} delta_point_t;

typedef struct broadcast_message_s
{
    type_t type;
    delta_point_t delta_point;
} broadcast_message_t;

int are_same_points(point_t *point1, point_t *point2);
int are_same_deltas(delta_point_t *delta1, delta_point_t *delta2);
int are_same_messages(broadcast_message_t *msg1, broadcast_message_t *msg2);

void print_message(broadcast_message_t *message);

int parse_msg_json(char const *json_content, const size_t length, broadcast_message_t *message);

#endif
