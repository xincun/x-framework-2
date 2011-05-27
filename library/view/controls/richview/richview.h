
#ifndef __view_richview_h__
#define __view_richview_h__

#pragma once

#include <windows.h>
#include <richedit.h>
#include <textserv.h>

#include "../../view/view.h"

namespace view
{

    // 实现无窗口的richedit控件.
    class RichView : public View, public ITextHost
    {
    public:
        // RichView的类名.
        static const char kViewClassName[];

        enum StyleFlags
        {
            STYLE_DEFAULT   = 0,
            STYLE_PASSWORD  = 1 << 0,
            STYLE_MULTILINE = 1 << 1,
        };

        RichView();
        explicit RichView(StyleFlags style);
        virtual ~RichView();

        // 获取/设置只读属性.
        bool IsReadOnly() const;
        void SetReadOnly(bool read_only);

        // 获取/设置密码属性.
        bool IsPassword() const;
        void SetPassword(bool password);

        // 获取/设置多行属性.
        bool IsMultiLine() const;
        void SetMultiLine(bool multi_line);

        // 访问style_.
        StyleFlags style() const { return style_; }

        // 获取/设置边界.
        const gfx::Insets& GetMargins() const;
        void SetMargins(const gfx::Insets& margins);

        // 获取/设置绘制边框属性.
        bool IsDrawBorder() const { return draw_border_; }
        void SetDrawBorder(bool draw_border);

        // Overridden from View:
        virtual gfx::Size GetPreferredSize();
        virtual bool IsFocusable() const;
        virtual void AboutToRequestFocusFromTabTraversal(bool reverse);
        virtual bool SkipDefaultKeyEventProcessing(const KeyEvent& e);
        virtual void SetEnabled(bool enabled);
        virtual void OnPaintBackground(gfx::Canvas* canvas);
        virtual void OnPaintFocusBorder(gfx::Canvas* canvas);
        virtual void OnPaint(gfx::Canvas* canvas);
        virtual bool OnKeyPressed(const KeyEvent& e);
        virtual bool OnKeyReleased(const KeyEvent& e);
        virtual void OnFocus();
        virtual void OnBlur();

        // IUnknown实现
        virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,
            void** ppvObject);
        virtual ULONG STDMETHODCALLTYPE AddRef(void);
        virtual ULONG STDMETHODCALLTYPE Release(void);

