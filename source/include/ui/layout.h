#pragma once

#define LAYOUT_BOX_AUTO_SIZE    ((DWORD)(-1))

// 指定父区域如何排布此区域
#define MLT_LEFT        0x0001       // 元素靠左浮动
#define MLT_RIGHT       0x0002       // 元素靠右浮动
#define MLT_HCENTER     0x0003       // 元素中间浮动
#define MLT_REL_X       0x0004       // 元素X轴坐标
#define MLT_HMASK       0x000F       // 元素X轴布局Mask

#define MLT_TOP         0x0010       // 元素靠上浮动
#define MLT_BOTTOM      0x0020       // 元素靠下浮动
#define MLT_VCENTER     0x0030       // 元素垂直居中
#define MLT_REL_Y       0x0040       // 元素Y轴坐标
#define MLT_VMASK       0x00F0       // 元素Y轴布局Mask

#define MLT_HFILL       0x0100       // 元素宽度自适应（充满整个区域），如果没有指定这个标志，那么将使用Layout的Width
#define MLT_VFILL       0x0200       // 元素高度自适应（充满整个区域），如果没有指定这个标志，那么将使用Layout的Height

// {65ADBF9E-42D7-4e51-8EB1-0FA38CFC8DCA}
MIDL_INTERFACE("65ADBF9E-42D7-4e51-8EB1-0FA38CFC8DCA")
ILayoutBox : IUnknown
{
public:
    // Hierarchy
    virtual HRESULT STDMETHODCALLTYPE AddChild(ILayoutBox *pBox) = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveChild(ILayoutBox *pBox) = 0;
    virtual DWORD STDMETHODCALLTYPE GetChildrenCount() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetChildren(ILayoutBox **ppBox, DWORD nCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetParent(ILayoutBox **ppBox) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetRoot(ILayoutBox **ppBox) = 0;

    // Box attribute setters
    virtual HRESULT STDMETHODCALLTYPE SetLayoutType(DWORD nLayout) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetLayoutX(INT32 nX) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetLayoutY(INT32 nY) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetLayoutPosition(POINT *pPosition) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetLayoutWidth(INT32 nWidth) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetLayoutHeight(INT32 nHeight) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetLayoutSize(SIZE *pSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetLayoutMinSize(SIZE *pSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetLayoutMaxSize(SIZE *pSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetLayoutMargin(RECT *pRect) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetLayoutPadding(RECT *pRect) = 0;

    // Box attribute getters
    virtual DWORD STDMETHODCALLTYPE GetLayoutType() = 0;
    virtual INT32 STDMETHODCALLTYPE GetLayoutX() = 0;
    virtual INT32 STDMETHODCALLTYPE GetLayoutY() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetLayoutPosition(POINT *pPosition) = 0;
    virtual INT32 STDMETHODCALLTYPE GetLayoutWidth() = 0;
    virtual INT32 STDMETHODCALLTYPE GetLayoutHeight() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetLayoutSize(SIZE *pSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetLayoutMinSize(SIZE *pSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetLayoutMaxSize(SIZE *pSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetLayoutMargin(RECT *pRect) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetLayoutPadding(RECT *pRect) = 0;

    // Layout
    virtual BOOL STDMETHODCALLTYPE IsBoxVisible() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetBoxClientRect(RECT *pRect) = 0;
    virtual HRESULT STDMETHODCALLTYPE Layout(RECT *pRect, BOOL bForce) = 0;
};

// {EA149005-C307-41b3-A7E9-005C5589CB65}
MIDL_INTERFACE("EA149005-C307-41b3-A7E9-005C5589CB65")
ILayoutHContainer : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetColSize(DWORD nCol) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetColWidth(DWORD nCol, INT32 nWidth) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetColWeight(DWORD nCol, DWORD nWeight) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetChildBox(DWORD nColId, ILayoutBox *pBox) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetColSize(DWORD *pCol) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetColWidth(DWORD nCol, INT32 *pWidth) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetColWeight(DWORD nCol, DWORD *pWeight) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetChildBox(DWORD nColId, ILayoutBox **ppBox) = 0;
};

// {D920F936-91A5-45b5-95DA-13842554905D}
MIDL_INTERFACE("D920F936-91A5-45b5-95DA-13842554905D")
ILayoutVContainer : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetRowSize(DWORD nRow) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetRowHeight(DWORD nRow, INT32 nHeight) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetRowWeight(DWORD nRow, DWORD nWeight) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetChildBox(DWORD nRowId, ILayoutBox *pBox) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetRowSize(DWORD *pRow) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetRowHeight(DWORD nRow, INT32 *pHeight) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetRowWeight(DWORD nRow, DWORD *pWeight) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetChildBox(DWORD nRowId, ILayoutBox **ppBox) = 0;
};

// {E44F8EA5-C9FD-465b-8E66-5E2F5E15EC26}
MIDL_INTERFACE("E44F8EA5-C9FD-465b-8E66-5E2F5E15EC26")
ILayoutNativeWindow : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetHWND(HWND hWindow) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetHWND(HWND *pWindow) = 0;
};

HRESULT InitLayout();
HRESULT UninitLayout();
HRESULT CreateLayoutBox(ILayoutBox **ppContainer, DWORD nLayoutType = 0);
HRESULT CreateLayoutNativeWindow(ILayoutBox **ppContainer, HWND hWindow, DWORD nLayoutType = 0);
HRESULT CreateLayoutHContainer(ILayoutHContainer **ppContainer, DWORD nLayoutType = 0);
HRESULT CreateLayoutVContainer(ILayoutVContainer **ppContainer, DWORD nLayoutType = 0);