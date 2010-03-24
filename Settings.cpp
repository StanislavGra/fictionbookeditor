#include "stdafx.h"
#include "Settings.h"

const wchar_t KEEP_ENCODING_KEY[]		= L"KeepEncoding";
const wchar_t DEFAULT_ENCODING_KEY[]	= L"DefaultSaveEncoding";
const wchar_t SEARCH_OPTIONS_KEY[]		= L"SearchOptions";
const wchar_t COLOR_BG_KEY[]			= L"ColorBG";
const wchar_t COLOR_FG_KEY[]			= L"ColorFG";
const wchar_t FONT_SIZE_KEY[]			= L"FontSize";
const wchar_t XML_SRC_WRAP_KEY[]		= L"XMLSrcWrap";
const wchar_t XML_SRC_SYNTAX_HL_KEY[]	= L"XMLSrcSyntaxHL";
const wchar_t XML_SRC_SHOW_EOL_KEY[]	= L"XMLSrcShowEOL";
const wchar_t FAST_MODE_KEY[]			= L"FastMode";
const wchar_t FONT_KEY[]				= L"Font";
const wchar_t VIEW_STATUS_BAR_KEY[]		= L"ViewStatusBar";
const wchar_t VIEW_DOCUMENT_TREE_KEY[]	= L"ViewDocumentTree";
const wchar_t SPLITTER_POS_KEY[]		= L"SplitterPos";
const wchar_t TOOLBARS_SETTINGS_KEY[]	= L"Toolbars";
const wchar_t WINDOW_POS_KEY[]			= L"WindowPosition";
const wchar_t RESTORE_FILE_POS_KEY[]	= L"RestoreFilePosition";
const wchar_t INTERFACE_LANG_KEY[]		= L"IntefaceLangID";
const wchar_t SCRIPTS_FOLDER_KEY[]		= L"ScriptsFolder";
const wchar_t INSIMAGE_ASKING[]			= L"InsImageDialog";
const wchar_t SCRIPTS_HKEY_ERR_NTF[]	= L"ScrHkErrDialog";
const wchar_t INS_CLEAR_IMAGE[]			= L"InsClearImage";

const wchar_t DEFAULT_ENCODING[]		= L"utf-8";
const wchar_t DEFAULT_FONT[]			= L"Trebuchet MS";
const wchar_t DEFAULT_SCRIPTS_FOLDER[]	= L"cmd";

// Hotkeys branch
const wchar_t HOTKEYS_BRANCH[] = L"Hotkeys";

CSettings::CSettings():m_need_restart(false), keycodes(0)
{
}

CSettings::~CSettings()
{
}

void CSettings::Init()
{
	TCHAR filepath[MAX_PATH];
	DWORD pathlen = ::GetModuleFileName(_Module.GetModuleInstance(),filepath,MAX_PATH);
	TCHAR *appname;
	if (pathlen == 0)
		appname = L"FictionBook Editor";
	else 
	{
		CString   tmp = U::GetFullPathName(filepath);
		int	      pos = tmp.ReverseFind(_T('\\'));
		if (pos >= 0)
			tmp.Delete(0, pos+1);
		pos = tmp.ReverseFind(_T('.'));
		if (pos >= 0) 
		{
			const TCHAR *cp = tmp;
			cp += pos;
			if(_tcsicmp(cp,_T(".exe")) == 0 || _tcsicmp(cp, _T(".dll")) == 0)
				tmp.Delete(pos, tmp.GetLength() - pos);
		}
		if (tmp.IsEmpty())
			appname=L"FictionBook Editor";
		else 
		{
			lstrcpyn(filepath, tmp, MAX_PATH);
			appname = L"FictionBook Editor"/*filepath*/;
		}
	}
	m_key_path = L"Software\\LitRes\\";
	m_key_path += appname;
	m_key.Create(HKEY_CURRENT_USER, m_key_path);
}

