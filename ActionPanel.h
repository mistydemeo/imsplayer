#ifndef _BUTTONPANEL_H_
#define _BUTTONPANEL_H_

class MainFrame;
class GButton;
class GSlider;

class ButtonPanel : public wxPanel
{
public:
	ButtonPanel(wxWindow* parent, MainFrame *myFrame, wxWindowID id,	
			const wxPoint& pos, const wxSize& size, 
			long style = wxBORDER_RAISED); 
    ~ButtonPanel();

public:
	MainFrame *myFrame;
	wxPanel *m_panel;

	GButton *m_previousButton;
	GButton *m_stopButton;
	GButton *m_playButton;
	GButton *m_nextButton;
	GButton *m_browseButton;
	GSlider *m_slider;
	
	void SetValue(float value);
	void SetRange(float min, float max);
	float GetValue();
	
private:
	void OnMotion(wxMouseEvent& e);
	void OnIdle(wxIdleEvent& event);
	void OnClose(wxCloseEvent& WXUNUSED(event));
	void OnSize(wxSizeEvent& event);
	void OnStop(wxCommandEvent& WXUNUSED(event));
	void OnPlay(wxCommandEvent& WXUNUSED(event));
	void OnNext(wxCommandEvent& WXUNUSED(event));
	void OnPrev(wxCommandEvent& WXUNUSED(event));
	void OnOpenFiles(wxCommandEvent& WXUNUSED(event));
	void DrawButton(wxPaintDC &dc);
	void OnPaint(wxPaintEvent &WXUNUSED(event)) ;
	void OnChange(wxCommandEvent& WXUNUSED(event));
	void OnChangeComplete(wxMouseEvent& WXUNUSED(event));

	DECLARE_EVENT_TABLE()
};

#endif
