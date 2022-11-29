#include <stdio.h>
#include <cpc.h>
#include <psg.h>

#define SOUND_QUEUE 0xbcaa
#define SOUND_CHECK 0xbcad
#define SOUND_SET_ENV 0xbcc2
#define SOUND_SET_ENT 0xbcbf

unsigned char play_sound(unsigned char nChannelStatus, int nTonePeriod, int nDuration,
                    unsigned char nVolume, char nVolumeEnvelope, char nToneEnvelope,
                    unsigned char nNoisePeriod, char* soundBuff)
{
  //This function uses 9 bytes of memory for sound buffer. Firmware requires it in the central 32K of RAM (0x4000 to 0xC000)
  /*
    The bytes required to define the sound are as follows
    byte 0 - channel status byte
    byte 1 - volume envelope to use
    byte 2 - tone envelope to use
    bytes 3&4 - tone period
    byte 5 - noise period
    byte 6 - start volume
    bytes 7&8 - duration of the sound, or envelope repeat count 
  */
  
  #asm
    EXTERN firmware
    ld hl, sp
    push hl
		pop ix

		// Args:
    // 16: nChannelStatus
    // 14,15: nTonePeriod
    // 12,13: nDuration
    // 10: nVolume
    // 8: nVolumeEnvelope
    // 6: nToneEnvelope
    // 4: nNoisePeriod
    // 2,3: soundBuff
    // 0,1: Return address

		ld h, (ix+3)
		ld l, (ix+2)

    ld a, (ix+16) // nChannelStatus
    ld (hl), a
    inc hl

    ld a, (ix+8) // nVolumeEnvelope
    ld (hl), a
    inc hl

    ld a, (ix+6) // nToneEnvelope
    ld (hl), a
    inc hl

    ld a, (ix+14) // nTonePeriod
    ld (hl), a
    inc hl
    ld a, (ix+15) // nTonePeriod
    ld (hl), a
    inc hl

    ld a, (ix+4) // nNoisePeriod
    ld (hl), a
    inc hl

    ld a, (ix+10) // nVolume
    ld (hl), a
    inc hl

    ld a, (ix+12) // nDuration
    ld (hl), a
    inc hl
    ld a, (ix+13) // nDuration
    ld (hl), a
    inc hl

    ld h, (ix+3)
    ld l, (ix+2)
    call firmware
    defw SOUND_QUEUE
  
    ld a, 0
    jp nc, _endsound
    ld a, 1
    _endsound:
  #endasm
}


void ent(unsigned char nEnvelopeNumber, unsigned char nNumberOfSteps,
         char nTonePeriodOfStep, unsigned char nTimePerStep,
         char* entBuff)
{
  //This function uses 16 bytes of memory for ent buffer. Firmware requires it in the central 32K of RAM (0x4000 to 0xC000)
  #asm
    EXTERN firmware
    ld hl, sp
    push hl
    pop ix

		// Args:
    // 10: nEnvelopeNumber
    // 8: nNumberOfSteps
    // 6: nTonePeriodOfStep
    // 4: nTimePerStep
    // 2,3: entBuff
    // 0,1: Return address
    ld h, (ix+3)
    ld l, (ix+2)

    ld a, 0x1
    ld (hl), a
    inc hl

    ld a, (ix+8) // nNumberOfSteps
    ld (hl), a
    inc hl

    ld a, (ix+6) // nTonePeriodOfStep
    ld (hl), a
    inc hl

    ld a, (ix+4) // nTimePerStep
    ld (hl), a
    inc hl

    ld a, (ix+10) // nEnvelopeNumber
    ld h, (ix+3)
    ld l, (ix+2)
    call firmware
    defw SOUND_SET_ENT
  #endasm
}

