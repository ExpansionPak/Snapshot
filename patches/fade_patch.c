#include "patches.h"
#include "graphics.h"

#include "common.h"

extern Gfx D_800AECB0[];
extern s32 D_800AF054;
extern s32 D_800AF058;
extern s32 D_800AF05C;
extern s32 D_800AF060;

static f32 get_widescreen_aspect_multiplier(void) {
    u32 width = 0;
    u32 height = 0;

    recomp_get_window_resolution(&width, &height);

    if (width > 0 && height > 0) {
        f32 currentAspect = (f32) width / (f32) height;
        f32 baseAspect = 4.0f / 3.0f;
        f32 mult = currentAspect / baseAspect;
        if (mult >= 1.0f) {
            return mult;
        }
    }

    return 1.0f;
}

static void fadeCoverFrame(DObj* dobj) {
    u32 width = 0;
    u32 height = 0;
    if (dobj->scale.v.y == 3.2f && dobj->scale.v.z == 2.4f) {
        f32 mult = get_widescreen_aspect_multiplier();
        dobj->scale.v.y = 3.3f * mult;
        dobj->scale.v.z = 2.5f;
    }
}

RECOMP_PATCH void func_800A750C(GObj* gobj) {
    Gfx* gfx;
    s32 sp30;

    fadeCoverFrame(gobj->data.dobj);
    gfx = gMainGfxPos[1];
    gDPPipeSync(gfx++);
    sp30 = renPrepareModelMatrix(&gfx, gobj->data.dobj);
    renLoadTextures(gobj->data.dobj, &gfx);

    gDPSetPrimColor(gfx++, 0, 0, D_800AF054, D_800AF058, D_800AF05C, D_800AF060);
    gDPSetRenderMode(gfx++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gSPDisplayList(gfx++, D_800AECB0);

    if (sp30 != 0 && (gobj->data.dobj->parent == (void*) 1 || gobj->data.dobj->next != NULL)) {
        gSPPopMatrix(gfx++, G_MTX_MODELVIEW);
    }
    gMainGfxPos[1] = gfx;
}

RECOMP_PATCH void func_800A7A58(GObj* gobj) {
    Gfx* gfx;
    s32 sp30;

    fadeCoverFrame(gobj->data.dobj);
    gfx = gMainGfxPos[1];
    gDPPipeSync(gfx++);
    sp30 = renPrepareModelMatrix(&gfx, gobj->data.dobj);
    renLoadTextures(gobj->data.dobj, &gfx);

    gDPSetPrimColor(gfx++, 0, 0, D_800AF054, D_800AF058, D_800AF05C, D_800AF060);
    gDPSetRenderMode(gfx++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gSPDisplayList(gfx++, D_800AECB0);

    if (sp30 != 0 && (gobj->data.dobj->parent == (void*) 1 || gobj->data.dobj->next != NULL)) {
        gSPPopMatrix(gfx++, G_MTX_MODELVIEW);
    }
    gMainGfxPos[1] = gfx;
}