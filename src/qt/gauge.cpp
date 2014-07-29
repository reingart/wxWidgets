/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/gauge.cpp
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/gauge.h"
#include "wx/qt/converter.h"
#include "wx/qt/private/winevent.h"


class wxQtProgressBar : public wxQtEventSignalHandler< QProgressBar, wxGauge >
{
public:
    wxQtProgressBar( wxWindow *parent, wxGauge *handler );

private:
    void valueChanged(int value);
};

wxQtProgressBar::wxQtProgressBar( wxWindow *parent, wxGauge *handler )
    : wxQtEventSignalHandler< QProgressBar, wxGauge >( parent, handler )
{
}


wxGauge::wxGauge()
{
}

wxGauge::wxGauge(wxWindow *parent,
        wxWindowID id,
        int range,
        const wxPoint& pos,
        const wxSize& size,
        long style,
        const wxValidator& validator,
        const wxString& name)
{
    Create( parent, id, range, pos, size, style, validator, name );
}

bool wxGauge::Create(wxWindow *parent,
            wxWindowID id,
            int range,
            const wxPoint& pos,
            const wxSize& size,
            long style,
            const wxValidator& validator,
            const wxString& name)
{
    m_qtWindow = new wxQtProgressBar( parent, this);
    GetQProgressBar()->setOrientation( wxQtConvertOrientation( style, wxGA_HORIZONTAL ));
    GetQProgressBar()->setRange( 0, range );
    GetQProgressBar()->setTextVisible( style & wxGA_TEXT );

    return QtCreateControl( parent, id, pos, size, style, validator, name );
}

// set/get the control range and value

void wxGauge::SetRange(int range)
{
    // note that in wx minimun range is fixed at 0
    GetQProgressBar()->setMaximum(range);
}

int wxGauge::GetRange() const
{
    return GetQProgressBar()->maximum();
}

void wxGauge::SetValue(int pos)
{
    GetQProgressBar()->setValue(pos);
}

int wxGauge::GetValue() const
{
    return GetQProgressBar()->value();
}
