#ifndef DISABLE_CLANG_DEPRECATIONS_H1
#define DISABLE_CLANG_DEPRECATIONS_H1
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif // defined(__clang__)
#else
#if defined(__clang__)
#pragma clang diagnostic pop
#endif // defined(__clang__)
#endif // DISABLE_CLANG_DEPRECATIONS_H1

