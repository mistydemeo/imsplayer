#include <Main.h>

#include "bmp/imsplayer.h"
#include "bmp/empty.h"
#include "bmp/playing.h"

Uint8 *pcm_buffer[2][PREPARE_BUFFER_COUNT];
unsigned int song_tick[2][PREPARE_BUFFER_COUNT];
ISS_RECORD *lyrics_buffer[2][PREPARE_BUFFER_COUNT];
void callback(void *userdata, Uint8 *pcm_buffer, int len);

// 현재 연주중인 버퍼
int m_playingBuffer=0;
// 현재 연주중인 버퍼의 인덱스
int m_playingBufferIndex=0;
// 마지막 보여준 가사 라인
int m_lastLyricsLine= -1;
// 마지막 보여준 가사 레코드
int m_lastLyricsRecord = 0;

// 연주가 종료된 버퍼
int m_endBuffer;
// 연주가 종료된 버퍼의 인덱스
int m_endBufferIndex;

// 연주중인 리스트 인덱스
long m_playingItem = -1;

wxString exePath;

int m_volume = SDL_MIX_MAXVOLUME;
int m_playMode = STOPPED;
int m_repeatMode = REPEAT_ALL;

// -----------------
class MainApp : public wxApp
{
public:
    bool OnInit();
};

DECLARE_APP(MainApp)
IMPLEMENT_APP(MainApp)

bool MainApp::OnInit()
{
	wxFrame *mainFrame = new MainFrame(NULL,
		 wxID_ANY, wxT("IMS Player"), 
		 wxDefaultPosition, wxSize(620, 600));
	mainFrame->SetMinSize(wxSize(620,600));
    SetTopWindow(mainFrame);
    mainFrame->Show();

    return true;
}

// ------------------
bool DnD::OnDropFiles(wxCoord, wxCoord, const wxArrayString& filenames)
{
	if ( filenames.GetCount() > 0 )
	{
		for(unsigned int i=0; i<filenames.size(); i++)
		{
			// 파일이면 추가
			wxFileName file(filenames[i]);
			if ( file.FileExists() )
			{
				wxString ext = file.GetExt();
				if ( ext.Lower() == wxT("ims") )
				{
					m_frame->AddFile(filenames[i]);
				}

			// 디렉토리이면 재귀검색
			} else {
				wxArrayString files;
				SongSearch traverser(files);

				wxDir dir(filenames[i]);
				dir.Traverse(traverser);

				for(unsigned int x=0; x<files.size(); x++)
				{
					wxFileName file2(files[x]);

					// 파일이면 추가
					wxString ext = file2.GetExt();
					if ( ext.Lower() == wxT("ims") )
					{
						m_frame->AddFile(files[x]);
					}
				}

			}
		}
	}

	return true;
}

// ------------------

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_SIZE(MainFrame::OnSize)
    EVT_CLOSE(MainFrame::OnCloseApp)
	EVT_TIMER(wxID_ANY, MainFrame::OnTimer)
END_EVENT_TABLE()

