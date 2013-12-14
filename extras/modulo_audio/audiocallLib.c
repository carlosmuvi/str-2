#include <stdio.h>
#include <string.h>
#include <syscall.h>
#include <rtpLib.h>

int iniciarAudio_Linux ()
{
	return syscall (0,0,0,0,0,0,0,0,SYSCALL_NUMBER(2,0));
}

int iniciarAudio_Windows ()
{
	return syscall (0,0,0,0,0,0,0,0,SYSCALL_NUMBER(2,1));
}

int reproducir (unsigned char buffer[8000])
{
	return syscall ((int)buffer,0,0,0,0,0,0,0,SYSCALL_NUMBER(2,2));
}

int reproducir_4000 (unsigned char buffer[4000])
{
	return syscall ((int)buffer,0,0,0,0,0,0,0,SYSCALL_NUMBER(2,3));
}

int reproducir_1bit_4000 (unsigned char buffer[500])
{
	return syscall ((int)buffer,0,0,0,0,0,0,0,SYSCALL_NUMBER(2,4));
}


int finalizarAudio ()
{
	return syscall (0,0,0,0,0,0,0,0,SYSCALL_NUMBER(2,5));
}

