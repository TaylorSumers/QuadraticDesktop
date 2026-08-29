#pragma once

#include "MainWindow.g.h"
#include "Interop/QuadraticCoreAdapter.h"

namespace winrt::QuadraticDesktop::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();

        fire_and_forget SolveButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        fire_and_forget GraphView_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void GraphView_NavigationCompleted(winrt::Microsoft::UI::Xaml::Controls::WebView2 const& sender, winrt::Microsoft::Web::WebView2::Core::CoreWebView2NavigationCompletedEventArgs const& args);

      private:
        bool graphReady_{ false };
        bool hasGraphData_{ false };
        double graphA_{};
        double graphB_{};
        double graphC_{};
        double graphX1_{};
        double graphX2_{};
        int graphNRoots_{};

        void UpdateGraph(double a, double b, double c);
        void SendGraphData();
        Microsoft::UI::Xaml::Controls::ContentDialog GetErrorDialog(winrt::hstring const& header, winrt::hstring const& message);
        winrt::fire_and_forget ShowResult(Interop::Solution result);
        void ShowGraphInfo(double a, double b, double c, Interop::GraphInfo graphInfo);
    };
}

namespace winrt::QuadraticDesktop::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
