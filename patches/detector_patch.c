#include "patches.h"

// Decomp headers
#include "common.h"
#include "sys/om.h"
#include "sys/vi.h"
#include "app_level.h"

extern s32 gHasPokemonInFocus;
extern u16 gPokemonFlagsInFocus;

RECOMP_PATCH void renDrawSprite(GObj* arg0) {
    SObj* sobj = arg0->data.sobj;
    while (sobj != NULL) {
        if (!(sobj->sprite.attr & SP_HIDDEN)) {
            sobj->sprite.rsp_dl_next = gMainGfxPos[0];
            spDraw(&sobj->sprite);
            gMainGfxPos[0] = sobj->sprite.rsp_dl_next - 1;
        }
        sobj = sobj->next;
    }

    if (arg0->id == OBJID_UI_RETICLE) {
        if (gHasPokemonInFocus && gDirectionIndex == -1 && (gPokemonFlagsInFocus & POKEMON_FLAG_4)) {
            // s32 cx = SCREEN_WIDTH / 2;
            // s32 cy = SCREEN_HEIGHT / 2;
            
            gDPPipeSync(gMainGfxPos[0]++);
            gDPSetCycleType(gMainGfxPos[0]++, G_CYC_1CYCLE);
            gDPSetRenderMode(gMainGfxPos[0]++, G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2);
            gDPSetCombineMode(gMainGfxPos[0]++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
            gDPSetPrimColor(gMainGfxPos[0]++, 0, 0, 255, 0, 0, 255);

            gDPFillRectangle(gMainGfxPos[0]++, 158, 117, 161, 118);
            gDPFillRectangle(gMainGfxPos[0]++, 157, 118, 162, 121);
            gDPFillRectangle(gMainGfxPos[0]++, 158, 121, 161, 122);

            // Restore RDP state for the HUD sprites
            gDPPipeSync(gMainGfxPos[0]++);
            gDPSetCycleType(gMainGfxPos[0]++, G_CYC_1CYCLE);
            gDPSetRenderMode(gMainGfxPos[0]++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
            gDPSetCombineMode(gMainGfxPos[0]++, G_CC_DECALRGBA, G_CC_DECALRGBA);
            gDPSetTexturePersp(gMainGfxPos[0]++, G_TP_NONE);
        }
    }
}