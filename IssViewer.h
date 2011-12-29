#ifndef _ISSVIEWER_H_
#define _ISSVIEWER_H_

class MainFrame;
class Hangul;
class GHanLabel;

class IssViewer : public wxPanel
{
public:
	IssViewer(wxWindow* parent, MainFrame *myFrame, wxWindowID id,	
			const wxPoint& pos, const wxSize& size, 
			long style = wxNO_BORDER); 
    ~IssViewer();

public:
	MainFrame *myFrame;
	GHanLabel *m_issLabel;

	int m_issBitmapX;
	int m_issBitmapY;
	
	void SetText(char *text);
	void Clear();
	void SetRangeColour(int from, int to, wxColour fg, wxColour bg);

private:
	void Draw(wxPaintDC &dc);
	void DrawPanel(wxPaintDC &dc);
	void OnSize(wxSizeEvent& event);
	void OnIdle(wxIdleEvent& event);
	void OnPaint( wxPaintEvent &event );
	void OnTextChange(wxCommandEvent& WXUNUSED(event));

	DECLARE_EVENT_TABLE()
};

#endif
