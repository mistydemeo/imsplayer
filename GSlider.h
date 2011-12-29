//// GSlider.h /////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GSLIDER_H
#define GSLIDER_H

#include <wx/window.h>
#include <wx/panel.h>
#include <wx/dcbuffer.h> 
#include <wx/msgdlg.h>
#include <wx/image.h>
#include <vector>

#define FDR_NO_MOUSE_CAPTURE 0x0001 

//Device class to send to callback
///////////////////////////////////
typedef struct{
	wxBitmap slider;
	wxBitmap focused;
	bool hover;
	bool click;
	bool knobFocused;
	int knobPosX;
	int knobPosY; 
	int knobWidth; 
	int knobHeight;
	float currentValue;
	float defaultValue;
	int mouseOffsetX;
}Knobs;

/////////////////////////////////////////
class GSliderEvent: public wxCommandEvent 
{ 

public: 
	GSliderEvent(wxEventType commandType = wxEVT_NULL, int id = 0 ); 

	float GetValue() const { return m_faderValue; } 
	void SetValue(float value) { m_faderValue = value; } 
	int GetKnobNumber() const { return knobNumber; } 
	void SetKnobNumber(int value) { knobNumber = value; } 

	virtual wxEvent *Clone() const { return new GSliderEvent(*this); } 

private: 
	float m_faderValue;
	int knobNumber;

	DECLARE_DYNAMIC_CLASS(GSliderEvent) 
}; 


BEGIN_DECLARE_EVENT_TYPES() 
	DECLARE_EVENT_TYPE(wxEVT_COMMAND_GSLIDER, -1) 
	DECLARE_EVENT_TYPE(wxEVT_LEFT_UP_GSLIDER, -1) 
END_DECLARE_EVENT_TYPES() 

typedef void (wxEvtHandler::*wxGSliderEventFunction)(GSliderEvent&); 

#define EVT_GSLIDER(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY(wxEVT_COMMAND_GSLIDER, id, -1,  \
	(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)  \
	wxStaticCastEvent(wxGSliderEventFunction, & fn), (wxObject *) NULL ),
#define EVT_LEFT_UP_GSLIDER(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY(wxEVT_LEFT_UP_GSLIDER, id, -1,  \
	(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)  \
	wxStaticCastEvent(wxGSliderEventFunction, & fn), (wxObject *) NULL ),


/////////////////////////////
class GSlider: public wxPanel
{ 
	public: 
		GSlider(); 
		GSlider(wxWindow* parent,
			wxWindowID id,// = wxID_ANY, 
			const wxPoint& pos,// = wxDefaultPosition, 
			long style,// = 0, 
			const wxString& name,// = wxPanelNameStr,
			const wxBitmap& back,
			const wxBitmap& backFocused,
			const wxBitmap& slider,
			const wxBitmap& hover,
			const wxBitmap& click,
			const wxBitmap& focused); 

		GSlider(wxWindow* parent,
			wxWindowID id,// = wxID_ANY, 
			const wxPoint& pos,// = wxDefaultPosition, 
			long style,// = 0, 
			const wxString& name,// = wxPanelNameStr,
			const wxString& back,
			const wxString& backFocused,
			const wxString& slider,
			const wxString& hover,
			const wxString& click,
			const wxString& focused);

		GSlider(wxWindow* parent,
			wxWindowID id,// = wxID_ANY, 
			const wxPoint& pos,// = wxDefaultPosition, 
			long style,// = 0, 
			const wxString& name,// = wxPanelNameStr,
			const char** back,
			const char** backFocused,
			const char** slider,
			const char** hover,
			const char** click,
			const char** focused);

		virtual ~GSlider(); 

		bool Create(wxWindow* parent,// = (wxWindow*) NULL, 
			wxWindowID id,// = wxID_ANY, 
			const wxPoint& pos,// = wxDefaultPosition, 
			long style,// = 0, 
			const wxString& name,// = wxPanelNameStr
			const wxBitmap& back,
			const wxBitmap& backFocused,
			const wxBitmap& slider,
			const wxBitmap& hover,
			const wxBitmap& click,
			const wxBitmap& focused);

		bool Create(wxWindow* parent,// = (wxWindow*) NULL, 
			wxWindowID id,// = wxID_ANY, 
			const wxPoint& pos,// = wxDefaultPosition, 
			long style,// = 0, 
			const wxString& name,// = wxPanelNameStr
			const wxString& back,
			const wxString& backFocused,
			const wxString& slider,
			const wxString& hover,
			const wxString& click,
			const wxString& focused); 

		bool Create(wxWindow* parent,// = (wxWindow*) NULL, 
			wxWindowID id,// = wxID_ANY, 
			const wxPoint& pos,// = wxDefaultPosition, 
			long style,// = 0, 
			const wxString& name,// = wxPanelNameStr
			const char** back,
			const char** backFocused,
			const char** slider,
			const char** hover,
			const char** click,
			const char** focused); 

		//virtual float GetValue() const; 	
		//virtual void SetValue(float value); 
		virtual void SetRange(float minValue, float maxValue); 
		virtual float GetMin() const; 
		virtual float GetMax() const; 
		virtual float GetValue(int num) const;
		virtual void SetValue(int num, float value); 
		virtual void SetDefault(int num, float value);
		void CalcGeometry(int num);
		void AddKnob();
		void DeleteKnob();


		int GetWheelStep() const; 
		void SetWheelStep(int step); 

		bool backFocused;

	protected: 

		void Init(); 
		float NormalizeValue(float value) const; 
		bool UseMouseCapture(); 
		wxCoord ValueToHorizPosition(float value); 
		float ValueFromVertPosition(wxCoord position); 
		void DrawBackground(wxDC &dc); 
		void DrawFaders(wxDC &dc); 
		wxString FormatValue(float value) const; 

		void OnSize(wxSizeEvent &event); 
		void OnEraseBackground(wxEraseEvent &event);
		void OnPaint(wxPaintEvent &event); 
		void OnKillFocus(wxFocusEvent &event);
		void OnMouse(wxMouseEvent &event); 
		void OnKeyDown(wxKeyEvent &event);

	private: 
		// Track size 
		int m_trackPosX, 
			m_trackPosY, 
			m_trackWidth, 
			m_trackHeight; 

		// Knob size 
		int m_knobPosX, 
			m_knobPosY, 
			m_knobWidth, 
			m_knobHeight; 

		int m_tickPosY, 
			m_tickPosX, 
			m_tickWidth, 
			m_tickHeight; 

		// Mouse logic 
		//int m_mouseOffsetY;
		bool m_draggingFader;
		int activeKnob;

		// value 
		float m_minValue;
		float m_maxValue; 
		float m_currentValue; 

		wxBitmap c_slider;
		wxBitmap c_back;
		wxBitmap c_backFocused;
		wxBitmap c_hover;
		wxBitmap c_click;
		wxBitmap c_focused;

		bool hover;
		bool click;
		bool knobFocused;

		/// Multiple Knobs ////////
		std::vector<Knobs> knobs;

		public:
		int knobCount;
		///////////////////////////

		DECLARE_EVENT_TABLE() 
};

#endif //GSLIDER_H
