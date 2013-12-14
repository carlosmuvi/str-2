/*
 * HEADERS of serialcallLib
 */
int initSerialMod_LINUX_9600 ();
int initSerialMod_LINUX_115200 ();
int initSerialMod_WIN_9600 ();
int initSerialMod_WIN_115200 ();
int readSerialMod_9 (char buffer[9]);
int writeSerialMod_9 (char buffer[9]);
int writeSerialMod_256 (char buffer[256]);
int closeSerialMod ();

