
# Logging utilities library for code model library
add_library(cm-log
            log_init.cpp)
target_link_libraries(cm-log PUBLIC
                      Boost::program_options
                      Boost::log
                      ${CM_BOOST_HEADERS_LIBRARIES})
target_include_directories(cm-log PUBLIC "${CM_INCLUDE_DIR}")
