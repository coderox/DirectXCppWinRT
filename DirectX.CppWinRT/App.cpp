#include "pch.h"

using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt; 

namespace DirectX_CppWinRT {

	struct App : ApplicationT<App>
	{
	public:

		App() { }

		void OnWindowCreated(const IInspectable&) {
			auto window = CoreWindow::GetForCurrentThread();
			window.Activate();
		}

	private:
	};
}


int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	Application::Start([](auto &&)
	{
		winrt::make<DirectX_CppWinRT::App>();
	});
}