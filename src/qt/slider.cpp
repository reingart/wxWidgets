/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/slider.cpp
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/slider.h"
#include "wx/qt/converter.h"
#include "wx/qt/private/winevent.h"


class wxQtSlider : public wxQtEventSignalHandler< QSlider, wxSlider >
{
public:
    wxQtSlider( wxWindow *parent, wxSlider *handler );

private:
    void valueChanged(int position);
};

wxQtSlider::wxQtSlider( wxWindow *parent, wxSlider *handler )
    : wxQtEventSignalHandler< QSlider, wxSlider >( parent, handler )
{
    connect(this, &QSlider::valueChanged, this, &wxQtSlider::valueChanged);
}

void wxQtSlider::valueChanged(int position)
{
    wxCommandEvent event( wxEVT_SLIDER, GetHandler()->GetId() );
    event.SetInt( position );
    EmitEvent( event );
}


wxSlider::wxSlider()
{
}

wxSlider::wxSlider(wxWindow *parent,
         wxWindowID id,
         int value, int minValue, int maxValue,
         const wxPoint& pos,
         const wxSize& size,
         long style,
         const wxValidator& validator,
         const wxString& name)
{
    Create( parent, id, value, minValue, maxValue, pos, size, style, validator, name );
}

bool wxSlider::Create(wxWindow *parent,
            wxWindowID id,
            int value, int minValue, int maxValue,
            const wxPoint& pos,
            const wxSize& size,
            long style,
            const wxValidator& validator,
            const wxString& name)
{
    m_qtWindow = new wxQtSlider( parent, this );
    GetQSlider()->setOrientation( wxQtConvertOrientation( style, wxSL_HORIZONTAL ) );
    SetRange( minValue, maxValue );
    // draw ticks marks (default bellow if horizontal, right if vertical):
    if ( style & wxSL_VERTICAL )
    {
        GetQSlider()->setTickPosition( style & wxSL_LEFT ? QSlider::TicksLeft :
                                                         QSlider::TicksRight );
    }
    else // horizontal slider
    {
        GetQSlider()->setTickPosition( style & wxSL_TOP ? QSlider::TicksAbove :
                                                        QSlider::TicksBelow );
    }
    return QtCreateControl( parent, id, pos, size, style, validator, name );
}

int wxSlider::GetValue() const
{
    return GetQSlider()->value();
}

void wxSlider::SetValue(int value)
{
    GetQSlider()->setValue( value );
}

void wxSlider::SetRange(int minValue, int maxValue)
{
    GetQSlider()->setRange( minValue, maxValue );
}

int wxSlider::GetMin() const
{
    return GetQSlider()->minimum();
}

int wxSlider::GetMax() const
{
    return GetQSlider()->maximum();
}

void wxSlider::DoSetTickFreq(int freq)
{
    GetQSlider()->setTickInterval(freq);
}

int wxSlider::GetTickFreq() const
{
    return GetQSlider()->tickInterval();
}

void wxSlider::SetLineSize(int lineSize)
{
}

void wxSlider::SetPageSize(int pageSize)
{
    GetQSlider()->setPageStep(pageSize);
}

int wxSlider::GetLineSize() const
{
    return 0;
}

int wxSlider::GetPageSize() const
{
    return GetQSlider()->pageStep();
}

void wxSlider::SetThumbLength(int lenPixels)
{
}

int wxSlider::GetThumbLength() const
{
    return 0;
}
