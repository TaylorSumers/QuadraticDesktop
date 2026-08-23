#include "pch.h"
#include "App.xaml.h"
#include "MainWindow.xaml.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Windowing;
using namespace Windows::Graphics;

namespace winrt::QuadraticDesktop::implementation
{
    /// <summary>
    /// Initializes the singleton application object.  This is the first line of authored code
    /// executed, and as such is the logical equivalent of main() or WinMain().
    /// </summary>
    App::App()
    {
        // Xaml objects should not call InitializeComponent during construction.
        // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
        UnhandledException([](IInspectable const&, UnhandledExceptionEventArgs const& e)
        {
            if (IsDebuggerPresent())
            {
                auto errorMessage = e.Message();
                __debugbreak();
            }
        });
#endif
    }

    /// <summary>
    /// Invoked when the application is launched.
    /// </summary>
    /// <param name="e">Details about the launch request and process.</param>
    void App::OnLaunched([[maybe_unused]] LaunchActivatedEventArgs const& e)
    {
        window = make<MainWindow>();  // TODO: Learn
  

        AppWindow appWindow = window.AppWindow();

        std::wstring iconPath{
        winrt::Windows::ApplicationModel::Package::Current()
            .InstalledLocation()
            .Path()
            .c_str()
        };
        iconPath += L"\\Assets\\AppIcon.ico";
        appWindow.SetTitleBarIcon(winrt::hstring{ iconPath });
        appWindow.SetTaskbarIcon(winrt::hstring{ iconPath });

        appWindow.ResizeClient(SizeInt32{ 1000, 700 });
        if (auto presenter = appWindow.Presenter().try_as<OverlappedPresenter>()) {
          presenter.IsResizable(false);
          presenter.IsMaximizable(false);
        }

        window.Activate();
    }
}
