#include "pch.h"
#include "App.xaml.h"
#include "MainWindow.xaml.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Windowing;
using namespace Windows::Graphics;

namespace winrt::QuadraticDesktop::implementation
{
    void App::OnLaunched([[maybe_unused]] LaunchActivatedEventArgs const& e)
    {
        window_ = make<MainWindow>();

        AppWindow appWindow = window_.AppWindow();

        winrt::hstring iconPath{
        winrt::Windows::ApplicationModel::Package::Current()
            .InstalledLocation()
            .Path()
            + L"\\Assets\\AppIcon.ico"
        };
        appWindow.SetTitleBarIcon(winrt::hstring{ iconPath });
        appWindow.SetTaskbarIcon(winrt::hstring{ iconPath });

        appWindow.ResizeClient(SizeInt32{ 1300, 900 });
        if (auto presenter = appWindow.Presenter().try_as<OverlappedPresenter>()) {
          presenter.PreferredMinimumWidth(winrt::box_value(1300).as<winrt::Windows::Foundation::IReference<int32_t>>());
          presenter.PreferredMinimumHeight(winrt::box_value(700).as<winrt::Windows::Foundation::IReference<int32_t>>());
        }

        window_.Activate();
    }
}
