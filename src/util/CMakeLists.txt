
include_directories (
    "${CMAKE_CURRENT_LIST_DIR}"
    )

target_sources(atsdb
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/config.h"
        "${CMAKE_CURRENT_LIST_DIR}/files.h"
        "${CMAKE_CURRENT_LIST_DIR}/global.h"
        "${CMAKE_CURRENT_LIST_DIR}/savedfile.h"
        "${CMAKE_CURRENT_LIST_DIR}/number.h"
        "${CMAKE_CURRENT_LIST_DIR}/stringconv.h"
        "${CMAKE_CURRENT_LIST_DIR}/singleton.h"
        "${CMAKE_CURRENT_LIST_DIR}/system.h"
        "${CMAKE_CURRENT_LIST_DIR}/logger.h"
        "${CMAKE_CURRENT_LIST_DIR}/format.h"
        "${CMAKE_CURRENT_LIST_DIR}/formatselectionwidget.h"
        "${CMAKE_CURRENT_LIST_DIR}/datatypeformatselectionwidget.h"
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/config.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/files.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/format.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/logger.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/number.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/formatselectionwidget.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/datatypeformatselectionwidget.cpp"
)


