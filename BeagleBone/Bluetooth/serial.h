#ifndef _RS232_H
#define _RS232_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/file.h>


int serialOpenComport(int, int, int, int, int, int);
int serialPollComport(int, unsigned char *, int);
int serialSendByte(int, unsigned char);
int serialSendBuf(int, unsigned char *, int);
void serialCloseComport(int);
void serialCputs(int, const char *);
int serialIsDCDEnabled(int);
int serialIsCTSEnabled(int);
int serialIsDSREnabled(int);
void serialEnableDTR(int);
void serialDisableDTR(int);
void serialEnableRTS(int);
void serialDisableRTS(int);
void serialFlushRX(int);
void serialFlushTX(int);
void serialFlushRXTX(int);

#ifdef __cplusplus
} 
#endif



