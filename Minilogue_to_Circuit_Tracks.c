// RK002_DUY
// Name: Minilogue_to_Circuit_Tracks
// Author: karmakaze

// DEVICE REFERENCES:
// - Source: Korg Minilogue (original) MIDI CC implementation
// - Target: Novation Circuit Tracks Synth 1 (Ch 0) and Synth 2 (Ch 1)
// MIDI channel 3+ messages are unchanged
//
// NOTES:
// - RK-002 uses 0-based channel indexing: ch 0 = MIDI Ch 1, ch 1 = MIDI Ch 2
// - Only processes channels 0-1 (Circuit Tracks synth channels)
// - Includes NRPN support for full parameter mapping

// ============================================================
// STATE TRACKING
// ============================================================
// Track LFO target mode per channel (0=Pitch, 1=Shape/PWM, 2=Cutoff)
byte lfoTarget[2] = {0, 0};  // Default to Pitch for channels 0 and 1

// Track current waveform per oscillator per channel (0=Saw, 1=Tri, 2=Pulse)
byte osc1Wave[2] = {0, 0};   // Default to Saw
byte osc2Wave[2] = {0, 0};   // Default to Saw

// ============================================================
// HELPER FUNCTION: Send NRPN Message
// ============================================================
void sendNRPN(byte channel, byte nrpn_msb, byte nrpn_lsb, byte value) {
  // Send NRPN selection
  RK002_sendControlChange(channel, 99, nrpn_msb);  // NRPN MSB (CC 99)
  RK002_sendControlChange(channel, 98, nrpn_lsb);  // NRPN LSB (CC 98)

  // Send data entry
  RK002_sendControlChange(channel, 6, value);      // Data Entry MSB (CC 6)

  // Send null NRPN to prevent stuck values (best practice)
  RK002_sendControlChange(channel, 101, 127);      // RPN MSB = null
  RK002_sendControlChange(channel, 100, 127);      // RPN LSB = null
}

