/*
 * HEADERS of audiocallLib
 */
int iniciarAudio_Linux ();
int iniciarAudio_Windows ();
int reproducir (unsigned char buffer[8000]);
int reproducir_4000 (unsigned char buffer[4000]);
int reproducir_1bit_4000 (unsigned char buffer[500]);
int finalizarAudio (char buffer[9]);


