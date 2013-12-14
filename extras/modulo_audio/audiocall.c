#include <stdio.h>
#include "vxWorks.h"
#include "vxsimHostLib.h"
#include "syscallLib.h" 
#include "private/syscallLibP.h"
#include "iv.h"
#include "intLib.h"
#include "scMemVal.h" 
#include "string.h"


#define AUDIO_DLL_LINUX "audio/Audiodll.so"
#define AUDIO_DLL_WINDOWS "c:/Temp/Audiodll.dll"

/*****************************************
 * VARIABLES GLOBALES 
 *****************************************/

FUNCPTR pDllInitAudio=NULL;
FUNCPTR pDllRefillSample=NULL;
FUNCPTR pDllRefillBuffer=NULL;
FUNCPTR pDllRefillBuffer_4000=NULL;
FUNCPTR pDllRefillBuffer_4000_1bit=NULL;
FUNCPTR pDllEndAudio=NULL;
        
int iniciarAudio_Linux()
{
    int lvl;

    /* Load the test DLL */
    if (vxsimHostDllLoad (AUDIO_DLL_LINUX) == ERROR) {  /* LINUX */
      printf ("Error: Unable to load Audiodll\n");
      return (ERROR);
    }
    
    /* Get the address of the DLL initAudio routine */
    pDllInitAudio = vxsimHostProcAddrGet ("initAudio");
    if (pDllInitAudio == NULL) {
      printf ("Error: Unable to find initAudio() symbol in loaded DLLs\n");
      return (ERROR);
    }
    /* Get the address of the DLL refillBuffer routine */
    pDllRefillBuffer = vxsimHostProcAddrGet ("refillBuffer");
    if (pDllRefillBuffer == NULL) {
      printf ("Error: Unable to find refillBuffer() symbol in loaded DLLs\n");
      return (ERROR);
    }
    /* Get the address of the DLL refillSample routine */
    pDllRefillSample = vxsimHostProcAddrGet ("refillSample");
    if (pDllRefillSample == NULL) {
      printf ("Error: Unable to find refillSample() symbol in loaded DLLs\n");
      return (ERROR);
    }
	/* Get the address of the DLL refillBuffer_4000 routine */
    pDllRefillBuffer_4000 = vxsimHostProcAddrGet ("refillBuffer_4000");
    if (pDllRefillSample == NULL) {
      printf ("Error: Unable to find refillBuffer_4000() symbol in loaded DLLs\n");
      return (ERROR);
    }
    /* Get the address of the DLL refillBuffer_4000_1bit routine */
    pDllRefillBuffer_4000_1bit = vxsimHostProcAddrGet ("refillBuffer_4000_1bit");
    if (pDllRefillSample == NULL) {
      printf ("Error: Unable to find refillBuffer_4000_1bit() symbol in loaded DLLs\n");
      return (ERROR);
    }

    /* Get the address of the DLL endAudio routine */
    pDllEndAudio = vxsimHostProcAddrGet ("endAudio");
    if (pDllEndAudio == NULL) {
      printf ("Error: Unable to find endAudio() symbol in loaded DLLs\n");
      return (ERROR);
    }

     /* inicializar audio */
    lvl = intLock ();         /* lock interrupts */
    pDllInitAudio ();         /* invoke the DLL initAudio routine */
    intUnlock (lvl);          /* unlock interrupts */
    
	return (0);
}

int iniciarAudio_Windows()
{
    int lvl;

    /* Load the test DLL */
    if (vxsimHostDllLoad (AUDIO_DLL_WINDOWS) == ERROR) {  /* WINDOWS */
      printf ("Error: Unable to load Audiodll\n");
      return (ERROR);
    }
    
    /* Get the address of the DLL initAudio routine */
    pDllInitAudio = vxsimHostProcAddrGet ("_Z9initAudiov");
    if (pDllInitAudio == NULL) {
      printf ("Error: Unable to find initAudio() symbol in loaded DLLs\n");
      return (ERROR);
    }
    /* Get the address of the DLL refillBuffer routine */
    pDllRefillBuffer = vxsimHostProcAddrGet ("_Z12refillBufferPh");
    if (pDllRefillBuffer == NULL) {
      printf ("Error: Unable to find refillBuffer() symbol in loaded DLLs\n");
      return (ERROR);
    }
    /* Get the address of the DLL refillSample routine */
    pDllRefillSample = vxsimHostProcAddrGet ("_Z12refillSamplehm");
    if (pDllRefillSample == NULL) {
      printf ("Error: Unable to find refillSample() symbol in loaded DLLs\n");
      return (ERROR);
    }
	/* Get the address of the DLL refillBuffer_4000 routine */
    pDllRefillBuffer_4000 = vxsimHostProcAddrGet ("_Z17refillBuffer_4000Ph");
    if (pDllRefillSample == NULL) {
      printf ("Error: Unable to find refillBuffer_4000() symbol in loaded DLLs\n");
      return (ERROR);
    }
    /* Get the address of the DLL refillBuffer_4000_1bit routine */
    pDllRefillBuffer_4000_1bit = vxsimHostProcAddrGet ("_Z22refillBuffer_4000_1bitPh");
    if (pDllRefillSample == NULL) {
      printf ("Error: Unable to find refillBuffer_4000_1bit() symbol in loaded DLLs\n");
      return (ERROR);
    }

    /* Get the address of the DLL endAudio routine */
    pDllEndAudio = vxsimHostProcAddrGet ("_Z8endAudiov");
    if (pDllEndAudio == NULL) {
      printf ("Error: Unable to find endAudio() symbol in loaded DLLs\n");
      return (ERROR);
    }

     /* inicializar audio */
    lvl = intLock ();         /* lock interrupts */
    pDllInitAudio ();         /* invoke the DLL initAudio routine */
    intUnlock (lvl);          /* unlock interrupts */
    
	return (0);
}

