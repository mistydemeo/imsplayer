#ifndef _SOUND_H_
#define _SOUND_H_

class MainPanel;
class MainFrame;
class PeakMeter;
class Bnk;
class Ims;
class Iss;
class Sound;

class PlayThread: public wxThread
{
public:
	PlayThread(MainFrame *parent, 
			PeakMeter *meter,
			wxString bnkPath,
			wxString imsPath,
			wxString issPath);

	Sound *m_sound;
	PeakMeter *m_meter;
	MainFrame *m_mainFrame;

	virtual void* Entry();
	virtual void OnExit();

	Sound *GetSound();
	Bnk *GetBnk();
	Ims *GetIms();
	Iss *GetIss();

	void Stop();
};

class Sound
{
public:
	MainFrame *myFrame;
	SDL_AudioSpec m_spec;
	Bnk *m_bnk;
	Ims *m_ims;
	Iss *m_iss;

	wxString m_file;
	
public:
	Sound(MainFrame *parent);
	~Sound();

	Ims *GetIms();
	Bnk *GetBnk();
	Iss *GetIss();

	void Stop();
	bool Play();
};

#endif
