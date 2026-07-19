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

void sound_setup()
{
  unsigned char entBuff[16] = {0};
  unsigned char envBuff[16] = {0};

  // Explode sound tone and volume envelopes
  env(1, 25, 15, 5, envBuff);
  ent(1, 25, 120, 6, entBuff);
  // Missile shoot tone envelope
  ent(2, 20, -11, 1, entBuff);
  // Game over sound
  ent(3, 100, 2, 2, entBuff);
}

unsigned char soundBuff[9] = {0};

void explode_sound()
{
  play_sound(1, 428, 25, 15, 1, 1, 14, soundBuff);
}

void crash_sound()
{
  // Same as explode_sound for now
  // TODO: Find a different sound either for this or for explosion
  play_sound(1, 428, 25, 15, 1, 1, 14, soundBuff);
}

void missile_sound()
{
  play_sound(1, 428, 10, 15, 0, 2, 0, soundBuff);
}

void game_over_sound()
{
  play_sound(1, 284, 200, 15, 0, 3, 0, soundBuff);
}

