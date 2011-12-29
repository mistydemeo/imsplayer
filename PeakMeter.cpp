#include "Main.h"

#include "bmp/meter.h"
#include "bmp/play_mode_normal.h"
#include "bmp/play_mode_normal_hover.h"
#include "bmp/button.h"
#include "bmp/convert.h"
#include "bmp/convert_hover.h"
#include "bmp/convert_clicked.h"
#include "bmp/repeat_no.h"
#include "bmp/repeat_no_hover.h"
#include "bmp/repeat_all.h"
#include "bmp/repeat_all_hover.h"
#include "bmp/repeat_this.h"
#include "bmp/repeat_this_hover.h"

PeakMeterData& PeakMeterData::Copy(const PeakMeterData& pm)
{
    value = pm.value;
    fallOff = pm.fallOff;
    peak = pm.peak;
    return *this;
}

bool PeakMeterData::IsEqual(const PeakMeterData& pm)
{
    return (value == pm.value);
}

bool PeakMeterData::IsGreater(const PeakMeterData& pm)
{
    return ( value > pm.value );
}

bool PeakMeterData::IsLower(const PeakMeterData& pm)
{
    return ( value < pm.value );
}

BEGIN_EVENT_TABLE(PeakMeter, wxPanel) 
	EVT_PAINT (PeakMeter::OnPaint)
	EVT_SIZE(PeakMeter::OnSize)
	EVT_TIMER(wxID_ANY, PeakMeter::OnTimer)
	EVT_MENU(ID_REPEAT_NO, PeakMeter::OnRepeatNo)
	EVT_MENU(ID_REPEAT_ALL, PeakMeter::OnRepeatAll)
	EVT_MENU(ID_REPEAT_THIS, PeakMeter::OnRepeatThis)
END_EVENT_TABLE() 

PeakMeter::PeakMeter(
	wxWindow* parent, wxWindowID id, 
	const wxPoint& pos, const wxSize& size, long style)
	: wxPanel(parent, id, pos, size, style)
{ 
	m_menu = NULL;

	m_convertButton= NULL;
	m_repeatButton= NULL;
	m_playModeButton= NULL;

	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	InitData();
}

void PeakMeter::OnSize(wxSizeEvent& event)
{
    bool bIsRunning = IsStarted();
    
	if ( bIsRunning )
        Stop();

	wxAutoBufferedPaintDC  dc(this);

	m_colorBack.Set(wxT("#DDE1E6"));
	dc.SetBackground(wxBrush(m_colorBack));
	dc.Clear();
	
	wxRect rc = GetClientRect();

	int rw = rc.GetWidth();
	int rh = rc.GetHeight();

	// draw meter
	wxBitmap bmp = wxGetBitmapFromMemory(meter_bmp, sizeof(meter_bmp));
	int bw = bmp.GetWidth();
	int bh = bmp.GetHeight();
	
	m_meterBitmapX = (rw-bw)/2;
	m_meterBitmapY = (rh-bh)/2;
	dc.DrawBitmap(bmp, m_meterBitmapX, m_meterBitmapY, true);

	// draw repeat button
	int x = m_meterBitmapX+20;

	if ( m_repeatButton == NULL )
	{
		wxBitmap bmp1 = wxGetBitmapFromMemory(repeat_all_bmp, 
				sizeof(repeat_all_bmp));
		wxBitmap bmp2 = wxGetBitmapFromMemory(repeat_all_hover_bmp,
				sizeof(repeat_all_hover_bmp));
		m_repeatButton= new GButton(this, wxID_ANY, 
				wxDefaultPosition, bmp1, bmp2);
		m_repeatButton->SetToolTip(wxT("Choose Repeat mode (Repeat All)"));
		Connect(m_repeatButton->GetId(), wxEVT_COMMAND_GBUTTON,
				(wxObjectEventFunction) (wxEventFunction) &PeakMeter::OnRepeat);
	}
	m_repeatButton->Move(x, m_meterBitmapY+25);
	
	if ( m_playModeButton == NULL )
	{
		wxBitmap bmp1 = wxGetBitmapFromMemory(play_mode_normal_bmp,
				sizeof(play_mode_normal_bmp));
		wxBitmap bmp2 = wxGetBitmapFromMemory(play_mode_normal_hover_bmp, 
				sizeof(play_mode_normal_hover_bmp));
		m_playModeButton= new GButton(this, wxID_ANY, 
				wxDefaultPosition, bmp1, bmp2);
		m_playModeButton->SetToolTip(wxT("Choose Play mode"));
	}
	m_playModeButton->Move(x, m_meterBitmapY+45);

	// draw convert button
	bmp = wxGetBitmapFromMemory(button_bmp, sizeof(button_bmp));
	int bw2 = bmp.GetWidth();
	bh = bmp.GetHeight();

	x = m_meterBitmapX-bw2-10;
	if ( m_convertButton == NULL )
	{
		wxBitmap bmp1 = wxGetBitmapFromMemory(convert_bmp, 
				sizeof(convert_bmp));
		wxBitmap bmp2 = wxGetBitmapFromMemory(convert_hover_bmp, 
				sizeof(convert_hover_bmp));
		wxBitmap bmp3 = wxGetBitmapFromMemory(convert_clicked_bmp, 
				sizeof(convert_clicked_bmp));
		m_convertButton= new GButton(this, wxID_ANY, 
				wxDefaultPosition, bmp1, bmp2, bmp3);
	}
	m_convertButton->Move(x, m_meterBitmapY+5);

	dc.DrawBitmap(bmp, x, m_meterBitmapY+40, true);
	
	x = m_meterBitmapX+bw+10;
	dc.DrawBitmap(bmp, x, m_meterBitmapY+5, true);
	dc.DrawBitmap(bmp, x, m_meterBitmapY+40, true);
    
	// Auto-restart
    if ( bIsRunning )
        Start( m_delay );

	Refresh();
}

