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


#pragma warning (push)
#pragma warning (disable: 4127)

namespace icstdsp
{
	#include "icst/AudioAnalysis.cpp"
	#include "icst/fftoourad.cpp"
	#include "icst/fftoouraf.cpp"
	#include "icst/AudioSynth.cpp"
	#include "icst/BlkDsp.cpp"
	#include "icst/Chart.cpp"
	#include "icst/Neuro.cpp"
	#include "icst/SpecMath.cpp"
}

	

#pragma warning (pop)

#if JUCE_MSVC
#pragma warning (push)
#pragma warning (disable: 4244 4127 4267)

#endif

extern "C"
{
#include "kiss_fft/kiss_fft.c"
#include "kiss_fft/kiss_fftr.c"
}



#if JUCE_MSVC
#pragma warning (pop)
#endif
