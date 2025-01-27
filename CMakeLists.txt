cmake_minimum_required(VERSION 3.1)

set(project_name async-api) ## rename your project here

project(${project_name})

set(CMAKE_CXX_STANDARD 11)

# 在设置标志之前，添加一个条件判断，如果已经设置了该标志，则将其移除
if(OATPP_COMPAT_BUILD_NO_THREAD_LOCAL)
    message(STATUS "Removing OATPP_COMPAT_BUILD_NO_THREAD_LOCAL flag")
    remove_definitions(-DOATPP_COMPAT_BUILD_NO_THREAD_LOCAL)
endif()

# add_definitions(-DOATPP_COMPAT_BUILD_NO_THREAD_LOCAL)

add_library(${project_name}-lib
        src/AppComponent.hpp
        src/controller/MyController.cpp
        src/controller/MyController.hpp
        src/dto/MyDTOs.hpp
        src/predictor/OnnxRuntimePredictor.hpp
)

## link libs

find_package(oatpp 1.3.0 REQUIRED)
find_package(onnxruntime HINTS /usr/local/include/onnxruntime)

# target_link_libraries(${project_name}-lib
#         PUBLIC oatpp::oatpp
#         PUBLIC oatpp::oatpp-test
#         PUBLIC onnxruntime
#         PUBLIC profiler 
#         PUBLIC unwind
# )

target_link_libraries(${project_name}-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-test
        PUBLIC onnxruntime
)

target_include_directories(${project_name}-lib PUBLIC src)

## add executables

add_executable(${project_name}-server
        src/App.cpp
)
# target_link_libraries(${project_name}-server ${project_name}-lib onnxruntime profiler unwind)
target_link_libraries(${project_name}-server ${project_name}-lib onnxruntime)
add_dependencies(${project_name}-server ${project_name}-lib)

add_executable(${project_name}-test
        test/tests.cpp
)
# target_link_libraries(${project_name}-test ${project_name}-lib onnxruntime profiler unwind)
target_link_libraries(${project_name}-test ${project_name}-lib onnxruntime)
add_dependencies(${project_name}-test ${project_name}-lib)

set_target_properties(${project_name}-lib ${project_name}-server ${project_name}-test PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)

enable_testing()
add_test(project-tests ${project_name}-test)
