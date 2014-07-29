/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/textctrl.cpp
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/textctrl.h"
#include "wx/qt/converter.h"
#include "wx/qt/private/winevent.h"
#include "wx/qt/utils.h"

class wxQtLineEdit : public wxQtEventSignalHandler< QLineEdit, wxTextCtrl >
{
public:
    wxQtLineEdit( wxWindow *parent, wxTextCtrl *handler );

private:
    void textChanged(const QString &text);
    void returnPressed();
};

wxQtLineEdit::wxQtLineEdit( wxWindow *parent, wxTextCtrl *handler )
    : wxQtEventSignalHandler< QLineEdit, wxTextCtrl >( parent, handler )
{
    connect(this, &QLineEdit::textChanged,
            this, &wxQtLineEdit::textChanged);
    connect(this, &QLineEdit::returnPressed,
            this, &wxQtLineEdit::returnPressed);
}

void wxQtLineEdit::textChanged(const QString &text)
{
    wxCommandEvent event( wxEVT_TEXT, GetHandler()->GetId() );
    event.SetString( wxQtConvertString( text ) );
    EmitEvent( event );
}

void wxQtLineEdit::returnPressed()
{
    if ( GetHandler()->HasFlag(wxTE_PROCESS_ENTER) )
    {
        wxCommandEvent event( wxEVT_TEXT_ENTER, GetHandler()->GetId() );
        event.SetString( GetHandler()->GetValue() );
        EmitEvent( event );
    }
}


class wxQtTextEdit : public wxQtEventSignalHandler< QTextEdit, wxTextCtrl >
{
public:
    wxQtTextEdit( wxWindow *parent, wxTextCtrl *handler );

private:
    void textChanged();
};

wxQtTextEdit::wxQtTextEdit( wxWindow *parent, wxTextCtrl *handler )
    : wxQtEventSignalHandler< QTextEdit, wxTextCtrl >( parent, handler )
{
    connect(this, &QTextEdit::textChanged,
            this, &wxQtTextEdit::textChanged);
}

void wxQtTextEdit::textChanged()
{
    wxCommandEvent event( wxEVT_TEXT, GetHandler()->GetId() );
    event.SetString( GetHandler()->GetValue() );
    EmitEvent( event );
}


wxTextCtrl::wxTextCtrl()
{
}

wxTextCtrl::wxTextCtrl(wxWindow *parent,
           wxWindowID id,
           const wxString &value,
           const wxPoint &pos,
           const wxSize &size,
           long style,
           const wxValidator& validator,
           const wxString &name)
{
    Create( parent, id, value, pos, size, style, validator, name );
}

bool wxTextCtrl::Create(wxWindow *parent,
            wxWindowID id,
            const wxString &value,
            const wxPoint &pos,
            const wxSize &size,
            long style,
            const wxValidator& validator,
            const wxString &name)
{
    bool multiline = (style & wxTE_MULTILINE) != 0;

    if (!multiline)
        m_qtWindow = new wxQtLineEdit( parent, this );
    else
        m_qtWindow = new wxQtTextEdit( parent, this );

    if ( QtCreateControl( parent, id, pos, size, style, validator, name ) )
    {
        // set the initial text value without sending the event:
        // (done here as needs CreateBase called to set flags for IsMultiLine)
        ChangeValue( value );
        return true;
    }
}

wxSize wxTextCtrl::DoGetBestSize() const
{
    return wxTextCtrlBase::DoGetBestSize();

}
int wxTextCtrl::GetLineLength(long lineNo) const
{
    return 0;
}

wxString wxTextCtrl::GetLineText(long lineNo) const
{
    return wxString();
}

int wxTextCtrl::GetNumberOfLines() const
{
    return 0;
}

bool wxTextCtrl::IsModified() const
{
    return false;
}

void wxTextCtrl::MarkDirty()
{
}

void wxTextCtrl::DiscardEdits()
{
}

bool wxTextCtrl::SetStyle(long start, long end, const wxTextAttr& style)
{
    return false;
}

bool wxTextCtrl::GetStyle(long position, wxTextAttr& style)
{
    return false;
}

