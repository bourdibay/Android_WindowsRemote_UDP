
#include <stdio.h>

#include "main_loop.h"
#include "server.h"
#include "json_msg_parser.h"
#include "mouse_controller.h"

#define BUFFER_LENGTH 100

static void interpret_message(broadcast_message_t *message)
{
    if (message->type == MOVE) {
        move_mouse(&message->delta_point.from_point, &message->delta_point.to_point);
    }
    else if (message->type == LEFT_DOWN_CLICK) {
        left_down();
    }
    else if (message->type == RIGHT_DOWN_CLICK) {
        right_down();
    }
    else if (message->type == LEFT_UP_CLICK) {
        left_up();
    }
    else if (message->type == RIGHT_UP_CLICK) {
        right_up();
    }
}

static int fill_message(SOCKET *socket_server, broadcast_message_t *message, char (*buffer)[BUFFER_LENGTH])
{
    SOCKADDR_IN from = { 0 };
    int fromsize = sizeof(from);
    int length_read = 0;

    length_read = recvfrom(*socket_server, *buffer, BUFFER_LENGTH * sizeof(char), 0,
        (SOCKADDR *)&from, &fromsize);
    if (length_read < 0)
    {
        return -1;
    }
    (*buffer)[length_read] = '\0';
    return 0;
}

void run_main_loop(SOCKET *socket_server)
{
    broadcast_message_t previous_message = { 0 };
    char buffer[BUFFER_LENGTH] = { 0 };

    while (1)
    {
        broadcast_message_t message = { 0 };
        if (fill_message(socket_server, &message, &buffer) < 0) {
            break;
        }
        if (parse_msg_json(buffer, strlen(buffer), &message) != 0) {
            fprintf(stderr, "Fail parse json\n");
        }
        else {
            if (!are_same_messages(&previous_message, &message))
            {
                /*
                print_message(&previous_message);
                print_message(&message);
                */
                interpret_message(&message);
            }
        }
        previous_message = message;
    }
}
