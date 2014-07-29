/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/statline.cpp
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/statline.h"

wxStaticLine::wxStaticLine()
{
}

wxStaticLine::wxStaticLine( wxWindow *parent,
              wxWindowID id,
              const wxPoint& pos,
              const wxSize& size,
              long style,
              const wxString &name)
{
    Create( parent, id, pos, size, style, name );
}

bool wxStaticLine::Create( wxWindow *parent,
             wxWindowID id,
             const wxPoint& pos,
             const wxSize& size,
             long style,
             const wxString &name)
{
    m_qtWindow = new QFrame( parent->GetHandle() );
    if ( style & wxLI_HORIZONTAL )
        GetQFrame()->setFrameStyle( QFrame::HLine );
    else if ( style & wxLI_VERTICAL )
        GetQFrame()->setFrameStyle( QFrame::VLine );

    return QtCreateControl( parent, id, pos, size, style, wxDefaultValidator, name );
}