int reproducir(int *intBuffer)
{
	int lvl;
    STATUS ret;
    unsigned char * buffer;
    unsigned char bufAux[8000];
    
    buffer = (unsigned char *)(*intBuffer);
    ret = scMemValidate((void *)buffer, 8000, SC_PROT_READ);
    if (ret == OK) {
    	memcpy (bufAux,buffer,8000);
		lvl = intLock ();         /* lock interrupts */	
		pDllRefillBuffer(bufAux); /* invoke the DLL refillBuffer routine */
		intUnlock (lvl);          /* unlock interrupts */
	} else {
		printf ("reproducir: error en buffer\n");
	}
	return(0);	
}

int reproducir_4000(int *intBuffer)
{
	int lvl;
    STATUS ret;
    unsigned char * buffer;
    unsigned char bufAux[4000];
    
    buffer = (unsigned char *)(*intBuffer);
    ret = scMemValidate((void *)buffer, 4000, SC_PROT_READ);
    if (ret == OK) {
    	memcpy (bufAux,buffer,4000);
		printf ("reproducir_4000: begin\n");
		lvl = intLock ();         /* lock interrupts */	
		pDllRefillBuffer_4000(bufAux); /* invoke the DLL refillBuffer routine */
		intUnlock (lvl);          /* unlock interrupts */
		printf ("reproducir_4000: end\n");
    } else {
		printf ("reproducir_4000: error en buffer\n");
	}
	return(0);	
}

int reproducir_1bit_4000(int *intBuffer)
{
	int lvl;
    STATUS ret;
    unsigned char * buffer;
    unsigned char bufAux[500];
    
    buffer = (unsigned char *)(*intBuffer);
    ret = scMemValidate((void *)buffer, 500, SC_PROT_READ);
    if (ret == OK) {
    	memcpy (bufAux,buffer,500);
		printf ("reproducir_4000_1bit: begin\n");
		lvl = intLock ();         /* lock interrupts */	
		pDllRefillBuffer_4000_1bit(bufAux); /* invoke the DLL refillBuffer routine */
		intUnlock (lvl);          /* unlock interrupts */
		printf ("reproducir_4000_1bit: end\n");
	} else {
		printf ("reproducir_4000_1bit: error en buffer\n");
	}
	return(0);	
}

int finalizarAudio()
{
    int lvl;

    //printf ("finalizarAudio: Done\n");
    lvl = intLock ();         /* lock interrupts */
    pDllEndAudio();			  /* invoke the DLL endAudio routine */
    intUnlock (lvl);          /* unlock interrupts */ 
    return 0;
}		

_WRS_DATA_ALIGN_BYTES(16) SYSCALL_RTN_TBL_ENTRY audioScRtnTbl [] =
		{
		{ (FUNCPTR) iniciarAudio_Linux, 0, "iniciarAudio_Linux", 0},
		{ (FUNCPTR) iniciarAudio_Windows, 0, "iniciarAudio_Windows", 1},
		{ (FUNCPTR) reproducir, 1, "reproducir", 2},
		{ (FUNCPTR) reproducir_4000, 1, "reproducir_4000", 3},
		{ (FUNCPTR) reproducir_1bit_4000, 1, "reproducir_1bit_4000", 4},
		{ (FUNCPTR) finalizarAudio, 0, "finalizarAudio", 5},
		};

int audioCall () 
{
	BOOL ret;
	ret = syscallGroupRegister (2, "audioCallGroup", 6, audioScRtnTbl, 0);
	if (ret == OK) {
		printf ("AudioCalls registered sucessfully\n");
	} else {
		printf ("ERROR: AudioCalls fails at register!!!\n");
	}
	return 0;
}
