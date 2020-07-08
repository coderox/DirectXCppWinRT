#include "pch.h"
#include "Common\DeviceResources.h"
#include "Main.h"

using namespace winrt;
using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::Composition;
using namespace winrt::Windows::Foundation::Collections;

WINRT_EXPORT namespace DirectX_CppWinRT {

	struct App : implements<App, IFrameworkViewSource, IFrameworkView>
	{
		IFrameworkView CreateView() const
		{
			return *this;
		}

		static void Initialize(CoreApplicationView const&)
		{
		}

		static void Load(hstring)
		{
		}

		static void Uninitialize()
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
				m_main = std::make_unique<Main>(m_deviceResources);
			}

			m_windowClosed = false;
			m_windowVisible = true;
		}

		void Run() const
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
		bool m_windowClosed = false;
		bool m_windowVisible = false;

		CoreWindow::Activated_revoker m_activated;
		CompositionTarget m_target{ nullptr };
	};
}


int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	CoreApplication::Run(winrt::make<DirectX_CppWinRT::App>());
	return 0;
}