void CSettings::InitHotkeyGroups()
{
	// File group hotkeys
	CHotkeysGroup file_hotkeys_group(L"File", IDS_HOTKEY_GROUP_FILE);

	// Open
	CHotkey FileOpen(L"Open", IDS_HOTKEY_FILE_OPEN, FCONTROL, ID_FILE_OPEN, U::StringToKeycode(L"O"));
	file_hotkeys_group.m_hotkeys.push_back(FileOpen);

	// Save
	CHotkey FileSave(L"Save", IDS_HOTKEY_FILE_SAVE, NULL, ID_FILE_SAVE, VK_F2);
	file_hotkeys_group.m_hotkeys.push_back(FileSave);

	// Save as...
	CHotkey FileSaveAs(L"SaveAs", IDS_HOTKEY_FILE_SAVEAS, FSHIFT, ID_FILE_SAVE_AS, VK_F2);
	file_hotkeys_group.m_hotkeys.push_back(FileSaveAs);

	// Validate
	CHotkey FileValidate(L"Validate", IDS_HOTKEY_FILE_VALIDATE, NULL, ID_FILE_VALIDATE, VK_F8);
	file_hotkeys_group.m_hotkeys.push_back(FileValidate);

	//Edit group hotkeys
	CHotkeysGroup edit_hotkeys_group(L"Edit", IDS_HOTKEY_GROUP_EDIT);

	// Add annotation
	CHotkey EditAddAnnotation(L"AddAnnotation",
								IDS_HOTKEY_EDIT_ADD_ANNOTATION,
								FCONTROL,
								ID_EDIT_ADD_ANN,
								U::StringToKeycode(L"J"));
	edit_hotkeys_group.m_hotkeys.push_back(EditAddAnnotation);

	// Add body
	CHotkey EditAddBody(L"AddBody", IDS_HOTKEY_EDIT_ADD_BODY, FALT, ID_EDIT_ADD_BODY, U::StringToKeycode(L"B"));
	edit_hotkeys_group.m_hotkeys.push_back(EditAddBody);

	// Add epigraph
	CHotkey EditAddEpigraph(L"AddEpigraph",
							IDS_HOTKEY_EDIT_ADD_EPIGRAPH,
							FCONTROL,
							ID_EDIT_ADD_EPIGRAPH,
							U::StringToKeycode(L"N"));
	edit_hotkeys_group.m_hotkeys.push_back(EditAddEpigraph);

	// Add section image
	CHotkey EditAddSectionImage(L"AddSectionImage", IDS_HOTKEY_EDIT_ADD_IMAGE, FCONTROL, ID_EDIT_ADD_IMAGE,
		U::StringToKeycode(L"G"));
	edit_hotkeys_group.m_hotkeys.push_back(EditAddSectionImage);

	// Add text author
	CHotkey EditAddTextAuthor(L"AddTextAuthor", IDS_HOTKEY_EDIT_ADD_TA, FCONTROL, ID_EDIT_ADD_TA, U::StringToKeycode(L"D"));
	edit_hotkeys_group.m_hotkeys.push_back(EditAddTextAuthor);

	// Add title
	CHotkey EditAddTitle(L"AddTitle", IDS_HOTKEY_EDIT_ADD_TITLE, FCONTROL, ID_EDIT_ADD_TITLE,U::StringToKeycode(L"T"));
	edit_hotkeys_group.m_hotkeys.push_back(EditAddTitle);

	// Bold
	CHotkey EditBold(L"Bold", IDS_HOTKEY_EDIT_BOLD, FCONTROL, ID_EDIT_BOLD, U::StringToKeycode(L"B"));
	edit_hotkeys_group.m_hotkeys.push_back(EditBold);

	// Clone
	CHotkey EditClone(L"Clone", IDS_HOTKEY_EDIT_CLONE, FCONTROL, ID_EDIT_CLONE, VK_RETURN);
	edit_hotkeys_group.m_hotkeys.push_back(EditClone);

	// Copy
	CHotkey EditCopy(L"Copy", IDS_HOTKEY_EDIT_COPY, FCONTROL, ID_EDIT_COPY, U::StringToKeycode(L"C"));
	edit_hotkeys_group.m_hotkeys.push_back(EditCopy);

	// Cut
	CHotkey EditCut(L"Cut", IDS_HOTKEY_EDIT_CUT, FCONTROL, ID_EDIT_CUT, U::StringToKeycode(L"X"));
	edit_hotkeys_group.m_hotkeys.push_back(EditCut);

	// Find
	CHotkey EditFind(L"Find", IDS_HOTKEY_EDIT_FIND, FCONTROL, ID_EDIT_FIND, U::StringToKeycode(L"F"));
	edit_hotkeys_group.m_hotkeys.push_back(EditFind);

	// Find next
	CHotkey EditFindNext(L"FindNext", IDS_HOTKEY_EDIT_FIND_NEXT, NULL, ID_EDIT_FINDNEXT, VK_F3);
	edit_hotkeys_group.m_hotkeys.push_back(EditFindNext);

	// Incremental search
	CHotkey EditIncrementalSearch(L"IncrementalSearch",
									IDS_HOTKEY_EDIT_INCREMENTAL_SEARCH,
									FALT,
									ID_EDIT_INCSEARCH,
									U::StringToKeycode(L"I"));
	edit_hotkeys_group.m_hotkeys.push_back(EditIncrementalSearch);

	// Insert cite
	CHotkey EditInsertCite(L"InsertCite",
							IDS_HOTKEY_EDIT_INSERT_CITE,
							FALT,
							ID_EDIT_INS_CITE,
							U::StringToKeycode(L"C"));
	edit_hotkeys_group.m_hotkeys.push_back(EditInsertCite);

	// Insert image
	CHotkey EditInsertImage(L"InsertImage",
							IDS_HOTKEY_EDIT_INSERT_IMAGE,
							FCONTROL,
							ID_EDIT_INS_IMAGE,
							U::StringToKeycode(L"M"));
	edit_hotkeys_group.m_hotkeys.push_back(EditInsertImage);

	// Insert poem
	CHotkey EditInsertPoem(L"InsertPoem",
							IDS_HOTKEY_EDIT_INSERT_POEM,
							FCONTROL,
							ID_EDIT_INS_POEM,
							U::StringToKeycode(L"P"));
	edit_hotkeys_group.m_hotkeys.push_back(EditInsertPoem);

	// Italic
	CHotkey EditItalic(L"Italic", IDS_HOTKEY_EDIT_ITALIC, FCONTROL, ID_EDIT_ITALIC, U::StringToKeycode(L"I"));
	edit_hotkeys_group.m_hotkeys.push_back(EditItalic);

	// Merge
	CHotkey EditMerge(L"Merge", IDS_HOTKEY_EDIT_MERGE, FALT, ID_EDIT_MERGE, VK_DELETE);
	edit_hotkeys_group.m_hotkeys.push_back(EditMerge);

	// Paste
	CHotkey EditPaste(L"Paste", IDS_HOTKEY_EDIT_PASTE, FCONTROL, ID_EDIT_PASTE, U::StringToKeycode(L"V"));
	edit_hotkeys_group.m_hotkeys.push_back(EditPaste);

	// Redo
	CHotkey EditRedo(L"Redo", IDS_HOTKEY_EDIT_REDO, FCONTROL, ID_EDIT_REDO, U::StringToKeycode(L"Y"));
	edit_hotkeys_group.m_hotkeys.push_back(EditRedo);

	// Replace
	CHotkey EditReplace(L"Replace", IDS_HOTKEY_EDIT_REPLACE, FCONTROL, ID_EDIT_REPLACE, U::StringToKeycode(L"H"));
	edit_hotkeys_group.m_hotkeys.push_back(EditReplace);

	// Split
	CHotkey EditSplit(L"Split", IDS_HOTKEY_EDIT_SPLIT, FSHIFT, ID_EDIT_SPLIT, VK_RETURN);
	edit_hotkeys_group.m_hotkeys.push_back(EditSplit);

	// Undo
	CHotkey EditUndo(L"Undo", IDS_HOTKEY_EDIT_UNDO, FCONTROL, ID_EDIT_UNDO, U::StringToKeycode(L"Z"));
	edit_hotkeys_group.m_hotkeys.push_back(EditUndo);

	// Insert table
	CHotkey EditInsertTable(L"InsertTable",
							IDS_HOTKEY_EDIT_INSERT_TABLE,
							FALT,
							ID_INSERT_TABLE,
							U::StringToKeycode(L"T"));
	edit_hotkeys_group.m_hotkeys.push_back(EditInsertTable);

	//Navigation group hotkeys
	CHotkeysGroup navigation_hotkeys_group(L"Navigation", IDS_HOTKEY_GROUP_NAVIGATION);

	// Goto reference
	CHotkey NavigationGotoReference(L"GotoReference",
									IDS_HOTKEY_NAVIGATION_GOTO_REFERENCE,
									FCONTROL,
									ID_GOTO_REFERENCE,
									VK_BACK);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationGotoReference);

	// Next item
	CHotkey NavigationNextItem(L"NextItem",
									IDS_HOTKEY_NAVIGATION_NEXT_ITEM,
									FCONTROL,
									ID_NEXT_ITEM,
									VK_TAB);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationNextItem);

	// Collapse tree 1 level
	CHotkey NavigationCollapse1(L"Collapse1",
		IDS_HOTKEY_NAVIGATION_SCI_COLLAPSE1,
		NULL,
		ID_SCI_COLLAPSE1,
		NULL);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationCollapse1);

	// Collapse tree 2 levels
	CHotkey NavigationCollapse2(L"Collapse2",
		IDS_HOTKEY_NAVIGATION_SCI_COLLAPSE2,
		NULL,
		ID_SCI_COLLAPSE2,
		NULL);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationCollapse2);

	// Collapse tree 3 levels
	CHotkey NavigationCollapse3(L"Collapse3",
		IDS_HOTKEY_NAVIGATION_SCI_COLLAPSE3,
		NULL,
		ID_SCI_COLLAPSE3,
		NULL);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationCollapse3);

	// Collapse tree 4 levels
	CHotkey NavigationCollapse4(L"Collapse4",
		IDS_HOTKEY_NAVIGATION_SCI_COLLAPSE4,
		NULL,
		ID_SCI_COLLAPSE4,
		NULL);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationCollapse4);

	// Collapse tree 5 levels
	CHotkey NavigationCollapse5(L"Collapse5",
		IDS_HOTKEY_NAVIGATION_SCI_COLLAPSE5,
		NULL,
		ID_SCI_COLLAPSE5,
		NULL);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationCollapse5);

	// Collapse tree 6 levels
	CHotkey NavigationCollapse6(L"Collapse6",
		IDS_HOTKEY_NAVIGATION_SCI_COLLAPSE6,
		NULL,
		ID_SCI_COLLAPSE6,
		NULL);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationCollapse6);

	// Collapse tree 7 levels
	CHotkey NavigationCollapse7(L"Collapse7",
		IDS_HOTKEY_NAVIGATION_SCI_COLLAPSE7,
		NULL,
		ID_SCI_COLLAPSE7,
		NULL);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationCollapse7);

	// Collapse tree 8 levels
	CHotkey NavigationCollapse8(L"Collapse8",
		IDS_HOTKEY_NAVIGATION_SCI_COLLAPSE8,
		NULL,
		ID_SCI_COLLAPSE8,
		NULL);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationCollapse8);

	// Collapse tree 9 levels
	CHotkey NavigationCollapse9(L"Collapse9",
		IDS_HOTKEY_NAVIGATION_SCI_COLLAPSE9,
		NULL,
		ID_SCI_COLLAPSE9,
		NULL);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationCollapse9);

	// Expand tree 1 level
	CHotkey NavigationExpand1(L"Expand1",
		IDS_HOTKEY_NAVIGATION_SCI_EXPAND1,
		NULL,
		ID_SCI_EXPAND1,
		NULL);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationExpand1);

	// Expand tree 2 levels
	CHotkey NavigationExpand2(L"Expand2",
		IDS_HOTKEY_NAVIGATION_SCI_EXPAND2,
		NULL,
		ID_SCI_EXPAND2,
		NULL);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationExpand2);

	// Expand tree 3 levels
	CHotkey NavigationExpand3(L"Expand3",
		IDS_HOTKEY_NAVIGATION_SCI_EXPAND3,
		NULL,
		ID_SCI_EXPAND3,
		NULL);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationExpand3);

	// Expand tree 4 levels
	CHotkey NavigationExpand4(L"Expand4",
		IDS_HOTKEY_NAVIGATION_SCI_EXPAND4,
		NULL,
		ID_SCI_EXPAND4,
		NULL);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationExpand4);

	// Expand tree 5 levels
	CHotkey NavigationExpand5(L"Expand5",
		IDS_HOTKEY_NAVIGATION_SCI_EXPAND5,
		NULL,
		ID_SCI_EXPAND5,
		NULL);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationExpand5);

	// Expand tree 6 levels
	CHotkey NavigationExpand6(L"Expand6",
		IDS_HOTKEY_NAVIGATION_SCI_EXPAND6,
		NULL,
		ID_SCI_EXPAND6,
		NULL);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationExpand6);

	// Expand tree 7 levels
	CHotkey NavigationExpand7(L"Expand7",
		IDS_HOTKEY_NAVIGATION_SCI_EXPAND7,
		NULL,
		ID_SCI_EXPAND7,
		NULL);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationExpand7);

	// Expand tree 8 levels
	CHotkey NavigationExpand8(L"Expand8",
		IDS_HOTKEY_NAVIGATION_SCI_EXPAND8,
		NULL,
		ID_SCI_EXPAND8,
		NULL);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationExpand8);

	// Expand tree 9 levels
	CHotkey NavigationExpand9(L"Expand9",
		IDS_HOTKEY_NAVIGATION_SCI_EXPAND9,
		NULL,
		ID_SCI_EXPAND9,
		NULL);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationExpand9);

	// Select href
	CHotkey NavigationSelectHref(L"SelectHref",
		IDS_HOTKEY_NAVIGATION_SELECT_HREF,
		FALT,
		ID_SELECT_HREF,
		U::StringToKeycode(L"H"));
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationSelectHref);

	// Select ID
	CHotkey NavigationSelectID(L"SelectID",
		IDS_HOTKEY_NAVIGATION_SELECT_ID,
		NULL,
		ID_SELECT_ID,
		NULL);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationSelectID);

	// Select table ID
	CHotkey NavigationSelectTableID(L"SelectTableID",
		IDS_HOTKEY_NAVIGATION_SELECT_ID_TABLE,
		NULL,
		ID_SELECT_IDT,
		NULL);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationSelectTableID);

	// Select text
	CHotkey NavigationSelectText(L"SelectText",
		IDS_HOTKEY_NAVIGATION_SELECT_TEXT,
		NULL,
		ID_SELECT_TEXT,
		VK_ESCAPE);
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationSelectText);

	// Select tree
	CHotkey NavigationSelectTree(L"SelectTree",
		IDS_HOTKEY_NAVIGATION_SELECT_TREE,
		FALT,
		ID_SELECT_TREE,
		U::StringToKeycode(L"Q"));
	navigation_hotkeys_group.m_hotkeys.push_back(NavigationSelectTree);

	//Style group hotkeys
	CHotkeysGroup style_hotkeys_group(L"Style", IDS_HOTKEY_GROUP_STYLE);

	// Link
	CHotkey StyleLink(L"Link", IDS_HOTKEY_STYLE_LINK, FCONTROL, ID_STYLE_LINK, U::StringToKeycode(L"L"));
	style_hotkeys_group.m_hotkeys.push_back(StyleLink);

	// No link
	CHotkey StyleNoLink(L"NoLink", IDS_HOTKEY_STYLE_NO_LINK, FCONTROL, ID_STYLE_NOLINK, U::StringToKeycode(L"U"));
	style_hotkeys_group.m_hotkeys.push_back(StyleNoLink);

	// Normal
	CHotkey StyleNormal(L"Normal", IDS_HOTKEY_STYLE_NORMAL, FALT, ID_STYLE_NORMAL, U::StringToKeycode(L"N"));
	style_hotkeys_group.m_hotkeys.push_back(StyleNormal);

	// Note
	CHotkey StyleNote(L"Note", IDS_HOTKEY_STYLE_NOTE, FCONTROL, ID_STYLE_NOTE, U::StringToKeycode(L"W"));
	style_hotkeys_group.m_hotkeys.push_back(StyleNote);

	// Subtitle
	CHotkey StyleSubtitle(L"Subtitle", IDS_HOTKEY_STYLE_SUBTITLE, FALT, ID_STYLE_SUBTITLE, U::StringToKeycode(L"S"));
	style_hotkeys_group.m_hotkeys.push_back(StyleSubtitle);

	// Text author
	CHotkey StyleTextAuthor(L"TextAuthor",
								IDS_HOTKEY_STYLE_TEXT_AUTHOR,
								FALT,
								ID_STYLE_TEXTAUTHOR,
								U::StringToKeycode(L"A"));
	style_hotkeys_group.m_hotkeys.push_back(StyleTextAuthor);

	//View group hotkeys
	CHotkeysGroup view_hotkeys_group(L"View", IDS_HOTKEY_GROUP_VIEW);

	// View body
	CHotkey ViewBody(L"Body", IDS_HOTKEY_VIEW_BODY, FALT, ID_VIEW_BODY, VK_F2);
	view_hotkeys_group.m_hotkeys.push_back(ViewBody);

	// View description
	CHotkey ViewDescription(L"Description", IDS_HOTKEY_VIEW_DESCRIPTION, FALT, ID_VIEW_DESC, VK_F1);
	view_hotkeys_group.m_hotkeys.push_back(ViewDescription);

	// View source
	CHotkey ViewSource(L"Source", IDS_HOTKEY_VIEW_SOURCE, FALT, ID_VIEW_SOURCE, VK_F3);
	view_hotkeys_group.m_hotkeys.push_back(ViewSource);

	// Scripts group hotkeys
	CHotkeysGroup scripts_hotkeys_group(L"Scripts", IDS_HOTKEY_GROUP_SCRIPTS);

	// Last script
	CHotkey ScriptsLastScript(L"LastScript",
								IDS_HOTKEY_SCRIPTS_LAST_SCRIPT,
								FCONTROL,
								ID_LAST_SCRIPT,
								VK_OEM_3);
	scripts_hotkeys_group.m_hotkeys.push_back(ScriptsLastScript);

	// Symbols group hotkeys
	CHotkeysGroup symbols_hotkeys_group(L"Symbols", IDS_HOTKEY_GROUP_SYMBOLS);

	// Acute accent (´)
	CHotkey SymbolsAcuteAccent(L"AcuteAccent",
		IDS_HOTKEY_SYMBOL_ACUTE_ACCENT,
		FCONTROL,
		ID_EDIT_INS_SYMBOL + 1,
		U::StringToKeycode(L"1"));
	symbols_hotkeys_group.m_hotkeys.push_back(SymbolsAcuteAccent);

	// Quotation mark (")
	CHotkey SymbolsQuotationMark(L"QuotationMark",
		IDS_HOTKEY_SYMBOL_QUOTATION_MARK,
		FCONTROL,
		ID_EDIT_INS_SYMBOL + 2,
		U::StringToKeycode(L"2"));
	symbols_hotkeys_group.m_hotkeys.push_back(SymbolsQuotationMark);

	// Degree sign (°)
	CHotkey SymbolsDegreeSign(L"DegreeSign",
		IDS_HOTKEY_SYMBOL_DEGREE_SIGN,
		FCONTROL,
		ID_EDIT_INS_SYMBOL + 3,
		U::StringToKeycode(L"3"));
	symbols_hotkeys_group.m_hotkeys.push_back(SymbolsDegreeSign);

	// Superscript one (¹)
	CHotkey SymbolsSuperscriptOne(L"SuperscriptOne",
		IDS_HOTKEY_SYMBOL_SUPERSCRIPT_ONE,
		FCONTROL,
		ID_EDIT_INS_SYMBOL + 4,
		U::StringToKeycode(L"4"));
	symbols_hotkeys_group.m_hotkeys.push_back(SymbolsSuperscriptOne);

	// Superscript two (²)
	CHotkey SymbolsSuperscriptTwo(L"SuperscriptTwo",
		IDS_HOTKEY_SYMBOL_SUPERSCRIPT_TWO,
		FCONTROL,
		ID_EDIT_INS_SYMBOL + 5,
		U::StringToKeycode(L"5"));
	symbols_hotkeys_group.m_hotkeys.push_back(SymbolsSuperscriptTwo);

	// Superscript three (³)
	CHotkey SymbolsSuperscriptThree(L"SuperscriptThree",
		IDS_HOTKEY_SYMBOL_SUPERSCRIPT_THREE,
		FCONTROL,
		ID_EDIT_INS_SYMBOL + 6,
		U::StringToKeycode(L"6"));
	symbols_hotkeys_group.m_hotkeys.push_back(SymbolsSuperscriptThree);

	// Multiplication sign (×)
	CHotkey SymbolsMultiplicationSign(L"MultiplicationSign",
		IDS_HOTKEY_SYMBOL_MULTIPLICATION_SIGN,
		FCONTROL,
		ID_EDIT_INS_SYMBOL + 7,
		U::StringToKeycode(L"7"));
	symbols_hotkeys_group.m_hotkeys.push_back(SymbolsMultiplicationSign);

	// Division sign (÷)
	CHotkey SymbolsDivisionSign(L"DivisionSign",
		IDS_HOTKEY_SYMBOL_DIVISION_SIGN,
		FCONTROL,
		ID_EDIT_INS_SYMBOL + 8,
		U::StringToKeycode(L"8"));
	symbols_hotkeys_group.m_hotkeys.push_back(SymbolsDivisionSign);

	// Left single quotation mark (‘)
	CHotkey SymbolsLeftSingleQuotationMark(L"LeftSingleQuotationMark",
		IDS_HOTKEY_SYMBOL_LEFT_SINGLE_QUOTATION_MARK,
		FCONTROL,
		ID_EDIT_INS_SYMBOL + 9,
		VK_OEM_4); // [
	symbols_hotkeys_group.m_hotkeys.push_back(SymbolsLeftSingleQuotationMark);

	// Right single q uotation mark (’)
	CHotkey SymbolsRightSingleQuotationMark(L"RightSingleQuotationMark",
		IDS_HOTKEY_SYMBOL_RIGHT_SINGLE_QUOTATION_MARK,
		FCONTROL,
		ID_EDIT_INS_SYMBOL + 10,
		VK_OEM_6); // ]
	symbols_hotkeys_group.m_hotkeys.push_back(SymbolsRightSingleQuotationMark);

	// Left double quotation mark (“)
	CHotkey SymbolsLeftDoubleQuotationMark(L"LeftDoubleQuotationMark",
		IDS_HOTKEY_SYMBOL_LEFT_DOUBLE_QUOTATION_MARK,
		FCONTROL,
		ID_EDIT_INS_SYMBOL + 11,
		VK_OEM_1); // :
	symbols_hotkeys_group.m_hotkeys.push_back(SymbolsLeftDoubleQuotationMark);

	// Right double quotation mark (”)
	CHotkey SymbolsRightDoubleQuotationMark(L"RightDoubleQuotationMark",
		IDS_HOTKEY_SYMBOL_RIGHT_DOUBLE_QUOTATION_MARK,
		FCONTROL,
		ID_EDIT_INS_SYMBOL + 12,
		VK_OEM_7); // "
	symbols_hotkeys_group.m_hotkeys.push_back(SymbolsRightDoubleQuotationMark);

	// Left-pointing double quotation mark («)
	CHotkey SymbolsLeftPointingDoubleQuotationMark(L"LeftPointingDoubleQuotationMar",
		IDS_HOTKEY_SYMBOL_LEFT_POINTING_DOUBLE_QUOTATION_MARK,
		FCONTROL,
		ID_EDIT_INS_SYMBOL + 13,
		VK_OEM_COMMA); // <
	symbols_hotkeys_group.m_hotkeys.push_back(SymbolsLeftPointingDoubleQuotationMark);

	// Right-pointing double quotation mark (»)
	CHotkey SymbolsRightPointingDoubleQuotationMark(L"RightPointingDoubleQuotationMar",
		IDS_HOTKEY_SYMBOL_RIGHT_POINTING_DOUBLE_QUOTATION_MARK,
		FCONTROL,
		ID_EDIT_INS_SYMBOL + 14,
		VK_OEM_PERIOD); // >
	symbols_hotkeys_group.m_hotkeys.push_back(SymbolsRightPointingDoubleQuotationMark);

	// Horizontal ellipsis (…)
	CHotkey SymbolsHorizontalEllipsis(L"HorizontalEllipsis",
		IDS_HOTKEY_SYMBOL_HORIZONTAL_ELLIPSIS,
		FCONTROL,
		ID_EDIT_INS_SYMBOL + 15,
		VK_OEM_2); // ?
	symbols_hotkeys_group.m_hotkeys.push_back(SymbolsHorizontalEllipsis);

	// En dash (–)
	CHotkey SymbolsEnDash(L"EnDash",
		IDS_HOTKEY_SYMBOL_EN_DASH,
		FCONTROL,
		ID_EDIT_INS_SYMBOL + 16,
		U::StringToKeycode(L"0"));
	symbols_hotkeys_group.m_hotkeys.push_back(SymbolsEnDash);

	// Em dash (—)
	CHotkey SymbolsEmDash(L"EmDash",
		IDS_HOTKEY_SYMBOL_EM_DASH,
		FCONTROL,
		ID_EDIT_INS_SYMBOL + 17,
		VK_OEM_MINUS); // -
	symbols_hotkeys_group.m_hotkeys.push_back(SymbolsEmDash);

	// Collect all hotkey groups and sort
	m_hotkey_groups.push_back(file_hotkeys_group);
	m_hotkey_groups.push_back(edit_hotkeys_group);
	m_hotkey_groups.push_back(navigation_hotkeys_group);
	m_hotkey_groups.push_back(style_hotkeys_group);
	m_hotkey_groups.push_back(view_hotkeys_group);
	m_hotkey_groups.push_back(scripts_hotkeys_group);
	m_hotkey_groups.push_back(symbols_hotkeys_group);

	std::sort(m_hotkey_groups.begin(), m_hotkey_groups.end());

	m_key_hotkeys_path = m_key_path + L"\\" + HOTKEYS_BRANCH;
	m_key_hotkeys.Create(HKEY_CURRENT_USER, m_key_hotkeys_path);
}

