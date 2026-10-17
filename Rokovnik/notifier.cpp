#include "notifier.h"
#include "wintoastlib.h"
#include <windows.h>
using namespace WinToastLib;
namespace {

	std::wstring toWide(const std::string& s) {
		if (s.empty())return L"";
		int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), nullptr, 0);
		std::wstring w(n, L'\0');
		MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), &w[0], n);
		return w;
	}

	class Handler : public IWinToastHandler {
	public:
		void toastActivated() const override {}
		void toastActivated(int) const override {}
		void toastActivated(std::wstring response) const override {};
		void toastDismissed(WinToastDismissalReason) const override {}
		void toastFailed() const override {}
	};
}
bool showToast(const std::string& title, const std::string& message) {
	if (!WinToast::isCompatible()) {
		std::cerr << "Windows verzija nije podrzana\n";
		return false;
	}
	WinToast::instance()->setAppName(L"Rokovnik");
	const auto aumi = WinToast::configureAUMI(L"Lazar",
		L"Rokovnik", L"Podsetnik", L"1");
	WinToast::instance()->setAppUserModelId(aumi);

	WinToast::WinToastError error;
	if (!WinToast::instance()->initialize(&error)) {
		std::cerr << "Init greska, kod: " << (int)error << "\n";
		return false;
	}

	WinToastTemplate temp1(WinToastTemplate::Text02);
	temp1.setTextField(toWide(title), WinToastTemplate::FirstLine);
	temp1.setTextField(toWide(message), WinToastTemplate::SecondLine);

	if (WinToast::instance()->showToast(temp1, new Handler(), &error) < 0) {
		std::cerr << "Show greska, kod: " << (int)error << "\n";
		return false;
	}

	Sleep(5000);   // da program ne izadje pre nego sto se toast prikaze
	return true;
}