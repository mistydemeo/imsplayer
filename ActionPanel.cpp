#include "Main.h"

#include "bmp/previous.h"
#include "bmp/previous_hover.h"
#include "bmp/previous_clicked.h"
#include "bmp/stop.h"
#include "bmp/stop_hover.h"
#include "bmp/stop_clicked.h"
#include "bmp/play.h"
#include "bmp/play_hover.h"
#include "bmp/play_clicked.h"
#include "bmp/next.h"
#include "bmp/next_hover.h"
#include "bmp/next_clicked.h"
#include "bmp/browse.h"
#include "bmp/browse_hover.h"
#include "bmp/browse_clicked.h"
#include "bmp/panel_background.h"
#include "bmp/volume_background.h"
#include "bmp/volume_slider.h"
#include "bmp/volume_slider_hover.h"

BEGIN_EVENT_TABLE(ButtonPanel, wxPanel)
	EVT_PAINT (ButtonPanel::OnPaint)
	EVT_MOTION (ButtonPanel::OnMotion)
    EVT_SIZE(ButtonPanel::OnSize)
END_EVENT_TABLE()

ButtonPanel::ButtonPanel(
	wxWindow* parent, MainFrame *myFrame, wxWindowID id, 
	const wxPoint& pos, const wxSize& size, long style)
	: wxPanel(parent, id, pos, size, style)
{ 
	this->myFrame = myFrame;

	SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	wxBitmap bmp1 = wxGetBitmapFromMemory(previous_bmp, 
			sizeof(previous_bmp));
	wxBitmap bmp2 = wxGetBitmapFromMemory(previous_hover_bmp, 
			sizeof(previous_hover_bmp));
	wxBitmap bmp3 = wxGetBitmapFromMemory(previous_clicked_bmp, 
			sizeof(previous_clicked_bmp));
	m_previousButton= new GButton(this, wxID_ANY, 
			wxDefaultPosition, bmp1, bmp2, bmp3);
	m_previousButton->SetToolTip(wxT("Previous"));
	Connect(m_previousButton->GetId(), wxEVT_COMMAND_GBUTTON,
			(wxObjectEventFunction) (wxEventFunction) &ButtonPanel::OnPrev);
	
	bmp1 = wxGetBitmapFromMemory(stop_bmp, 
			sizeof(stop_bmp));
	bmp2 = wxGetBitmapFromMemory(stop_hover_bmp, 
			sizeof(stop_hover_bmp));
	bmp3 = wxGetBitmapFromMemory(stop_clicked_bmp, 
			sizeof(stop_clicked_bmp));
	m_stopButton = new GButton(this, wxID_ANY, 
			wxDefaultPosition, bmp1, bmp2, bmp3);
	m_stopButton->SetToolTip(wxT("Stop"));
	Connect(m_stopButton->GetId(), wxEVT_COMMAND_GBUTTON,
			(wxObjectEventFunction) (wxEventFunction) &ButtonPanel::OnStop);

	bmp1 = wxGetBitmapFromMemory(play_bmp, 
			sizeof(play_bmp));
	bmp2 = wxGetBitmapFromMemory(play_hover_bmp, 
			sizeof(play_hover_bmp));
	bmp3 = wxGetBitmapFromMemory(play_clicked_bmp, 
			sizeof(play_clicked_bmp));
	m_playButton = new GButton(this, wxID_ANY, 
			wxDefaultPosition, bmp1, bmp2, bmp3);
	m_playButton->SetToolTip(wxT("Play"));
	Connect(m_playButton->GetId(), wxEVT_COMMAND_GBUTTON,
			(wxObjectEventFunction) (wxEventFunction) &ButtonPanel::OnPlay);
	
	bmp1 = wxGetBitmapFromMemory(next_bmp, sizeof(next_bmp));
	bmp2 = wxGetBitmapFromMemory(next_hover_bmp, sizeof(next_hover_bmp));
	bmp3 = wxGetBitmapFromMemory(next_clicked_bmp, sizeof(next_clicked_bmp));
	m_nextButton = new GButton(this, wxID_ANY, 
			wxDefaultPosition, bmp1, bmp2, bmp3);
	m_nextButton->SetToolTip(wxT("Next"));
	Connect(m_nextButton->GetId(), wxEVT_COMMAND_GBUTTON,
			(wxObjectEventFunction) (wxEventFunction) &ButtonPanel::OnNext);
	
	bmp1 = wxGetBitmapFromMemory(browse_bmp, sizeof(browse_bmp));
	bmp2 = wxGetBitmapFromMemory(browse_hover_bmp, sizeof(browse_hover_bmp));
	bmp3 = wxGetBitmapFromMemory(browse_clicked_bmp, sizeof(browse_clicked_bmp));
	m_browseButton = new GButton(this, wxID_ANY, 
			wxDefaultPosition, bmp1, bmp2, bmp3);
	m_browseButton->SetToolTip(wxT("Open File(s)"));
	Connect(m_browseButton->GetId(), wxEVT_COMMAND_GBUTTON,
			(wxObjectEventFunction) (wxEventFunction) &ButtonPanel::OnOpenFiles);
	
	bmp1 = wxGetBitmapFromMemory(volume_slider_bmp, sizeof(volume_slider_bmp));
	bmp2 = wxGetBitmapFromMemory(volume_background_bmp, 
			sizeof(volume_background_bmp));
	bmp3 = wxGetBitmapFromMemory(volume_slider_hover_bmp, 
			sizeof(volume_slider_hover_bmp));

	m_slider = new GSlider(this, wxID_ANY, wxDefaultPosition, wxNO_BORDER, 
			wxT("aaaa"), bmp2, bmp2, bmp1, bmp3, bmp3, bmp3);
	m_slider->SetToolTip(wxT("Volume 100%"));
	Connect(m_slider->GetId(), wxEVT_COMMAND_GSLIDER,
			(wxObjectEventFunction) (wxEventFunction) &ButtonPanel::OnChange);
	Connect(m_slider->GetId(), wxEVT_LEFT_UP_GSLIDER,
			(wxObjectEventFunction) (wxEventFunction) &ButtonPanel::OnChangeComplete);

	// default value
	SetRange((float)0, (float)128);
	SetValue((float)m_volume);
}

