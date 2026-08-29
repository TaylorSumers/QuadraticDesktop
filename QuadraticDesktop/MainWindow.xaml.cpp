#include "pch.h"
#include "MainWindow.xaml.h"

#include <cmath>

#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::Web::WebView2::Core;
using namespace Windows::Foundation;
using namespace winrt::QuadraticDesktop;

namespace winrt::QuadraticDesktop::implementation
{
  MainWindow::MainWindow() 
  {
    InitializeComponent();
  }

  fire_and_forget MainWindow::SolveButton_Click(IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e) 
  {
    double a = nbxCoeffA().Value();
    double b = nbxCoeffB().Value();
    double c = nbxCoeffC().Value();

    if (!std::isfinite(a) || !std::isfinite(b) || !std::isfinite(c)) 
    {
      ContentDialog errorDialog = GetErrorDialog(L"Ошибка ввода", L"Введите числовые коэффициенты a, b и c.");
      co_await errorDialog.ShowAsync();
      co_return;
    }

    ShowResult(Interop::Solve(a, b, c));
    UpdateGraph(a, b, c);
    ShowGraphInfo(a, b, c, Interop::GetGraphInfo(a, b, c));
  }

  fire_and_forget MainWindow::GraphView_Loaded(IInspectable const& sender, RoutedEventArgs const& e)
  {
    bool initializationFailed = false;
    hstring errorMessage{};

    try {
      co_await GraphView().EnsureCoreWebView2Async();

      hstring plotlyFolder{
          winrt::Windows::ApplicationModel::Package::Current()
              .InstalledLocation()
              .Path()
              + L"\\Assets\\Plotly"
      };

      CoreWebView2 core = GraphView().CoreWebView2();

      core.SetVirtualHostNameToFolderMapping(
        L"appassets.example",
        plotlyFolder,
        winrt::Microsoft::Web::WebView2::Core::
        CoreWebView2HostResourceAccessKind::DenyCors);

      core.Navigate(L"https://appassets.example/graph.html");
    }
    catch (winrt::hresult_error const& error) {
      initializationFailed = true;
      errorMessage = error.message();
    }

    if (initializationFailed) {
      ContentDialog errorDialog = GetErrorDialog(L"Ошибка графика", errorMessage);
      co_await errorDialog.ShowAsync();
    }
  }

  void MainWindow::GraphView_NavigationCompleted(WebView2 const& sender, CoreWebView2NavigationCompletedEventArgs const& e)
  {
    graphReady_ = e.IsSuccess();

    if (graphReady_ && hasGraphData_) {
      SendGraphData();
    }
  }

  void MainWindow::UpdateGraph(double a, double b, double c) 
  {
    graphA_ = a;
    graphB_ = b;
    graphC_ = c;
    hasGraphData_ = true;

    if (graphReady_) {
      SendGraphData();
    }
  }

  void MainWindow::ShowGraphInfo(double a, double b, double c, Interop::GraphInfo graphInfo) {
    tbEquationString().Text(winrt::hstring{ std::format(L"y = {:.6g}x² + {:.6g}x + {:.6g}", a, b, c) });
    brdEquationString().Visibility(Visibility::Visible);

    tbVertexCords().Text(winrt::hstring{ std::format(L"({:.3g}, {:.3g})", graphInfo.vertex.x, graphInfo.vertex.y) });
    tbAxisOfSymmetry().Text(winrt::hstring{ std::format(L"x = {:.3g}", graphInfo.vertex.x) });

    winrt::hstring branchesDirectionStr{};
    switch (graphInfo.branchesDirection) {
      case UP:
        branchesDirectionStr = L"вверх";
        break;
      case DOWN:
        branchesDirectionStr = L"вниз";
        break;
      case NO_BRANCHES:
        branchesDirectionStr = L"–";
        tbVertexCords().Text(L"–");
        tbAxisOfSymmetry().Text(L"–");
        break;
    }
    tbBranchesDirection().Text(branchesDirectionStr);
  }