//void CSettings::InitScriptsHotkeys(CSimpleArray<CMainFrame::ScrInfo>& scripts)
//{
//	
//}

void CSettings::Close()
{
	m_key.Close();
	m_key_hotkeys.Close();
}

void CSettings::Load()
{
	m_keep_encoding			= U::QueryIV(m_key, KEEP_ENCODING_KEY, true) != 0;
	m_default_encoding		= U::QuerySV(m_key, DEFAULT_ENCODING_KEY, DEFAULT_ENCODING);
	m_search_options		= U::QueryIV(m_key, SEARCH_OPTIONS_KEY, true);
	m_collorBG				= U::QueryIV(m_key, COLOR_BG_KEY, CLR_DEFAULT);
	m_collorFG				= U::QueryIV(m_key, COLOR_FG_KEY, CLR_DEFAULT);
	m_font_size				= U::QueryIV(m_key, FONT_SIZE_KEY, 12);
	m_xml_src_wrap			= U::QueryIV(m_key, XML_SRC_WRAP_KEY, true) != 0;
	m_xml_src_syntaxHL		= U::QueryIV(m_key, XML_SRC_SYNTAX_HL_KEY, true) != 0;
	m_xml_src_showEOL		= U::QueryIV(m_key, XML_SRC_SHOW_EOL_KEY, false) != 0;	
	m_fast_mode				= U::QueryIV(m_key, FAST_MODE_KEY, false) != 0;	
	m_font					= U::QuerySV(m_key, FONT_KEY, DEFAULT_FONT);	
	m_view_status_bar		= U::QueryIV(m_key, VIEW_STATUS_BAR_KEY, true) != 0;	
	m_view_doc_tree			= U::QueryIV(m_key, VIEW_DOCUMENT_TREE_KEY, true) != 0;	
	m_splitter_pos			= U::QueryIV(m_key, SPLITTER_POS_KEY, 200);	
	m_toolbars_settings		= U::QuerySV(m_key, TOOLBARS_SETTINGS_KEY, L"");	
	m_restore_file_position	= U::QueryIV(m_key, RESTORE_FILE_POS_KEY, false) != 0;		
	m_interface_lang_id		= U::QueryIV(m_key, INTERFACE_LANG_KEY, PRIMARYLANGID(GetUserDefaultLangID()));
	m_scripts_folder        = U::QuerySV(m_key, SCRIPTS_FOLDER_KEY, GetDefaultScriptsFolder());
	m_insimage_ask			= U::QueryIV(m_key, INSIMAGE_ASKING, true) != 0;
	m_script_hotkey_err		= U::QueryIV(m_key, SCRIPTS_HKEY_ERR_NTF, true) != 0;
	m_ins_clear_image		= U::QueryIV(m_key, INS_CLEAR_IMAGE, false) != 0;
}

