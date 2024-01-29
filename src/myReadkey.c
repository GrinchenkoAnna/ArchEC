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
    else if (buffer[0] == 'q') { *key = KEY_quit; }
    else if (!strcmp(buffer, "\E[15~") || !strcmp(buffer, "\E[[E")
             || !strcmp(buffer, "^[[15·"))
        { *key = KEY_F5; }
    else if (!strcmp(buffer, "\E[17~"))
        { *key = KEY_F6; }
    else if (!strcmp(buffer, "\EOA") || !strcmp(buffer, "\E[A"))
        { *key = KEY_up; }
    else if (!strcmp(buffer, "\EOB") || !strcmp(buffer, "\E[B"))
        { *key = KEY_down; }
    else if (!strcmp(buffer, "\EOC") || !strcmp(buffer, "\E[C")
             || !strcmp(buffer, "^[[C"))
        { *key = KEY_right; }
       else if (!strcmp(buffer, "\EOD") || !strcmp(buffer, "\E[D"))
        { *key = KEY_left; }
    else { *key = KEY_default; }

    return 0;
}

/*сохраняет текущие параметры терминала*/
int rk_mytermsave()
{
    struct termios settings;
    FILE *myterm_settings;
    int fd_term = open("/dev/tty", O_RDWR);
    myterm_settings = fopen("src/myterm_settings.txt", "wb");

    if (fd_term == -1)
    {
        printf("Ошибка open\n");
        return -1;
    }
    if (tcgetattr(fd_term, &settings) == -1)
    {
        printf("Не удалось получить параметры терминала\n");
        return -1;
    }
    if (myterm_settings == NULL)
    {
        printf("Не удалось открыть файл для записи параметров терминала\n");
        return -1;
    }

    fwrite(&settings, sizeof(settings), 1, myterm_settings);
    fclose(myterm_settings);

    return 0;
}

/*восстанавливает сохранѐнные параметры терминала*/
int rk_mytermrestore()
{
    struct termios settings;
    FILE *myterm_settings;
    int fd_term = open("/dev/tty", O_RDWR);
    myterm_settings = fopen("src/myterm_settings.txt", "rb");

    if (fd_term == -1)
    {
        printf("Ошибка open\n");
        return -1;
    }
    if (fread(&settings, sizeof(settings), 1, myterm_settings) == 0)
    {
        printf("Не удалось считать параметры терминала\n");
        return -1;
    }
    if (tcsetattr(fd_term, TCSANOW, &settings) == -1)
    {
        printf("Не удалось применить считанные параметры терминала\n");
        return -1;
    }

    return 0;
}

/*переключает терминал между режимами. Для неканонического режима
используются значения второго и последующего параметров*/
int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint)
{
    int fd_term = open("/dev/tty", O_RDWR);
    struct termios settings;

    if (fd_term == -1)
    {
        printf("Ошибка open\n");
        return -1;
    }
    if (tcgetattr(fd_term, &settings) == -1)
    {
        printf("Не удалось получить параметры терминала\n");
        return -1;
    }

    //канонический режим
    if (regime == 1)
    {
        settings.c_lflag |= ICANON;
        settings.c_cc[VTIME] = vtime; //время ожидания в децисекундах (0)
        settings.c_cc[VMIN] = vmin;   //min количество символов (0)
        settings.c_lflag |= ECHO; //на экране терминала сразу отображаются
                                  //вводимые с клавиатуры символы (да)
        settings.c_lflag |= ISIG; //обработка управляющих символов
                                  //(н-р, CTRL+C, CTRL+Z и т.д.) (включена)
    }
    //неканонический режим
    else if (!regime) { settings.c_lflag &= ~ICANON; }
    else { return -1; }

    if (!regime)
    {
        settings.c_cc[VTIME] = vtime;
        settings.c_cc[VMIN] = vmin;

        if (echo == 1) { settings.c_lflag |= ECHO; }
        else if (!echo) { settings.c_lflag &= ~ECHO; }
        else { return -1; }

        if (sigint == 1) { settings.c_lflag |= ISIG; }
        else if (!sigint) { settings.c_lflag &= ~ISIG; }
        else { return -1; }
    }

    if (tcsetattr(fd_term, TCSANOW, &settings) == -1)
    {
        printf("Не удалось применить установленные параметры терминала\n");
        return -1;
    }

    return 0;
}
