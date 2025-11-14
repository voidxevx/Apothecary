#include "../../DebugLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include "../../../Application.h"

#include <iostream>

namespace apothec::debug
{
	
	void
	DebugLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  

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

		io.DisplaySize = ImVec2((float)win.GetWidth(), (float)win.GetHeight());
		io.DeltaTime = (float)deltaTime;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Dockspace
		//ImGui::DockSpaceOverViewport();

		m_WidgetChain->WidgetUpdate(deltaTime);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void DebugLayer::OnEvent(lithium::events::Event& event)
	{

	}

}