# FindMQSDK.cmake
#
# Locates the Metasequoia 4 plugin SDK.
#
# The SDK is not redistributable, so users must download it from
#   https://www.metaseq.net/jp/download/sdk/
# and place its contents under <repo>/sdk/mqsdk/ (or set MQSDK_ROOT).
#
# Defines:
#   MQSDK_FOUND        - TRUE if the SDK was located
#   MQSDK_INCLUDE_DIR  - directory containing MQPlugin.h
#   MQSDK_SOURCES      - SDK sources to compile alongside the plugin

set(_mqsdk_search_paths
    "${MQSDK_ROOT}"
    "$ENV{MQSDK_ROOT}"
    "${CMAKE_SOURCE_DIR}/sdk/mqsdk"
    "${CMAKE_SOURCE_DIR}/sdk"
)

find_path(MQSDK_INCLUDE_DIR
    NAMES MQPlugin.h
    PATHS ${_mqsdk_search_paths}
    NO_DEFAULT_PATH
)

set(MQSDK_SOURCES "")
if(MQSDK_INCLUDE_DIR)
    foreach(_src mqsdk.cpp MQBasePlugin.cpp MQWidget.cpp MQ3DLib.cpp MQHandleObject.cpp MQSetting.cpp)
        if(EXISTS "${MQSDK_INCLUDE_DIR}/${_src}")
            list(APPEND MQSDK_SOURCES "${MQSDK_INCLUDE_DIR}/${_src}")
        endif()
    endforeach()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MQSDK
    REQUIRED_VARS MQSDK_INCLUDE_DIR
)

mark_as_advanced(MQSDK_INCLUDE_DIR MQSDK_SOURCES)