void PeakMeter::OnRepeat(wxCommandEvent& WXUNUSED(event))
{
	if ( m_menu ) delete m_menu;

	wxMenuItem* item;

	m_menu = new wxMenu();
	item = new wxMenuItem(m_menu, ID_REPEAT_NO, wxT("No Repeat"));
	m_menu->Append(item);
	item = new wxMenuItem(m_menu, ID_REPEAT_THIS, wxT("Repeat This"));
	m_menu->Append(item);
	item = new wxMenuItem(m_menu, ID_REPEAT_ALL, wxT("Repeat All"));
	m_menu->Append(item);

	PopupMenu(m_menu);
}

void PeakMeter::OnRepeatNo(wxCommandEvent& WXUNUSED(event))
{
	wxBitmap bmp1 = wxGetBitmapFromMemory(repeat_no_bmp, 
			sizeof(repeat_no_bmp));
	wxBitmap bmp2 = wxGetBitmapFromMemory(repeat_no_hover_bmp, 
			sizeof(repeat_no_hover_bmp));
	m_repeatButton->SetBitmap(bmp1, bmp2);
	m_repeatButton->SetToolTip(wxT("Choose Repeat mode (No Repeat)"));
	m_repeatMode=REPEAT_NO;
}

void PeakMeter::OnRepeatAll(wxCommandEvent& WXUNUSED(event))
{
	wxBitmap bmp1 = wxGetBitmapFromMemory(repeat_all_bmp, 
			sizeof(repeat_all_bmp));
	wxBitmap bmp2 = wxGetBitmapFromMemory(repeat_all_hover_bmp, 
			sizeof(repeat_all_hover_bmp));
	m_repeatButton->SetBitmap(bmp1, bmp2);
	m_repeatButton->SetToolTip(wxT("Choose Repeat mode (Repeat All)"));
	m_repeatMode=REPEAT_ALL;
}

void PeakMeter::OnRepeatThis(wxCommandEvent& WXUNUSED(event))
{
	wxBitmap bmp1 = wxGetBitmapFromMemory(repeat_this_bmp, 
			sizeof(repeat_this_bmp));
	wxBitmap bmp2 = wxGetBitmapFromMemory(repeat_this_hover_bmp, 
			sizeof(repeat_this_hover_bmp));
	m_repeatButton->SetBitmap(bmp1, bmp2);
	m_repeatButton->SetToolTip(wxT("Choose Repeat mode (Repeat This)"));
	m_repeatMode=REPEAT_THIS;
}

