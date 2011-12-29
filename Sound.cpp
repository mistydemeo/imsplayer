#include "Main.h"

PlayThread::PlayThread(MainFrame *parent, 
		PeakMeter *meter,
		wxString bnkPath,
		wxString imsPath,
		wxString issPath)
	: wxThread()
{
	m_mainFrame = parent;
	m_meter = meter;
	m_sound = new Sound(parent);

	// BANK
	Bnk *bnk = m_sound->GetBnk();
	bnk->Open(bnkPath);

	// IMS
	Ims *ims = m_sound->GetIms();
	ims->Open(imsPath);
	ims->MatchBnk(bnk->GetBnk());
	
	char *title = ims->GetTitleJohab();
	//parent->m_issViewer->OutTextXy(0, 0, title);
	
	ims->Reset();

	// ISS
	Iss *iss = m_sound->GetIss();
	if ( iss->Open(issPath) == true )
	{
		char *writer = iss->GetWriter();
		//parent->m_issViewer->OutTextXy(0, 16, writer);
		char *composer = iss->GetComposer();
		//parent->m_issViewer->OutTextXy(0, 32, composer);
	}
	
	// PEAK METER
    m_meter->Stop();
	// Peak Meter 업데이트 주기
    m_meter->Start(30);	// 30ms
}

void PlayThread::OnExit()
{
	delete m_sound;
	m_playMode = STOPPED;
}

void* PlayThread::Entry()
{
	m_playMode = PLAYING;
	m_sound->Play();
}

void PlayThread::Stop()
{
	m_sound->Stop();
}

Ims* PlayThread::GetIms()
{
	return m_sound->GetIms();
}

Bnk* PlayThread::GetBnk()
{
	return m_sound->GetBnk();
}

Iss* PlayThread::GetIss()
{
	return m_sound->GetIss();
}

// ---------------------------------------------------------
Sound::Sound(MainFrame *parent)
{
	this->myFrame = parent;

	ym3812p = YM3812Init(3579545L, 22050);
	SoundWarmInit();

	m_ims = new Ims();
	m_bnk = new Bnk();
	m_iss = new Iss();
}

Sound::~Sound() 
{
	m_ims->Reset();

	delete m_ims;
	delete m_bnk;
	delete m_iss;
	
	YM3812Shutdown(ym3812p);
	
	free_prepare_pcm_buffer (0);
	free_prepare_pcm_buffer (1);
}

Ims *Sound::GetIms()
{
	return m_ims;
}
	
Bnk *Sound::GetBnk()
{
	return m_bnk;
}

Iss *Sound::GetIss()
{
	return m_iss;
}

void Sound::Stop()
{
	m_playMode=STOP;
}

bool Sound::Play()
{
	init_prepare_pcm_buffer();

	alloc_prepare_pcm_buffer(0);
	alloc_prepare_pcm_buffer(1);

	prepare_next(myFrame, PREPARE_BUFFER_SIZE, 0);
		
	SDL_PauseAudio(0);
	
	while(1)
	{
		wxMilliSleep(100);
		
		if ( m_playMode == STOP )
			break;
	}

	SDL_PauseAudio(1);
}
