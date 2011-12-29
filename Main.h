#include <Base.h>

#ifndef _MYADLIB_H_
#define _MYADLIB_H_

const int METER_FREQUENCY[] = { 30, 60, 100, 160, 240, 300, 350, 400, 440, 500, 600, 800, 1000, 1500, 2000, 2600, 3000, 4000, 6000, 8000, 10000, 14000, 16000 };
const int NUM_FREQUENCY = sizeof(METER_FREQUENCY)/sizeof(int);

class wxString;
class MainPanel;

class DnD : public wxFileDropTarget
{
public:
	DnD(MainFrame *child)
	{
		m_frame = child;
	}

	virtual bool OnDropFiles(wxCoord x, wxCoord y,
		const wxArrayString& filenames);

private:
	MainFrame *m_frame;
};

class SongSearch : public wxDirTraverser
{
public:
	SongSearch(wxArrayString& files) : m_files(files) { }

	virtual wxDirTraverseResult OnFile(const wxString& filename)
	{
		m_files.Add(filename);
		return wxDIR_CONTINUE;
	}

	virtual wxDirTraverseResult OnDir(const wxString& WXUNUSED(dirname))
	{
		return wxDIR_CONTINUE;
	}

private:
	wxArrayString& m_files;
};

//
// MyTcl 메인 프레임
//
class MainFrame : public wxFrame
{

public:
    MainFrame(wxWindow* parent,
            wxWindowID id,
            const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE);

    ~MainFrame();

public:
	wxAuiManager m_mgr;
	wxMenuBar* m_menuBar;
	wxAuiToolBar* m_playToolBar;
	//Config *m_config;
	MainPanel *m_mainPanel;
	wxStatusBar *m_statusBar;
    SliderPanel *m_slider;
    PlayThread *m_thread;
	wxListCtrl *m_playList;
	PeakMeter *m_meter;
	bool m_init;

    MainPanel *m_panel;
	
	SDL_AudioSpec m_spec;

	wxTimer *m_timer;
	IssViewer *m_issViewer;


public:
	long GetSelectItem();
	long GetNextItem();
	long GetPrevItem();
	bool FFT(void *audiobuf, int len);
	unsigned char GetBytePerSampleSize();
	void OnSlider( wxCommandEvent &WXUNUSED(event) );
	void OnLeftDClicked( wxListEvent& event);
	void OnSize(wxSizeEvent& event);
	void AddFile (wxString path);
	wxString GetTimeFormat(int sec);

	Sound *GetSound();

	bool Stop();
	void Play(long item);
	void Play(wxString imsFile);
	void Next();
	void Prev();

    
private:
	void OnCloseApp(wxCloseEvent& WXUNUSED(event));
	void OnTimer(wxTimerEvent &event);

    wxArrayString m_perspectives;
    wxMenu* m_perspectives_menu;
    long m_notebook_style;

    DECLARE_EVENT_TABLE()
};

#endif
