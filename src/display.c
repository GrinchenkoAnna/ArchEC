
#include <math.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

#include "myReadkey.c"
#include "Signal.c"

void key_convert(enum keys key)
{
    int ignoring_clock_pulses;
    sc_regGet(IGNORING_CLOCK_PULSES, &ignoring_clock_pulses);
    if (ignoring_clock_pulses != 0)
    {
        switch (key)
        {
        case KEY_l:
            sc_memoryLoad(filename);
            break;

        case KEY_s:
            sc_memorySave(filename);
            break;        

        case KEY_up:
            if (instructionCounter - 10 >= 0) { instructionCounter -= 10; }
            else { instructionCounter += 90; }
            break;

        case KEY_down:
            if (instructionCounter + 10 <= 99) { instructionCounter += 10; }
            else { instructionCounter -= 90; }
            break;

        case KEY_left:
            if (instructionCounter - 1 >= 0) { instructionCounter--; }
            else { instructionCounter = 99; }
            break;

        case KEY_right:
            if (instructionCounter + 1 <= 99) { instructionCounter++; }
            else { instructionCounter = 0; }
            break;

        case KEY_F5:
            // read(terminal, &buffer, sizeof(buffer));

            // if (buffer[0] == '-')
            // {
            //     accumulator = atoi(buffer + 1);
            //     if (accumulator > 0) { accumulator *= -1; }
            // }
            // else if (buffer[0] == '+' || buffer[0] == '0' || buffer[0] == '1' ||
            //          buffer[0] == '2' || buffer[0] == '3' || buffer[0] == '4' ||
            //          buffer[0] == '5' || buffer[0] == '6' || buffer[0] == '7' ||
            //          buffer[0] == '8' || buffer[0] == '9')
            // {
            //     if (buffer[0] == '+') { accumulator = atoi(buffer + 1); }
            //     else { accumulator = atoi(buffer); }

            //     if (accumulator < 0) { accumulator *= -1; }
            // }

            // if (accumulator > 9999 || accumulator < -9999)
            // {
            //     sc_regSet(OPERATION_OVERFLOW, 1);
            //     if (accumulator > 0) { accumulator = 9999; }
            //     else { accumulator = -9999; }
            // }
            // else { sc_regSet(OPERATION_OVERFLOW, 0); }
            accumulator = sc_memory[instructionCounter];

            break;

        case KEY_F6:
            break;

        case KEY_enter:
            break;

        case KEY_default:
            printf("Unknown command\n");
            break;

        default: break;
        }
    }
    if (key == KEY_r)
    {            
        sc_regSet(IGNORING_CLOCK_PULSES, 0);
        timerHandler(SIGALRM);
        //show_GUI(bg_color, fg_color);
    }

    else if (key == KEY_t)
    {
        sc_regSet(IGNORING_CLOCK_PULSES, 1);
        timerHandler(SIGALRM);
        //show_GUI(bg_color, fg_color);
    }

    else if (key == KEY_i) { raise(SIGUSR1); }       
}


int main()
{
    enum keys key;    

    sc_memorySet(26, 2020);
    sc_memorySave(filename);
    sc_memoryInit();
    sc_regInit();

    set_signals();

    rk_mytermsave();
    rk_mytermregime(1, 0, 0, 1, 1);

    fg_color = WHITE;
    bg_color = BLACK; 

    show_GUI(bg_color, fg_color);

    do
    {
        rk_readkey(&key);
        key_convert(key);
        show_GUI(bg_color, fg_color);
    } while (key != KEY_quit);

    mt_clrscr();
    printf("Exit My Simple Computer. Terminal settings restored\n");
    rk_mytermrestore();

    return 0;
}