  void MainWindow::SendGraphData() 
  {
    std::wstring json = std::format(
      LR"({{ "a":{:.17g},"b":{:.17g},"c":{:.17g}, "x1":{:.17g}, "x2":{:.17g} }})",
      graphA_,
      graphB_,
      graphC_,
      graphX1_,
      graphX2_
    );

    GraphView().CoreWebView2().PostWebMessageAsJson(hstring{ json });
  }

  fire_and_forget MainWindow::ShowResult(Interop::Solution result)
  {
    graphX1_ = result.x1;
    graphX2_ = result.x2;


    tbWaitCoeff().Visibility(Visibility::Collapsed);
    grdOutput().Visibility(Visibility::Visible);

    switch (result.nRoots) {
    case NO_ROOTS:
      brdOneRootMsg().Visibility(Visibility::Collapsed);
      brdTwoRootsMsg().Visibility(Visibility::Collapsed);
      brdNoRootsMsg().Visibility(Visibility::Visible);
      brdInfRootsMsg().Visibility(Visibility::Collapsed);

      dField().Text(winrt::hstring{ std::format(L"{:.6g}", result.d) });
      tbFirstRoot().Visibility(Visibility::Collapsed);
      tbSecondRoot().Visibility(Visibility::Collapsed);
      break;
    case ONE_ROOT:
      brdOneRootMsg().Visibility(Visibility::Visible);
      brdTwoRootsMsg().Visibility(Visibility::Collapsed);
      brdNoRootsMsg().Visibility(Visibility::Collapsed);
      brdInfRootsMsg().Visibility(Visibility::Collapsed);


      dField().Text(winrt::hstring{ std::format(L"{:.6g}", result.d) });
      x1Field().Text(winrt::hstring{ std::format(L"{:.6g}", result.x1) });
      tbFirstRoot().Visibility(Visibility::Visible);
      tbSecondRoot().Visibility(Visibility::Collapsed);
      break;
    case TWO_ROOTS:
      brdOneRootMsg().Visibility(Visibility::Collapsed);
      brdTwoRootsMsg().Visibility(Visibility::Visible);
      brdNoRootsMsg().Visibility(Visibility::Collapsed);
      brdInfRootsMsg().Visibility(Visibility::Collapsed);

      dField().Text(winrt::hstring{ std::format(L"{:.6g}", result.d) });
      x1Field().Text(to_hstring(winrt::hstring{ std::format(L"{:.6g}", result.x1) }));
      x2Field().Text(to_hstring(winrt::hstring{ std::format(L"{:.6g}", result.x2) }));
      tbFirstRoot().Visibility(Visibility::Visible);
      tbSecondRoot().Visibility(Visibility::Visible);
      break;
    case INF_ROOTS:
      brdOneRootMsg().Visibility(Visibility::Collapsed);
      brdTwoRootsMsg().Visibility(Visibility::Collapsed);
      brdNoRootsMsg().Visibility(Visibility::Collapsed);
      brdInfRootsMsg().Visibility(Visibility::Visible);

      dField().Text(winrt::hstring{ std::format(L"{:.6g}", result.d) });
      tbFirstRoot().Visibility(Visibility::Collapsed);
      tbSecondRoot().Visibility(Visibility::Collapsed);
      break;
    default:
      tbWaitCoeff().Visibility(Visibility::Visible);
      grdOutput().Visibility(Visibility::Collapsed);

      auto errorDialog = GetErrorDialog(L"Ошибка вычисления", L"Неизвестный результат");
      co_await errorDialog.ShowAsync();
      break;
    }
  }

  ContentDialog MainWindow::GetErrorDialog(hstring const& header, hstring const& message)
  {
    ContentDialog dialog;
    dialog.XamlRoot(RootGrid().XamlRoot());
    dialog.Title(box_value(header));
    dialog.Content(box_value(message));
    dialog.CloseButtonText(L"ОК");
    dialog.DefaultButton(ContentDialogButton::Close);

    return dialog;
  }
}
                           