
#include <stdio.h>
#include "error_handling.h"

void print_error(DWORD error)
{
    TCHAR lpBuffer[256];
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,                 // It´s a system error
        NULL,                                      // No string to be formatted needed
        error,                               // Hey Windows: Please explain this error!
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  // Do it in the standard language
        lpBuffer,              // Put the message here
        sizeof(lpBuffer) - 1,                     // Number of bytes to store the message
        NULL);
    fprintf(stderr, "Error: (%d) %s\n", error, lpBuffer);
}
