#include "Main.h"

#include "bmp/slider.h"
#include "bmp/slider_hover.h"
#include "bmp/slider_background.h"

BEGIN_EVENT_TABLE(SliderPanel, wxPanel)
	EVT_PAINT (SliderPanel::OnPaint)
	EVT_MOTION (SliderPanel::OnMotion)
    EVT_SIZE(SliderPanel::OnSize)
END_EVENT_TABLE()

SliderPanel::SliderPanel(
	wxWindow* parent, MainFrame *myFrame, wxWindowID id, 
	const wxPoint& pos, const wxSize& size, long style)
	: wxPanel(parent, id, pos, size, style)
{ 
	this->myFrame = myFrame;

	SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	// slider
	wxBitmap bmp1 = wxGetBitmapFromMemory(slider_bmp, 
			sizeof(slider_bmp));
	// back
	wxBitmap bmp2 = wxGetBitmapFromMemory(slider_background_bmp, 
			sizeof(slider_background_bmp));
	// slider hover
	wxBitmap bmp3 = wxGetBitmapFromMemory(slider_hover_bmp, 
			sizeof(slider_hover_bmp));

	m_slider = new GSlider(this, wxID_ANY, wxDefaultPosition, wxNO_BORDER, 
			wxT("aaaa"), bmp2, bmp2, bmp1, bmp3, bmp3, bmp3);

	// default value
	SetRange((float)0, (float)100);
	SetValue((float)0);
}

SliderPanel::~SliderPanel () 
{
}

void SliderPanel::SetValue(float value)
{
	m_slider->SetValue(0, value);
}

void SliderPanel::SetRange(float min, float max)
{
	m_slider->SetRange((float)min, (float)max);
}

float SliderPanel::GetValue()
{
	return m_slider->GetValue(0);
}

void SliderPanel::OnSize(wxSizeEvent& event)
{
	wxPaintDC dc(this);
	DrawSlider(dc);
}

void SliderPanel::OnMotion(wxMouseEvent& e)
{
	//wxPaintDC dc(this);
	//DrawSlider(dc);
}

void SliderPanel::DrawSlider(wxPaintDC &dc)
{
	wxColour color;
	color.Set(wxT("#DDE1E6"));
	dc.SetBackground(wxBrush(color));
	dc.Clear();

	wxSize size = GetSize();
	int width = size.GetWidth();
	int height = size.GetHeight();

	wxSize size2 = m_slider->GetSize();
	int width2 = size2.GetWidth();
	int height2 = size2.GetHeight();

	int x = (width-width2)/2;
	int y = (height-height2)/2;

	m_slider->Move(wxPoint(x,y));
}

void SliderPanel::OnPaint(wxPaintEvent &WXUNUSED(event)) 
{
	wxPaintDC dc(this);
	DrawSlider(dc);
}
