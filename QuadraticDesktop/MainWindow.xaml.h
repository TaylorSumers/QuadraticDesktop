#pragma once

#include "MainWindow.g.h"
#include "Interop/QuadraticCoreAdapter.h"

const winrt::hstring PLOTLY_RELATIVE_PATH = L"\\Assets\\Plotly";
const winrt::hstring VIRTUAL_HOST_NAME = L"appassets.example";
const winrt::hstring GRAPH_URL = L"https://appassets.example/graph.html";

namespace winrt::QuadraticDesktop::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();

        fire_and_forget SolveButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        fire_and_forget GraphView_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void GraphView_NavigationCompleted(winrt::Microsoft::UI::Xaml::Controls::WebView2 const& sender, winrt::Microsoft::Web::WebView2::Core::CoreWebView2NavigationCompletedEventArgs const& args);

      private:
        struct GraphData {
          bool isReady{ false };
          bool hasData{ false };
          double coeffA{};
          double coeffB{};
          double coeffC{};
          double x1{};
          double x2{};
          RootAmount nRoots { SOLVE_ERROR };
        };

        GraphData graphData_{};

        Microsoft::UI::Xaml::Controls::ContentDialog GetErrorDialog(winrt::hstring const& header, winrt::hstring const& message);
        winrt::Windows::Foundation::IAsyncAction ShowSolutionAsync(Interop::Solution result);
        void ShowGraphInfo(double a, double b, double c, Interop::GraphInfo graphInfo);
        void UpdateGraph(double a, double b, double c, Interop::Solution solution);
        void SendGraphData();
        void ShowBranchesDirection(BranchesDirection direction);
        void ShowEquationString(double a, double b, double c);
        void ShowNoRootsResult(double d);
        void ShowOneRootResult(double d, double x1);
        void ShowTwoRootsResult(double d, double x1, double x2);
        void ShowInfRootsResult(double d);
        void ShowSolutionOutput();
        void HideSolutionOutput();
    };
}

namespace winrt::QuadraticDesktop::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