MainFrame::MainFrame(wxWindow* parent,
	 wxWindowID id,
	 const wxString& title,
	 const wxPoint& pos,
	 const wxSize& size,
	 long style)
	: wxFrame(parent, id, title, pos, size, style)
{
	m_thread = NULL;

    m_mgr.SetManagedWindow(this);

	m_meter = NULL;

	wxInitAllImageHandlers();
	
#ifdef __MINGW32__
	char _drv[MAX_PATH], _path[MAX_PATH];
	char fname[MAX_PATH], ext[MAX_PATH];
	WCHAR path[MAX_PATH];

	GetModuleFileName(NULL, path, 1024);
	char temp[MAX_PATH];
	sprintf(temp, "%ls\n", path);	// wide char to char
	_splitpath(temp, _drv, _path, fname, ext);
	wxFileName exe( wxString(_drv, wxConvUTF8) + 
			wxString(_path, wxConvUTF8));
	exePath = exe.GetPath();
#else
	char _procname[FILENAME_MAX];
	int len = readlink("/proc/self/exe", _procname, FILENAME_MAX - 1);
	_procname[len] = '\0';
	wxString procname = wxString(_procname, wxConvUTF8);
	wxFileName exe(procname);
	exePath = exe.GetPath();
#endif
    
	wxBitmap bmp = wxGetBitmapFromMemory(imsplayer_bmp, 
			sizeof(imsplayer_bmp));
	wxIcon icon;
	icon.CopyFromBitmap(bmp);
	SetIcon(icon);

	exePath.Replace(wxT("\\"), wxT("/"), true);
	
	unsigned int flags = m_mgr.GetFlags();
	flags &= ~wxAUI_MGR_TRANSPARENT_HINT;
	flags &= ~wxAUI_MGR_RECTANGLE_HINT;
	flags &= ~wxAUI_MGR_ALLOW_ACTIVE_PANE;
	m_mgr.SetFlags(flags);
	m_mgr.SetFlags(m_mgr.GetFlags() ^ wxAUI_MGR_RECTANGLE_HINT);
	m_mgr.SetFlags(m_mgr.GetFlags() ^ wxAUI_MGR_ALLOW_ACTIVE_PANE);
	m_mgr.Update();

	// ------------------------------
	m_statusBar = new wxStatusBar(this, -1);
	m_statusBar->SetFieldsCount(3);
	int width[3] = { 150, 150, -1 };
	m_statusBar->SetStatusWidths(0, width);
	m_statusBar->SetStatusText(wxT("inhak.min@gmail.com"), 0);
	
	SetStatusBar(m_statusBar);

	// play list
	wxPanel *m_panel = new wxPanel(this, wxID_ANY);
    m_mgr.AddPane(m_panel, wxAuiPaneInfo().
		Name(wxT("center")).
		Caption(wxT("Center")).
		CenterPane());

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	m_panel->SetSizer(sizer);

	// peak meter
	m_meter = new PeakMeter(m_panel, wxID_ANY, 
			wxDefaultPosition, wxSize(-1,74), 
			wxNO_BORDER);

	// NUM_FREQUENCY: 분석할 주파수 갯수
	// 20: 각 주파수의 높이
    m_meter->SetMeterBands(NUM_FREQUENCY, 20);
	
	sizer->Add(m_meter, 0, wxEXPAND|wxTOP, 3);
	
	// iss viewer
	m_issViewer = new IssViewer(m_panel, this, wxID_ANY, 
			wxDefaultPosition, wxSize(-1,50), wxNO_BORDER);
	sizer->Add(m_issViewer, 0, wxEXPAND|wxALL, 0);

	// play list
	m_playList = new wxListCtrl(m_panel, wxID_ANY,
		wxDefaultPosition, wxSize(-1,-1), wxLC_REPORT|wxLC_VRULES);
	sizer->Add(m_playList, 3, wxEXPAND|wxLEFT|wxRIGHT, 3);
	
	wxImageList *imgList = new wxImageList (16, 16);
	bmp = wxGetBitmapFromMemory(empty_bmp, sizeof(empty_bmp));
	imgList->Add(wxBitmap(bmp));
	bmp = wxGetBitmapFromMemory(playing_bmp, sizeof(playing_bmp));
	imgList->Add(wxBitmap(bmp));
	m_playList->AssignImageList(imgList, wxIMAGE_LIST_SMALL); 
	Connect(m_playList->GetId(), wxEVT_COMMAND_LIST_ITEM_ACTIVATED, 
			wxListEventHandler(MainFrame::OnLeftDClicked));

	wxListItem itemCol; 
	itemCol.SetText(_T("Path")); 
	itemCol.SetAlign(wxLIST_FORMAT_LEFT); 
	itemCol.SetWidth(0); 
	m_playList->InsertColumn(0, itemCol); 
	
	itemCol.SetText(_T("P")); 
	itemCol.SetAlign(wxLIST_FORMAT_LEFT); 
	itemCol.SetWidth(20); 
	m_playList->InsertColumn(1, itemCol); 
	
	itemCol.SetText(_T("File")); 
	itemCol.SetAlign(wxLIST_FORMAT_LEFT); 
	itemCol.SetWidth(100); 
	m_playList->InsertColumn(2, itemCol); 
	
	itemCol.SetText(_T("Title")); 
	itemCol.SetAlign(wxLIST_FORMAT_LEFT); 
	itemCol.SetWidth(250); 
	m_playList->InsertColumn(3, itemCol); 
	
	itemCol.SetText(_T("Duration")); 
	itemCol.SetAlign(wxLIST_FORMAT_LEFT); 
	itemCol.SetWidth(100); 
	m_playList->InsertColumn(4, itemCol); 
	
	itemCol.SetText(_T("Singer")); 
	itemCol.SetAlign(wxLIST_FORMAT_LEFT); 
	itemCol.SetWidth(100); 
	m_playList->InsertColumn(5, itemCol); 
	
	m_playList->SetDropTarget(new DnD(this));
	
	// slider
	m_slider= new SliderPanel(
			m_panel, this, wxID_ANY,
			wxDefaultPosition, wxSize(-1,50), wxNO_BORDER);

	m_slider->SetRange((float)0, (float)100);
	m_slider->SetValue((float)0);
	sizer->Add(m_slider, 0, wxEXPAND|wxALL, 0);

	Connect(m_slider->m_slider->GetId(), wxEVT_COMMAND_GSLIDER, 
		wxCommandEventHandler(MainFrame::OnSlider));
	
	// button
	ButtonPanel *m_buttonPanel = new ButtonPanel(
			m_panel, this, wxID_ANY,
			wxDefaultPosition, wxSize(-1,38), wxNO_BORDER);
	sizer->Add(m_buttonPanel, 0, wxEXPAND|wxALL, 0);
	
	m_mgr.Update();
	
	// audio initial
	memset(&m_spec, 0x00, sizeof(SDL_AudioSpec));

	if(SDL_Init(SDL_INIT_AUDIO) < 0) 
	{
		wxMessageBox(wxString::Format(wxT("Unable to initialize SDL.\n%s"), 
			SDL_GetError()), wxT("IMS Player"), wxICON_ERROR);
		return;
	}

	m_spec.freq = 22050;	// 1초당 22050개 샘플 (보통 게임에서 많이 사용)
	m_spec.format = AUDIO_S16SYS; 	// 16bit
	m_spec.channels = 1;	// mono
	m_spec.samples = 512;	// buffer
	// len 4096이 넘어간다.
	// len = samples * channels * format (16bit <-- 2byte)
	m_spec.callback = callback;		
	m_spec.userdata = this;

	if(SDL_OpenAudio(&m_spec, NULL) < 0) 
	{
		wxMessageBox(wxString::Format(wxT("Unable to open audio.\n%s"), 
		SDL_GetError()), wxT("IMS Player"), wxICON_ERROR);
		return;
	}
	
	// ---------
	m_timer = new wxTimer(this, wxID_ANY);
	
}

