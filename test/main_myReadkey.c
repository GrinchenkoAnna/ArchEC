#include "../src/myReadkey.c"

void check_flags(struct termios term1, struct termios term2)
{
    printf("\nПроверка флагов\n");
    if (term1.c_iflag == term2.c_iflag)
        { printf("Флаг c_iflag успешно проверен\n"); }
    else { printf("Не удалось записать/восстановить флаг c_iflag\n"); }
    if (term1.c_oflag == term2.c_oflag)
        { printf("Флаг c_oflag успешно проверен\n"); }
    else { printf("Не удалось записать/восстановить флаг c_oflag\n"); }
    if (term1.c_cflag == term2.c_cflag)
        { printf("Флаг c_cflag успешно проверен\n"); }
    else { printf("Не удалось записать/восстановить флаг c_cflag\n"); }
    if (term1.c_lflag == term2.c_lflag)
        { printf("Флаг c_lflag успешно проверен\n"); }
    else { printf("Не удалось записать/восстановить флаг c_lflag\n"); }
    if (term1.c_cc[VINTR] == term2.c_cc[VINTR])
        { printf("Флаг c_cc[VINTR] успешно проверен\n"); }
    else { printf("Не удалось записать/восстановить флаг c_cc[VINTR]\n"); }
    if (term1.c_cc[VQUIT] == term2.c_cc[VQUIT])
        { printf("Флаг c_cc[VQUIT] успешно проверен\n"); }
    else { printf("Не удалось записать флаг c_cc[VQUIT]\n"); }
    if (term1.c_cc[VERASE] == term2.c_cc[VERASE])
        { printf("Флаг c_cc[VERASE] успешно проверен\n"); }
    else { printf("Не удалось записать/восстановить флаг c_cc[VERASE]\n"); }
    if (term1.c_cc[VKILL] == term2.c_cc[VKILL])
        { printf("Флаг c_cc[VKILL] успешно проверен\n"); }
    else { printf("Не удалось записать/восстановить флаг c_cc[VKILL]\n"); }
    if (term1.c_cc[VEOF] == term2.c_cc[VEOF])
        { printf("Флаг c_cc[VEOF] успешно проверен\n"); }
    else { printf("Не удалось записать/восстановить флаг c_cc[VEOF]\n"); }
    if (term1.c_cc[VMIN] == term2.c_cc[VMIN])
        { printf("Флаг c_cc[VMIN] успешно проверен\n"); }
    else { printf("Не удалось записать/восстановить флаг c_cc[VMIN]\n"); }
    if (term1.c_cc[VEOL] == term2.c_cc[VEOL])
        { printf("Флаг c_cc[VEOL] успешно проверен\n"); }
    else { printf("Не удалось записать/восстановить флаг c_cc[VEOL]\n"); }
    if (term1.c_cc[VTIME] == term2.c_cc[VTIME])
        { printf("Флаг c_cc[VTIME] успешно проверен\n"); }
    else { printf("Не удалось записать/восстановить флаг c_cc[VTIME]\n"); }
    if (term1.c_cc[VEOL2] == term2.c_cc[VEOL2])
        { printf("Флаг c_cc[VEOL2] успешно проверен\n"); }
    else { printf("Не удалось записать/восстановить флаг c_cc[VEOL2]\n"); }
    if (term1.c_cc[VSWTC] == term2.c_cc[VSWTC])
        { printf("Флаг c_cc[VSWTC] успешно проверен\n"); }
    else { printf("Не удалось записать/восстановить флаг c_cc[VSWTC]\n"); }
    if (term1.c_cc[VSWTC] == term2.c_cc[VSWTC])
        { printf("Флаг c_cc[VSWTC] успешно проверен\n"); }
    else { printf("Не удалось записать/восстановить флаг c_cc[VSWTC]\n"); }
    if (term1.c_cc[VSTART] == term2.c_cc[VSTART])
        { printf("Флаг c_cc[VSTART] успешно проверен\n"); }
    else { printf("Не удалось записать/восстановить флаг c_cc[VSTART]\n"); }
    if (term1.c_cc[VSTOP] == term2.c_cc[VSTOP])
        { printf("Флаг c_cc[VSTOP] успешно проверен\n"); }
    else { printf("Не удалось записать/восстановить флаг c_cc[VSTOP]\n"); }
    if (term1.c_cc[VSUSP] == term2.c_cc[VSUSP])
        { printf("Флаг c_cc[VSUSP] успешно проверен\n"); }
    else { printf("Не удалось записать/восстановить флаг c_cc[VSUSP]\n"); }
    if (term1.c_cc[VREPRINT] == term2.c_cc[VREPRINT])
        { printf("Флаг c_cc[VREPRINT] успешно проверен\n"); }
    else { printf("Не удалось записать/восстановить флаг c_cc[VREPRINT]\n"); }
    if (term1.c_cc[VDISCARD] == term2.c_cc[VDISCARD])
        { printf("Флаг c_cc[VDISCARD] успешно проверен\n"); }
    else { printf("Не удалось записать/восстановить флаг c_cc[VDISCARD]\n"); }
    printf("Проверка флагов завершена\n");
}

