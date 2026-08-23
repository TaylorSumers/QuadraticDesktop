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

  void MainWindow::ShowResult(quadratic::interop::Result result) {
    spOutput().Visibility(Visibility::Collapsed);
    tbResultMessage().Visibility(Visibility::Collapsed);
    tbSecondRoot().Visibility(Visibility::Visible);
    switch (result.nRoots) {
    case 0:
      tbResultMessage().Text(L"Нет корней");
      tbResultMessage().Visibility(Visibility::Visible);
      break;
    case 1:
      x1Field().Text(winrt::hstring{ std::format(L"{:.6g}", result.x1) });
      tbSecondRoot().Visibility(Visibility::Collapsed);
      spOutput().Visibility(Visibility::Visible);
      break;
    case 2:
      x1Field().Text(to_hstring(winrt::hstring{ std::format(L"{:.6g}", result.x1) }));
      x2Field().Text(to_hstring(winrt::hstring{ std::format(L"{:.6g}", result.x2) }));
      spOutput().Visibility(Visibility::Visible);
      break;
    case 3:
      tbResultMessage().Text(L"Любое число является корнем");
      tbResultMessage().Visibility(Visibility::Visible);
      break;
    default:
      tbResultMessage().Text(L"Неизвестный результат вычисления");
      tbResultMessage().Visibility(Visibility::Visible);
      break;
    }
  }
}


