include(FetchContent)

macro(add_cexception)
    FetchContent_Declare(
            cexception
            GIT_REPOSITORY https://github.com/ThrowTheSwitch/CException.git
            GIT_TAG v1.3.3)
    FetchContent_MakeAvailable(cexception)

    add_library(CException__impl ${cexception_SOURCE_DIR}/lib/CException.c)
    add_library(CException__hdrs INTERFACE)
    target_include_directories(CException__hdrs
            INTERFACE ${cexception_SOURCE_DIR}/lib/)
    add_library(CException INTERFACE)
    target_link_libraries(CException INTERFACE CException__hdrs CException__impl)
endmacro()

macro(add_ctest)
    include(CTest)
endmacro()

macro(add_unity)
    FetchContent_Declare(
            unity
            GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
            GIT_TAG v2.5.2
            OVERRIDE_FIND_PACKAGE)
    FetchContent_MakeAvailable(unity)
    find_package(unity)
endmacro()