void PeakMeter::OnTimer(wxTimerEvent &event)
{
	//printf("timer...\n"); fflush(stdout);
	DoTimerProcessing();
}

void PeakMeter::DoTimerProcessing()
{
    Refresh();

    int nDecValue  = m_maxValue / m_ledBand;
    bool bNoChange = true;

    for(int i=0; i < static_cast<int>(m_meterData.size()); i++)
    {
        PeakMeterData pm = m_meterData.at(i);

        if (pm.value > 0)
        {
            pm.value -= ( m_ledBand>1 ? 
					nDecValue : m_maxValue*BAND_PERCENT/100);
            if (pm.value < 0)
                pm.value = 0;
            bNoChange = false;
        }

        if (pm.peak > 0)
        {
            pm.peak -= 1;
            bNoChange = false;
        }

        if (pm.peak == 0 && pm.fallOff > 0)
        {
            pm.fallOff -= (m_ledBand>1 ? nDecValue>>1 : 5);
            if (pm.fallOff < 0)
                pm.fallOff = 0;
            bNoChange = false;
        }

        // re-assign PeakMeterData
        m_meterData.at(i) = pm;
    }

    if (bNoChange) // Stop timer if no more data but do not reset ID
    {
        //Stop();
    }
}

void PeakMeter::InitData()
{
    const wxColour colLime = wxColour(0, 255, 0);
    const wxColour colRed  = wxColour(255, 0, 0);
    const wxColour colYellow  = wxColour(255, 255, 0);

    m_showFallOff  = true;
    m_delay        = 10;	// 10ms
    m_minValue     = 60;       // Min Range 0-60
    m_medValue     = 80;       // Med Range 60-80
    m_maxValue     = 100;      // Max Range 80-100
    m_numBand     = BAND_DEFAULT;
    m_ledBand     = LEDS_DEFAULT;
    m_colorBack = GetBackgroundColour();
    m_colorNorm     = colLime;
    m_colorMedium     = colYellow;
    m_colorHigh       = colRed;
    m_speed        = DEFAULT_SPEED;

	m_timer = new wxTimer(this, wxID_ANY);

    m_meterData.clear();
}
	
PeakMeter::~PeakMeter() 
{
}

void PeakMeter::SetBackgroundColor(wxColour colorBgnd)
{
	m_colorBack = colorBgnd;
	Refresh();
}

void PeakMeter::SetBandsColor(
		wxColour colorNormal, 
		wxColour colorMedium, 
		wxColour colorHigh)
{
    m_colorNorm = colorNormal;
    m_colorMedium = colorMedium;
    m_colorHigh   = colorHigh;
}

void PeakMeter::SetMeterBands(int nNumBands, int nLedBands)
{
    assert (nNumBands>0 && nLedBands>0 );

    if (nNumBands <= 0 || nLedBands <= 0)
        return;

    m_numBand = nNumBands;
    m_ledBand = nLedBands;

    // Reset vector
    ResetControl();
}

void PeakMeter::SetRangeValue(int nMin, int nMed, int nMax)
{
    assert( nMax > nMed && nMed > nMin && nMin > 0);

    m_minValue = nMin;
    m_medValue = nMed;
    m_maxValue = nMax;
}

void PeakMeter::GetRangeValue(int* lpiMin, int* lpiMed, int* lpiMax)
{
    if (NULL != lpiMin)
        *lpiMin = m_minValue;
    if (NULL != lpiMed)
        *lpiMed = m_medValue;
    if (NULL != lpiMax)
        *lpiMax = m_maxValue;
}

void PeakMeter::SetFalloffDelay(int nSpeed)
{
    m_speed = nSpeed;
}

void PeakMeter::SetFalloffEffect(bool bFalloffEffect)
{
    if (m_showFallOff != bFalloffEffect)
    {
        m_showFallOff = bFalloffEffect;
        Refresh();
    }
}

