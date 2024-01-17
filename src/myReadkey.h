#ifndef MYREADKEY_H
#define MYREADKEY_H

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 8

enum keys
{
  KEY_l,
  KEY_s,
  KEY_r,
  KEY_t,
  KEY_i,
  KEY_F5,
  KEY_F6,
  KEY_up,
  KEY_down,
  KEY_right,
  KEY_left,
  KEY_enter,
  KEY_default
};

int rk_readkey(enum keys*);
int rk_mytermsave(void);
int rk_mytermrestore(void);
int rk_mytermregime(int, int, int, int, int);

#endif
