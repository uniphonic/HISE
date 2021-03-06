/*  ===========================================================================
*
*   This file is part of HISE.
*   Copyright 2016 Christoph Hart
*
*   HISE is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   HISE is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with HISE.  If not, see <http://www.gnu.org/licenses/>.
*
*   Commercial licences for using HISE in an closed source project are
*   available on request. Please visit the project's website to get more
*   information about commercial licencing:
*
*   http://www.hartinstruments.net/hise/
*
*   HISE is based on the JUCE library,
*   which also must be licenced for commercial applications:
*
*   http://www.juce.com
*
*   ===========================================================================
*/

/******************************************************************************

BEGIN_JUCE_MODULE_DECLARATION

  ID:               hi_modules
  vendor:           Hart Instruments
  version:          0.999
  name:             HISE Processor Modules
  description:      All processors for HISE
  website:          http://hise.audio
  license:          GPL / Commercial

  dependencies:      juce_audio_basics, juce_audio_devices, juce_audio_formats, juce_audio_processors, juce_core, juce_cryptography, juce_data_structures, juce_events, juce_graphics, juce_gui_basics, juce_gui_extra, juce_opengl, hi_core

END_JUCE_MODULE_DECLARATION

******************************************************************************/

#ifndef HI_MODULES_INCLUDED
#define HI_MODULES_INCLUDED

#include "AppConfig.h"
#include "../hi_scripting/hi_scripting.h"

/** @defgroup modulatorTypes ModulatorTypes =========================================================================
*	@ingroup modulator
*
*	Here are all actual Modulators that can be used.
*/

#include "modulators/mods/ConstantModulator.h"
#include "modulators/mods/ControlModulator.h"
#include "modulators/mods/LFOModulator.h"
#include "modulators/mods/AudioFileEnvelope.h"
#include "modulators/mods/MacroControlModulator.h"
#include "modulators/mods/PluginParameterModulator.h"
#include "modulators/mods/RandomModulator.h"
#include "modulators/mods/SimpleEnvelope.h"
#include "modulators/mods/KeyModulator.h"
#include "modulators/mods/AhdsrEnvelope.h"
#include "modulators/mods/PitchWheelModulator.h"
#include "modulators/mods/TableEnvelope.h"
#include "modulators/mods/VelocityModulator.h"
#include "modulators/mods/GlobalModulators.h"
#include "modulators/mods/ArrayModulator.h"
#include "modulators/mods/CCEnvelope.h"
#include "modulators/mods/CCDucker.h"
#include "modulators/mods/GainMatcher.h"

#if USE_BACKEND

#include "modulators/editors/AhdsrEnvelopeEditor.h"
#include "modulators/editors/ConstantEditor.h"
#include "modulators/editors/ControlEditor.h"
#include "modulators/editors/CCDuckerEditor.h"
#include "modulators/editors/CCEnvelopeEditor.h"
#include "modulators/editors/LfoEditor.h"
#include "modulators/editors/AudioFileEnvelopeEditor.h"
#include "modulators/editors/KeyEditor.h"
#include "modulators/editors/MacroControlModulatorEditor.h"
#include "modulators/editors/PitchWheelEditor.h"
#include "modulators/editors/PluginParameterEditor.h"
#include "modulators/editors/RandomEditor.h"
#include "modulators/editors/SimpleEnvelopeEditor.h"
#include "modulators/editors/TableEnvelopeEditor.h"
#include "modulators/editors/VelocityEditor.h"
#include "modulators/editors/ArrayModulatorEditor.h"
#include "modulators/editors/GlobalModulatorEditor.h"
#include "modulators/editors/GainMatcherEditor.h"

#endif

/** @defgroup midiTypes MidiProcessor Types =======================================================================
*	@ingroup midiProcessor
*
*	All actual MidiProcessors that can be used.
*	There are almost none, because everything can also be achieved using scripts (or hardcoded scripts)
*/

#include "midi_processor/mps/MidiDelay.h"
#include "midi_processor/mps/Transposer.h"
#include "midi_processor/mps/SampleRaster.h"
#include "midi_processor/mps/RoundRobin.h"



#if USE_BACKEND

#include "midi_processor/editors/TransposerEditor.h"

#endif

/** @defgroup effectTypes Effect Types ===========================================================================
*	@ingroup dsp
*
*	Contains all audio effect classes
*/

#include "effects/MdaEffectWrapper.h"

#include "effects/fx/RouteFX.h"
#include "effects/fx/Filters.h"
#include "effects/fx/HarmonicFilter.h"
#include "effects/fx/CurveEq.h"
#include "effects/fx/StereoFX.h"
#include "effects/fx/SimpleReverb.h"
#include "effects/fx/Delay.h"
#include "effects/fx/GainEffect.h"
#include "effects/fx/Chorus.h"
#include "effects/fx/Phaser.h"
#include "effects/fx/GainCollector.h"
#include "effects/convolution/AtkConvolution.h"
#include "effects/convolution/Convolution.h"
#include "effects/mda/mdaLimiter.h"
#include "effects/mda/mdaDegrade.h"
#include "effects/fx/Saturator.h"
#include "effects/fx/AudioProcessorWrapper.h"


#if USE_BACKEND

#include "effects/editors/FilterEditor.h"
#include "effects/editors/HarmonicFilterEditor.h"
#include "effects/editors/CurveEqEditor.h"
#include "effects/editors/StereoEditor.h"
#include "effects/editors/ReverbEditor.h"
#include "effects/editors/DelayEditor.h"
#include "effects/editors/GainEditor.h"
#include "effects/editors/ChorusEditor.h"
#include "effects/editors/PhaserEditor.h"
#include "effects/editors/ConvolutionEditor.h"
#include "effects/editors/MdaLimiterEditor.h"
#include "effects/editors/MdaDegradeEditor.h"
#include "effects/editors/GainCollectorEditor.h"
#include "effects/editors/RouteFXEditor.h"
#include "effects/editors/SaturationEditor.h"

#endif

#include "effects/editors/AudioProcessorEditorWrapper.h"

#include "effects/fx/WrappedAudioProcessors/WrappedAudioProcessors.h"


/** @defgroup synthTypes Synth Types ===========================================================================
*	@ingroup dsp
*
*	Contains all synth classes
*/

#include "synthesisers/synths/GlobalModulatorContainer.h"
#include "synthesisers/synths/SineSynth.h"
#include "synthesisers/synths/NoiseSynth.h"
#include "synthesisers/synths/WaveSynth.h"
#include "synthesisers/synths/WavetableSynth.h"
#include "synthesisers/synths/AudioLooper.h"

#if USE_BACKEND

#include "synthesisers/editors/SineSynthBody.h"
#include "synthesisers/editors/WaveSynthBody.h"
#include "synthesisers/editors/GroupBody.h"
#include "synthesisers/editors/ModulatorSynthChainBody.h"
#include "synthesisers/editors/WavetableBody.h"
#include "synthesisers/editors/AudioLooperEditor.h"

#endif

#endif   // HI_MODULES_INCLUDED
