
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
        case KEY_l: sc_memoryLoad(filename); break;

        case KEY_s: sc_memorySave(filename); break;

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

        case KEY_F5: accumulator = sc_memory[instructionCounter]; break;

        case KEY_F6: instructionCounter = sc_memory[instructionCounter]; break;

        case KEY_enter:
            // убрать?
            break;

        case KEY_default: printf("Unknown command\n"); break;

        default: break;
        }
    }
    if (key == KEY_r)
    {
        show_GUI(bg_color, fg_color);
        sc_regSet(IGNORING_CLOCK_PULSES, 0);
        timerHandler(SIGALRM);
    }

    else if (key == KEY_t)
    {
        sc_regSet(IGNORING_CLOCK_PULSES, 1);
        timerHandler(SIGALRM);
    }

    else if (key == KEY_i) { raise(SIGUSR1); }       
}

int main()
{
    enum keys key;

    set_signals();
    raise(SIGUSR1);

    rk_mytermsave();
    //rk_mytermregime(1, 0, 0, 1, 1);

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