ButtonPanel::~ButtonPanel () 
{
}

void ButtonPanel::SetValue(float value)
{
	m_slider->SetValue(0, value);
}

void ButtonPanel::SetRange(float min, float max)
{
	m_slider->SetRange((float)min, (float)max);
}

float ButtonPanel::GetValue()
{
	return m_slider->GetValue(0);
}

void ButtonPanel::OnChange(wxCommandEvent& WXUNUSED(event))
{
	m_volume = (int)m_slider->GetValue(0);
}

void ButtonPanel::OnChangeComplete(wxMouseEvent& WXUNUSED(event))
{
	int percent = m_volume / (float)128 * (float)100;
	m_slider->SetToolTip(wxString::Format(wxT("Volume %d%%"), percent));
}

void ButtonPanel::OnStop(wxCommandEvent& WXUNUSED(event))
{
	myFrame->Stop();
}

void ButtonPanel::OnPlay(wxCommandEvent& WXUNUSED(event))
{
	myFrame->Play(-1);
}

void ButtonPanel::OnNext(wxCommandEvent& WXUNUSED(event))
{
	myFrame->Next();
}

void ButtonPanel::OnOpenFiles(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog dlg(this,
		wxT("Open File(s)"), wxEmptyString,
		wxEmptyString,
		wxT("IMS Files (*.ims)|*.ims|All Files (*)|*"),
		wxMULTIPLE|wxOPEN
	);
    dlg.CentreOnParent();

    if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString paths;
		dlg.GetPaths(paths);

		for(int i=0; i<paths.GetCount(); i++)
		{
			wxString path = paths.Item(i);
			myFrame->AddFile(path);
		}
	}
}

void ButtonPanel::OnPrev(wxCommandEvent& WXUNUSED(event))
{
	myFrame->Prev();
}

void ButtonPanel::OnSize(wxSizeEvent& event)
{
	wxPaintDC dc(this);
	DrawButton(dc);
}

void ButtonPanel::OnMotion(wxMouseEvent& e)
{
}

void ButtonPanel::DrawButton(wxPaintDC &dc)
{
	int totalWidth=0;
	int maxHeight = m_playButton->GetSize().GetHeight();
	totalWidth+=m_playButton->GetSize().GetWidth();
	totalWidth+=m_previousButton->GetSize().GetWidth();
	totalWidth+=m_stopButton->GetSize().GetWidth();
	totalWidth+=m_nextButton->GetSize().GetWidth();
	totalWidth+=m_browseButton->GetSize().GetWidth();
	totalWidth+=m_slider->GetSize().GetWidth();
	totalWidth+=25;
	
	wxSize size = GetSize();

	// draw background
	wxBitmap bg = wxGetBitmapFromMemory(panel_background_bmp, 
			sizeof(panel_background_bmp));
	for(int x=0; x<size.GetWidth(); x+=bg.GetWidth())
		dc.DrawBitmap(bg, x, 0, true);
	
	// previous
	int x = (size.GetWidth()-totalWidth)/2;
	wxSize buttonSize = m_previousButton->GetSize();
	int y = (maxHeight-buttonSize.GetHeight())/2;
	m_previousButton->Move(wxPoint(x,y));

	// stop
	x = x + buttonSize.GetWidth()+5;
	buttonSize = m_stopButton->GetSize();
	y = (maxHeight-buttonSize.GetHeight())/2;
	m_stopButton->Move(wxPoint(x,y));
	
	// play
	x = x + buttonSize.GetWidth()+5;
	buttonSize = m_playButton->GetSize();
	y = (maxHeight-buttonSize.GetHeight())/2;
	m_playButton->Move(wxPoint(x,y));
	
	// next
	x = x + buttonSize.GetWidth()+5;
	buttonSize = m_nextButton->GetSize();
	y = (maxHeight-buttonSize.GetHeight())/2;
	m_nextButton->Move(wxPoint(x,y));
	
	// browse
	x = x + buttonSize.GetWidth()+10;
	buttonSize = m_browseButton->GetSize();
	y = (maxHeight-buttonSize.GetHeight())/2;
	m_browseButton->Move(wxPoint(x,y));
	
	// volume slider
	x = x + buttonSize.GetWidth()+20;
	buttonSize = m_slider->GetSize();
	y = (maxHeight-buttonSize.GetHeight())/2;
	m_slider->Move(wxPoint(x,y));
}

void ButtonPanel::OnPaint(wxPaintEvent &WXUNUSED(event)) 
{
	wxPaintDC dc(this);
	DrawButton(dc);
}
