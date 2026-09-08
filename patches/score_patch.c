#include "patches.h"

// Decomp headers
#include "common.h"
#include "app_render.h"
#include "photo_check/photo_check.h"

u16 score_PokemonCount;
s32 score_PixelCount[12];
s32 score_PixelCountUnobstructed[12];
s32 score_ApproxTotalPixelCount[12];
s32 score_PixelCountInCenter[12];
ScoreData D_800BE170;

void score_CalculateScore(ScoreData* score, PhotoData* photo, s32 id);

void func_800A0E9C(ScoreData* arg0);

RECOMP_PATCH struct ScoreData* func_800A0EA4(GObj* camera, PhotoData* photo, u16* buffer, s32 width, s32 height, u16* zbuffer) {
    s32 i;
    s32 targetSlot = -1;
    f32 minDistance = 999999.0f;

    D_800BE170.totalScore = 0;
    D_800BE170.pokemonInFocus = 0;
    D_800BE170.specialID = 0;
    D_800BE170.proximityScore = 0;
    D_800BE170.completenessScore = 0;
    D_800BE170.specialBonus = 0;
    D_800BE170.poseID = 0;
    D_800BE170.posePts = 0;
    D_800BE170.isWellFramed = 0;
    D_800BE170.samePkmnBonus = 0;
    D_800BE170.samePkmnNumber = 0;
    
    f32 camEyeX = photo->unk_08.x;
    f32 camEyeY = photo->unk_08.y;
    f32 camEyeZ = photo->unk_08.z;

    f32 fwdX = photo->unk_14.x - camEyeX;
    f32 fwdY = photo->unk_14.y - camEyeY;
    f32 fwdZ = photo->unk_14.z - camEyeZ;
    f32 fwdLen = sqrtf(fwdX * fwdX + fwdY * fwdY + fwdZ * fwdZ);

    if (fwdLen > 0.0001f) {
        fwdX /= fwdLen;
        fwdY /= fwdLen;
        fwdZ /= fwdLen;
    }

    score_PokemonCount = 0;

    // Find the Pokemon closest to the center line of sight
    for (i = 0; i < 12; i++) {
        if (photo->pokemons[i].pokemonID <= 0 || photo->pokemons[i].pokemonID > POKEDEX_MAX) {
            continue;
        }

        f32 dx = photo->pokemons[i].position.x - camEyeX;
        f32 dy = photo->pokemons[i].position.y - camEyeY;
        f32 dz = photo->pokemons[i].position.z - camEyeZ;
        f32 dist = sqrtf(dx * dx + dy * dy + dz * dz);

        if (dist < 50.0f || dist > 12000.0f) {
            continue;
        }

        f32 dirX = dx / dist;
        f32 dirY = dy / dist;
        f32 dirZ = dz / dist;

        f32 dot = (fwdX * dirX) + (fwdY * dirY) + (fwdZ * dirZ);

        // Subject is in front of the lens
        if (dot > 0.707f) {
            s32 slot = score_PokemonCount++;
            photo_PokemonIndexes[slot] = i;

            // Near = ~600px, Far = ~100px (max score cap is 768)
            s32 simulatedPixels = (s32)(1200.0f - (dist * 0.25f));
            if (simulatedPixels > 768) simulatedPixels = 768;
            if (simulatedPixels < 40)  simulatedPixels = 40;

            score_PixelCount[slot] = simulatedPixels;
            score_PixelCountUnobstructed[slot] = simulatedPixels;
            score_ApproxTotalPixelCount[slot] = simulatedPixels;

            // Reticle center alignment test (~12 degree cone)
            if (dot > 0.975f) {
                score_PixelCountInCenter[slot] = 4;
            } else {
                score_PixelCountInCenter[slot] = 0;
            }

            if (dist < minDistance) {
                minDistance = dist;
                targetSlot = slot;
            }
        }
    }

    if (targetSlot != -1) {
        score_CalculateScore(&D_800BE170, photo, targetSlot);
    }

    func_800A0E9C(&D_800BE170);
    return &D_800BE170;
}