void CSettings::LoadHotkeyGroups()
{
	for(unsigned int i = 0; i < m_hotkey_groups.size(); ++i)
	{
		CString group_path = m_key_hotkeys_path + L"\\" + m_hotkey_groups.at(i).m_reg_name;
		CRegKey group_key;
		group_key.Create(HKEY_CURRENT_USER, group_path);

		for(unsigned int j = 0; j < m_hotkey_groups.at(i).m_hotkeys.size(); ++j)
		{
			CString hotkey_path = m_hotkey_groups.at(i).m_hotkeys.at(j).m_reg_name;
			m_hotkey_groups.at(i).m_hotkeys.at(j).m_accel = 
				U::QueryBV<ACCEL>(group_key, hotkey_path, m_hotkey_groups.at(i).m_hotkeys.at(j).m_def_accel);
			ACCEL accel = m_hotkey_groups.at(i).m_hotkeys.at(j).m_accel;

			if(accel.fVirt != NULL && accel.key != NULL && accel.cmd != NULL)
				keycodes++;

			if(m_hotkey_groups.at(i).m_reg_name == L"Scripts")
			{
				ACCEL def_accel = m_hotkey_groups.at(i).m_hotkeys.at(j).m_def_accel;
				if(accel.fVirt != def_accel.fVirt || accel.key != def_accel.key)
				{
					m_hotkey_groups.at(i).m_hotkeys.at(j).m_def_accel.fVirt = m_hotkey_groups.at(i).m_hotkeys.at(j).m_accel.fVirt;
					m_hotkey_groups.at(i).m_hotkeys.at(j).m_def_accel.key = m_hotkey_groups.at(i).m_hotkeys.at(j).m_accel.key;
					m_hotkey_groups.at(i).m_hotkeys.at(j).m_accel.cmd = m_hotkey_groups.at(i).m_hotkeys.at(j).m_def_accel.cmd;
				}
			}
		}

		group_key.Close();
	}
}