bool PeakMeter::GetFalloffEffect()
{
    return m_showFallOff;
}

inline int __max(int a, int b) 
{ 
	return (a > b) ? a : b; 
}

inline int __min(int a, int b) 
{ 
	return (a < b) ? a : b; 
}

bool PeakMeter::SetData(const int ArrayValue[], int nOffset, int nSize)
{
    assert( nOffset >= 0 && ArrayValue != NULL);

    if (nOffset < 0 || ArrayValue == NULL)
       return false;

    for(int i=0; i < nSize; i++)
    {
		PeakMeterData pm = m_meterData.at( i );
		pm.value = __min(ArrayValue[i], m_maxValue);
		pm.value = __max(pm.value, 0);
		if (pm.fallOff < pm.value)
		{
			pm.fallOff = pm.value;
			pm.peak    = m_speed;
		}

		m_meterData.at(i) = pm;
    }

    return true;
}

bool PeakMeter::Start(int delay)
{
	if ( IsStarted() == false)
	{
		m_delay = delay;
		m_timer->Start(m_delay);
	}
	else
		return false;
	
	return true;
}

bool PeakMeter::IsStarted()
{
	return m_timer->IsRunning();
}

bool PeakMeter::Stop()
{
	m_timer->Stop();
	return true;
}

void PeakMeter::ResetControl()
{
    // Initialize vector
    m_meterData.resize( m_numBand);

    // default data
    PeakMeterData pm;
    pm.value = m_maxValue;
    pm.fallOff = m_maxValue;
    pm.peak  = m_speed;

    for(size_t i=0; i < m_meterData.size(); i++)
    {
        m_meterData.at(i) = pm;
    }
}

void PeakMeter::OnPaint(wxPaintEvent &WXUNUSED(event)) 
{
	DoTimerProcessing();

	m_timer->Notify();
	
	wxAutoBufferedPaintDC  dc(this);

	if ( IsStarted() )
	{
		DrawBand(dc, m_meterBitmapX+67, m_meterBitmapY+60);
	}
}

void PeakMeter::DrawBand(wxAutoBufferedPaintDC  &dc, int x, int y)
{
	wxColour color1;	// clear color
	color1.Set(wxT("#2090D1"));
	wxPen clearPen = wxPen(color1);
	
	wxColour color2;	// clear color
	color2.Set(wxT("#A8E5F6"));
	wxPen clearPen2 = wxPen(color2);
	
	wxColour color3;	// clear color
	color3.Set(wxT("#ffffff")); // band color
	wxPen bandPen = wxPen(color3);
	
	wxColour color4;	// clear color
	color4.Set(wxT("#0F639C")); // fall off color
	wxPen fallPen = wxPen(color4);

    for(int bandNum=0; bandNum<m_numBand; bandNum++)
    {
		int value = m_meterData.at(bandNum).value;

		int meterValue = (int)((double)value/100.0*(double)m_ledBand);
		int bandx = x+(bandNum*12);
		int bandy = y;
	
		// clear
		for(int doty=0; doty<=m_ledBand; doty++)
		{
			for(int dotx=0; dotx<=7; dotx++)
			{
				if((dotx%2)==0)
					dc.SetPen(clearPen);
				else
					dc.SetPen(clearPen2);

				int sx = bandx+dotx;
				int sy = bandy+(-(doty*2));
				dc.DrawPoint(sx, sy);
			}
		}
		
		
		// draw band
		dc.SetPen(bandPen);
		for(int doty=0; doty<meterValue; doty++)
		{
			int sx = bandx;
			int sy = bandy+(-(doty*2));
			dc.DrawLine(sx, sy, sx+7, sy);
		}

        if (m_showFallOff)
		{
			dc.SetPen(fallPen);

			int nMaxHeight=m_ledBand;
			int sx = bandx;
			int sy = m_meterData[bandNum].fallOff*
				nMaxHeight/m_maxValue;
			sy = bandy+(-(sy*2));
			dc.DrawLine(sx, sy, sx+7, sy);
		}
    }
}
