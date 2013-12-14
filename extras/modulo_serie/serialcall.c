#include <stdio.h>
#include "vxWorks.h"
#include "vxsimHostLib.h"
#include "syscallLib.h" 
#include "private/syscallLibP.h"
#include "iv.h"
#include "intLib.h"
#include "scMemVal.h" 
#include "string.h"

#define SERIAL_DLL_LINUX "serial/serial_dll.so"
#define DEV_NAME_LINUX "/dev/ttyACM0"
#define SERIAL_DLL_WINDOWS "c:/Users/str/Desktop/practica1/Serialdll_win_compiled/Serialdll.dll"
#define DEV_NAME_WINDOWS "COM3"

/*****************************************
 * VARIABLES GLOBALES 
 *****************************************/

FUNCPTR pDllInitSerial=NULL;
FUNCPTR pDllReadSerial=NULL;
FUNCPTR pDllWriteSerial=NULL;
FUNCPTR pDllCloseSerial=NULL;
        
int initSerialMod(char *serialDll, char *devName, int baudios)
{
    int lvl;
	int ret;
	
    /* Load the test DLL */
	if (vxsimHostDllLoad (serialDll) == ERROR) {
      printf ("Error: Unable to load %s\n",serialDll);
      return (ERROR);
    }
    
    /* Get the address of the DLL initAudio routine */
    pDllInitSerial = vxsimHostProcAddrGet ("initSerial");
    if (pDllInitSerial == NULL) {
      printf ("Error: Unable to find initSerial() symbol in loaded DLLs\n");
      return (ERROR);
    }
    /* Get the address of the DLL refillBuffer routine */
    pDllReadSerial = vxsimHostProcAddrGet ("readSerial");
    if (pDllReadSerial == NULL) {
      printf ("Error: Unable to find readSerial() symbol in loaded DLLs\n");
      return (ERROR);
    }
    /* Get the address of the DLL refillSample routine */
    pDllWriteSerial = vxsimHostProcAddrGet ("writeSerial");
    if (pDllWriteSerial == NULL) {
      printf ("Error: Unable to find writeSerial() symbol in loaded DLLs\n");
      return (ERROR);
    }
    /* Get the address of the DLL endAudio routine */
    pDllCloseSerial = vxsimHostProcAddrGet ("closeSerial");
    if (pDllCloseSerial == NULL) {
      printf ("Error: Unable to find closeSerial() symbol in loaded DLLs\n");
      return (ERROR);
    }

     /* inicializar serial port */
    lvl = intLock ();         /* lock interrupts */
    ret = pDllInitSerial (devName,baudios);  /* invoke the DLL initAudio routine */
    intUnlock (lvl);          /* unlock interrupts */
    
	return (ret);
}

int initSerialMod_LINUX_9600()
{
	return initSerialMod(SERIAL_DLL_LINUX, DEV_NAME_LINUX, 9600);
}

int initSerialMod_LINUX_115200()
{
	return initSerialMod(SERIAL_DLL_LINUX, DEV_NAME_LINUX, 115200);
}

int initSerialMod_WIN_9600()
{
	return initSerialMod(SERIAL_DLL_WINDOWS, DEV_NAME_WINDOWS, 9600);
}

int initSerialMod_WIN_115200()
{
	return initSerialMod(SERIAL_DLL_WINDOWS, DEV_NAME_WINDOWS, 115200);
}

int readSerialMod_9(int *intBuffer) 
{
	int  lvl;
    STATUS ret;
    unsigned char * buffer;
    unsigned char bufAux[9];
    int i, size = 0;
    
    buffer = (unsigned char *)(*intBuffer);
    ret = scMemValidate((void *)buffer, 9, SC_PROT_WRITE);
    if (ret == OK) {
        while (size < 9) {
            lvl = intLock ();         /* lock interrupts */	
            /* invoke the DLL refillBuffer routine */
            i = pDllReadSerial(bufAux,(9-size)); 
            intUnlock (lvl);          /* unlock interrupts */
            if (i > 0) {
                memcpy (&(buffer[size]),bufAux,i);
                size = size +i;
            }
        }
		return(size);	
	} else {
		printf ("reproducir: error en buffer\n");
		return(-1);	
	}
}

int writeSerialMod_9(int *intBuffer)
{
	int  lvl;
    STATUS ret;
    unsigned char * buffer;
    unsigned char bufAux[9];
    int size = 0;
    
    buffer = (unsigned char *)(*intBuffer);
    ret = scMemValidate((void *)buffer, size, SC_PROT_READ);
    if (ret == OK) {
		memcpy (bufAux, buffer, 9);
		lvl = intLock ();         /* lock interrupts */	
		size = pDllWriteSerial(bufAux,9); /* invoke the DLL refillBuffer routine */
		intUnlock (lvl);          /* unlock interrupts */
		return(size);	
	} else {
		printf ("reproducir: error en buffer\n");
		return(-1);	
	}
}

int writeSerialMod_256(int *intBuffer)
{
	int  lvl;
    STATUS ret;
    unsigned char * buffer;
    unsigned char bufAux[256];
    int size = 0;
    
    buffer = (unsigned char *)(*intBuffer);
    ret = scMemValidate((void *)buffer, size, SC_PROT_READ);
    if (ret == OK) {
		memcpy (bufAux, buffer, 256);
		lvl = intLock ();         /* lock interrupts */	
		size = pDllWriteSerial(bufAux,256); /* invoke the DLL refillBuffer routine */
		intUnlock (lvl);          /* unlock interrupts */
		return(size);	
	} else {
		printf ("reproducir: error en buffer\n");
		return(-1);	
	}
}

int closeSerialMod()
{
    int lvl;

    //printf ("closeSerial: Done\n");
    lvl = intLock ();         /* lock interrupts */
    pDllCloseSerial();		  /* invoke the DLL endAudio routine */
    intUnlock (lvl);          /* unlock interrupts */ 
    return 0;
}		

_WRS_DATA_ALIGN_BYTES(16) SYSCALL_RTN_TBL_ENTRY serialScRtnTbl [] =
		{
		{ (FUNCPTR) initSerialMod_LINUX_9600, 0, "initSerialMod_LINUX_9600", 0},
		{ (FUNCPTR) initSerialMod_LINUX_115200, 0, "initSerialMod_LINUX_115200", 1},
		{ (FUNCPTR) initSerialMod_WIN_9600, 0, "initSerialMod_WIN_9600", 2},
		{ (FUNCPTR) initSerialMod_WIN_115200, 0, "initSerialMod_WIN_115200", 3},
		{ (FUNCPTR) readSerialMod_9, 1, "readSerialMod_9", 4},
		{ (FUNCPTR) writeSerialMod_9, 1, "writeSerialMod_9", 5},
		{ (FUNCPTR) writeSerialMod_256, 1, "writeSerialMod_256", 6},
		{ (FUNCPTR) closeSerialMod, 0, "closeSerialMod", 7},
		};

int serialCall () 
{
	BOOL ret;
	ret = syscallGroupRegister (2, "serialCallGroup", 8, serialScRtnTbl, 0);
	if (ret == OK) {
		printf ("SerialCalls registered sucessfully\n");
	} else {
		printf ("ERROR: SerialCalls fails at register!!!\n");
	}
	return ret;
}
