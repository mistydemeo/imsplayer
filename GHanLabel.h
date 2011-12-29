//// GHanLabel.h /////////////////////////////////////////////////////////////////////////////
//
//  Korea Johab Hangul Label Class
//  This class will allow you to use images files as buttons in wxWidgets.
//  It supports the basic button image, mouse over image, and click image.
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GHANLABEL_H
#define GHANLABEL_H

#include <wx/panel.h>
#include <wx/dcbuffer.h> 
#include <wx/msgdlg.h>
#include <wx/image.h>

/////////////////////////////////////////
class GHanLabelEvent: public wxCommandEvent 
{ 
	public: 
		GHanLabelEvent(wxEventType commandType = wxEVT_NULL, int id = 0 ); 

		virtual wxEvent *Clone() const { return new GHanLabelEvent(*this); } 

	private: 
	DECLARE_DYNAMIC_CLASS(GHanLabelEvent) 
}; 

BEGIN_DECLARE_EVENT_TYPES() 
	DECLARE_EVENT_TYPE(wxEVT_COMMAND_GHANLABEL, -1) 
END_DECLARE_EVENT_TYPES() 

typedef void (wxEvtHandler::*wxGHanLabelEventFunction)(GHanLabelEvent&); 

#define EVT_GHANLABEL(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY(wxEVT_COMMAND_GHANLABEL, id, -1,  \
	(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)  \
	wxStaticCastEvent(wxGHanLabelEventFunction, & fn), (wxObject *) NULL ),

/////////////////////////////
class GHanLabel: public wxPanel
{ 
	public: 
		GHanLabel(wxWindow* parent, wxWindowID id, const wxPoint& pos, 
			const wxSize& size, char *text, 
			const wxColour &fg, const wxColour &bg);

		// Destructor 
		virtual ~GHanLabel(); 

		void InitHangul();

		// Creators
		bool Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size);
		void SetLabel(char *text);
		void SetRangeColour(int from, int to, wxColour fg, wxColour bg);

		bool focused;
	
	private: 
		void DrawHangul(wxDC &dc); 
		void OnEraseBackground(wxEraseEvent& event); 
		void OnPaint(wxPaintEvent &event); 
		void OnMouse(wxMouseEvent &event); 

		bool allowHover;
		bool allowClick;
		bool allowFocused;

		wxBitmap c_button;
		wxBitmap c_hover;
		wxBitmap c_click;
		wxBitmap c_focusedButton;
		wxBitmap c_focusedHover;
		wxBitmap c_focusedClick;

		bool hover;
		bool click;

		Hangul *m_han;
		char *m_text;
		wxColour m_bg;
		wxColour m_fg;

		DECLARE_EVENT_TABLE() 
};

#endif //GHANLABEL_H
