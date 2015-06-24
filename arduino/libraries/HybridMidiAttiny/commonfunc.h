// Name: commonfunc.h

#ifndef __commonfunc_h_included__
#define __commonfunc_h_included__

#ifdef __cplusplus
extern "C"{
#endif

typedef uint8_t byte;

extern byte checkMidiMessage(byte *);
extern void (*cbNoteOff)(byte, byte, byte);
extern void (*cbNoteOn)(byte, byte, byte);
extern void (*cbCtlChange)(byte, byte, byte);
extern void handleMidiMessage(uchar *, uchar);



#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __commonfunc_h_included__ */
