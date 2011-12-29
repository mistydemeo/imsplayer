//// GButton.cpp ///////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////

#include "GButton.h" 

DEFINE_EVENT_TYPE(wxEVT_COMMAND_GBUTTON) 

IMPLEMENT_DYNAMIC_CLASS(GButtonEvent, wxCommandEvent)  // RTTI

///////////////////////////////////////////////////////////
GButtonEvent::GButtonEvent(wxEventType commandType, int id) 
		: wxCommandEvent(commandType, id) 
{ 
} 

////////////////////////////////////
BEGIN_EVENT_TABLE(GButton, wxPanel) 
	EVT_PAINT  (GButton::OnPaint) 
	EVT_MOUSE_EVENTS (GButton::OnMouse) 
	EVT_ERASE_BACKGROUND(GButton::OnEraseBackground) 
END_EVENT_TABLE() 

//////////////////
GButton::GButton() 
{ 
	#ifndef LINUX
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	#endif //LINUX
} 

///////////////////
GButton::~GButton() 
{ 
} 

/////////////////////////////////////////////////////////////////////////////////////////////
GButton::GButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxBitmap& button) 
{ 
	#ifndef LINUX
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	#endif //LINUX
	(void)Create(parent, id, pos, button, button, button); 
} 

/////////////////////////////////////////////////////////////////////////////////////////////
GButton::GButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxString& button) 
{ 
	#ifndef LINUX
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	#endif //LINUX
	(void)Create(parent, id, pos, button, button, button); 
} 

/////////////////////////////////////////////////////////////////////////////////////////////
GButton::GButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const char** button)
{
	#ifndef LINUX
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	#endif //LINUX
	(void)Create(parent, id, pos, button, button, button); 
}

/////////////////////////////////////////////////////////////////////////////////////////////
GButton::GButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxBitmap& button, 
				 const wxBitmap& hover) 
{
	#ifndef LINUX
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	#endif //LINUX
	(void)Create(parent, id, pos, button, hover); 
} 

/////////////////////////////////////////////////////////////////////////////////////////////
GButton::GButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxString& button,
			const wxString& hover)
{
	#ifndef LINUX
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	#endif //LINUX
	(void)Create(parent, id, pos, button, hover); 
}

/////////////////////////////////////////////////////////////////////////////////////////////
GButton::GButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const char** button,
			const char** hover)
{
	#ifndef LINUX
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	#endif //LINUX
	(void)Create(parent, id, pos, button, hover); 
}

/////////////////////////////////////////////////////////////////////////////////////////////
GButton::GButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxBitmap& button, 
				 const wxBitmap& hover, const wxBitmap& click) 
{ 
	#ifndef LINUX
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	#endif //LINUX
	(void)Create(parent, id, pos, button, hover, click); 
} 

/////////////////////////////////////////////////////////////////////////////////////////////
GButton::GButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxString& button,
			const wxString& hover, const wxString& click)
{
	#ifndef LINUX
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	#endif //LINUX
	(void)Create(parent, id, pos, button, hover, click); 
}

/////////////////////////////////////////////////////////////////////////////////////////////
GButton::GButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const char** button,
			const char** hover, const char** click)
{
	#ifndef LINUX
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	#endif //LINUX
	(void)Create(parent, id, pos, button, hover, click); 
}

/////////////////////////////////////////////////////////////////////////////////////////////
GButton::GButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const char** button,
			const char** hover, const char** click, const char** focusedButton,
			const char** focusedHover, const char** focusedClick)
{
	#ifndef LINUX
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	#endif //LINUX
	(void)Create(parent, id, pos, button, hover, click, focusedButton, focusedHover, focusedClick); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////
bool GButton::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxBitmap& button) 
{ 
	c_button = button.GetSubBitmap( wxRect( 0, 0, button.GetWidth(), button.GetHeight() ) );
  
	if(!wxPanel::Create(parent, id, pos, wxSize(button.GetWidth(), button.GetHeight()), 0, wxT(""))) 
		return false; 

	allowHover = false;
	allowClick = false;
	focused = false;
	return true; 
}

/////////////////////////////////////////////////////////////////////////////////////////////////
bool GButton::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxString& button) 
{ 
	const wxBitmap &temp = wxImage(button, wxBITMAP_TYPE_BMP);
	c_button = temp.GetSubBitmap( wxRect( 0, 0, temp.GetWidth(), temp.GetHeight() ) );

	if(!wxPanel::Create(parent, id, pos, wxSize(temp.GetWidth(), temp.GetHeight()), 0, wxT(""))) 
		return false; 

	allowHover = false;
	allowClick = false;
	focused = false;
	return true; 
} 

