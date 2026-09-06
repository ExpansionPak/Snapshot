#include "recomp.h"
#include "ultramodern/ultramodern.hpp"

extern "C" void __osSetWatchLo_recomp(uint8_t* rdram, recomp_context* ctx) {
    (void)rdram;
    (void)ctx;
}

extern "C" void __osContRamRead_recomp(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 11;
}

extern "C" void __osContRamWrite_recomp(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 11;
}