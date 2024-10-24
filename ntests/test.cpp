#include <iostream>
#include <vector>

#include <dlfcn.h>
#include <string.h>

constexpr size_t LSTR = 255;

typedef int (PROXY_MAIN)(int argc, char** argv);

int main(int _argc, char** _argv) {
    void* handle = dlopen("../build/examples/libheif-enc.so", RTLD_NOW | RTLD_LOCAL);
    if (handle == nullptr) {
        std::cout << "Failed to load the library\n";
        return -1;
    }

    PROXY_MAIN* ProxyMain = reinterpret_cast<PROXY_MAIN*>(dlsym(handle, "proxy_main"));
    if (ProxyMain == nullptr) {
        std::cout << "Failed to load proxy_main\n";
        return -1;
    }

    std::vector<std::string> files = { 
        "test1.png", 
        "test1.jpeg", 
        "test2.jpeg",
        "test3.jpeg",
    };

    const int argc = 6;
    char** argv = new char*[argc];
    for (int i = 0; i < argc; i++) argv[i] = new char[LSTR];
    
    strcpy(argv[0], "libheif-enc");
    strcpy(argv[1], "-q");
    strcpy(argv[2], "50");
    int index = 0;
    for (auto &x: files) {
        strcpy(argv[3], x.c_str());
        strcpy(argv[4], "-o");
        strcpy(argv[5], std::string("out" + std::to_string(index) + ".heic").c_str());

        ProxyMain(argc, argv);
        
        std::cout << "Conversion complete for image at index: " << index << '\n';
        index += 1;
    }

    return 0;
}