// ============================================================
// MAIN CONTROL CHANGE HANDLER
// ============================================================
bool RK002_onControlChange(byte ch, byte nr, byte val) {
  // Only process channels 0-1 (MIDI Ch 1-2 = Circuit Tracks Synth 1-2)
  if (ch > 1) return true;

  int cc = -1;    // If set, send regular CC
  byte v = val;   // Value to send (may be modified)

  switch (nr) {
    // ============================================================
    // MIXER SECTION
    // ============================================================

    case 33:  // Minilogue: Noise Level
      cc = 56;  // Circuit Tracks: Noise Level
      break;

    case 39:  // Minilogue: VCO 1 Level
      cc = 51;  // Circuit Tracks: OSC 1 Level
      break;

    case 40:  // Minilogue: VCO 2 Level
      cc = 52;  // Circuit Tracks: OSC 2 Level
      break;

    // ============================================================
    // OSCILLATOR 1
    // ============================================================

    case 34:  // Minilogue: VCO 1 Pitch
      cc = 26;  // Circuit Tracks: OSC 1 Semitones
      break;

    case 36:  // Minilogue: VCO 1 Shape
      // Route based on current waveform
      if (osc1Wave[ch] == 2) {
        cc = 21;  // Pulse → Pulse Width Index
      } else if (osc1Wave[ch] == 0) {
        cc = 24;  // Saw → Density (supersaw)
      } else {
        cc = 20;  // Triangle → Wave Interpolate
      }
      break;

    case 50:  // Minilogue: VCO 1 Wave
      cc = 19;  // Circuit Tracks: OSC 1 Wave
      // Map to similar waveforms and track state
      if (val < 43) {
        v = 2;           // Sawtooth
        osc1Wave[ch] = 0; // Track as Saw
      } else if (val < 86) {
        v = 1;           // Triangle
        osc1Wave[ch] = 1; // Track as Triangle
      } else {
        v = 12;          // Pulse Width (PWM-capable)
        osc1Wave[ch] = 2; // Track as Pulse
      }
      break;

    // ============================================================
    // OSCILLATOR 2
    // ============================================================

    case 35:  // Minilogue: VCO 2 Pitch
      cc = 37;  // Circuit Tracks: OSC 2 Semitones
      break;

    case 37:  // Minilogue: VCO 2 Shape
      // Route based on current waveform
      if (osc2Wave[ch] == 2) {
        cc = 31;  // Pulse → Pulse Width Index
      } else if (osc2Wave[ch] == 0) {
        cc = 35;  // Saw → Density (supersaw)
      } else {
        cc = 30;  // Triangle → Wave Interpolate
      }
      break;

    case 51:  // Minilogue: VCO 2 Wave
      cc = 29;  // Circuit Tracks: OSC 2 Wave
      // Map to similar waveforms and track state
      if (val < 43) {
        v = 2;           // Sawtooth
        osc2Wave[ch] = 0; // Track as Saw
      } else if (val < 86) {
        v = 1;           // Triangle
        osc2Wave[ch] = 1; // Track as Triangle
      } else {
        v = 12;          // Pulse Width (PWM-capable)
        osc2Wave[ch] = 2; // Track as Pulse
      }
      break;

    // ============================================================
    // OSCILLATOR MODULATION
    // ============================================================

    case 41:  // Minilogue: Cross Mod Depth (Voice Depth)
      cc = 33;  // Circuit Tracks: OSC 2 Virtual Sync Depth
      break;

    case 80:  // Minilogue: Sync (on/off switch)
      cc = 33;  // Circuit Tracks: OSC 2 Virtual Sync Depth
      v = (val > 63) ? 127 : 0;  // Convert switch to depth
      break;

    case 81:  // Minilogue: Ring Mod (on/off switch)
      cc = 54;  // Circuit Tracks: Ring Mod Level
      v = (val > 63) ? 127 : 0;  // Convert switch to level
      break;

    // ============================================================
    // FILTER SECTION
    // ============================================================

    case 43:  // Minilogue: Filter Cutoff
      cc = 74;  // Circuit Tracks: Filter Frequency
      break;

    case 44:  // Minilogue: Filter Resonance
      cc = 71;  // Circuit Tracks: Filter Resonance
      break;

    case 45:  // Minilogue: Filter EG Intensity
      cc = 79;  // Circuit Tracks: Env 2 to Frequency
      break;

    case 84:  // Minilogue: Filter Type
      cc = 68;  // Circuit Tracks: Filter Type
      // Minilogue: 0-127 (2-pole/4-pole toggle)
      // Circuit Tracks: 0=LP12, 1=LP24, 2=BP6/6, 3=BP12/12, 4=HP12, 5=HP24
      v = (val > 63) ? 0 : 1;  // Map to LP12/LP24
      break;

    // ============================================================
    // ENVELOPES - AMP EG (Envelope 1) - Uses CC
    // ============================================================

    case 16:  // Minilogue: Amp EG Attack
      cc = 73;  // Circuit Tracks: Envelope 1 Attack
      break;

    case 17:  // Minilogue: Amp EG Decay
      cc = 75;  // Circuit Tracks: Envelope 1 Decay
      break;

    case 18:  // Minilogue: Amp EG Sustain
      cc = 70;  // Circuit Tracks: Envelope 1 Sustain
      break;

    case 19:  // Minilogue: Amp EG Release
      cc = 72;  // Circuit Tracks: Envelope 1 Release
      break;

    // ============================================================
    // ENVELOPES - MOD EG (Envelope 2) - Uses NRPN
    // ============================================================

    case 20:  // Minilogue: Mod EG Attack
      sendNRPN(ch, 0, 1, val);
      return false;

    case 21:  // Minilogue: Mod EG Decay
      sendNRPN(ch, 0, 2, val);
      return false;

    case 22:  // Minilogue: Mod EG Sustain
      sendNRPN(ch, 0, 3, val);
      return false;

    case 23:  // Minilogue: Mod EG Release
      sendNRPN(ch, 0, 4, val);
      return false;

    // ============================================================
    // LFO SECTION - Intelligent routing based on target mode
    // ============================================================

    case 56:  // Minilogue: LFO Target (sets the mode)
      // Minilogue LFO Target values (approximate):
      // 0-42: Pitch
      // 43-85: Shape (PWM)
      // 86-127: Cutoff
      if (val < 43) {
        lfoTarget[ch] = 0;  // Pitch mode
      } else if (val < 86) {
        lfoTarget[ch] = 1;  // Shape/PWM mode
      } else {
        lfoTarget[ch] = 2;  // Cutoff mode
      }
      return false;  // Don't send anything - just sets internal state

    case 24:  // Minilogue: LFO Rate
      // Pitch/Cutoff use LFO 1, Shape uses LFO 2
      if (lfoTarget[ch] == 1) {
        sendNRPN(ch, 0, 85, val);  // LFO 2 Rate
      } else {
        sendNRPN(ch, 0, 76, val);  // LFO 1 Rate
      }
      return false;

    case 57:  // Minilogue: LFO EG Mod (not directly mappable)
      // Circuit Tracks doesn't have direct LFO EG mod
      return true;  // Pass through

    case 58:  // Minilogue: LFO Wave
      // Pitch/Cutoff use LFO 1, Shape uses LFO 2
      v = (val * 37) / 127;  // Scale from 0-127 to 0-37 waveforms
      if (lfoTarget[ch] == 1) {
        sendNRPN(ch, 0, 79, v);  // LFO 2 Waveform
      } else {
        sendNRPN(ch, 0, 70, v);  // LFO 1 Waveform
      }
      return false;

    case 26:  // Minilogue: LFO Depth/Intensity
      // Route to appropriate modulation matrix slot based on target

      if (lfoTarget[ch] == 0) {
        // Pitch mode: LFO 1 -> Pitch (mod matrix slot 1)
        sendNRPN(ch, 1, 84, 7);   // Source 1: LFO 1 +/- (value 7)
        sendNRPN(ch, 1, 85, 0);   // Source 2: Direct (value 0)
        sendNRPN(ch, 1, 87, 0);   // Destination: Osc 1 & 2 Pitch (value 0)
        sendNRPN(ch, 1, 86, val); // Depth

      } else if (lfoTarget[ch] == 1) {
        // Shape/PWM mode: LFO 2 -> Pulse Width (mod matrix slot 2)
        sendNRPN(ch, 1, 89, 9);   // Source 1: LFO 2 +/- (value 9)
        sendNRPN(ch, 1, 90, 0);   // Source 2: Direct
        sendNRPN(ch, 1, 92, 5);   // Destination: Osc 1 Pulse Width (value 5)
        sendNRPN(ch, 1, 91, val); // Depth

      } else {
        // Cutoff mode: LFO 1 -> Filter Freq (mod matrix slot 3)
        sendNRPN(ch, 1, 94, 7);   // Source 1: LFO 1 +/- (value 7)
        sendNRPN(ch, 1, 95, 0);   // Source 2: Direct
        sendNRPN(ch, 1, 97, 12);  // Destination: Filter Frequency (value 12)
        sendNRPN(ch, 1, 96, val); // Depth
      }
      return false;

    // ============================================================
    // DELAY SECTION
    // ============================================================
    // Circuit Tracks delay is on Channel 16 (Project Control)
    // Not mapping these - would need to send on different channel

    case 29:  // Minilogue: Delay Hi-Pass
    case 30:  // Minilogue: Delay Time
    case 31:  // Minilogue: Delay Feedback
      return true;  // Pass through (skip)
  }

  // ============================================================
  // SEND REGULAR CC IF MAPPED
  // ============================================================

  if (cc != -1) {
    RK002_sendControlChange(ch, cc, v);
    return false;  // Block original message
  }

  // Pass through unmapped CCs
  return true;
}

void setup() {
  // No setup required
}

void loop() {
  // No loop code required
}
