#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;

// ��ȿ�� �˻� ���̾�
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
		// ��ȿ�� �˻� ���̾� ���
		if (enableValidationLayers && !checkValidationLayerSupport())
		{
			throw runtime_error("validation layers requested, but not available!");
		}



		// ���ø����̼ǿ� ���� ������ ��� struct�� ä���ִ´�.
		// �̷��� �����ʹ� ���û��������� ����̹��� Ư�� ���� ���α׷��� ����
		// ����ȭ �� �� �ִ� ������ ������ ���� �����ϱ� ����.
		VkApplicationInfo appInfo = {}; // ����ü ȣ��
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0, );
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		// ������ ���� ����ü ������ ������ Vulkan ����̹����� � ���� Ȯ�� �� ��ȿ�� �˻� ������ ������� �˷��ش�.
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;


		auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		// VkInstanceCrateInfo���� ��ȿ�� �˻� ���̾� �̸��� �����ϵ��� ����ü �ν��Ͻ�ȭ�� ����
		if (enableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}







		// ���ϴ� ���� Ȯ���� ����. GLFW ����ü�� ������ �� �ִ� Ȯ�� ����� ��ȯ �ϴ� �⺻ ���� �Լ� glfwGetRequiredInstanceExtensions�� ���� �Ѵ�.
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		// ����ü ������ �θ���� Ȱ��ȭ �� ���� ��ȿ�� �˻� ���� ����. ������ ����.
		createInfo.enabledLayerCount = 0;

		// Vulkan �ν��Ͻ� ������ �ʿ��� ������ ���������Ƿ� �ν��Ͻ� �����Ѵ�.
		// vkCreateInstance (���� ���� ����ü ������, ����� ���� �ݹ� �Լ� ������, �� ��ü�� ���� �ڵ��� �����ϴ� ������ ������)
		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

		if (result != VK_SUCCESS)
			throw std::runtime_error("failed to crate instance!");
		else
			cout << "success to create instance" << endl;

		//// Ȯ�� ���α׷� ���� Ȯ��
		//// Ȯ�� ���� ���� �迭 �Ҵ��� ���� ���� Ȯ�� ���� �޾ƿ´�.
		//uint32_t extensionCount = 0;
		//vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		//// �迭 �Ҵ�
		//vector<VkExtensionProperties> extensions(extensionCount);
		//
		//// Ȯ�� ���� ���� ����
		//vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		//// vkExtensionProperties ����ü���� Ȯ���� �̸��� ������ ����
		//cout << "available extensions :" << endl;

		//for (const auto& extension : extensions)
		//{
		//	cout << "\t" << extension.extensionName << "\t" << extension.specVersion <<endl;
		//}

		//// ������ VkInstance�� ���α׷� �����ϱ� ���� �ı��ؾ��Ѵ�.
		//// ����� ���� �Լ� cleanup���� vkDestroyInstance�� ȣ���� �ı��Ѵ�.
		
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

	// ��ȿ�� �˻� ���̾� �޼��� �ݹ��� ���� ��ȿ�� �˻� ���̾��� ��� ���� ���ο� ���� �ʿ��� Ȯ�� ���α׷� ����� ��ȯ�ϴ� �Լ�
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

	// ��û�� ��� ���̾ ����� �� �ִ��� Ȯ���ϴ� �Լ�
	bool checkValidationLayerSupport()
	{
		// ��� ������ ��� ���̾� ����
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

	// ����� �ݹ� �Լ�
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageServerity, // �޼��� �ɰ��� �÷���
		VkDebugUtilsMessageTypeFlagsEXT messageType, // ��� �� ���� ���� �Ǻ�
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, // ���� �߿��� ������. ����, �޽��� ��ü�� ���λ����� �����ϴ� ����ü
		void* pUserData) // �ݹ� ���� �߿� ������ ������. �ڽ��� �����͸� �ݹ鿡 ���� ����
	{
		cerr << "validation layer : " << pCallbackData->pMessage << endl;

		return VK_FALSE; // �ݹ��� true�� ��ȯ�ϸ� ���� �߻��Ͽ� ȣ���� �ߴܵȴ�.
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