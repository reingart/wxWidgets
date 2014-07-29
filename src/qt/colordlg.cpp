/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/colordlg.cpp
// Author:      Sean D'Epagnier
// Copyright:   (c) Sean D'Epagnier 2014
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/qt/private/winevent.h"
#include "wx/colordlg.h"

class wxQtColorDialog : public wxQtEventSignalHandler< QColorDialog, wxDialog >
{
public:
    wxQtColorDialog( wxWindow *parent, wxDialog *handler)
        : wxQtEventSignalHandler(parent, handler)
        { }
};

wxColourData wxColourDialog::s_data;

bool wxColourDialog::Create(wxWindow *parent, wxColourData *data )
{
    m_qtWindow = new wxQtColorDialog( parent, this );

    if(data)
        s_data = *data;

    if(s_data.GetChooseFull())
        for(int i=0; i<wxColourData::NUM_CUSTOM; i++)
            QColorDialog::setCustomColor(i, s_data.GetCustomColour(i).GetHandle());

    GetQColorDialog()->setCurrentColor(s_data.GetColour().GetHandle());

    return wxTopLevelWindow::Create( parent, wxID_ANY, "Color Dialog");
}

wxColourData &wxColourDialog::GetColourData()
{
    for(int i=0; i<wxColourData::NUM_CUSTOM; i++)
        s_data.SetCustomColour(i, GetQColorDialog()->customColor(i));
    
    s_data.SetColour(GetQColorDialog()->currentColor());
    
    return s_data;
}

