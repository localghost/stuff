#ifndef CORE_EXCEPTION_H
#define CORE_EXCEPTION_H

#include <string>
#include <sstream>
#include <boost/exception/all.hpp>
#include "inspect.h"

namespace core {
namespace error_info {
#if SUPPORTED(CPP11)
using errinfo_message = boost::error_info<struct errinfo_message_, std::string>;
#else
typedef boost::error_info<struct error_info_message_, std::string> message;
#endif
}

struct exception : virtual std::exception, virtual boost::exception {};
}

#define MAKE_EXCEPTION_TYPE(ClassName) \
        struct ClassName : virtual ::core::exception { }

#define MAKE_EXCEPTION(ClassName) \
        ::boost::enable_error_info(ClassName{}) \
            << ::boost::throw_file{__FILE__} \
            << ::boost::throw_line{int(__LINE__)} \
            << ::boost::throw_function{BOOST_CURRENT_FUNCTION}

#define THROW(ClassName) ::boost::throw_exception(MAKE_EXCEPTION(ClassName))

// FIXME Add some prefix
#define THROW_MESSAGE(ClassName, Message) \
        do { \
            ::std::ostringstream oss; \
            oss << Message; \
            ::boost::throw_exception( \
                MAKE_EXCEPTION(ClassName) \
                    << ::core::error_info::message{oss.str()}); \
        } while (false)

namespace core {
// FIXME split to sub-operations, e.g. file_not_found, file_open_failed ...
/// Exception resulted from an I/O operation
MAKE_EXCEPTION_TYPE(io_error);
}

// FIXME This should probably be moved to a separate file
namespace gl {
/// Exception resulted from errors returned by OpenGL related API
MAKE_EXCEPTION_TYPE(gl_error);
}

#endif // CORE_EXCEPTION_H
