#include "Main.h"

#include "bmp/iss_viewer.h"

BEGIN_EVENT_TABLE(IssViewer, wxPanel) 
	EVT_PAINT(IssViewer::OnPaint)
	EVT_SIZE(IssViewer::OnSize)
END_EVENT_TABLE() 

IssViewer::IssViewer(
	wxWindow* parent, MainFrame *myFrame, wxWindowID id, 
	const wxPoint& pos, const wxSize& size, long style)
	: wxPanel(parent, id, pos, size, style)
{ 
	m_issLabel=NULL;
	this->myFrame = myFrame;

	#ifndef LINUX
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	#endif //LINUX
}

IssViewer::~IssViewer () 
{
	delete m_issLabel;
}

void IssViewer::Clear()
{
	m_issLabel->SetLabel(" ");
}

void IssViewer::SetText(char *text)
{
	m_issLabel->SetLabel(text);
}

void IssViewer::SetRangeColour(int from, int to, wxColour fg, wxColour bg)
{
	m_issLabel->SetRangeColour(from, to, fg, bg);
}

void IssViewer::OnTextChange(wxCommandEvent& WXUNUSED(event))
{
}

void IssViewer::DrawPanel(wxPaintDC &dc)
{
	wxColour color;
	color.Set(wxT("#DDE1E6"));
	dc.SetBackground(wxBrush(color));
	dc.Clear();
	
	wxRect rc = GetClientRect();
	int rw = rc.GetWidth();
	int rh = rc.GetHeight();
	
	wxBitmap bmp = wxGetBitmapFromMemory(iss_viewer_bmp, 
			sizeof(iss_viewer_bmp));
	int w = bmp.GetWidth();
	int h = bmp.GetHeight();
	
	m_issBitmapX = (rw-w)/2;
	m_issBitmapY = (rh-h)/2;
	dc.DrawBitmap(bmp, m_issBitmapX, m_issBitmapY, true);

	if ( m_issLabel == NULL )
	{
		// ISS 가사는 한줄이 최대 64bytes
		m_issLabel= new GHanLabel(this, wxID_ANY, 
				wxDefaultPosition, wxSize(0,16),
				"", wxColour(wxT("#0F639C")), wxColour(wxT("#A8E5F6")));
		m_issLabel->SetToolTip(wxT("this is tooltip"));
		Connect(m_issLabel->GetId(), wxEVT_COMMAND_GHANLABEL,
				(wxObjectEventFunction) 
				(wxEventFunction) &IssViewer::OnTextChange);
	}
}

void IssViewer::Draw(wxPaintDC &dc)
{
	wxRect rc = GetClientRect();
	int rw = rc.GetWidth();
	int rh = rc.GetHeight();

	wxSize size = m_issLabel->GetSize();
	int w = size.GetWidth();
	int h = size.GetHeight();

	/*
	int x = (rw-w)/2;
	int y = (rh-h)/2;
	m_issLabel->Move(wxPoint(x,y));
	*/
	int x = m_issBitmapX+35;
	int y = (rh-h)/2;
	m_issLabel->Move(wxPoint(x,y));
}

void IssViewer::OnSize(wxSizeEvent& event)
{
	wxPaintDC dc(this);
	Refresh();
}

void IssViewer::OnPaint(wxPaintEvent &WXUNUSED(event)) 
{
	wxPaintDC dc(this);
	DrawPanel(dc);
	Draw(dc);
}
