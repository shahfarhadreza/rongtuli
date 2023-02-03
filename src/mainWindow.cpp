#include "stdinclude.h"
#include "mainWindow.h"
#include "rongtuli.h"
#include "button.h"
#include "canvasWindow.h"
#include "toolBar.h"
#include "colorPanel.h"
#include "propertiesPanel.h"

LRESULT CALLBACK MainWindowProcedure(HWND, UINT, WPARAM, LPARAM);

MainWindow* MainWindow::mInstance = nullptr;

MainWindow::MainWindow(const std::string& title)
    : mWindowHandle(nullptr), mInitialized(false), mCanvasWindow(nullptr) {
    assert(mInstance == nullptr);
    mInstance = this;

    const wchar_t szClassName[] = L"mainWindowCLS";

    WNDCLASSEX wincl;

    wincl.hInstance = RongtuliApp::getInstance()->getInstanceHandle();
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = MainWindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);

    wincl.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wincl.lpszMenuName = nullptr;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    if (::RegisterClassEx (&wincl)) {
        mWindowHandle = ::CreateWindowEx(
           0,
           szClassName,
           _T("Rongtuli"),
           WS_OVERLAPPEDWINDOW,
           CW_USEDEFAULT,
           CW_USEDEFAULT,
           1024,
           720,
           HWND_DESKTOP,
           nullptr,
           wincl.hInstance,
           nullptr
           );
        ::ShowWindow(mWindowHandle, SW_SHOW);

        mToolBar = std::make_shared<ToolBar>(mWindowHandle);

        mToolBar->addButton(L"Select", L".\\select.png", []() {
            auto app = RongtuliApp::getInstance();
            app->setCurrentTool(std::make_shared<SelectMoveTool>());
        });
        mToolBar->addButton(L"Line", L".\\line.png", []() {
            auto app = RongtuliApp::getInstance();
            app->setCurrentTool(std::make_shared<LineShapeTool>());
        });
        auto rectTool = mToolBar->addButton(L"Rectangle", L".\\rect.png", []() {
            auto app = RongtuliApp::getInstance();
            app->setCurrentTool(std::make_shared<RectangleShapeTool>());
        });
        mToolBar->addButton(L"Ellipse", L".\\ellipse.png", []() {
            auto app = RongtuliApp::getInstance();
            app->setCurrentTool(std::make_shared<EllipseShapeTool>());
        });
        mToolBar->addButton(L"Text", L".\\text.png", []() {
            auto app = RongtuliApp::getInstance();
            app->setCurrentTool(std::make_shared<TextTool>());
        });
        mToolBar->addButton(L"Delete", L".\\delete.png", [this]() {
            auto canvas = this->getCanvasWindow();
            canvas->deleteSelectedObject();
        });

        mToolBar->setDefaultTool(rectTool);

        mCanvasWindow = std::make_shared<CanvasWindow>(mWindowHandle);

        mColorPanel = std::make_shared<ColorPanel>(mWindowHandle);

        mColorPanel->addColor(Gdiplus::Color(0, 0, 0));
        mColorPanel->addColor(Gdiplus::Color(127, 127, 127));
        mColorPanel->addColor(Gdiplus::Color(136, 0, 21));
        mColorPanel->addColor(Gdiplus::Color(237, 28, 36));
        mColorPanel->addColor(Gdiplus::Color(255, 127, 39));
        mColorPanel->addColor(Gdiplus::Color(255, 242, 0));
        mColorPanel->addColor(Gdiplus::Color(34, 177, 76));
        mColorPanel->addColor(Gdiplus::Color(0, 162, 232));
        mColorPanel->addColor(Gdiplus::Color(63, 72, 204));
        mColorPanel->addColor(Gdiplus::Color(163, 73, 164));
        mColorPanel->addColor(Gdiplus::Color(255, 255, 255));
        mColorPanel->addColor(Gdiplus::Color(195, 195, 195));
        mColorPanel->addColor(Gdiplus::Color(185, 122, 87));
        mColorPanel->addColor(Gdiplus::Color(255, 174, 201));
        mColorPanel->addColor(Gdiplus::Color(255, 201, 14));
        mColorPanel->addColor(Gdiplus::Color(239, 228, 176));
        mColorPanel->addColor(Gdiplus::Color(181, 230, 29));
        mColorPanel->addColor(Gdiplus::Color(153, 217, 234));
        mColorPanel->addColor(Gdiplus::Color(112, 146, 190));
        mColorPanel->addColor(Gdiplus::Color(200, 191, 231));

        mPropertiesPanel = std::make_shared<PropertiesPanel>(mWindowHandle);

        mInitialized = true;

        RECT rect;
        ::GetClientRect(mWindowHandle, &rect);
        reLayout({rect.right, rect.bottom});
    }
}

MainWindow::~MainWindow() {
}

void MainWindow::reLayout(const Gdiplus::Size& size) {
    if (!mInitialized) {
        return;
    }
    const int margin = 4;
    const int toolBarX = margin;
    const int toolBarY = margin;
    const int toolBarW = 43;
    mToolBar->moveAndSize(toolBarX, toolBarY, toolBarW, size.Height - toolBarY - margin);


    const int colorPanelW = 170;
    const int colorPanelX = size.Width - colorPanelW - margin;
    const int colorPanelY = margin;
    const int colorPanelH = 200 - colorPanelY - margin;
    mColorPanel->moveAndSize(colorPanelX, colorPanelY, colorPanelW, colorPanelH);

    const int propertiesPanelW = 170;
    const int propertiesPanelX = size.Width - propertiesPanelW - margin;
    const int propertiesPanelY = colorPanelY + colorPanelH + margin;
    const int propertiesPanelH = size.Height - propertiesPanelY - margin;
    mPropertiesPanel->moveAndSize(propertiesPanelX, propertiesPanelY, propertiesPanelW, propertiesPanelH);

    const int canvasX = toolBarX + toolBarW + margin;
    const int canvasY = margin;
    const int canvasW = size.Width - canvasX - colorPanelW - margin - margin;
    const int canvasH = size.Height - canvasY - margin;
    mCanvasWindow->moveAndSize(canvasX, canvasY, canvasW, canvasH);
}

LRESULT CALLBACK MainWindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    auto app = RongtuliApp::getInstance();
    switch (message) {
        case WM_COMMAND: {
            const HWND handle = HWND(lParam);
            Control* control = app->findControl(handle);
            if (control != nullptr) {
                control->WMCommand(wParam);
            }
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            ::BeginPaint(hwnd, &ps);
            const HDC hdc = ps.hdc;
            Gdiplus::Graphics g(hdc);
            g.Clear(Gdiplus::Color(40, 40, 40));
            ::EndPaint(hwnd, &ps);
            break;
        }
        case WM_SIZE: {
            RECT rect;
            ::GetClientRect(hwnd, &rect);
            MainWindow::getInstance()->reLayout({rect.right, rect.bottom});
            break;
        }
        case WM_DESTROY:
            ::PostQuitMessage(0);
            break;
        default:
            return ::DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}


