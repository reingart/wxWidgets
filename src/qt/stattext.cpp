/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/stattext.cpp
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/stattext.h"
#include "wx/qt/converter.h"

wxStaticText::wxStaticText()
{
}

wxStaticText::wxStaticText(wxWindow *parent,
             wxWindowID id,
             const wxString &label,
             const wxPoint &pos,
             const wxSize &size,
             long style,
             const wxString &name)
{
    Create( parent, id, label, pos, size, style, name );
}

bool wxStaticText::Create(wxWindow *parent,
            wxWindowID id,
            const wxString &label,
            const wxPoint &pos,
            const wxSize &size,
            long style,
            const wxString &name)
{
    m_qtWindow = new QLabel( wxQtConvertString( label ), parent->GetHandle() );

    // Set the buddy to itself to get the mnemonic key but ensure that we don't have
    // any unwanted side effects, so disable the interaction:

    GetQLabel()->setBuddy( GetQLabel() );
    GetQLabel()->setTextInteractionFlags( Qt::NoTextInteraction );
    GetQLabel()->setWordWrap( true );

    return QtCreateControl( parent, id, pos, size, style, wxDefaultValidator, name );
}

void wxStaticText::SetLabel(const wxString& label)
{
    GetQLabel()->setText( wxQtConvertString( label ) );
}
