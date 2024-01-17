#include "../src/myReadkey.c"

int main()
{
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


    return 0;
}
