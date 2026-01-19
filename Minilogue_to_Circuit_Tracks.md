# Minilogue_to_Circuit_Tracks (MIDI Mapping)

## Complete Parameter Mapping Table

### Mixer Section

| Minilogue Parameter | Minilogue CC | Circuit Tracks Parameter | Circuit CC | Notes |
|---------------------|--------------|--------------------------|------------|-------|
| Noise Level | 33 | Noise Level | 56 | Direct 1:1 |
| VCO 1 Level | 39 | OSC 1 Level | 51 | Direct 1:1 |
| VCO 2 Level | 40 | OSC 2 Level | 52 | Direct 1:1 |

---

### Oscillator 1

| Minilogue Parameter | Minilogue CC | Circuit Tracks Parameter | Circuit CC/NRPN | Notes |
|---------------------|--------------|--------------------------|-----------------|-------|
| VCO 1 Pitch | 34 | OSC 1 Semitones | CC 26 | Direct mapping |
| VCO 1 Shape | 36 | **Context-dependent** | **Multiple CCs** | See Shape Routing below |
| VCO 1 Wave | 50 | OSC 1 Wave | CC 19 | See Wave Mapping below |

#### OSC 1 Shape Routing (CC 36)
**Routes based on current waveform:**
- **Pulse waveform** → CC 21 (Pulse Width Index) - PWM control
- **Saw waveform** → CC 24 (Density) - Supersaw thickness  
- **Triangle waveform** → CC 20 (Wave Interpolate) - Waveform morphing

#### OSC 1 Wave Mapping (CC 50)
| Minilogue Value | Circuit Tracks Value | Waveform | Internal State |
|-----------------|----------------------|----------|----------------|
| 0-42 | 2 | Sawtooth | Saw mode |
| 43-85 | 1 | Triangle | Triangle mode |
| 86-127 | 12 | Pulse Width | Pulse mode |

---

### Oscillator 2

| Minilogue Parameter | Minilogue CC | Circuit Tracks Parameter | Circuit CC/NRPN | Notes |
|---------------------|--------------|--------------------------|-----------------|-------|
| VCO 2 Pitch | 35 | OSC 2 Semitones | CC 37 | Direct mapping |
| VCO 2 Shape | 37 | **Context-dependent** | **Multiple CCs** | See Shape Routing below |
| VCO 2 Wave | 51 | OSC 2 Wave | CC 29 | See Wave Mapping below |

#### OSC 2 Shape Routing (CC 37)
**Routes based on current waveform:**
- **Pulse waveform** → CC 31 (Pulse Width Index) - PWM control
- **Saw waveform** → CC 35 (Density) - Supersaw thickness
- **Triangle waveform** → CC 30 (Wave Interpolate) - Waveform morphing

#### OSC 2 Wave Mapping (CC 51)
| Minilogue Value | Circuit Tracks Value | Waveform | Internal State |
|-----------------|----------------------|----------|----------------|
| 0-42 | 2 | Sawtooth | Saw mode |
| 43-85 | 1 | Triangle | Triangle mode |
| 86-127 | 12 | Pulse Width | Pulse mode |

---

### Oscillator Modulation

| Minilogue Parameter | Minilogue CC | Circuit Tracks Parameter | Circuit CC | Notes |
|---------------------|--------------|--------------------------|------------|-------|
| Cross Mod Depth | 41 | OSC 2 Virtual Sync Depth | CC 33 | Direct mapping |
| Sync | 80 | OSC 2 Virtual Sync Depth | CC 33 | On/Off → 0/127 |
| Ring Mod | 81 | Ring Mod Level | CC 54 | On/Off → 0/127 |

---

### Filter Section

| Minilogue Parameter | Minilogue CC | Circuit Tracks Parameter | Circuit CC | Notes |
|---------------------|--------------|--------------------------|------------|-------|
| Filter Cutoff | 43 | Filter Frequency | CC 74 | Direct 1:1 |
| Filter Resonance | 44 | Filter Resonance | CC 71 | Direct 1:1 |
| Filter EG Intensity | 45 | Env 2 to Frequency | CC 79 | Direct 1:1 |
| Filter Type | 84 | Filter Type | CC 68 | 0-63→LP24, 64-127→LP12 |

---

### Envelopes - Amplitude (Envelope 1)

| Minilogue Parameter | Minilogue CC | Circuit Tracks Parameter | Circuit CC | Notes |
|---------------------|--------------|--------------------------|------------|-------|
| Amp EG Attack | 16 | Envelope 1 Attack | CC 73 | Direct 1:1 |
| Amp EG Decay | 17 | Envelope 1 Decay | CC 75 | Direct 1:1 |
| Amp EG Sustain | 18 | Envelope 1 Sustain | CC 70 | Direct 1:1 |
| Amp EG Release | 19 | Envelope 1 Release | CC 72 | Direct 1:1 |

---

### Envelopes - Modulation (Envelope 2)

| Minilogue Parameter | Minilogue CC | Circuit Tracks Parameter | Circuit NRPN | Notes |
|---------------------|--------------|--------------------------|--------------|-------|
| Mod EG Attack | 20 | Env 2 Attack | NRPN 0:1 | Via NRPN |
| Mod EG Decay | 21 | Env 2 Decay | NRPN 0:2 | Via NRPN |
| Mod EG Sustain | 22 | Env 2 Sustain | NRPN 0:3 | Via NRPN |
| Mod EG Release | 23 | Env 2 Release | NRPN 0:4 | Via NRPN |