MainFrame::~MainFrame()
{
}

void MainFrame::OnTimer(wxTimerEvent &event)
{
	if ( m_playMode == STOPPED )
	{
		m_issViewer->Clear();
		m_timer->Stop();

		if ( m_repeatMode == REPEAT_ALL )
		{
			Next();
		}
	}
}

unsigned char MainFrame::GetBytePerSampleSize()
{
	// Hence one sample of a stereo 8 bit audio source is: 
	// 1 [8 bits] * 2 [channels] = 2 bytes.
	return m_spec.channels * (m_spec.format == AUDIO_U8 ? 1 : 2); 
}

void init_prepare_pcm_buffer(void)
{
	m_playingBuffer=0;
	m_playingBufferIndex=0;

	m_lastLyricsLine=-1;
	m_lastLyricsRecord=0;

	m_endBuffer=-1;
	m_endBufferIndex=-1;
}

void alloc_prepare_pcm_buffer (int bufferIndex)
{
	for(int i=0; i<PREPARE_BUFFER_COUNT; i++)
	{
		pcm_buffer[bufferIndex][i] = (Uint8 *)malloc(PREPARE_BUFFER_SIZE);
	}
}

void free_prepare_pcm_buffer (int bufferIndex)
{
	for(int i=0; i<PREPARE_BUFFER_COUNT; i++)
	{
		if(pcm_buffer[bufferIndex][i])
		{
			free(pcm_buffer[bufferIndex][i]);
			pcm_buffer[bufferIndex][i]=NULL;
		}
	}
}

