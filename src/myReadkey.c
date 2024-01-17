#include "myReadkey.h"

int rk_readkey(enum keys *key)
{
    char buffer[BUFFER_SIZE];

    int fd_term = open("/dev/tty", O_RDWR);
    int read_input = read(fd_term, buffer, BUFFER_SIZE);

    if (fd_term == -1 || read_input == -1) { return -1; }

    for (int i = read_input - 1; i < BUFFER_SIZE; i++) { buffer[i] = '\0'; }

    if (buffer[0] == '\0') { *key = KEY_enter; }
    else if (buffer[0] == 'l') { *key = KEY_l; }
    else if (buffer[0] == 's') { *key = KEY_s; }
    else if (buffer[0] == 'r') { *key = KEY_r; }
    else if (buffer[0] == 't') { *key = KEY_t; }
    else if (buffer[0] == 'i') { *key = KEY_i; }
    else if (!strcmp(buffer, "\E[15~") || !strcmp(buffer, "\E[[E"))
        { *key = KEY_F5; }
    else if (!strcmp(buffer, "\E[17~"))
        { *key = KEY_F6; }
    else if (!strcmp(buffer, "\EOA") || !strcmp(buffer, "\E[A"))
        { *key = KEY_up; }
    else if (!strcmp(buffer, "\EOB") || !strcmp(buffer, "\E[B"))
        { *key = KEY_down; }
    else if (!strcmp(buffer, "\EOC") || !strcmp(buffer, "\E[C"))
        { *key = KEY_right; }
       else if (!strcmp(buffer, "\EOD") || !strcmp(buffer, "\E[D"))
        { *key = KEY_left; }
    else { *key = KEY_default; }

    return 0;
}

/*сохраняет текущие параметры терминала*/
int rk_mytermsave()
{


    return 0;
}

/*восстанавливает сохранѐнные параметры терминала*/
int rk_mytermrestore()
{


    return 0;
}

/*переключает терминала между режимами. Для неканонического режима
используются значения второго и последующего параметров*/
int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint)
{


    return 0;
}
