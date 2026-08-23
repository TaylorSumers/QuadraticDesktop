#pragma once

#include "MainWindow.g.h"
#include "Interop/QuadraticCoreAdapter.h"

namespace winrt::QuadraticDesktop::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow()
        {
            
        }

        winrt::fire_and_forget SolveButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);

      private:
        winrt::Microsoft::UI::Xaml::Controls::ContentDialog GetErrorDialog(winrt::hstring const& header, winrt::hstring const& message);
        void ShowResult(quadratic::interop::Result result);
    };
}

namespace winrt::QuadraticDesktop::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