//////////////////////////////////////////////////////////////////////////////////////////////
bool GButton::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const char** button) 
{ 
	const wxBitmap &temp = button;//wxImage(button);
	c_button = temp.GetSubBitmap( wxRect( 0, 0, temp.GetWidth(), temp.GetHeight() ) );
  
	if(!wxPanel::Create(parent, id, pos, wxSize(temp.GetWidth(), temp.GetHeight()), 0, wxT(""))) 
		return false; 

	allowHover = false;
	allowClick = false;
	focused = false;
	return true; 
} 

/////////////////////////////////////////////////////////////////////////////////////////////////
bool GButton::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxBitmap& button, 
					 const wxBitmap& hover) 
{ 
	c_button = button.GetSubBitmap( wxRect( 0, 0, button.GetWidth(), button.GetHeight() ) );
	c_hover = hover.GetSubBitmap( wxRect( 0, 0, hover.GetWidth(), hover.GetHeight() ) );
  
	if(!wxPanel::Create(parent, id, pos, wxSize(button.GetWidth(), button.GetHeight()), 0, wxT(""))) 
		return false; 

	allowHover = true;
	allowClick = false;
	focused = false;
	return true; 
}

/////////////////////////////////////////////////////////////////////////////////////////////////
bool GButton::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxString& button, 
					 const wxString& hover) 
{ 
	const wxBitmap &temp = wxImage(button, wxBITMAP_TYPE_BMP);
	const wxBitmap &temp2 = wxImage(hover, wxBITMAP_TYPE_BMP);
	c_button = temp.GetSubBitmap( wxRect( 0, 0, temp.GetWidth(), temp.GetHeight() ) );
	c_hover = temp2.GetSubBitmap( wxRect( 0, 0, temp2.GetWidth(), temp2.GetHeight() ) );
  
	if(!wxPanel::Create(parent, id, pos, wxSize(temp.GetWidth(), temp.GetHeight()), 0, wxT(""))) 
		return false; 

	allowHover = true;
	allowClick = false;
	focused = false;
	return true; 
} 

//////////////////////////////////////////////////////////////////////////////////////////////
bool GButton::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const char** button,
					 const char** hover) 
{ 
	const wxBitmap &temp = button;//wxImage(button);
	const wxBitmap &temp2 = hover;//wxImage(hover);
	c_button = temp.GetSubBitmap( wxRect( 0, 0, temp.GetWidth(), temp.GetHeight() ) );
	c_hover = temp2.GetSubBitmap( wxRect( 0, 0, temp2.GetWidth(), temp2.GetHeight() ) );
  
	if(!wxPanel::Create(parent, id, pos, wxSize(temp.GetWidth(), temp.GetHeight()), 0, wxT(""))) 
		return false; 

	allowHover = true;
	allowClick = false;
	focused = false;
	return true; 
} 

/////////////////////////////////////////////////////////////////////////////////////////////////
bool GButton::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxBitmap& button, 
					 const wxBitmap& hover, const wxBitmap& click) 
{ 
	c_button = button.GetSubBitmap( wxRect( 0, 0, button.GetWidth(), button.GetHeight() ) );
	c_hover = hover.GetSubBitmap( wxRect( 0, 0, hover.GetWidth(), hover.GetHeight() ) );
	c_click = click.GetSubBitmap( wxRect( 0, 0, click.GetWidth(), click.GetHeight() ) );
  
	if(!wxPanel::Create(parent, id, pos, wxSize(button.GetWidth(), button.GetHeight()), 0, wxT(""))) 
		return false; 

	allowHover = true;
	allowClick = true;
	focused = false;
	return true; 
}

/////////////////////////////////////////////////////////////////////////////////////////////////
bool GButton::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxString& button, 
					 const wxString& hover, const wxString& click) 
{ 
	const wxBitmap &temp = wxImage(button, wxBITMAP_TYPE_BMP);
	const wxBitmap &temp2 = wxImage(hover, wxBITMAP_TYPE_BMP);
	const wxBitmap &temp3 = wxImage(click, wxBITMAP_TYPE_BMP);
	c_button = temp.GetSubBitmap( wxRect( 0, 0, temp.GetWidth(), temp.GetHeight() ) );
	c_hover = temp2.GetSubBitmap( wxRect( 0, 0, temp2.GetWidth(), temp2.GetHeight() ) );
	c_click = temp3.GetSubBitmap( wxRect( 0, 0, temp3.GetWidth(), temp3.GetHeight() ) );
  
	if(!wxPanel::Create(parent, id, pos, wxSize(temp.GetWidth(), temp.GetHeight()), 0, wxT(""))) 
		return false; 

	allowHover = true;
	allowClick = true;
	focused = false;
	return true; 
} 