// 다음 10개의 pcm 데이타를 준비한다.
void prepare_next (void *userdata, int len, int bufferIndex)
{
	if ( m_playMode == STOP || m_playMode == STOPPED ||
	  m_playMode == SONG_END ) return;

	MainFrame *self = (MainFrame *)userdata;

	Ims *m_ims = self->m_thread->GetIms();
	Iss *m_iss = self->m_thread->GetIss();

	for(int i=0; i<PREPARE_BUFFER_COUNT; i++)
	{
		static int remain_samples = 0;
		int to_write_len, samples;
		int delayTime = 0;
		short int *pcm_buffer_pos;
		pcm_buffer_pos = (short int *)pcm_buffer[bufferIndex][i];

		to_write_len = len / self->GetBytePerSampleSize();
		
		// 틱을 저장한다.
		int tick = m_ims->GetCurrentTick();
		song_tick[bufferIndex][i] = tick;
				
		// 가사 레코드를 찾는다.
		Iss *m_iss = self->m_thread->GetIss();
		if ( m_iss->IsOpened() == true )
		{
			for(int x=m_lastLyricsRecord; x<m_iss->m_header.rec_count; x++)
			{
				// 가사 파일에는 본래의 tick을 8로 나눈 값이 들어가 있다.
				if ( m_iss->m_record[x].kasa_tick >= tick/8 )
				{
					m_lastLyricsRecord = x;
					lyrics_buffer[bufferIndex][i] = &m_iss->m_record[x];
					break;
				}
			}
		}
		else
		{
			lyrics_buffer[bufferIndex][i] = NULL;
		}

		// 애드립 PCM 데이타를 만들어서 버퍼에 저장한다.
		while (to_write_len > 0) 
		{
			if (remain_samples > 0) 
			{
				samples = remain_samples;
			}
			else 
			{
				// 분당 박자수
				int basicTempo = m_ims->GetBasicTempo();

				delayTime = m_ims->PlayEvent();

				MYADLIB_IMS *ims = m_ims->m_ims;

				// 총 몇개의 쌤플을 쓸것인지 계산한다.
				samples = MulDiv(22050 * 60, 
						delayTime, basicTempo * ims->header.nTickBeat);
			}

			if (samples > to_write_len) 
			{
				// 애드립 PCM 데이타를 만들어서 버퍼에 저장한다.
				YM3812UpdateOne(ym3812p, pcm_buffer_pos, to_write_len);
				remain_samples = samples - to_write_len;
				break;
			}
			else 
			{
				// 애드립 PCM 데이타를 만들어서 버퍼에 저장한다.
				YM3812UpdateOne(ym3812p, pcm_buffer_pos, samples);
				remain_samples = 0;
			}

			pcm_buffer_pos = pcm_buffer_pos + samples;
			to_write_len = to_write_len - samples;
		}

		if ( m_playMode == STOP || m_playMode == STOPPED || 
				m_playMode == SONG_END)
		{
			// 종료된 위치를 저장
			m_endBuffer = bufferIndex;
			m_endBufferIndex = i;
			return;
		}
	}
}

// 다음 사운드가 연주 가능할때 호출된다.
void callback(void *userdata, Uint8 *audio, int len)
{
	// 버퍼에 싸아놓고 PCM 데이타를 보내는 형식이라
	// 바로 체크를 해서는 안된다.
	//if ( m_playMode == STOP || m_playMode == STOPPED ) return;

	MainFrame *self = (MainFrame *)userdata;

	// 연주할 버퍼를 얻는다.
	Uint8 *buffer = pcm_buffer[m_playingBuffer]
		[m_playingBufferIndex];
	
	// 현재 연주중인 틱
	int tick = song_tick[m_playingBuffer][m_playingBufferIndex];

	// 프로그레스바 업데이트
	self->m_slider->SetValue((float)tick);
				
	// 가사 업데이트
	ISS_RECORD *record = lyrics_buffer[m_playingBuffer][m_playingBufferIndex];
	if ( record != NULL )
	{
		int line = record->line;

		// 속도를 위해 이미 출력했던 라인의 가사는 출력하지 않는다.
		// 문제점은 정확한 가사 추적이 안된다.
		if ( m_lastLyricsLine != line )
		{
			m_lastLyricsLine = line;

			int from = 0;
			int start_x = record->start_x;
			int to = start_x - record->width_x;

			Iss *m_iss = self->m_thread->GetIss();
			char *lyrics = m_iss->m_script[line].script;

			if(lyrics != NULL)
			{
				//wxColour fg = wxColour(wxT("#FFFFFF"));
				//wxColour bg = wxColour(wxT("#A8E5F6"));
				//self->m_issViewer->SetRangeColour(0, to, fg, bg);
				self->m_issViewer->SetText(lyrics);
			}
		}
	}

	// FFT 연산후 이퀼라이저를 그린다.
	self->FFT((void *)buffer, len);
	
	// 사운드 카드로 pcm 데이타를 보내어 소리를 낸다.
	SDL_MixAudio(audio, buffer, len, m_volume);

	// 마지막까지 연주가 되었는지 검사한다.
	if ( m_endBuffer == m_playingBuffer &&
		m_endBufferIndex == m_playingBufferIndex )
	{
		m_playMode=STOP;
		return;
	}
	
	// 다음 버퍼를 미리 준비한다.
	if ( m_playingBufferIndex == 0 )
	{
		if ( m_playingBuffer == 0 )
			prepare_next(self, PREPARE_BUFFER_SIZE, 1);
		else
			prepare_next(self, PREPARE_BUFFER_SIZE, 0);
	}

	m_playingBufferIndex++;

	// 버퍼의 마지막이 연주 되었으면 다음 버퍼로 교체한다.
	if ( m_playingBufferIndex == PREPARE_BUFFER_COUNT )
	{
		m_playingBufferIndex=0;

		if (m_playingBuffer == 0 )
			m_playingBuffer = 1;
		else
			m_playingBuffer = 0;
	}
}