int main()
{
    //rk_mytermregime
    rk_mytermregime(0, 30, 5, 0, 0);
    /* неканонический режим работы
     * ожидание 3 секунды
     * нужно ввести минимум 5 символов
     * вводимые символы не отображаются на экране терминала сразу
     * управляющие символы не обрабатываются*/
    struct termios settings;
    int fd_term = open("/dev/tty", O_RDWR);
    tcgetattr(fd_term, &settings);
    printf("settings.c_lflag = %x\n", settings.c_lflag);

    //rk_readkey
    enum keys key;

    do
    {
        rk_readkey(&key);
        switch (key)
            {
            case KEY_l:
                printf("нажата l\n"); break;

            case KEY_s:
                printf("нажата s\n"); break;

            case KEY_r:
                printf("нажата r\n"); break;

            case KEY_t:
                printf("нажата t\n"); break;

            case KEY_i:
                printf("нажата i\n"); break;

            case KEY_F5:
                printf("нажата F5\n"); break;

            case KEY_F6:
                printf("нажата F6\n"); break;

            case KEY_up:
                printf("нажата up\n"); break;

            case KEY_down:
                printf("нажата down\n"); break;

            case KEY_right:
                printf("нажата right\n"); break;

            case KEY_left:
                printf("нажата left\n"); break;

            case KEY_enter:
                printf("нажата enter\n"); break;

            case KEY_default:
                printf("такой клавиши нет в списке\n"); break;
            }
    } while (key != KEY_default);

    //rk_mytermsave
    FILE *myterm_settings;
    struct termios default_settings;
    struct termios modified_settings;
    struct termios read_settings;

    tcgetattr(fd_term, &default_settings); //записаны настройки default
    tcgetattr(fd_term, &modified_settings); //записаны настройки modified

    modified_settings.c_lflag |= ECHO;
    modified_settings.c_cc[VEOF] = 'y';
    tcsetattr(fd_term, TCSANOW, &modified_settings); //применены настройки modified

    if (rk_mytermsave() == -1) //записаны настройки modified
        { printf("Ошибка сохранения текущих параметров терминала\n"); }
    else
    {
        tcsetattr(fd_term, TCSANOW, &default_settings); //применены настройки default
        myterm_settings = fopen("src/myterm_settings.txt", "rb");
        fread(&read_settings, sizeof(read_settings), 1, myterm_settings);
        check_flags(modified_settings, read_settings); //проверка
    }

    //rk_mytermrestore
    rk_mytermrestore(); //применены настройки modified
    struct termios new_settings;
    tcgetattr(fd_term, &new_settings); //записаны настройки modified в new
    tcsetattr(fd_term, TCSANOW, &default_settings); //применены настройки default
    check_flags(modified_settings, new_settings); //проверка

    /* возврат в нормальное состояние терминала*/
    rk_mytermregime(1, 0, 0, 0, 0);
    return 0;
}