void CSettings::Save()
{
	m_key.SetDWORDValue(KEEP_ENCODING_KEY, m_keep_encoding);
	m_key.SetStringValue(DEFAULT_ENCODING_KEY, m_default_encoding);	
	m_key.SetDWORDValue(SEARCH_OPTIONS_KEY, m_search_options);
	m_key.SetDWORDValue(COLOR_BG_KEY, m_collorBG);	
	m_key.SetDWORDValue(COLOR_FG_KEY, m_collorFG);	
	m_key.SetDWORDValue(FONT_SIZE_KEY, m_font_size);	
	m_key.SetDWORDValue(XML_SRC_WRAP_KEY, m_xml_src_wrap);	
	m_key.SetDWORDValue(XML_SRC_SYNTAX_HL_KEY, m_xml_src_syntaxHL);	
	m_key.SetDWORDValue(XML_SRC_SHOW_EOL_KEY, m_xml_src_showEOL);	
	m_key.SetDWORDValue(FAST_MODE_KEY, m_fast_mode);	
	m_key.SetStringValue(FONT_KEY, m_font);
	m_key.SetDWORDValue(VIEW_STATUS_BAR_KEY, m_view_status_bar);
	m_key.SetDWORDValue(VIEW_DOCUMENT_TREE_KEY, m_view_doc_tree);
	m_key.SetDWORDValue(SPLITTER_POS_KEY, m_splitter_pos);
	m_key.SetStringValue(TOOLBARS_SETTINGS_KEY, m_toolbars_settings);		
	m_key.SetDWORDValue(RESTORE_FILE_POS_KEY, m_restore_file_position);
	m_key.SetDWORDValue(INTERFACE_LANG_KEY, m_interface_lang_id);
	m_key.SetStringValue(SCRIPTS_FOLDER_KEY, m_scripts_folder);
	m_key.SetDWORDValue(INSIMAGE_ASKING, m_insimage_ask);
	m_key.SetDWORDValue(SCRIPTS_HKEY_ERR_NTF, m_script_hotkey_err);
	m_key.SetDWORDValue(INS_CLEAR_IMAGE, m_ins_clear_image);
}