#include "FFT.hpp"
using namespace Ernzo::DSP;

bool MainFrame::FFT(void *pcm_buffer, int len)
{
	if ( len == 0 ) return false;

	int samples = len/GetBytePerSampleSize();	// 512 samples

    std::vector<double> m_realIn;

	m_realIn.resize( samples );
	
	const short *data = reinterpret_cast<const short*>(pcm_buffer);
	for (int i = 0, j=0; i < samples; ++i, j++)
	{
		m_realIn[i] = (double)( data[j] );
	}
	
    std::vector<double> m_RealOut;
    std::vector<double> m_ImagOut;
    std::vector<double> m_Ampl;

	m_RealOut.resize( samples );
	m_ImagOut.resize( samples );
	m_Ampl.resize( samples );

	FFT::Compute<double>(samples, &m_realIn[0], NULL, 
			&m_RealOut[0], &m_ImagOut[0]);
	FFT::Norm<double>(samples/2, &m_RealOut[0], &m_ImagOut[0], &m_Ampl[0]);

	// 2^7, 2^15
	double maxAmpl = (m_spec.format == AUDIO_U8) ? 
		(127.0*127.0) : (32767.0*32767.0);

    std::vector<int> meterData;
    meterData.resize( NUM_FREQUENCY );
	
	// update meter
	for(int i=0; i < NUM_FREQUENCY; ++i)
	{
		int indice = (int)( 
				METER_FREQUENCY[i] * samples / m_spec.freq);
		int value  = (int)(20.0*log10(m_Ampl[indice]/maxAmpl));
		meterData[i] = value;
	}

	m_meter->SetData(&meterData[0], 0, NUM_FREQUENCY);

	return true;
}

void MainFrame::OnCloseApp(wxCloseEvent& WXUNUSED(event))
{
	m_mgr.UnInit();
	exit(0);
}

void MainFrame::OnLeftDClicked( wxListEvent& event)
{
	wxListItem info;
	info.m_itemId = event.m_itemIndex;
	info.m_col = 0;
	info.m_mask = wxLIST_MASK_TEXT;
	
	// 기존 아이콘 제거	
	m_playList->SetItemColumnImage(m_playingItem, 1, 0);
	
	if ( m_playList->GetItem(info) )
	{
		if ( m_playMode == PLAYING )
			Stop();

		m_playingItem = event.m_itemIndex;
		m_playList->SetItemColumnImage(m_playingItem, 1, 1);

		wxString imsFile = info.m_text;
		Play(imsFile);
	}
}

wxString MainFrame::GetTimeFormat(int sec)
{
	int h = sec/3600;
	int m = (sec%3600)/60;
	int s = (sec%3600)%60;

	return wxString::Format(wxT("%02d:%02d"), m, s);
}

bool MainFrame::Stop()
{
	if ( m_playMode == PLAYING )
	{
		m_timer->Stop();
		m_thread->Stop();

		while(1)
		{

			wxMilliSleep(100);
			if ( m_playMode == STOPPED )
				break;
		}
		
	}

	m_slider->SetRange((float)0, (float)100);
	m_slider->SetValue((float)0);
	m_issViewer->Clear();

	return true;
}

