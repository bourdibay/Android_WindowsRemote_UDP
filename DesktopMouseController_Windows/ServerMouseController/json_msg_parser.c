
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json_msg_parser.h"

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}

static int strToInt(char const *str)
{
    int i;
    sscanf_s(str, "%d", &i);
    return i;
}

static int parse_string(const char *json_content, jsmntok_t const *tokens,
    const int index, char **buffer)
{
    const size_t length = tokens[index + 1].end - tokens[index + 1].start;
    const size_t length_allocated = (length + 1) * sizeof(char);
    *buffer = malloc(length_allocated);
    if (!*buffer) {
        fprintf(stderr, "Error malloc in %s\n", __FUNCTIONW__);
        return -1;
    }
    strncpy_s(*buffer, length_allocated,
        json_content + tokens[index + 1].start, length);
    (*buffer)[length] = '\0';
    return 0;
}

static int parse_integer(const char *json_content, jsmntok_t const *tokens,
    const int index, int *integer)
{
    char *str = NULL;
    if (parse_string(json_content, tokens, index, &str) || !str) {
        return -1;
    }
    *integer = strToInt(str);
    free(str);
    return 0;
}

static int parse_type(const char *json_content, jsmntok_t const *tokens,
    const int index, type_t *type)
{
    char *str = NULL;
    if (parse_string(json_content, tokens, index, &str) || !str) {
        return -1;
    }
    if (!strcmp(str, "MOVE")) {
        *type = MOVE;
    }
    else if (!strcmp(str, "LEFT_DOWN_CLICK")) {
        *type = LEFT_DOWN_CLICK;
    }
    else if (!strcmp(str, "RIGHT_DOWN_CLICK")) {
        *type = RIGHT_DOWN_CLICK;
    }
    else if (!strcmp(str, "LEFT_UP_CLICK")) {
        *type = LEFT_UP_CLICK;
    }
    else if (!strcmp(str, "RIGHT_UP_CLICK")) {
        *type = RIGHT_UP_CLICK;
    }
    else {
        *type = UNKNOWN;
    }
    free(str);
    return 0;
}

int are_same_points(point_t *point1, point_t *point2)
{
    return point1->x == point2->x && point1->y == point2->y;
}

int are_same_deltas(delta_point_t *delta1, delta_point_t *delta2)
{
    return are_same_points(&delta1->from_point, &delta2->from_point) &&
        are_same_points(&delta1->to_point, &delta2->to_point);
}

int are_same_messages(broadcast_message_t *msg1, broadcast_message_t *msg2)
{
    return msg1->type == msg2->type && are_same_deltas(&msg1->delta_point, &msg2->delta_point);
}

void print_message(broadcast_message_t *message)
{
    printf("Message type=[%d] delta: from[%d, %d], to[%d, %d]\n", message->type,
        message->delta_point.from_point.x,
        message->delta_point.from_point.y,
        message->delta_point.to_point.x,
        message->delta_point.to_point.y);
}

int parse_msg_json(char const *json_content, const size_t length, broadcast_message_t *message)
{
    jsmn_parser parser;
    jsmntok_t tokens[128];
    jsmnerr_t nb_tokens;

    jsmn_init(&parser);
    nb_tokens = jsmn_parse(&parser, json_content, length,
        tokens, sizeof(tokens) / sizeof(tokens[0]));
    if (nb_tokens < 0) {
        fprintf(stderr, "Error during json parsing\n");
        return -1;
    }

    /* Assume the top-level element is an object */
    if (nb_tokens < 1 || tokens[0].type != JSMN_OBJECT) {
        fprintf(stderr, "Object expected, got: %d\n", tokens[0].type);
        return -1;
    }

    int i;
    for (i = 1; i < nb_tokens; ++i) {
        if (jsoneq(json_content, &tokens[i], "type") == 0) {
            if (parse_type(json_content, tokens, i, &message->type)) {
                return -1;
            }
            ++i;
        }
        else if (jsoneq(json_content, &tokens[i], "to_x") == 0) {
            if (parse_integer(json_content, tokens, i, &message->delta_point.to_point.x)) {
                return -1;
            }
            ++i;
        }
        else if (jsoneq(json_content, &tokens[i], "to_y") == 0) {
            if (parse_integer(json_content, tokens, i, &message->delta_point.to_point.y)) {
                return -1;
            }
            ++i;
        }
        else if (jsoneq(json_content, &tokens[i], "from_x") == 0) {
            if (parse_integer(json_content, tokens, i, &message->delta_point.from_point.x)) {
                return -1;
            }
            ++i;
        }
        else if (jsoneq(json_content, &tokens[i], "from_y") == 0) {
            if (parse_integer(json_content, tokens, i, &message->delta_point.from_point.y)) {
                return -1;
            }

            ++i;
        }
        else {
            fprintf(stderr, "Unexpected key: %.*s\n",
                tokens[i].end - tokens[i].start,
                json_content + tokens[i].start);
        }
    }
    return 0;
}


