




#ifndef EOL_OP_GLOBAL_H
#define EOL_OP_GLOBAL_H

#include <cassert>
#include <iostream>

namespace eol {

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define ASSERT_WITH_MSG(cond, msg)                                                  \
    if(!(cond)) {                                                                   \
        std::cerr << "Assertion failed: (" << #cond << "), " << msg << std::endl;   \
        assert(false);  \
    }

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define Q_DECL_EXPORT __declspec(dllexport)
#  define Q_DECL_IMPORT __declspec(dllimport)
#else
#  define Q_DECL_EXPORT     __attribute__((visibility("default")))
#  define Q_DECL_IMPORT     __attribute__((visibility("default")))
#endif

#if defined(OP_LIBRARY)
#  define OP_EXPORT Q_DECL_EXPORT
#else
#  define OP_EXPORT Q_DECL_IMPORT
#endif

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define OP_API __stdcall
#else
#define OP_API
#endif

}   // namespace eol

#endif // EOL_OP_GLOBAL_H