void CSettings::SaveHotkeyGroups()
{
	for(unsigned int i = 0; i < m_hotkey_groups.size(); ++i)
	{
		CString group_path = m_key_hotkeys_path + L"\\" + m_hotkey_groups.at(i).m_reg_name;
		CRegKey group_key;
		group_key.Create(HKEY_CURRENT_USER, group_path);

		for(unsigned int j = 0; j < m_hotkey_groups.at(i).m_hotkeys.size(); ++j)
		{
			CString name = m_hotkey_groups.at(i).m_hotkeys.at(j).m_reg_name;
			BYTE* accel = new BYTE[sizeof(ACCEL)];
			CopyMemory(accel, &m_hotkey_groups.at(i).m_hotkeys.at(j).m_accel, sizeof(ACCEL));

			ACCEL new_accel = m_hotkey_groups.at(i).m_hotkeys.at(j).m_accel;
			ACCEL def_accel = m_hotkey_groups.at(i).m_hotkeys.at(j).m_def_accel;

			if(new_accel.fVirt != def_accel.fVirt || new_accel.key != def_accel.key || new_accel.cmd != def_accel.cmd)
				group_key.SetBinaryValue(name, accel, sizeof(ACCEL));
			else
			{
				CopyMemory(accel, &def_accel, sizeof(ACCEL));
				group_key.SetBinaryValue(name, accel, sizeof(ACCEL));
			}
			delete[] accel;
		}

		group_key.Close();
	}
}

