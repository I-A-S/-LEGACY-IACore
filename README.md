# [LEGACY] IACore (Independent Architecture Core)

<div align="center">
  <img src="logo.svg" alt="IACore Logo" width="400"/>
  <br/>
  
  <img src="https://img.shields.io/badge/license-apache_v2-blue.svg" alt="License"/>
  <img src="https://img.shields.io/badge/standard-C%2B%2B20-yellow.svg" alt="C++ Standard"/>
  <img src="https://img.shields.io/badge/platform-Windows%20%7C%20Linux-lightgrey.svg" alt="Platform"/>

  <p>
    <b>A High-Performance Foundation for Modern C++ 20 Applications.</b>
  </p>
</div>

> [!WARNING]
> IACore, as of now is officially retired. It served as the foundation for all IA Software for over 3 years.
> 
> With the introduction of Auxid, and our new internal standard reevaluations, we concluded that the monolithic design of IACore is not the future, and we're splitting IACore up to smaller, more focused set of libraries.
>
> You can find the new bedrock library IACrux [here](https://github.com/I-A-S/IACrux). IACrux is a much smaller and stripped down to essentials version of IACore.
>
> AsyncOps, FileOps and IPC will soon be refactored into their own library repositories as well. Thank you for using and showing your interest in IACore, while this is the end for IACore, it is the begining of a new and exciting chapter for the larger IA ecosystem.
>
> Hope to see you at [IACrux](https://github.com/I-A-S/IACrux)!

## **Description**

IACore is a high-performance C++20 foundation library bundling essential systems (IPC, Logging, Networking, Compression, and Async Scheduling) into a single, coherent API.

IACore strictly follows the coding style and philosophy of [Auxid](https://github.com/I-A-S/Auxid).

Originally developed as the internal core for IASoft (PVT) LTD., it is now open-source to provide a standardized bedrock for C++ applications where performance matters.

## **Features**

* **High-Performance IPC:** Shared-Memory Ring Buffers with wait-free SPSC synchronization.
* **Networking:** Integrated HTTP/HTTPS client (wrapper around `libcurl`).
* **Async Scheduler:** A job system with high/normal priority queues.
* **File I/O:** Memory-mapped file operations and optimized binary stream readers/writers.
* **Compression:** Unified API for Zlib, Gzip, and Zstd.
* **Logging:** Thread-safe, colored console and disk logging.
* **Modern C++:** Heavily utilizes modern C++20 concepts and `Auxid` for security and error handling.

## **Usage Examples**
### 1. IPC (Manager & Node)
IACore provides a manager/node architecture using shared memory.

#### Manager:
```C++
#include <IACore/IPC.hpp>

// Spawns a child process and connects via Shared Memory
auto node_id = manager.spawn_node("MyChildNodeExe");
manager.wait_till_node_is_online(*node_id);

// Send data with zero-copy overhead
String msg = "Hello Node";
manager.send_packet(*node_id, 100, {(PCUINT8)msg.data(), msg.size()});
```

#### Node:
```C++
#include <IACore/IPC.hpp>

class Node : public IACore::IPC_Node {
public:
    void on_signal(uint8_t signal) override {
        // Handle signals
    }

    void on_packet(uint16_t packet_id, std::span<const uint8_t> payload) override {
        // Handle packets
    }
};

int main(int argc, char* argv[]) {
    // The connection string is passed as the first argument by the Manager
    if (argc < 2) return -1;
    
    Node node;
    // Connect back to the manager via Shared Memory
    if (!node.connect(argv[1])) return -1;
    
    while(true) {
        node.update();
    }
    return 0;
}
```

### 2. Async Jobs
```C++

#include <IACore/AsyncOps.hpp>

// Initialize worker threads (hardware_concurrency - 2)
IACore::AsyncOps::initialize_scheduler();

// Schedule a task
IACore::AsyncOps::Schedule *my_schedule = new IACore::AsyncOps::Schedule();

IACore::AsyncOps::schedule_task([](auto worker_id) {
    printf("Running on worker %d\n", worker_id);
}, 0, my_schedule);

// Wait for completion
IACore::AsyncOps::wait_for_schedule_completion(my_schedule);
```

### 3. HTTP Request
```C++
#include <IACore/Http/Client.hpp>

IACore::HttpClient client("https://api.example.com");
auto res = client.json_get<MyResponseStruct>("/data", {});

if (res) {
    std::cout << "Data: " << res->value << "\n";
} else {
    std::cerr << "Error: " << res.error() << "\n";
}
```

## **Integration**

IACore is built with CMake. You can include it in your project via `FetchContent` or by adding it as a subdirectory.

### CMake Example
```cmake
cmake_minimum_required(VERSION 3.28)
project(MyGame)

# Or you can use FetchContent
add_subdirectory(external/IACore)

# Apply IACore's standard project configuration
# This applies C++20 and strict warning flags globally to your targets.
iacore_setup_project()

# Define your targets
add_executable(MyGame src/main.cpp)

# Link IACore
target_link_libraries(MyGame PRIVATE IACore)
```

## **Contributing**

We welcome contributions from the community!

### What we accept immediately:
* **Documentation:** Improvements to comments, the README, or external docs.
* **Tests:** New unit tests (in `Tests/`) to improve coverage or reproduce bugs.
* **Examples:** New usage examples or sample projects.
* **Bug Reports:** detailed issues describing reproduction steps are highly valued.

### Core Library Policy (`Src/` Directory)
Currently, **we are not accepting Pull Requests that modify the core source code (`Src/`)**.

If you find a critical bug in `Src/`, please open an **Issue** rather than a PR, and the core team will implement a fix ASAP.

## **Dependencies**
IACore manages its own dependencies via CMake's FetchContent. You do not need to install these manually:

* **JSON:** `glaze`
* **SIMD:** `google-highway`
* **Networking:** `libcurl`
* **Compression:** `zlib-ng` & `zstd`
* **Utilities:** `auxid`, `tl-expected` & `unordered_dense`

**Note:** Following dependencies are not directly used by IACore, but bundles them (+ helper wrappers) for user convenience: `nlohmann_json`, `simdjson`, `pugixml`

## **License**

This project is licensed under the Apache License Version 2.0.

## **Use of Generative AI at IASoft**

While we never let Generative AI to make architecural/design decisions, and we hand-write almost all of the implementations, we do use Generative AI (Google Gemini) for the following and *(ONLY following)* tasks:

1) **Controlled Repititive Boilerplate Generation:** Each and **every single line of AI generated code** is **manually reviewed** one-by-one for hallucinations and logic errors. Trust, but verify. 

2) **Concept Research and Development:** Design pattern comparisions, cost-benefit analysis and as a second pair of eyes to evalute and critique our design decisions.

3) **Documentation:** Repititive method doc strings (parameter, return value descriptions) are done mostly using Generative AI.

4) **Code Review:** Automated logic checking and static analysis on top of deterministic tools.
