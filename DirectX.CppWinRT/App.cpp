#include "pch.h"
#include "Common\DeviceResources.h"
#include "Main.h"

using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt; 

WINRT_EXPORT namespace DirectX_CppWinRT {

	struct App : ApplicationT<App>
	{
	public:

		App() { }

		void OnWindowCreated(const IInspectable&) {
			
			// At this point we have access to the device. 
			// We can create the device-dependent resources.
			m_deviceResources = std::make_shared<DX::DeviceResources>();
			m_deviceResources->SetWindow(CoreWindow::GetForCurrentThread());

			auto window = CoreWindow::GetForCurrentThread();
			window.Activate();

			if (m_main == nullptr)
			{
				m_main = std::unique_ptr<Main>(new Main(m_deviceResources));
			}

			Run();

			m_windowClosed = false;
			m_windowVisible = true;
		}

	private:

		void Run()
		{
			CoreWindow::GetForCurrentThread().Dispatcher().RunAsync(CoreDispatcherPriority::Normal, [this]() {
				while (!m_windowClosed)
				{
					if (m_windowVisible)
					{
						CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

						m_main->Update();

						if (m_main->Render())
						{
							m_deviceResources->Present();
						}
					}
					else
					{
						CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
					}
				}
			});
		}

		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::unique_ptr<Main> m_main;
		bool m_windowClosed;
		bool m_windowVisible;
	};
}


int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	Application::Start([](auto &&)
	{
		winrt::make<DirectX_CppWinRT::App>();
	});
}