bool wxTextCtrl::SetDefaultStyle(const wxTextAttr& style)
{
    return false;
}

long wxTextCtrl::XYToPosition(long x, long y) const
{
    return 0;
}

bool wxTextCtrl::PositionToXY(long pos, long *x, long *y) const
{
    return false;
}

void wxTextCtrl::ShowPosition(long pos)
{
}

bool wxTextCtrl::DoLoadFile(const wxString& file, int fileType)
{
    return false;
}

bool wxTextCtrl::DoSaveFile(const wxString& file, int fileType)
{
    return false;
}

void wxTextCtrl::SetInsertionPoint(long pos)
{
    QTextCursor::MoveOperation op;
    QTextCursor cursor;

    // check if pos indicates end of text:
    if ( pos == -1 )
    {
        pos = 0;
        op = QTextCursor::End;
    }
    else
    {
        op = QTextCursor::Start;
    }
    if ( !IsMultiLine() )
    {
        if (op == QTextCursor::End)
            GetQLineEdit()->end(false);
        else
            GetQLineEdit()->setCursorPosition(pos);
    }
    else
    {
        cursor = GetQTextEdit()->textCursor();
        cursor.movePosition(op, QTextCursor::MoveAnchor, pos);
        GetQTextEdit()->setTextCursor(cursor);
        GetQTextEdit()->ensureCursorVisible();
    }
}

long wxTextCtrl::GetInsertionPoint() const
{
    QTextCursor cursor;

    if ( !IsMultiLine() )
    {
        return GetQLineEdit()->cursorPosition();
    }
    else
    {
        cursor = GetQTextEdit()->textCursor();
        return cursor.position();
    }
}

wxString wxTextCtrl::DoGetValue() const
{
    if ( IsMultiLine() )
        return wxQtConvertString( GetQTextEdit()->toPlainText() );
    else
        return wxQtConvertString( GetQLineEdit()->text() );
}

void wxTextCtrl::SetSelection( long from, long to )
{
    // SelectAll uses -1 to -1, adjust for qt:
    if (from == -1 && to == -1)
    {
        from = 0;
        to = GetValue().length();
    }
    if ( IsMultiLine() )
    {
        QTextCursor cursor = GetQTextEdit()->textCursor();
        cursor.setPosition(from);
        cursor.setPosition(to, QTextCursor::KeepAnchor);
        GetQTextEdit()->setTextCursor(cursor);
    }
    else // single line
    {
        GetQLineEdit()->setSelection(from, to);
    }
}

void wxTextCtrl::WriteText( const wxString &text )
{
    // Insert the text
    if ( !IsMultiLine() )
    {
        GetQLineEdit()->insert(wxQtConvertString( text ));
    }
    else
    {
        GetQTextEdit()->insertPlainText(wxQtConvertString( text ));
        // the cursor is moved to the end, ensure it is shown
        GetQTextEdit()->ensureCursorVisible();
    }
}

void wxTextCtrl::DoSetValue( const wxString &text, int flags )
{
    // do not fire qt signals for certain methods (i.e. ChangeText)
    if ( !(flags & SetValue_SendEvent) )
    {
        if ( !IsMultiLine() )
            GetQLineEdit()->blockSignals(true);
        else
            GetQTextEdit()->blockSignals(true);
    }

    // Replace the text int the control
    if ( !IsMultiLine() )
    {
        GetQLineEdit()->setText(wxQtConvertString( text ));
    }
    else
    {
        GetQTextEdit()->setPlainText(wxQtConvertString( text ));
        // the cursor is moved to the end, ensure it is shown
        GetQTextEdit()->ensureCursorVisible();
    }

    // re-enable qt signals
    if ( !(flags & SetValue_SendEvent) )
    {
        if ( !IsMultiLine() )
            GetQLineEdit()->blockSignals(false);
        else
            GetQTextEdit()->blockSignals(false);
    }
}

QScrollArea *wxTextCtrl::QtGetScrollBarsContainer() const
{
    if (IsMultiLine())
        return static_cast<QScrollArea *>(m_qtWindow);
    else
        return NULL;
}
