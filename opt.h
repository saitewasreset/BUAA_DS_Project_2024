
#ifndef __OPT_H
#define __OPT_H
#pragma GCC optimize("O3,no-stack-protector")
#pragma GCC target("sse3", "sse2", "sse")
#pragma GCC target("avx", "sse4", "sse4.1", "sse4.2", "ssse3")
#pragma GCC target("f16c")
#pragma GCC optimize("inline", "fast-math", "unroll-loops",                    \
                     "no-stack-protector")
#endif
