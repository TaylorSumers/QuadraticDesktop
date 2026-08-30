#include "pch.h"
#include "MainWindow.xaml.h"

#include <cmath>

#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;
using namespace winrt::Microsoft::Web::WebView2::Core;
using namespace winrt::Windows::Foundation;
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


    Interop::Solution solution = Interop::Solve(a, b, c);
    co_await ShowSolutionAsync(solution);
    UpdateGraph(a, b, c, solution);
    ShowGraphInfo(a, b, c, Interop::GetGraphInfo(a, b, c));
  }

  fire_and_forget MainWindow::GraphView_Loaded(IInspectable const& sender, RoutedEventArgs const& e)
  {
    bool initializationFailed = false;
    hstring errorMessage{};

    try {
      co_await GraphView().EnsureCoreWebView2Async();
      CoreWebView2 core = GraphView().CoreWebView2();

      hstring plotlyFolder{
          winrt::Windows::ApplicationModel::Package::Current()
              .InstalledLocation()
              .Path()
              + PLOTLY_RELATIVE_PATH
      };

      core.SetVirtualHostNameToFolderMapping(
        VIRTUAL_HOST_NAME,
        plotlyFolder,
        winrt::Microsoft::Web::WebView2::Core::
        CoreWebView2HostResourceAccessKind::DenyCors);

      core.Navigate(GRAPH_URL);
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
    graphData_.isReady = e.IsSuccess();

    if (graphData_.isReady && graphData_.hasData) {
      SendGraphData();
    }
  }

  void MainWindow::UpdateGraph(double a, double b, double c, Interop::Solution solution) 
  {
    graphData_.coeffA = a;
    graphData_.coeffB = b;
    graphData_.coeffC = c;
    graphData_.x1 = solution.x1;
    graphData_.x2 = solution.x2;
    graphData_.nRoots = solution.nRoots;
    graphData_.hasData = true;

    if (graphData_.isReady) {
      SendGraphData();
    }
  }

  void MainWindow::SendGraphData()
  {
    std::wstring json = std::format(
      LR"({{ "a":{:.17g},"b":{:.17g},"c":{:.17g}, "x1":{:.17g}, "x2":{:.17g}, "nRoots":{} }})",
      graphData_.coeffA,
      graphData_.coeffB,
      graphData_.coeffC,
      graphData_.x1,
      graphData_.x2,
      static_cast<int>(graphData_.nRoots)
    );

    GraphView().CoreWebView2().PostWebMessageAsJson(hstring{ json });
  }

  void MainWindow::ShowGraphInfo(double a, double b, double c, Interop::GraphInfo graphInfo)
  {
    ShowEquationString(a, b, c);

    if (graphInfo.branchesDirection == UP || graphInfo.branchesDirection == DOWN) {
      tbVertexCords().Text(winrt::hstring{ std::format(L"({:.3g}, {:.3g})", graphInfo.vertex.x, graphInfo.vertex.y) });
      tbAxisOfSymmetry().Text(winrt::hstring{ std::format(L"x = {:.3g}", graphInfo.vertex.x) });
    }
    else {
      tbVertexCords().Text(L"–");
      tbAxisOfSymmetry().Text(L"–");
    }

    ShowBranchesDirection(graphInfo.branchesDirection);
  }

  IAsyncAction MainWindow::ShowSolutionAsync(Interop::Solution result)
  {
    ShowSolutionOutput();

    switch (result.nRoots) {
    case NO_ROOTS:
      ShowNoRootsResult(result.d);
      break;
    case ONE_ROOT:
      ShowOneRootResult(result.d, result.x1);
      break;
    case TWO_ROOTS:
      ShowTwoRootsResult(result.d, result.x1, result.x2);
      break;
    case INF_ROOTS:
      ShowInfRootsResult(result.d);
      break;
    default:
      HideSolutionOutput();
      ContentDialog errorDialog = GetErrorDialog(L"Ошибка вычисления", L"Неизвестный результат");
      co_await errorDialog.ShowAsync();
      break;
    }
  }

  void MainWindow::ShowBranchesDirection(BranchesDirection direction) 
  {
    winrt::hstring branchesDirectionStr{};
    switch (direction) {
    case UP:
      branchesDirectionStr = L"вверх";
      break;
    case DOWN:
      branchesDirectionStr = L"вниз";
      break;
    default:
      branchesDirectionStr = L"–";
      break;
    }
    tbBranchesDirection().Text(branchesDirectionStr);
  }

  void MainWindow::ShowEquationString(double a, double b, double c)
  {
    tbEquationString().Text(winrt::hstring{ std::format(L"y = {:.6g}x² + {:.6g}x + {:.6g}", a, b, c) });
    brdEquationString().Visibility(Visibility::Visible);
  }

  void MainWindow::ShowSolutionOutput() 
  {
    tbWaitCoeff().Visibility(Visibility::Collapsed);
    grdOutput().Visibility(Visibility::Visible);
  }

  void MainWindow::HideSolutionOutput() 
  {
    tbWaitCoeff().Visibility(Visibility::Visible);
    grdOutput().Visibility(Visibility::Collapsed);
  }

  void MainWindow::ShowNoRootsResult(double d) 
  {
    brdOneRootMsg().Visibility(Visibility::Collapsed);
    brdTwoRootsMsg().Visibility(Visibility::Collapsed);
    brdNoRootsMsg().Visibility(Visibility::Visible);
    brdInfRootsMsg().Visibility(Visibility::Collapsed);

    dField().Text(winrt::hstring{ std::format(L"{:.6g}", d) });
    tbFirstRoot().Visibility(Visibility::Collapsed);
    tbSecondRoot().Visibility(Visibility::Collapsed);
  }

  void MainWindow::ShowOneRootResult(double d, double x1) 
  {
    brdOneRootMsg().Visibility(Visibility::Visible);
    brdTwoRootsMsg().Visibility(Visibility::Collapsed);
    brdNoRootsMsg().Visibility(Visibility::Collapsed);
    brdInfRootsMsg().Visibility(Visibility::Collapsed);


    dField().Text(winrt::hstring{ std::format(L"{:.6g}", d) });
    x1Field().Text(winrt::hstring{ std::format(L"{:.6g}", x1) });
    tbFirstRoot().Visibility(Visibility::Visible);
    tbSecondRoot().Visibility(Visibility::Collapsed);
  }

  void MainWindow::ShowTwoRootsResult(double d, double x1, double x2) 
  {
    brdOneRootMsg().Visibility(Visibility::Collapsed);
    brdTwoRootsMsg().Visibility(Visibility::Visible);
    brdNoRootsMsg().Visibility(Visibility::Collapsed);
    brdInfRootsMsg().Visibility(Visibility::Collapsed);

    dField().Text(winrt::hstring{ std::format(L"{:.6g}", d) });
    x1Field().Text(to_hstring(winrt::hstring{ std::format(L"{:.6g}", x1) }));
    x2Field().Text(to_hstring(winrt::hstring{ std::format(L"{:.6g}", x2) }));
    tbFirstRoot().Visibility(Visibility::Visible);
    tbSecondRoot().Visibility(Visibility::Visible);
  }

  void MainWindow::ShowInfRootsResult(double d) 
  {
    brdOneRootMsg().Visibility(Visibility::Collapsed);
    brdTwoRootsMsg().Visibility(Visibility::Collapsed);
    brdNoRootsMsg().Visibility(Visibility::Collapsed);
    brdInfRootsMsg().Visibility(Visibility::Visible);

    dField().Text(winrt::hstring{ std::format(L"{:.6g}", d) });
    tbFirstRoot().Visibility(Visibility::Collapsed);
    tbSecondRoot().Visibility(Visibility::Collapsed);
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
                           