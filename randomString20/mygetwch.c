#include "mygetwch.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
wint_t getwch(void)
{
    struct termios oldattr, newattr;    //атрибуты терминала
    wint_t ch;
    tcgetattr( STDIN_FILENO, &oldattr );    //получаем атрибуты
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );  //делаем небуферируемым
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getwchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );   //возвращаем атрибуты
    return ch;
}
