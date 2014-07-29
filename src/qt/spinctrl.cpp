/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/spinctrl.cpp
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if wxUSE_SPINCTRL

#include "wx/qt/utils.h"
#include "wx/qt/converter.h"
#include "wx/qt/private/winevent.h"
#include "wx/spinctrl.h"

// Define a derived helper class to get access to valueFromText:

template < typename Widget >
class wxQtSpinBoxBase : public wxQtEventSignalHandler< Widget, wxControl >
{
public:
    wxQtSpinBoxBase( wxWindow *parent, wxControl *handler )
        : wxQtEventSignalHandler< Widget, wxControl >( parent, handler )
    { }

    using Widget::valueFromText;
};

template< typename T, typename Widget >
wxQtSpinCtrlBase< T, Widget >::wxQtSpinCtrlBase()
{
}

template< typename T, typename Widget >
bool wxQtSpinCtrlBase< T, Widget >::Create( wxWindow *parent, wxWindowID id,
    const wxString& value, const wxPoint& pos, const wxSize& size, long style,
    T min, T max, T initial, T inc, const wxString& name )
{
    wxMISSING_IMPLEMENTATION( wxSTRINGIZE( style ));
    SetValue( initial );
    SetRange( min, max );
    SetIncrement( inc );

    if ( !value.IsEmpty() )
        SetValue( value );

    return QtCreateControl( parent, id, pos, size, style, wxDefaultValidator, name );
}

template< typename T, typename Widget >
void wxQtSpinCtrlBase< T, Widget >::SetValue( T val )
{
    GetQSpinBox()->setValue( val );
}

template< typename T, typename Widget >
void wxQtSpinCtrlBase< T, Widget >::SetRange( T min, T max )
{
    GetQSpinBox()->setRange( min, max );
}

template< typename T, typename Widget >
void wxQtSpinCtrlBase< T, Widget >::SetIncrement( T inc )
{
    GetQSpinBox()->setSingleStep( inc );
}

template< typename T, typename Widget >
T wxQtSpinCtrlBase< T, Widget >::GetValue() const
{
    return GetQSpinBox()->value();
}

template< typename T, typename Widget >
T wxQtSpinCtrlBase< T, Widget >::GetMin() const
{
    return GetQSpinBox()->minimum();
}

template< typename T, typename Widget >
T wxQtSpinCtrlBase< T, Widget >::GetMax() const
{
    return GetQSpinBox()->maximum();
}

template< typename T, typename Widget >
T wxQtSpinCtrlBase< T, Widget >::GetIncrement() const
{
    return GetQSpinBox()->singleStep();
}

template< typename T, typename Widget >
void wxQtSpinCtrlBase< T, Widget >::SetSnapToTicks(bool WXUNUSED(snap_to_ticks))
{
    wxMISSING_FUNCTION();
}

template< typename T, typename Widget >
bool wxQtSpinCtrlBase< T, Widget >::GetSnapToTicks() const
{
    wxMISSING_FUNCTION();

    return false;
}

template< typename T, typename Widget >
void wxQtSpinCtrlBase< T, Widget >::SetSelection(long WXUNUSED(from), long WXUNUSED(to))
{
    wxMISSING_FUNCTION();
}

template < typename T, typename Widget >
void wxQtSpinCtrlBase< T, Widget >::SetValue( const wxString &value )
{
    GetQSpinBox()->setValue( GetQSpinBox()->valueFromText( wxQtConvertString( value )));
}


class wxQtSpinBox : public wxQtSpinBoxBase< QSpinBox >
{
public:
    wxQtSpinBox( wxWindow *parent, wxControl *handler )
        : wxQtSpinBoxBase< QSpinBox >( parent, handler )
    {
        connect(this, static_cast<void (QSpinBox::*)(int index)>(&QSpinBox::valueChanged),
                this, &wxQtSpinBox::valueChanged);
    }
private:
    void valueChanged(int value);
};

class wxQtDoubleSpinBox : public wxQtSpinBoxBase< QDoubleSpinBox >
{
public:
    wxQtDoubleSpinBox( wxWindow *parent, wxControl *handler )
        : wxQtSpinBoxBase< QDoubleSpinBox >( parent, handler )
    { }
};


//##############################################################################

template class wxQtSpinCtrlBase< int, QSpinBox >;

wxSpinCtrl::wxSpinCtrl()
{
    Init(); 
}

wxSpinCtrl::wxSpinCtrl(wxWindow *parent, wxWindowID id, const wxString& value,
    const wxPoint& pos, const wxSize& size, long style,
    int min, int max, int initial,
    const wxString& name )
    : wxQtSpinCtrlBase< int, QSpinBox >()
{
    Init();
    Create( parent, id, value, pos, size, style, min, max, initial, name );
}

bool wxSpinCtrl::Create( wxWindow *parent, wxWindowID id, const wxString& value,
    const wxPoint& pos, const wxSize& size, long style,
    int min, int max, int initial,
    const wxString& name )
{
    m_qtWindow = new wxQtSpinBox( parent, this );
    return wxQtSpinCtrlBase< int, QSpinBox >::Create( parent, id, value,
        pos, size, style, min, max, initial, 1, name );
}


bool wxSpinCtrl::SetBase(int base)
{
    // Currently we only support base 10.
    if ( base != 10 )
        return false;

    if ( base == m_base )
        return true;

    m_base = base;

    return true;
}

void wxQtSpinBox::valueChanged(int value)
{
    wxCommandEvent event( wxEVT_SPINCTRL, GetHandler()->GetId() );
    event.SetInt( value );
    EmitEvent( event );
}

//##############################################################################

template class wxQtSpinCtrlBase< double, QDoubleSpinBox >;

wxIMPLEMENT_DYNAMIC_CLASS( wxSpinCtrlDouble, wxSpinCtrlBase );

wxSpinCtrlDouble::wxSpinCtrlDouble()
{
}

wxSpinCtrlDouble::wxSpinCtrlDouble(wxWindow *parent, wxWindowID id, const wxString& value,
    const wxPoint& pos, const wxSize& size, long style,
    double min, double max, double initial, double inc,
    const wxString& name )
    : wxQtSpinCtrlBase< double, QDoubleSpinBox >()
{
    Create( parent, id, value, pos, size, style, min, max, initial, inc, name );
}

bool wxSpinCtrlDouble::Create(wxWindow *parent, wxWindowID id, const wxString& value,
    const wxPoint& pos, const wxSize& size, long style,
    double min, double max, double initial, double inc,
    const wxString& name )
{
    m_qtWindow = new wxQtDoubleSpinBox( parent, this );
    return wxQtSpinCtrlBase< double, QDoubleSpinBox >::Create( parent, id, value,
        pos, size, style, min, max, initial, inc, name );
}

void wxSpinCtrlDouble::SetDigits(unsigned digits)
{
    GetQSpinBox()->setDecimals( digits );
}

unsigned wxSpinCtrlDouble::GetDigits() const
{
    return GetQSpinBox()->decimals();
}

#endif // wxUSE_SPINCTRL
