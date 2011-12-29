//// GButton.h /////////////////////////////////////////////////////////////////////////////
//
//  Graphic Button Class
//  This class will allow you to use images files as buttons in wxWidgets.
//  It supports the basic button image, mouse over image, and click image.
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GBUTTON_H
#define GBUTTON_H

#include <wx/panel.h>
#include <wx/dcbuffer.h> 
#include <wx/msgdlg.h>
#include <wx/image.h>

/////////////////////////////////////////
class GButtonEvent: public wxCommandEvent 
{ 
	public: 
		GButtonEvent(wxEventType commandType = wxEVT_NULL, int id = 0 ); 

		virtual wxEvent *Clone() const { return new GButtonEvent(*this); } 

	private: 
	DECLARE_DYNAMIC_CLASS(GButtonEvent) 
}; 

BEGIN_DECLARE_EVENT_TYPES() 
	DECLARE_EVENT_TYPE(wxEVT_COMMAND_GBUTTON, -1) 
END_DECLARE_EVENT_TYPES() 

typedef void (wxEvtHandler::*wxGButtonEventFunction)(GButtonEvent&); 

#define EVT_GBUTTON(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY(wxEVT_COMMAND_GBUTTON, id, -1,  \
	(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)  \
	wxStaticCastEvent(wxGButtonEventFunction, & fn), (wxObject *) NULL ),


/////////////////////////////
class GButton: public wxPanel
{ 
	public: 
		GButton(); 

		GButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxBitmap& button);
		GButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxString& button);
		GButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const char** button);
		GButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxBitmap& button,
			    const wxBitmap& hover); 
		GButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxString& button,
			    const wxString& hover); 
		GButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const char** button,
			    const char** hover); 
		GButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxBitmap& button,
			    const wxBitmap& hover, const wxBitmap& click); 
		GButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxString& button,
			    const wxString& hover, const wxString& click); 
		GButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const char** button,
			    const char** hover, const char** click); 
		GButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const char** button,
			    const char** hover, const char** click, const char** focusedButton,
			    const char** focusedHover, const char** focusedClick); 

		// Destructor 
		virtual ~GButton(); 

		// Creators
		bool Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxBitmap& button); 
		bool Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxString& button);
		bool Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const char** button);
		bool Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxBitmap& button,
			        const wxBitmap& hover); 
		bool Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxString& button, 
			        const wxString& hover);
		bool Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const char** button, 
			        const char** hover);
		bool Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxBitmap& button,
			        const wxBitmap& hover, const wxBitmap& click); 
		bool Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxString& button, 
			        const wxString& hover, const wxString& click);
		bool Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const char** button, 
			        const char** hover, const char** click); 
		bool Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const char** button, 
			        const char** hover, const char** click, const char** focusedButton,
					const char** focusedHover, const char** focusedClick); 

		bool focused;

	void SetBitmap(wxBitmap button, wxBitmap hover);
	
	private: 
		void DrawBackground(wxDC &dc); 
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

		DECLARE_EVENT_TABLE() 
};

#endif //GBUTTON_H