        // ITextHost实现
        virtual HDC TxGetDC();
        virtual INT TxReleaseDC(HDC hdc);
        virtual BOOL TxShowScrollBar(INT fnBar, BOOL fShow);
        virtual BOOL TxEnableScrollBar(INT fuSBFlags, INT fuArrowflags);
        virtual BOOL TxSetScrollRange(INT fnBar, LONG nMinPos,
            INT nMaxPos, BOOL fRedraw);
        virtual BOOL TxSetScrollPos(INT fnBar, INT nPos, BOOL fRedraw);
        virtual void TxInvalidateRect(LPCRECT prc, BOOL fMode);
        virtual void TxViewChange(BOOL fUpdate);
        virtual BOOL TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight);
        virtual BOOL TxShowCaret(BOOL fShow);
        virtual BOOL TxSetCaretPos(INT x, INT y);
        virtual BOOL TxSetTimer(UINT idTimer, UINT uTimeout);
        virtual void TxKillTimer(UINT idTimer);
        virtual void TxScrollWindowEx(INT dx, INT dy,
            LPCRECT lprcScroll, LPCRECT lprcClip,
            HRGN hrgnUpdate, LPRECT lprcUpdate, UINT fuScroll);
        virtual void TxSetCapture(BOOL fCapture);
        virtual void TxSetFocus();
        virtual void TxSetCursor(HCURSOR hcur, BOOL fText);
        virtual BOOL TxScreenToClient(LPPOINT lppt);
        virtual BOOL TxClientToScreen(LPPOINT lppt);
        virtual HRESULT TxActivate(LONG* plOldState);
        virtual HRESULT TxDeactivate(LONG lNewState);
        virtual HRESULT TxGetClientRect(LPRECT prc);
        virtual HRESULT TxGetViewInset(LPRECT prc);
        virtual HRESULT TxGetCharFormat(const CHARFORMATW** ppCF);
        virtual HRESULT TxGetParaFormat(const PARAFORMAT** ppPF);
        virtual COLORREF TxGetSysColor(int nIndex);
        virtual HRESULT TxGetBackStyle(TXTBACKSTYLE* pstyle);
        virtual HRESULT TxGetMaxLength(DWORD* plength);
        virtual HRESULT TxGetScrollBars(DWORD* pdwScrollBar);
        virtual HRESULT TxGetPasswordChar(TCHAR* pch);
        virtual HRESULT TxGetAcceleratorPos(LONG* pcp);
        virtual HRESULT TxGetExtent(LPSIZEL lpExtent);
        virtual HRESULT OnTxCharFormatChange(const CHARFORMATW* pcf);
        virtual HRESULT OnTxParaFormatChange(const PARAFORMAT* ppf);
        virtual HRESULT TxGetPropertyBits(DWORD dwMask, DWORD* pdwBits);
        virtual HRESULT TxNotify(DWORD iNotify, void* pv);
        virtual HIMC TxImmGetContext();
        virtual void TxImmReleaseContext(HIMC himc);
        virtual HRESULT TxGetSelectionBarWidth(LONG* lSelBarWidth);

    protected:
        virtual void OnBoundsChanged(const gfx::Rect& previous_bounds);
        virtual void ViewHierarchyChanged(bool is_add, View* parent, View* child);
        virtual std::string GetClassName() const;

    private:
        // 风格.
        StyleFlags style_;

        // 是否需要绘制边框.
        bool draw_border_;

        // 一次显示的文本行数.
        int num_lines_;

        // 是否已初始化.
        bool initialized_;

        // 边界.
        gfx::Insets margins_;

        ULONG cRefs_;					    // Reference Count

        ITextServices* pserv_;              // pointer to Text Services object

        // Properties
        DWORD       dwStyle_;               // style bits
        DWORD       dwExStyle_;             // extended style bits

        unsigned    fBorder_            :1; // control has border
        unsigned    fCustRect_          :1; // client changed format rect
        unsigned    fInBottomless_      :1; // inside bottomless callback
        unsigned    fInDialogBox_       :1; // control is in a dialog box
        unsigned    fEnableAutoWordSel_ :1; // enable Word style auto word selection?
        unsigned    fVertical_          :1; // vertical writing
        unsigned    fIconic_            :1; // control window is iconic
        unsigned    fHidden_            :1; // control window is hidden
        unsigned    fNotSysBkgnd_       :1; // not using system background color
        unsigned    fWindowLocked_      :1; // window is locked (no update)
        unsigned    fRegisteredForDrop_ :1; // whether host has registered for drop
        unsigned    fVisible_           :1; // Whether window is visible or not.
        unsigned    fResized_           :1; // resized while hidden
        unsigned    fWordWrap_          :1; // Whether control should word wrap
        unsigned    fAllowBeep_         :1; // Whether beep is allowed
        unsigned    fRich_              :1; // Whether control is rich text
        unsigned    fSaveSelection_     :1; // Whether to save the selection when inactive
        unsigned    fInplaceActive_     :1; // Whether control is inplace active
        unsigned    fTransparent_       :1; // Whether control is transparent
        unsigned    fTimer_             :1; // A timer is set

        LONG        lSelBarWidth_;          // Width of the selection bar

        COLORREF    crBackground_;          // background color
        LONG        cchTextMost_;           // maximum text size
        DWORD       dwEventMask_;           // Event mask to pass on to parent window

        LONG        icf_;
        LONG        ipf_;

        RECT        rcClient_;              // Client Rect for this control

        RECT        rcViewInset_;           // view rect inset

        SIZEL       sizelExtent_;           // Extent array

        CHARFORMAT2W cf_;                   // Default character format

        PARAFORMAT	pf_;                    // Default paragraph format

        LONG        laccelpos_;             // Accelerator position

        WCHAR       chPasswordChar_;        // Password character
    };

} //namespace view

#endif //__view_richview_h__