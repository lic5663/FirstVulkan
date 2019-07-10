#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;

// 유효성 검사 레이어
const vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif // NDEBUG



class HelloTriangleApplication
{
public:
	void run()
	{
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	GLFWwindow* window;

	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;

	void initWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	}

	void initVulkan()
	{
		createInstance();
		setupDebugMessenger();
	}

	void setupDebugMessenger()
	{
		if (!enableValidationLayers)
			return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = nullptr; // Optional
		
	}

	void createInstance()
	{
		// 유효성 검사 레이어 사용
		if (enableValidationLayers && !checkValidationLayerSupport())
		{
			throw runtime_error("validation layers requested, but not available!");
		}



		// 어플리케이션에 대한 정보가 담긴 struct를 채워넣는다.
		// 이러한 데이터는 선택사항이지만 드라이버가 특정 응용 프로그램을 위해
		// 최적화 할 수 있는 유용한 정보를 제공 가능하기 때문.
		VkApplicationInfo appInfo = {}; // 구조체 호출
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0, );
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		// 사전에 만든 구조체 정보를 전달해 Vulkan 드라이버에게 어떤 전역 확장 및 유효성 검사 계층을 사용할지 알려준다.
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;


		auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		// VkInstanceCrateInfo에서 유효성 검사 레이어 이름을 포함하도록 구조체 인스턴스화를 수정
		if (enableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}







		// 원하는 전역 확장을 지정. GLFW 구조체에 전달할 수 있는 확장 기능을 반환 하는 기본 제공 함수 glfwGetRequiredInstanceExtensions가 존재 한다.
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		// 구조체 마지막 두멤버가 활성화 할 전역 유효성 검사 계층 결정. 지금은 비운다.
		createInfo.enabledLayerCount = 0;

		// Vulkan 인스턴스 생성에 필요한 모든것을 지정했으므로 인스턴스 생성한다.
		// vkCreateInstance (생성 정보 구조체 포인터, 사용자 정의 콜백 함수 포인터, 새 객체에 대한 핸들을 저장하는 변수의 포인터)
		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

		if (result != VK_SUCCESS)
			throw std::runtime_error("failed to crate instance!");
		else
			cout << "success to create instance" << endl;

		//// 확장 프로그램 지원 확인
		//// 확장 정보 저장 배열 할당을 위해 현재 확장 수를 받아온다.
		//uint32_t extensionCount = 0;
		//vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		//// 배열 할당
		//vector<VkExtensionProperties> extensions(extensionCount);
		//
		//// 확장 세부 정보 쿼리
		//vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		//// vkExtensionProperties 구조체에는 확장의 이름과 버전이 포함
		//cout << "available extensions :" << endl;

		//for (const auto& extension : extensions)
		//{
		//	cout << "\t" << extension.extensionName << "\t" << extension.specVersion <<endl;
		//}

		//// 생성된 VkInstance는 프로그램 종료하기 전에 파괴해야한다.
		//// 사용자 정의 함수 cleanup에서 vkDestroyInstance를 호출해 파괴한다.
		
	}

	void mainLoop()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}
	}

	void cleanup()
	{
		vkDestroyInstance(instance, nullptr);

		glfwDestroyWindow(window);

		glfwTerminate();
	}

	// 유효성 검사 레이어 메세지 콜백을 위한 유효성 검사 레이어의 사용 가능 여부에 따라 필요한 확장 프로그램 목록을 반환하는 함수
	vector<const char*> getRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	// 요청된 모든 레이어를 사용할 수 있는지 확인하는 함수
	bool checkValidationLayerSupport()
	{
		// 사용 가능한 모든 레이어 나열
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers)
		{
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				return false;
			}
		}
		
		return true;
	}

	// 디버그 콜백 함수
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageServerity, // 메세지 심각도 플래그
		VkDebugUtilsMessageTypeFlagsEXT messageType, // 사양 및 성능 문제 판별
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, // 가장 중요한 구성원. 상태, 메시지 자체의 세부사항을 포함하는 구조체
		void* pUserData) // 콜백 설정 중에 지정된 포인터. 자신의 데이터를 콜백에 전달 가능
	{
		cerr << "validation layer : " << pCallbackData->pMessage << endl;

		return VK_FALSE; // 콜백이 true를 반환하면 오류 발생하여 호출이 중단된다.
	}

};

int main()
{
	HelloTriangleApplication app;

	try
	{
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}