#ifndef _PEAKMETER_H_
#define _PEAKMETER_H_

#define HORZ_DEFAULT	8
#define VERT_DEFAULT	8
#define BAND_DEFAULT	8
#define LEDS_DEFAULT	8
#define BAND_PERCENT	10	// 10% of Max Range (Auto Decrease)
#define GRID_INCREASEBY	5	// Increase Grid color based on Background color
#define FALL_INCREASEBY 60	// Increase Falloff color based on Background
#define DELAY_10MS		10
#define DEFAULT_SPEED	10

// Peak Meter Control styles
#define PM_HORIZONTAL	0x0000	// Horizontal band
#define PM_VERTICAL	0x0001	// Vertical band

class GButton;

struct PeakMeterData {
public:
	int  value;
	int  fallOff;
	int  peak;
	PeakMeterData() :
	value(0), fallOff(0), peak(0)
	{
	}
	
	~PeakMeterData() { }
	PeakMeterData& Copy(const PeakMeterData& pm);
	PeakMeterData& operator=(const PeakMeterData& pm) 
		{ return Copy( pm ); }
	bool IsEqual(const PeakMeterData& pm);
	bool IsGreater(const PeakMeterData& pm);
	bool IsLower(const PeakMeterData& pm);
	bool operator==(const PeakMeterData& pm) 
		{ return IsEqual( pm ); }
	bool operator!=(const PeakMeterData& pm) 
		{ return !IsEqual( pm ); }
	bool operator<(const PeakMeterData& pm)  
		{ return IsLower( pm ); }
	bool operator>(const PeakMeterData& pm)  
		{ return IsGreater( pm ); }
	bool operator<=(const PeakMeterData& pm) 
		{ return !IsGreater( pm ); }
	bool operator>=(const PeakMeterData& pm) 
		{ return !IsLower( pm ); }
};

typedef std::vector<PeakMeterData> Vector_MeterData;

class PeakMeter: public wxPanel
{ 
public: 
	PeakMeter(wxWindow* parent, wxWindowID id,	
			const wxPoint& pos, const wxSize& size, 
			long style = wxBORDER_RAISED); 
	~PeakMeter(); 
	
	void InitData();
	void ResetControl();
	void DoTimerProcessing();
	bool IsStarted();
	bool Stop();
	bool Start(int delay);
	void SetBackgroundColor(wxColour colorBgnd);
	void SetBandsColor(wxColour colorNormal, 
			wxColour colorMedium, 
			wxColour colorHigh);
	void SetBandWidth(int width);
	void SetMeterBands(int nNumBands, int nLedBands);
	void SetRangeValue(int nMin, int nMed, int nMax);
	void GetRangeValue(int* lpiMin, int* lpiMed, int* lpiMax);
	void SetFalloffDelay(int nSpeed);
	void SetFalloffEffect(bool bFalloffEffect);
	bool GetFalloffEffect();
	bool SetData(const int ArrayValue[], int nOffset, int nSize);
	void DrawBand(wxAutoBufferedPaintDC  &dc, int x, int y);

	void Draw();
	void SetMeters(double data[], int length);

protected:
	wxColour m_colorBack;	// Control background color
	wxColour m_colorNorm;	// Normal-range value band color
	wxColour m_colorMedium;	// Medium-range value band color
	wxColour m_colorHigh;	// High-range value band color
	int		 m_minValue;	// Minimum value
	int		 m_medValue;	// Medium value
	int		 m_maxValue;	// Maximum value
	int		 m_numBand;		// number of bands
	int		 m_ledBand;		// number of leds per band
	int		 m_speed;		// Falloff Speed
	bool	 m_showFallOff;	// Show Falloff
	int 	 m_meterBitmapX;
	int 	 m_meterBitmapY;
	unsigned int	 m_delay;	// Saved delay value

	GButton *m_playModeButton;
	GButton *m_repeatButton;
	GButton *m_convertButton;

	Vector_MeterData	m_meterData;

	wxMenu *m_menu;
	wxTimer *m_timer;

private:
	void OnRepeat(wxCommandEvent& WXUNUSED(event));
	void OnRepeatNo(wxCommandEvent& WXUNUSED(event));
	void OnRepeatAll(wxCommandEvent& WXUNUSED(event));
	void OnRepeatThis(wxCommandEvent& WXUNUSED(event));
	void OnPaint(wxPaintEvent &WXUNUSED(event));
	void OnSize(wxSizeEvent& event);
	void OnTimer(wxTimerEvent &event);
	
	DECLARE_EVENT_TABLE();
};

#endif
