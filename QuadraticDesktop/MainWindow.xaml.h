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
        winrt::fire_and_forget GraphView_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void GraphView_NavigationCompleted(winrt::Microsoft::UI::Xaml::Controls::WebView2 const& sender, winrt::Microsoft::Web::WebView2::Core::CoreWebView2NavigationCompletedEventArgs const& args);

      private:
        void UpdateGraph(double a, double b, double c);
        void SendGraphData();

        bool graphReady_{ false };
        bool hasGraphData_{ false };

        double graphA_{};
        double graphB_{};
        double graphC_{};

        winrt::Microsoft::UI::Xaml::Controls::ContentDialog GetErrorDialog(winrt::hstring const& header, winrt::hstring const& message);
        winrt::fire_and_forget ShowResult(quadratic::interop::Result result);
    };
}

namespace winrt::QuadraticDesktop::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