void env(unsigned char nEnvelopeNumber, unsigned char nNumberOfSteps,
         char nSizeOfStep, unsigned char nTimePerStep, char* envBuff)
{
  // This function uses 16 bytes of memory for env buffer. Firmware requires it in the central 32K of RAM (0x4000 to 0xC000)
  #asm
    EXTERN firmware
    ld hl, sp
    push hl
    pop ix

		// Args:
    // 10: nEnvelopeNumber
    // 8: nNumberOfSteps
    // 6: nSizeOfStep
    // 4: nTimePerStep
    // 2,3: envBuff
    // 0,1: Return address
    ld h, (ix+3)
    ld l, (ix+2)

    ld a, 0x1
    ld (hl), a
    inc hl

    ld a, (ix+8) // nNumberOfSteps
    ld (hl), a
    inc hl

    ld a, (ix+6) // nSizeOfStep
    ld (hl), a
    inc hl

    ld a, (ix+4) // nTimePerStep
    ld (hl), a
    inc hl

    ld a, (ix+10) // nEnvelopeNumber

    ld h, (ix+3)
    ld l, (ix+2)
    call firmware
    defw SOUND_SET_ENT
  #endasm
}

unsigned char soundcheck(unsigned char nChannelStatus)
{
  #asm
    EXTERN firmware
    ld hl, sp
    push hl
    pop ix

		// Args:
    // 2: nChannelStatus
    // 0,1: Return address
    ld a, (ix+2) // nChannelStatus
    call firmware
    defw SOUND_CHECK
    and 0x80
    ld h, 0
    ld l, a
  #endasm
}

main()
{
  // Firmware requires these buffers in the central 32K of RAM (0x4000 to 0xC000)
  unsigned char soundBuff[9] = {0};
  unsigned char entBuff[16] = {0};
  unsigned char envBuff[16] = {0};

  int aSounds[12] = { 478, 451, 426, 402, 379, 358, 338, 319, 301, 284, 268, 253 };
  int n = 0;

  printf("1\n\r");

  for(n=0; n < 12; n++)
  {
    while(!play_sound(1, aSounds[n], 20, 15, 0, 0, 0, soundBuff));
  }

  while(soundcheck(1));

  printf("Press key\n\r");
  fgetc_cons();
  printf("2\n\r");

  ent(1, 20, -11, 1, entBuff);
  play_sound(1, 200, 20, 15, 0, 1, 0, soundBuff);

  while(soundcheck(1));

  printf("Press key\n\r");
  fgetc_cons();
  printf("3\n\r");

  ent(1, 20, -11, 1, entBuff);
  play_sound(1, 428, 10, 15, 0, 1, 0, soundBuff);

  while(soundcheck(1));

  printf("Press key\n\r");
  fgetc_cons();
  printf("4\n\r");

  env(1, 25, 15, 5, envBuff);
  ent(1, 25, 120, 6, entBuff);
  play_sound(1, 428, 25, 15, 1, 1, 14, soundBuff);

  while(soundcheck(1));

  printf("Press key\n\r");
  fgetc_cons();
  printf("5\n\r");

  play_sound(1, 200, 20, 15, 0, 0, 0, soundBuff);
  
  while(soundcheck(1));

  printf("Press key\n\r");
  fgetc_cons();
  printf("6\n\r");
  
  env(1, 10, 1, 100, envBuff);
  play_sound(1, 284, 1000, 1, 1, 0, 0, soundBuff);
  
  while(soundcheck(1));

  printf("Press key\n\r");
  fgetc_cons();
  printf("7\n\r");

  ent(1, 100, 2, 2, entBuff);
  play_sound(1,284,200,15,0,1,0, soundBuff);
  
  while(soundcheck(1));

  printf("Press key\n\r");
  fgetc_cons();
  printf("8\n\r");

  ent(1, 100, -2, 2, entBuff);
  play_sound(1, 284, 200, 15, 0, 1, 0, soundBuff);
  
  while(soundcheck(1));

  printf("Press key\n\r");
  fgetc_cons();
}