long MainFrame::GetSelectItem()
{
	long item = -1;
	int i=0;
	for ( ;; )
	{
		if ( i == m_playList->GetItemCount() ) return -1;

		item = m_playList->GetNextItem(item, 
				wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if ( item != -1 )
			return item;
		i++;
	}

	return -1;
}

long MainFrame::GetNextItem()
{
	//long item = GetSelectItem();
	long item = m_playingItem;

	for ( ;; )
	{
		item = m_playList->GetNextItem(item, wxLIST_NEXT_ALL);
		if ( item != -1 )
			return item;
		else
			break;
	}

	return -1;
}

long MainFrame::GetPrevItem()
{
	long item = m_playingItem;

	for ( ;; )
	{
		item = m_playList->GetNextItem(item, wxLIST_NEXT_ABOVE);
		if ( item != -1 )
			return item;
		else
			break;
	}

	return -1;
}

void MainFrame::Play(long item)
{
	if ( item == -1 )
		item = GetSelectItem();

	if ( item == -1 ) return;

	wxListItem info;
	info.m_itemId = item;
	info.m_col = 0;
	info.m_mask = wxLIST_MASK_TEXT;

	if ( m_playList->GetItem(info) )
	{
		if ( m_playMode == PLAYING )
			Stop();

		// 기존 아이콘 제거	
		m_playList->SetItemColumnImage(m_playingItem, 1, 0);

		m_playingItem = item;
		m_playList->SetItemColumnImage(m_playingItem, 1, 1);

		wxString path = info.m_text;
		Play(path);
	}
}

void MainFrame::Next()
{
	if ( m_playList->GetItemCount() == 0 ) return;

	long item = GetNextItem();
	if ( item == -1 ) return;

	Play(item);
}

void MainFrame::Prev()
{
	if ( m_playList->GetItemCount() == 0 ) return;

	long item = GetPrevItem();
	if ( item == -1 ) return;

	Play(item);
}

void MainFrame::Play(wxString imsFile)
{
	wxFileName file(imsFile);

	// bnk
	file.SetExt(wxT("bnk"));
	wxString bnkFile = file.GetFullPath();

	// iss
	file.SetExt(wxT("iss"));
	wxString issFile = file.GetFullPath();

	Bnk *bnk = new Bnk();

	// 뱅크 파일과 악기 갯수 표시
	if ( wxFileName::FileExists(bnkFile) == false )
	{
		bnkFile = wxT("STANDARD.BNK");
		bnk->Open(bnkFile);
		m_statusBar->SetStatusText(bnkFile + 
			wxString::Format(wxT(" (%d)"), bnk->GetTotalInst()), 1);
	}
	else
	{
		wxFileName file2(bnkFile);
		bnk->Open(file2.GetFullPath());
		m_statusBar->SetStatusText(file2.GetFullName() + 
			wxString::Format(wxT(" (%d)"), bnk->GetTotalInst()), 1);
	}

	delete bnk;

	// 가사 유무 체크
	if ( wxFileName::FileExists(issFile) == false )
	{
		m_statusBar->SetStatusText(wxT("No Lyrics."), 2);
	}
	else
	{
		m_statusBar->SetStatusText( file.GetFullName(), 2);
	}

	// play
	m_thread = new PlayThread(this, m_meter, bnkFile, imsFile, issFile);
	m_thread->Create();
	m_thread->SetPriority(WXTHREAD_DEFAULT_PRIORITY);
	m_thread->Run();
	
	Ims *m_ims = m_thread->GetIms();
	int totalTick = m_ims->GetTotalTick();
	m_slider->SetRange((float)0, (float)totalTick);
		
	m_timer->Start(1000);
}

void MainFrame::OnSlider( wxCommandEvent &WXUNUSED(event) )
{
	long pos = m_slider->GetValue();
}

void MainFrame::AddFile (wxString path)
{
	Ims *ims = new Ims();
	ims->Open(path);
	
	int index = m_playList->GetItemCount();

	// full path
	long item = m_playList->InsertItem(index, path);
	
	// icon
	m_playList->SetItem(index, 1, wxT("P"), 0);
	
	// file
	wxFileName file(path);
	m_playList->SetItem(index, 2, file.GetFullName());

	// title
	m_playList->SetItem(index, 3, ims->GetTitle());
	
	// duration
	long du = ims->GetDuration();
	int m = (du%3600)/60;
	int s = (du%3600)%60;

	m_playList->SetItem(index, 4, 
			wxString::Format(wxT("%02d:%02d"), m, s));
	
	// Singer
	wxFileName issFile(path);
	issFile.SetExt(wxT("iss"));

	Iss *iss = new Iss();
	if ( iss->Open(issFile.GetFullPath()) == true )
	{
		m_playList->SetItem(index, 5, iss->GetSinger());
	}

	delete iss;

	// total tick
	//long ticks = ims->GetTotalTick();
	//m_playList->SetItem(index, 5, wxString::Format(wxT("%d"), ticks));

	delete ims;
}

void MainFrame::OnSize(wxSizeEvent& event)
{
}

wxBitmap wxGetBitmapFromMemory(const void * data, size_t length)
{
	wxMemoryInputStream stream(data, length);
	wxImage image(stream);
	return wxBitmap(image);
}
