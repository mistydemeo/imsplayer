// vim: set fdm=marker ts=4 sw=4:

#ifndef _BASE_H_
#define _BASE_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include <wx/generic/aboutdlgg.h>
#include <wx/aboutdlg.h>
#include <wx/url.h>         //for wxURL/wxURI
#include <wx/cmdline.h>
#include <wx/dnd.h>
#include <wx/process.h>
#include <wx/txtstrm.h>
#include <wx/list.h>
#include <wx/tokenzr.h>
#include <wx/app.h>
#include <wx/grid.h>
#include <wx/laywin.h>
#include <wx/treectrl.h>
#include <wx/spinctrl.h>
#include <wx/artprov.h>
#include <wx/clipbrd.h>
#include <wx/combobox.h>
#include <wx/colordlg.h>
#include <wx/wxhtml.h>
#include <wx/imaglist.h>
#include <wx/dataobj.h>
#include <wx/dcclient.h>
#include <wx/bmpbuttn.h>
#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/tooltip.h>
#include <wx/statusbr.h>
#include <wx/msgdlg.h>
#include <wx/textdlg.h>
#include <wx/timer.h>
#include <wx/statline.h>
#include <wx/listctrl.h>
#include <wx/dir.h>
#include <wx/dirctrl.h>
#include <wx/file.h>
#include <wx/mimetype.h>
#include <wx/regex.h>
#include <wx/taskbar.h>
#include <wx/xml/xml.h>
#include <wx/aui/aui.h>
#include <wx/stc/stc.h>  
#include <wx/ffile.h>
#include <wx/bitmap.h> 
#include <wx/mstream.h> 
#include <wx/stream.h> 
#include <wx/sstream.h>
#include <wx/config.h>
#include <wx/confbase.h>
#include <wx/fileconf.h> 
#include <wx/fontenum.h>
#include <wx/socket.h>
#include <wx/hash.h>
#include <wx/clipbrd.h>
#include <wx/thread.h>
#include <wx/image.h>
#include <wx/dcbuffer.h>
#include <wx/colordlg.h>
#include <wx/popupwin.h>
#include <wx/strconv.h>

// boost
#include <boost/shared_ptr.hpp>
#include <boost/regex.hpp>

// stl
#include <vector>
#include <map>
#include <algorithm>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

wxBitmap wxGetBitmapFromMemory(const void * data, size_t length);

// adlib
#include "adlib.h"
#include "outchip.h"
#include "setfreq.h"
#include "fmopl.h"
#include "convert.h"
#include "Bnk.h"
#include "Ims.h"
#include "PeakMeter.h"
#include "Sound.h"
#include "ActionPanel.h"
#include "SliderPanel.h"
#include "IssViewer.h"
#include "Hangul.h"
#include "Iss.h"


// gfxwidgets
#include "GButton.h"
#include "GSlider.h"
#include "GHanLabel.h"

using namespace std;

#define STOP		0
#define PLAYING		1
#define STOPPED		2
#define SONG_END	3

#define REPEAT_NO		0	// 지금 곡 연주하고 중지
#define REPEAT_ALL		1	// 지금 곡 연주 끝나면 다음곡으로..
#define REPEAT_THIS		2	// 지금 곡 반복 연주

#define PREPARE_BUFFER_COUNT	20
#define PREPARE_BUFFER_SIZE		512*2

// --------------------------------------------
extern wxString exePath;

extern int m_volume;
extern int m_playMode;
extern int m_repeatMode;

extern int m_playingBuffer;
extern int m_playingBufferIndex;

void init_prepare_pcm_buffer(void);
void alloc_prepare_pcm_buffer (int bufferIndex);
void free_prepare_pcm_buffer (int bufferIndex);
void prepare_next (void *userdata, int len, int bufferIndex);

// --------------------------------------------
enum {
	ID_INVOKE_APP = wxID_HIGHEST+1,

	ID_OPEN_FILE,

	ID_ABORT,
	ID_START,
	ID_RUN,
	ID_GO,
	ID_STEP_INTO,
	ID_STEP_OUT,
	ID_STEP_OVER,
	ID_RUN_TO_CURSOR,
	ID_TOGGLE_BREAKPOINT,
	ID_CLEAR_ALL_BREAKPOINTS,
	ID_STOP,

	ID_VIEW_CONSOLE,
	ID_VIEW_WATCH,
	ID_VIEW_CALLSTACK,
	ID_VIEW_BREAKPOINT,
	ID_VIEW_REGISTER,
	ID_VIEW_MEMORY,

	ID_REPEAT_NO,
	ID_REPEAT_THIS,
	ID_REPEAT_ALL,
};

// --------------------------------------------
#define MENU_BAR					myFrame->m_menuBar
#define STATUS_BAR					myFrame->m_statusBar
#define CONFIG						myFrame->m_config
#define M_MGR						myFrame->m_mgr
#define MEMORY						myFrame->m_memory
#define SLIDER						myFrame->m_slider

#define GET_BITMAP(name)	\
	wxBitmap(exePath + wxT("/icons/") + \
			wxString(name, wxConvUTF8) + \
			wxT(".png"), wxBITMAP_TYPE_PNG)
#endif
