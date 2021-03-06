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
#ifndef SCRIPTINGCODEEDITOR_H_INCLUDED
#define SCRIPTINGCODEEDITOR_H_INCLUDED



/** A subclass of CodeEditorComponent which improves working with Javascript scripts.
*	@ingroup scripting
*
*	It tokenises the code using Javascript keywords and dedicated HI keywords from the Scripting API
*	and applies some neat auto-intending features.
*/
class JavascriptCodeEditor: public CodeEditorComponent,
							public SettableTooltipClient,
							public Timer,
							public DragAndDropTarget,
							public CopyPasteTarget,
                            public SafeChangeListener
{
public:

	// ================================================================================================================

	enum DragState
	{
		Virgin = 0,
		JSONFound,
		NoJSONFound
	};

	enum ContextActions
	{
		SaveScriptFile = 101,
		LoadScriptFile,
		SaveScriptClipboard,
		LoadScriptClipboard,
		SearchReplace,
		AddCodeBookmark,
		CreateUiFactoryMethod,
		AddMissingCaseStatements,
		OpenExternalFile,
		OpenInPopup,
		ExportAsCompressedScript,
		ImportCompressedScript
	};


	typedef Range<int> CodeRegion;

	// ================================================================================================================

	JavascriptCodeEditor(CodeDocument &document, CodeTokeniser *codeTokeniser, JavascriptProcessor *p);;
	virtual ~JavascriptCodeEditor();

	// ================================================================================================================

    void changeListenerCallback(SafeChangeBroadcaster *) override;
	void timerCallback() override;

    void focusGained(FocusChangeType ) override;
	void focusLost(FocusChangeType t) override;
    
	virtual String getObjectTypeName() override;
	virtual void copyAction();
	virtual void pasteAction();;

	bool isInterestedInDragSource (const SourceDetails &dragSourceDetails) override;
	void itemDropped (const SourceDetails &dragSourceDetails) override;
	void itemDragEnter(const SourceDetails &dragSourceDetails) override;;
	void itemDragMove(const SourceDetails &dragSourceDetails);

	void addPopupMenuItems(PopupMenu &m, const MouseEvent *e) override;
    void performPopupMenuAction(int menuId) override;

	void showAutoCompleteNew();
	void closeAutoCompleteNew(const String returnString);

	void selectLineAfterDefinition(Identifier identifier);
	bool selectJSONTag(const Identifier &identifier);
	bool componentIsDefinedWithFactoryMethod(const Identifier& identifier);
	String createNewDefinitionWithFactoryMethod(const String &oldId, const String &newId, int newX, int newY);

	void createMissingCaseStatementsForComponents();

	void paintOverChildren(Graphics& g);

	void rebuildHighlightedSelection(Array<CodeRegion> &newArray) { highlightedSelection.swapWith(newArray); repaint(); }

	bool keyPressed(const KeyPress& k) override;
	void handleReturnKey() override;;
	void handleEscapeKey() override;
	void insertTextAtCaret(const String& newText) override;

	// ================================================================================================================

private:

	class AutoCompletePopup;

	// ================================================================================================================

	Array<CodeRegion> highlightedSelection;
	
	Range<int> getCurrentTokenRange() const;
	bool isNothingSelected() const;
	void handleDoubleCharacter(const KeyPress &k, char openCharacter, char closeCharacter);

	struct Helpers
	{
		static String getLeadingWhitespace(String line);
		static int getBraceCount(String::CharPointerType line);
		static bool getIndentForCurrentBlock(CodeDocument::Position pos, const String& tab,
											 String& blockIndent, String& lastLineIndent);

		static char getCharacterAtCaret(CodeDocument::Position pos, bool beforeCaret = false);
		
		static Range<int> getFunctionParameterTextRange(CodeDocument::Position pos);
	};

	Component::SafePointer<Component> currentModalWindow;

	ScopedPointer<AutoCompletePopup> currentPopup;

	JavascriptProcessor *scriptProcessor;
	Processor *processor;

	PopupMenu m;
	

	PopupLookAndFeel plaf;

	DragState positionFound;
    
	const int bookmarkOffset = 0xffff;

	struct Bookmarks
	{
		Bookmarks() :
			title(""),
			line(-1)
		{};

		Bookmarks(const String& lineText, int lineNumber)
		{
			title = lineText.removeCharacters("/!=-_");
			line = lineNumber;
		};

		String title;
		int line;
	};

	Array<Bookmarks> bookmarks;
	
	void increaseMultiSelectionForCurrentToken();
};



class CodeEditorWrapper: public Component,
						 public Timer
{
public:

	// ================================================================================================================

	CodeEditorWrapper(CodeDocument &document, CodeTokeniser *codeTokeniser, JavascriptProcessor *p);
	virtual ~CodeEditorWrapper();

	ScopedPointer<JavascriptCodeEditor> editor;

	void resized() override;;
	void timerCallback();

	void mouseDown(const MouseEvent &m) override;;
	void mouseUp(const MouseEvent &) override;;

	int currentHeight;

	// ================================================================================================================

private:

	ScopedPointer<ResizableEdgeComponent> dragger;

	ComponentBoundsConstrainer restrainer;

	LookAndFeel_V2 laf2;
	
	// ================================================================================================================

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CodeEditorWrapper);
};



#endif  // SCRIPTINGCODEEDITOR_H_INCLUDED
