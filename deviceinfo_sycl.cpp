#include <CL/sycl.hpp>
#include <vector>

template <typename T, typename convType = uint32_t>
void printVec(std::vector<T> &vec)
{
    for (auto &v : vec)
    {
        std::cout << (convType)v << "\t";
    }
    std::cout << std::endl;
}
int main()
{

    auto devices = sycl::device::get_devices();

    for (auto &device : devices)
    {
        std::cout << (const std::string)device.get_info<sycl::info::device::name>() << std::endl;
        std::cout << "Is cpu:\t" << device.is_cpu() << std::endl;
        auto exec_caps = device.get_info<sycl::info::device::execution_capabilities>();
        std::cout << "Execution capabilities:\t";
        printVec(exec_caps);

        auto extensions = device.get_info<sycl::info::device::aspects>();
        std::cout << "Extensions:\t";
        printVec<std::string, std::string>(extensions);

    }
}
