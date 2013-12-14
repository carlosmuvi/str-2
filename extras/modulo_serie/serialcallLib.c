#include <stdio.h>
#include <string.h>
#include <syscall.h>
#include <rtpLib.h>

int initSerialMod_LINUX_9600 ()
{
	return syscall (0,0,0,0,0,0,0,0,SYSCALL_NUMBER(2,0));
}

int initSerialMod_LINUX_115200 ()
{
	return syscall (0,0,0,0,0,0,0,0,SYSCALL_NUMBER(2,1));
}

int initSerialMod_WIN_9600 ()
{
	return syscall (0,0,0,0,0,0,0,0,SYSCALL_NUMBER(2,2));
}

int initSerialMod_WIN_115200 ()
{
	return syscall (0,0,0,0,0,0,0,0,SYSCALL_NUMBER(2,3));
}

int readSerialMod_9 (char buffer[9])
{
	return syscall ((int)buffer,0,0,0,0,0,0,0,SYSCALL_NUMBER(2,4));
}

int writeSerialMod_9 (char buffer[9])
{
	return syscall ((int)buffer,0,0,0,0,0,0,0,SYSCALL_NUMBER(2,5));
}

int writeSerialMod_256 (char buffer[256])
{
	return syscall ((int)buffer,0,0,0,0,0,0,0,SYSCALL_NUMBER(2,6));
}

int closeSerialMod ()
{
	return syscall (0,0,0,0,0,0,0,0,SYSCALL_NUMBER(2,7));
}