bool CSettings::KeepEncoding()const
{
	return m_keep_encoding;
}
bool CSettings::XmlSrcWrap()const
{
	return m_xml_src_wrap;
}
bool CSettings::XmlSrcSyntaxHL()const
{
	return m_xml_src_syntaxHL;
}
bool CSettings::XmlSrcShowEOL()const
{
	return m_xml_src_showEOL;
}
bool CSettings::FastMode()const
{
	return m_fast_mode;
}
bool CSettings::ViewStatusBar()const
{
	return m_view_status_bar;
}
bool CSettings::ViewDocumentTree()const
{
	return m_view_doc_tree;
}
bool CSettings::RestoreFilePosition()const
{
	return m_restore_file_position;
}
bool CSettings::NeedRestart()const
{
	return m_need_restart;
}


DWORD CSettings::GetSearchOptions()const
{
	return m_search_options;
}
DWORD CSettings::GetFontSize()const
{
	return m_font_size;
}
CString CSettings::GetFont()const
{
	return m_font;
}
DWORD CSettings::GetSplitterPos()const
{
	return m_splitter_pos;
}
CString CSettings::GetToolbarsSettings()const
{
	return m_toolbars_settings;
}
CString CSettings::GetKeyPath()const
{
	return m_key_path;
}
const CRegKey& CSettings::GetKey()const
{
	return m_key;
}
bool CSettings::GetExtElementStyle(const CString& elem)const
{
	return U::QueryIV(m_key, elem, true) != 0;	
}
bool CSettings::GetWindowPosition(WINDOWPLACEMENT &wpl)const
{	
	DWORD		    sz=sizeof(wpl);
	DWORD		    type=REG_BINARY;
	return (::RegQueryValueEx(m_key, WINDOW_POS_KEY,0,&type,(BYTE*)&wpl,&sz)==ERROR_SUCCESS &&
		type==REG_BINARY && sz==sizeof(wpl) && wpl.length==sizeof(wpl));
}
CString CSettings::GetDefaultEncoding()const
{
	return m_default_encoding;
}
DWORD CSettings::GetColorBG()const
{
	return m_collorBG;	
}
DWORD CSettings::GetColorFG()const
{
	return m_collorFG;	
}

DWORD CSettings::GetInterfaceLanguageID()const
{
	return m_interface_lang_id;
}

CString CSettings::GetInterfaceLanguageDllName()const
{
	switch(m_interface_lang_id)
	{
	case LANG_RUSSIAN:
		return L"res_rus.dll";
	default:
		return L"";
	}
}
CString CSettings::GetLocalizedGenresFileName()const
{
	switch(m_interface_lang_id)
	{
	case LANG_RUSSIAN:
		return L"genres.rus.txt";
	default:
		return L"genres.txt";
	}
}

CString CSettings::GetInterfaceLanguageName()const
{
	switch(m_interface_lang_id)
	{
	case LANG_RUSSIAN:
		return L"russian";
	default:
		return L"english";
	}
}

CString CSettings::GetScriptsFolder() const
{
	return m_scripts_folder;
}

CString CSettings::GetDefaultScriptsFolder()
{
  TCHAR filepath[MAX_PATH];
  DWORD pathlen = ::GetModuleFileName(_Module.GetModuleInstance(), filepath, MAX_PATH);
  CString tmp = U::GetFullPathName(filepath);
  int pos = tmp.ReverseFind(_T('\\'));
  if (pos >= 0)
  {
	tmp.Delete(pos, tmp.GetLength() - pos);
	tmp.Append(L"\\");
	tmp.Append(DEFAULT_SCRIPTS_FOLDER);
	tmp.Append(L"\\");
  }

  tmp.MakeLower();

  return tmp;
}

