// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file log.hpp
/// Main include file for logging utilities in code model library.

#include <boost/log/trivial.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/log/attributes/constant.hpp>


namespace cm::log {


template <auto N>
struct log_attr_string {
    constexpr log_attr_string(const char (&s)[N]) {
        std::copy_n(s, N, value);
    }

    /// Returns string value containing attribute
    std::string string() const {
        return std::string{&value[0], sizeof(value) - 1};
    }

    char value[N];
};


using logger_t = boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>;


/// Returns reference to logger with category attribute
template <log_attr_string Category>
inline logger_t & get_logger() {
    auto init_logger = [] {
        logger_t logger;
        logger.add_attribute("Category", boost::log::attributes::make_constant(Category.string()));
        return logger;
    };
    static logger_t logger = init_logger();
    return logger;
}


/// Returns reference to logger with category and subcategory attributes
template <log_attr_string Category, log_attr_string Subcategory>
inline logger_t & get_logger_scat() {
    auto init_logger = [] {
        logger_t logger;
        logger.add_attribute("Category", boost::log::attributes::make_constant(Category.string()));
        logger.add_attribute("Subcategory",
                             boost::log::attributes::make_constant(Subcategory.string()));
        return logger;
    };
    static logger_t logger = init_logger();
    return logger;
}


#define CM_LOG(cat, level) \
    BOOST_LOG_SEV((::cm::log::get_logger<#cat>()), level)

#define CM_LOG_TRACE(cat)       CM_LOG(cat, ::boost::log::trivial::trace)
#define CM_LOG_DEBUG(cat)       CM_LOG(cat, ::boost::log::trivial::debug)
#define CM_LOG_INFO(cat)        CM_LOG(cat, ::boost::log::trivial::info)
#define CM_LOG_WARNING(cat)     CM_LOG(cat, ::boost::log::trivial::warning)
#define CM_LOG_ERROR(cat)       CM_LOG(cat, ::boost::log::trivial::error)
#define CM_LOG_FATAL(cat)       CM_LOG(cat, ::boost::log::trivial::fatal)


#define CM_LOG_SCAT(cat, scat, level) \
    BOOST_LOG_SEV((::cm::log::get_logger_scat<#cat, #scat>()), level)

#define CM_LOG_SCAT_TRACE(cat, scat)    CM_LOG_SCAT(cat, scat, ::boost::log::trivial::trace)
#define CM_LOG_SCAT_DEBUG(cat, scat)    CM_LOG_SCAT(cat, scat, ::boost::log::trivial::debug)
#define CM_LOG_SCAT_INFO(cat, scat)     CM_LOG_SCAT(cat, scat, ::boost::log::trivial::info)
#define CM_LOG_SCAT_WARNING(cat, scat)  CM_LOG_SCAT(cat, scat, ::boost::log::trivial::warning)
#define CM_LOG_SCAT_ERROR(cat, scat)    CM_LOG_SCAT(cat, scat, ::boost::log::trivial::error)
#define CM_LOG_SCAT_FATAL(cat, scat)    CM_LOG_SCAT(cat, scat, ::boost::log::trivial::fatal)


}
