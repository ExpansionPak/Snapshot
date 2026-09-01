#include <cstdint>
#include "librecomp/helpers.hpp"

extern "C" {

// Some OS stubs for Pokemon Snap

void __osSetWatchLo_recomp(uint8_t* rdram, recomp_context* ctx) {
    (void)rdram;
    (void)ctx;
}

void __osContRamRead_recomp(uint8_t* rdram, recomp_context* ctx) {
    (void)rdram;
    ctx->r2 = static_cast<gpr>(-1);
}

void __osContRamWrite_recomp(uint8_t* rdram, recomp_context* ctx) {
    (void)rdram;
    ctx->r2 = static_cast<gpr>(-1);
}

}