bool CSettings::IsDefaultScriptsFolder()
{
	return GetScriptsFolder() == GetDefaultScriptsFolder();
}

bool CSettings::GetInsImageAsking() const
{
	return m_insimage_ask;
}

bool CSettings::GetScriptsHkErrNotify() const
{
	return m_script_hotkey_err;
}

bool CSettings::GetIsInsClearImage() const
{
	return m_ins_clear_image;
}

bool CSettings::GetDocTreeItemState(const ATL::CString &item, bool default_state)
{
	return U::QueryIV(m_key, item, default_state) != 0;	
}

void CSettings::SetKeepEncoding(bool keep, bool apply)
{
	m_keep_encoding = keep;
	if(apply)
		m_key.SetDWORDValue(KEEP_ENCODING_KEY, m_keep_encoding);		
}
void CSettings::SetSearchOptions(DWORD opt, bool apply)
{
	m_search_options = opt;
	if(apply)
		m_key.SetDWORDValue(SEARCH_OPTIONS_KEY, m_search_options);
}
void CSettings::SetFontSize(DWORD size, bool apply)
{
	m_font_size = size;
	if(apply)
		m_key.SetDWORDValue(FONT_SIZE_KEY, m_font_size);	
}
void CSettings::SetXmlSrcWrap(bool wrap, bool apply)
{
	m_xml_src_wrap = wrap;
	if(apply)
		m_key.SetDWORDValue(XML_SRC_WRAP_KEY, m_xml_src_wrap);		
}
void CSettings::SetXmlSrcSyntaxHL(bool hl, bool apply)
{
	m_xml_src_syntaxHL = hl;
	if(apply)
		m_key.SetDWORDValue(XML_SRC_SYNTAX_HL_KEY, m_xml_src_syntaxHL);		
}
void CSettings::SetXmlSrcShowEOL(bool eol, bool apply)
{
	m_xml_src_showEOL = eol;
	if(apply)
		m_key.SetDWORDValue(XML_SRC_SHOW_EOL_KEY, m_xml_src_showEOL);		
}
void CSettings::SetFastMode(bool mode,  bool apply)
{
	m_fast_mode = mode;
	if(apply)
		m_key.SetDWORDValue(FAST_MODE_KEY, m_fast_mode);
}
void CSettings::SetFont(const CString& font, bool apply)
{
	m_font = font;
	if(apply)
		m_key.SetStringValue(FONT_KEY, m_font);	
}
void CSettings::SetViewStatusBar(bool view, bool apply)
{
	m_view_status_bar = view;
	if(apply)
		m_key.SetDWORDValue(VIEW_STATUS_BAR_KEY, m_view_status_bar);	
}
void CSettings::SetViewDocumentTree(bool view, bool apply)
{
	m_view_doc_tree = view;
	if(apply)
		m_key.SetDWORDValue(VIEW_DOCUMENT_TREE_KEY, m_view_doc_tree);	
}
void CSettings::SetSplitterPos(DWORD pos, bool apply)
{
	m_splitter_pos = pos;
	if(apply)
		m_key.SetDWORDValue(SPLITTER_POS_KEY, m_splitter_pos);	
}
void CSettings::SetToolbarsSettings(CString& settings, bool apply)
{
	m_toolbars_settings = settings;
	if(apply)
		m_key.SetStringValue(TOOLBARS_SETTINGS_KEY, m_toolbars_settings);	
}
void CSettings::SetExtElementStyle(const CString& elem, bool ext, bool apply)
{
	m_key.SetDWORDValue(elem, ext);	
}
void CSettings::SetWindowPosition(const WINDOWPLACEMENT &wpl, bool apply)
{
	m_key.SetBinaryValue(WINDOW_POS_KEY, (BYTE*)&wpl, sizeof(wpl));	
}
void CSettings::SetDefaultEncoding(const CString &enc, bool apply)
{
	m_default_encoding = enc;
	if(apply)
		m_key.SetStringValue(DEFAULT_ENCODING_KEY, m_default_encoding);	
}
void CSettings::SetColorBG(DWORD col, bool apply)
{
	m_collorBG = col;
	if(apply)
		m_key.SetDWORDValue(COLOR_BG_KEY, m_collorBG);	
}
void CSettings::SetColorFG(DWORD col, bool apply)
{
	m_collorFG = col;
	if(apply)
		m_key.SetDWORDValue(COLOR_FG_KEY, m_collorFG);					
}
void CSettings::SetRestoreFilePosition(bool restore, bool apply)
{
	m_restore_file_position = restore;
	if(apply)
		m_key.SetDWORDValue(RESTORE_FILE_POS_KEY, m_restore_file_position);
}
void CSettings::SetInterfaceLanguage(DWORD lang_id, bool apply)
{
	if(m_interface_lang_id != lang_id)
	{
		m_interface_lang_id = lang_id;
		if(apply)
			m_key.SetDWORDValue(INTERFACE_LANG_KEY, m_interface_lang_id);
	}
}

void CSettings::SetScriptsFolder(const CString fullpath, bool apply)
{
	if(apply)
	{
		if(m_scripts_folder != fullpath)
		{
			m_scripts_folder = fullpath;
		}
	}
}

void CSettings::SetInsImageAsking(bool ask, bool apply)
{
	m_insimage_ask = ask;
	if(apply)
		m_key.SetDWORDValue(INSIMAGE_ASKING, m_insimage_ask);	
}

void CSettings::SetScriptsHkErrNotify(bool notify, bool apply)
{
	m_script_hotkey_err = notify;
	if(apply)
		m_key.SetDWORDValue(SCRIPTS_HKEY_ERR_NTF, m_script_hotkey_err);	
}

void CSettings::SetIsInsClearImage(bool clear, bool apply)
{
	m_ins_clear_image = clear;
	if(apply)
		m_key.SetDWORDValue(INS_CLEAR_IMAGE, m_ins_clear_image);	
}

void CSettings::SetNeedRestart()
{
	m_need_restart = true;
}

void CSettings::SetDocTreeItemState(const ATL::CString &item, bool state)
{
	m_key.SetDWORDValue(item, state);	
}
