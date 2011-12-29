//// GHanLabel.cpp ///////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////

#include "Main.h"

DEFINE_EVENT_TYPE(wxEVT_COMMAND_GHANLABEL) 

IMPLEMENT_DYNAMIC_CLASS(GHanLabelEvent, wxCommandEvent)  // RTTI

///////////////////////////////////////////////////////////
GHanLabelEvent::GHanLabelEvent(wxEventType commandType, int id) 
: wxCommandEvent(commandType, id) 
{ 
} 

////////////////////////////////////
BEGIN_EVENT_TABLE(GHanLabel, wxPanel) 
	EVT_PAINT  (GHanLabel::OnPaint) 
	EVT_MOUSE_EVENTS (GHanLabel::OnMouse) 
	EVT_ERASE_BACKGROUND(GHanLabel::OnEraseBackground) 
END_EVENT_TABLE() 

///////////////////////////////////////////////////////////////////////////////
GHanLabel::GHanLabel(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos, const wxSize& size, 
		char *text, const wxColour &fg, const wxColour &bg) 
{ 
	m_text = text;
	m_fg = fg;
	m_bg = bg;

	InitHangul();

	#ifndef LINUX
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	#endif //LINUX
	(void)Create(parent, id, pos, size); 
} 

///////////////////
GHanLabel::~GHanLabel() 
{
   delete m_han;	
} 

////////////////////////////////////////////////////
bool GHanLabel::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size) 
{
	//int width = strlen(m_text)*8;
	if(!wxPanel::Create(parent, id, pos, size, 0, wxT(""))) 
		return false; 

	allowHover = false;
	allowClick = false;
	focused = false;
	return true; 
}

////////////////////////////////////////////////////
void GHanLabel::InitHangul()
{
	m_han = new Hangul();

	m_han->SangYongCode();
	m_han->SetHanFontRoom(HAN_8x4x4);
	m_han->LoadHanFont(wxT("HAN.FNT"), HAN_8x4x4);

	m_han->SetEngFontRoom(ENG_256);
	m_han->LoadEngFont(wxT("ENG.FNT"), ENG_256);
	
	m_han->SetSpecialFontRoom(SPECIAL_CODE);
	// 조합형 특수문자
	//m_han->LoadSpecialFont("IKS.FNT", SPECIAL_CODE);
	// 삼보 특수문자
	m_han->LoadSpecialFont(wxT("ISPC.FNT"), SPECIAL_CODE);
	m_han->SetSpecialCharOn();
}

////////////////////////////////////////////////////
void GHanLabel::OnPaint(wxPaintEvent &WXUNUSED(event)) 
{ 
	wxPaintDC dc(this); 
	wxColour color;
	color.Set(wxT("#A8E5F6"));
	dc.SetBackground(wxBrush(color));
	dc.Clear();
	DrawHangul(dc); 
}
	
void GHanLabel::SetRangeColour(int from, int to, wxColour fg, wxColour bg)
{
	m_han->RangeColour(from, to, fg, bg);
}

void GHanLabel::SetLabel(char *text)
{
	wxPaintDC dc(this); 

	m_text = text;

	int width=strlen(text)*8;
	SetSize(wxSize(width,16));

	Refresh(false);
}

void GHanLabel::OnEraseBackground(wxEraseEvent& event)
{
}

//////////////////////////////////////////
void GHanLabel::OnMouse(wxMouseEvent &event) 
{ 

	if(allowHover && hover != true){
		hover = true;
		Refresh();
	}

	if(event.LeftDown()) 
	{ 
		if(allowClick){
			click = true;
			Refresh();
		}
	} 

	if(event.LeftUp())
	{
		click = false;
		Refresh();

		// Create and event 
		GHanLabelEvent event(wxEVT_COMMAND_GHANLABEL, GetId()); 
		event.SetEventObject(this); 

		// Send it 
		GetEventHandler()->ProcessEvent(event); 
	}

	if(event.Leaving())
	{
		hover = false;
		click = false;
		Refresh();
	}

	if(event.RightUp())
	{
		//Alter this to give the object it's own right click properties
		GetParent()->ProcessEvent(event); //Let right clicks bleed through to the parent.
	}
} 

//////////////////////////////////////
void GHanLabel::DrawHangul(wxDC &dc) 
{
	m_han->OutTextXy(dc, 0, 0, m_text, m_fg, m_bg);
	//dc.DrawBitmap( c_hover, 0, 0);
}
