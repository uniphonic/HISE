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

#ifndef VOICECPUBPMCOMPONENT_H_INCLUDED
#define VOICECPUBPMCOMPONENT_H_INCLUDED

class VuMeter;

class VoiceCpuBpmComponent : public Component,
	public Timer,
	public ButtonListener
{
public:

	VoiceCpuBpmComponent(MainController *mc_);

	// ================================================================================================================

	void buttonClicked(Button *b) override;
	void timerCallback() override;
	void resized() override;

	void paint(Graphics& g) override;
	void paintOverChildren(Graphics& g) override;

private:

	// ================================================================================================================

	MainController *mc;

	ScopedPointer<ShapeButton> panicButton;
	ScopedPointer<ShapeButton> midiButton;

	ScopedPointer<Label> voiceLabel;
	ScopedPointer<VuMeter> cpuSlider;
	ScopedPointer<Label> bpmLabel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VoiceCpuBpmComponent)
};



#endif  // VOICECPUBPMCOMPONENT_H_INCLUDED
