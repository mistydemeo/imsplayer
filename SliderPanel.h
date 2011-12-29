#ifndef _SLIDERPANEL_H_
#define _SLIDERPANEL_H_

class MainFrame;
class GSlider;

class SliderPanel : public wxPanel
{
public:
	SliderPanel(wxWindow* parent, MainFrame *myFrame, wxWindowID id,	
			const wxPoint& pos, const wxSize& size, 
			long style = wxBORDER_RAISED); 
    ~SliderPanel();

public:
	MainFrame *myFrame;
	wxPanel *m_panel;

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
	void DrawSlider(wxPaintDC &dc);
	void OnPaint(wxPaintEvent &WXUNUSED(event)) ;

	DECLARE_EVENT_TABLE()
};

#endif
