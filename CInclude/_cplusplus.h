#ifndef _CPLUSPLUS_H
#define _CPLUSPLUS_H

#ifdef __cplusplus

# define _CXX_BEGIN extern "C" {
# define _CXX_END }

# define _CXX_NS std::

# define _CXX_NS_BEGIN namespace std {
# define _CXX_NS_END }

#else

# define _CXX_BEGIN
# define _CXX_END

# define _CXX_NS

# define _CXX_NS_BEGIN
# define _CXX_NS_END

#endif

#endif