//////////////////////////////////////////////////////////////////////////////////////////////
bool GButton::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const char** button,
					 const char** hover, const char** click) 
{ 
	const wxBitmap &temp = button;//wxImage(button);
	const wxBitmap &temp2 = hover;//wxImage(hover);
	const wxBitmap &temp3 = click;//wxImage(click);
	c_button = temp.GetSubBitmap( wxRect( 0, 0, temp.GetWidth(), temp.GetHeight() ) );
	c_hover = temp2.GetSubBitmap( wxRect( 0, 0, temp2.GetWidth(), temp2.GetHeight() ) );
	c_click = temp3.GetSubBitmap( wxRect( 0, 0, temp3.GetWidth(), temp3.GetHeight() ) );
  
	if(!wxPanel::Create(parent, id, pos, wxSize(temp.GetWidth(), temp.GetHeight()), 0, wxT(""))) 
		return false; 

	allowHover = true;
	allowClick = true;
	focused = false;
	return true; 
} 

//////////////////////////////////////////////////////////////////////////////////////////////
bool GButton::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const char** button,
					 const char** hover, const char** click, const char** focusedButton,
					 const char** focusedHover, const char** focusedClick) 
{ 
	const wxBitmap &temp = button;//wxImage(button);
	const wxBitmap &temp2 = hover;//wxImage(hover);
	const wxBitmap &temp3 = click;//wxImage(click);
	const wxBitmap &temp4 = focusedButton;//wxImage(focusedButton);
	const wxBitmap &temp5 = focusedHover;//wxImage(focusedHover);
	const wxBitmap &temp6 = focusedClick;//wxImage(focusedClick);
	c_button = temp.GetSubBitmap( wxRect( 0, 0, temp.GetWidth(), temp.GetHeight() ) );
	c_hover = temp2.GetSubBitmap( wxRect( 0, 0, temp2.GetWidth(), temp2.GetHeight() ) );
	c_click = temp3.GetSubBitmap( wxRect( 0, 0, temp3.GetWidth(), temp3.GetHeight() ) );
	c_focusedButton = temp4.GetSubBitmap( wxRect( 0, 0, temp4.GetWidth(), temp4.GetHeight() ) );
	c_focusedHover = temp5.GetSubBitmap( wxRect( 0, 0, temp5.GetWidth(), temp5.GetHeight() ) );
	c_focusedClick = temp6.GetSubBitmap( wxRect( 0, 0, temp6.GetWidth(), temp6.GetHeight() ) );
  
	if(!wxPanel::Create(parent, id, pos, wxSize(temp.GetWidth(), temp.GetHeight()), 0, wxT(""))) 
		return false; 

	allowHover = true;
	allowClick = true;
	focused = false;
	return true; 
} 

////////////////////////////////////////////////////
void GButton::OnPaint(wxPaintEvent &WXUNUSED(event)) 
{ 
	wxPaintDC dc(this); 
	DrawBackground(dc); 
} 

void GButton::OnEraseBackground(wxEraseEvent& event)
{
	
}

void GButton::SetBitmap(wxBitmap button, wxBitmap hover)
{
	c_button = button;
	c_hover = hover;
	Refresh();
}

//////////////////////////////////////////
void GButton::OnMouse(wxMouseEvent &event) 
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
		GButtonEvent event(wxEVT_COMMAND_GBUTTON, GetId()); 
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
void GButton::DrawBackground(wxDC &dc) 
{ 
	if(!focused){
		if(hover == true && allowHover){
			dc.DrawBitmap( c_hover, 0, 0);
		}
		if(click == true && allowClick){
			dc.DrawBitmap( c_click, 0, 0);
		}
		if(hover != true && click != true){
			dc.DrawBitmap( c_button, 0, 0);
		}
	}
	else{
		if(hover == true && allowHover){
			dc.DrawBitmap( c_focusedHover, 0, 0);
		}
		if(click == true && allowClick){
			dc.DrawBitmap( c_focusedClick, 0, 0);
		}
		if(hover != true && click != true){
			dc.DrawBitmap( c_focusedButton, 0, 0);
		}
	}
}
