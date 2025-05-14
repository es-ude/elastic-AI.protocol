# EAIP: C

## Include via `CMakeFetchContent`

To include this project with the `CMakeFetchContent` directives you can paste the following code snippet to your `CMakeLists.txt`

```text
FetchContent_Declare(
        elasticai_protocol
        GIT_REPOSITORY https://github.com/es-ude/elastic-AI.protocol.git
        GIT_TAG v3.1.0)
FetchContent_MakeAvailable(elasticai_protocol)
```

> [!IMPORTANT]
> Update the tag inside the `FetchContent_Declare` directive if you want to use a different version of the elastic-AI protocol implementation.