---

### LFO Section (Intelligent Routing)

#### LFO Target Mode Selection (CC 56)
**Sets internal routing mode - no MIDI sent:**

| Minilogue Value | LFO Target Mode | LFO Assignment |
|-----------------|-----------------|----------------|
| 0-42 | Pitch | Uses LFO 1 |
| 43-85 | Shape/PWM | Uses LFO 2 |
| 86-127 | Cutoff | Uses LFO 1 |

#### LFO Rate (CC 24)
**Routes based on current LFO Target mode:**

| LFO Target Mode | Circuit Tracks Parameter | Circuit NRPN |
|-----------------|--------------------------|--------------|
| Pitch | LFO 1 Rate | NRPN 0:76 |
| Shape/PWM | LFO 2 Rate | NRPN 0:85 |
| Cutoff | LFO 1 Rate | NRPN 0:76 |

#### LFO Wave (CC 58)
**Routes based on current LFO Target mode:**  
**Value scaled from 0-127 to 0-37 (Circuit Tracks waveforms)**

| LFO Target Mode | Circuit Tracks Parameter | Circuit NRPN |
|-----------------|--------------------------|--------------|
| Pitch | LFO 1 Waveform | NRPN 0:70 |
| Shape/PWM | LFO 2 Waveform | NRPN 0:79 |
| Cutoff | LFO 1 Waveform | NRPN 0:70 |

#### LFO Intensity/Depth (CC 26)
**Configures modulation matrix based on current LFO Target mode:**

##### Pitch Mode (Mod Matrix Slot 1)
| Parameter | Circuit NRPN | Value |
|-----------|--------------|-------|
| Source 1 | NRPN 1:84 | 7 (LFO 1 +/-) |
| Source 2 | NRPN 1:85 | 0 (Direct) |
| Destination | NRPN 1:87 | 0 (Osc 1 & 2 Pitch) |
| Depth | NRPN 1:86 | CC value |

##### Shape/PWM Mode (Mod Matrix Slot 2)
| Parameter | Circuit NRPN | Value |
|-----------|--------------|-------|
| Source 1 | NRPN 1:89 | 9 (LFO 2 +/-) |
| Source 2 | NRPN 1:90 | 0 (Direct) |
| Destination | NRPN 1:92 | 5 (Osc 1 Pulse Width) |
| Depth | NRPN 1:91 | CC value |

##### Cutoff Mode (Mod Matrix Slot 3)
| Parameter | Circuit NRPN | Value |
|-----------|--------------|-------|
| Source 1 | NRPN 1:94 | 7 (LFO 1 +/-) |
| Source 2 | NRPN 1:95 | 0 (Direct) |
| Destination | NRPN 1:97 | 12 (Filter Frequency) |
| Depth | NRPN 1:96 | CC value |

#### LFO EG Mod (CC 57)
**Not mapped** - Circuit Tracks doesn't have direct LFO EG modulation control

---

### Delay Section

| Minilogue Parameter | Minilogue CC | Circuit Tracks Parameter | Notes |
|---------------------|--------------|--------------------------|-------|
| Delay Hi-Pass | 29 | - | Not mapped (delay on Ch 16) |
| Delay Time | 30 | - | Not mapped (delay on Ch 16) |
| Delay Feedback | 31 | - | Not mapped (delay on Ch 16) |

**Note:** Circuit Tracks delay parameters are on MIDI Channel 16 (Project Control), not on synth channels 1-2.

---

## State Tracking

The implementation maintains internal state per MIDI channel for:

### LFO Target Mode
- Tracks which parameter the LFO is modulating (Pitch/Shape/Cutoff)
- Set by CC 56 (LFO Target)
- Determines routing for CC 24 (Rate), CC 58 (Wave), and CC 26 (Depth)

### Oscillator Waveforms
- Tracks current waveform for OSC 1 and OSC 2
- Set by CC 50 (OSC 1 Wave) and CC 51 (OSC 2 Wave)
- Determines routing for CC 36 (OSC 1 Shape) and CC 37 (OSC 2 Shape)

---

## MIDI Channel Configuration

**RK-002 uses 0-based channel indexing:**
- **Channel 0** → MIDI Channel 1 → Circuit Tracks Synth 1
- **Channel 1** → MIDI Channel 2 → Circuit Tracks Synth 2

Only these two channels are processed by the mapping code.

---

## Value Conversions

### On/Off to Continuous
Some Minilogue switches are converted to continuous parameters:
- **Sync** (CC 80): >63 → 127, ≤63 → 0
- **Ring Mod** (CC 81): >63 → 127, ≤63 → 0

### Filter Type
- **Minilogue** (CC 84): 0-127
- **Circuit Tracks** (CC 68): 0-63 → LP24 (value 1), 64-127 → LP12 (value 0)

### LFO Wave Scaling
- **Minilogue** (CC 58): 0-127 (continuous)
- **Circuit Tracks** (NRPN): 0-37 (discrete waveforms)
- **Conversion**: `(value * 37) / 127`

---

## Summary Statistics

- **Direct CC mappings:** 21 parameters
- **NRPN mappings:** 8 parameters
- **Context-dependent routings:** 6 parameters (Shape × 2, LFO Rate/Wave/Depth)
- **State variables:** 3 types (LFO Target, OSC1 Wave, OSC2 Wave)
- **Not mapped:** 4 parameters (Delay section + LFO EG Mod)
