
import std;
import glm;
import vulkan_hpp;

int main() {
    glm::vec4 vec(1, 2, 3, 4);
    glm::mat4 mat(1);
    glm::ortho(1, 2, 1, 2, 1, 10);

    std::cout << "Hello, World!" << std::endl;

    vk::InstanceCreateInfo createInfo;

    vk::Instance instance = vk::createInstance(createInfo);

    return 0;
}