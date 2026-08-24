#include "pch.h"
#include "MainWindow.xaml.h"

#include <cmath>

#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;

namespace winrt::QuadraticDesktop::implementation
{
  winrt::fire_and_forget MainWindow::SolveButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e) {
    double a = nbxCoeffA().Value();
    double b = nbxCoeffB().Value();
    double c = nbxCoeffC().Value();

    if (!std::isfinite(a) || !std::isfinite(b) || !std::isfinite(c)) {
      ContentDialog errorDialog = GetErrorDialog(L"Ошибка ввода", L"Введите числовые коэффициенты a, b и c.");
      co_await errorDialog.ShowAsync();
      co_return;
    }

    quadratic::interop::Result result = quadratic::interop::Solve(a, b, c);
    ShowResult(result);
  }

  winrt::Microsoft::UI::Xaml::Controls::ContentDialog MainWindow::GetErrorDialog(winrt::hstring const& header, winrt::hstring const& message) {
    ContentDialog dialog;
    dialog.XamlRoot(RootGrid().XamlRoot());
    dialog.Title(box_value(header));
    dialog.Content(box_value(message));
    dialog.CloseButtonText(L"ОК");
    dialog.DefaultButton(ContentDialogButton::Close);

    return dialog;
  }

  winrt::fire_and_forget MainWindow::ShowResult(quadratic::interop::Result result) {
    tbWaitCoeff().Visibility(Visibility::Collapsed);
    grdOutput().Visibility(Visibility::Visible);

    switch (result.nRoots) {
    case 0:
      brdOneRootMsg().Visibility(Visibility::Collapsed);
      brdTwoRootsMsg().Visibility(Visibility::Collapsed);
      brdNoRootsMsg().Visibility(Visibility::Visible);
      brdInfRootsMsg().Visibility(Visibility::Collapsed);

      dField().Text(winrt::hstring{ std::format(L"{:.6g}", result.d) });
      tbFirstRoot().Visibility(Visibility::Collapsed);
      tbSecondRoot().Visibility(Visibility::Collapsed);
      break;
    case 1:
      brdOneRootMsg().Visibility(Visibility::Visible);
      brdTwoRootsMsg().Visibility(Visibility::Collapsed);
      brdNoRootsMsg().Visibility(Visibility::Collapsed);
      brdInfRootsMsg().Visibility(Visibility::Collapsed);


      dField().Text(winrt::hstring{ std::format(L"{:.6g}", result.d) });
      x1Field().Text(winrt::hstring{ std::format(L"{:.6g}", result.x1) });
      tbFirstRoot().Visibility(Visibility::Visible);
      tbSecondRoot().Visibility(Visibility::Collapsed);
      break;
    case 2:
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
    case 3:
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
}


