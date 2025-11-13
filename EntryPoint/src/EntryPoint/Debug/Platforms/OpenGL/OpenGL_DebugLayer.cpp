#include "../../DebugLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include "../../../Application.h"

namespace apothec::debug
{

	DebugLayer::DebugLayer()
	{

	}

	DebugLayer::~DebugLayer()
	{

	}

	void
	DebugLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; 
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  

		Application& app = Application::Get();
		lithium::Window& win = app.GetWindow();
		GLFWwindow* glWindow = (GLFWwindow*)win.GetRawWindow();

		ImGui_ImplGlfw_InitForOpenGL(glWindow, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void 
	DebugLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void 
	DebugLayer::OnUpdate(double deltaTime)
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		lithium::Window& win = app.GetWindow();

		io.DisplaySize = ImVec2(win.GetWidth(), win.GetHeight());
		io.DeltaTime = (float)deltaTime;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		ImGui::UpdatePlatformWindows();
	}

	void
	DebugLayer::OnEvent(lithium::events::Event& event)
	{

	}

}