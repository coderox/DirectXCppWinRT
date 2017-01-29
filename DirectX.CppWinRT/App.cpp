#include "pch.h"
#include "Common\DeviceResources.h"
#include "Main.h"

using namespace winrt;
using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::Composition;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;

WINRT_EXPORT namespace DirectX_CppWinRT {

	struct App : implements<App, IFrameworkViewSource, IFrameworkView>
	{
		IFrameworkView CreateView()
		{
			return *this;
		}

		void Initialize(CoreApplicationView const &)
		{
		}

		void Load(hstring_ref)
		{
		}

		void Uninitialize()
		{
		}

		void SetWindow(CoreWindow const & window)
		{
			// At this point we have access to the device. 
			// We can create the device-dependent resources.
			m_deviceResources = std::make_shared<DX::DeviceResources>();
			m_deviceResources->SetWindow(CoreWindow::GetForCurrentThread());

			window.Activate();

			if (m_main == nullptr)
			{
				m_main = std::unique_ptr<Main>(new Main(m_deviceResources));
			}

			m_windowClosed = false;
			m_windowVisible = true;
		}

		void Run()
		{
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
		}

	private:

		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::unique_ptr<Main> m_main;
		bool m_windowClosed;
		bool m_windowVisible;

		CoreWindow::Activated_revoker m_activated;
		CompositionTarget m_target{ nullptr };
	};
}


int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	/*Application::Start([](auto &&)
	{
		winrt::make<DirectX_CppWinRT::App>();
	});*/
	CoreApplication::Run(DirectX_CppWinRT::App());
}