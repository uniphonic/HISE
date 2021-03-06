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

#ifndef SINGLETONPOPUP_H_INCLUDED
#define SINGLETONPOPUP_H_INCLUDED


class BaseDebugArea;

class AutoPopupDebugComponent
{
public:

	void showComponentInDebugArea(bool shouldBeVisible);

	virtual ~AutoPopupDebugComponent() {};

protected:

	AutoPopupDebugComponent(BaseDebugArea *area) :
		parentArea(area)
	{};

private:

	BaseDebugArea *parentArea;
};


/** A small utility class that allows popups for console and plotter. 
*
*	@ingroup utility
*/
class PopupWindow: public DocumentWindow
{
public:

	/** Creates a basic Popup window with all Buttons .
	*
	*	@param t the Popup title.
	*/
	PopupWindow(const String &t): DocumentWindow(t, Colours::lightgrey, DocumentWindow::allButtons, true)
	{
		
	};

	virtual ~PopupWindow()
	{ };

	void setDeleteOnClose(bool shouldBeDeleted)
	{
		deleteWhenClosed = shouldBeDeleted;
	};

	/** Removes the instance from the desktop. You have to take care about ownership elsewhere! */
	virtual void closeButtonPressed()
	{ 
		deleteWhenClosed ? delete this : removeFromDesktop(); 
	};

private: 

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PopupWindow)

	

	bool deleteWhenClosed;
};

class AboutPage;

class TooltipBar: public Component,
				  public Timer
{
public:

	enum ColourIds
	{
		backgroundColour = 0x100,
		textColour = 0x010,
		iconColour = 0x001
	};

	TooltipBar();

	void paint(Graphics &g);

	void timerCallback();
	

	void setText(const String &t)
	{
		if(t != currentText)
		{
			isClear = false;
			counterSinceLastTextChange = 0;
			currentText = t;
			repaint();
		}
		
	}

	void clearText()
	{
		if(!isClear)
		{
			isClear = true;
			currentText = String();
			counterSinceLastTextChange = 0;
			repaint();
		}
	}

	void mouseDown(const MouseEvent &e);

private:

	int counterSinceLastTextChange;

	String currentText;
	bool isClear;

	Point<float> lastMousePosition;
	bool newPosition;

	
};


#endif  // SINGLETONPOPUP_H_INCLUDED
