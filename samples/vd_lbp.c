#include "vd_lbp.h"
#include <stdlib.h>

typedef enum {
    VERSION_INITIAL_RELEASE,
    VERSION_SHIELD,
    VERSION_GOD,
    VERSION_ORCS,
    VERSION_LATEST = VERSION_ORCS
} Version;

#pragma pack(push, 1)
typedef struct {
    int32_t num_orcs;
    float   player_health;
} LevelDataRelease;

// + player_shield
typedef struct {
    int32_t num_orcs;
    float   player_health;
    float   player_shield;
} LevelDataShield;

// - player_health
typedef struct {
    int32_t num_orcs;
    float player_shield;
} LevelDataGod;

typedef struct {
    int32_t x;
    int32_t y;
} Position;

// + orc_positions
typedef struct {
    int32_t   num_orcs;
    float     player_shield;
    Position  *orc_positions;
} LevelDataOrcs;

#pragma pack(pop)

#define VD_LBP_ADD(ser, field, type, version_added) \
    do { \
        if (ser->ver >= version_added) { \
            VD_LBP_SER(ser, field, type); \
        } \
    } while (0)

#define VD_LBP_REM(ser, field, type, version_added, version_removed) \
    type field = {0}; \
    do { \
        if ((ser->ver >= version_added) && (ser->ver < version_removed)) { \
            VD_LBP_SERIALIZE(ser, field, type); \
        } \
    } while (0)

VD_LBP_SER_IMPL(LevelDataRelease)
{
    VD_LBP_ADD(ser, num_orcs, int32_t, VERSION_INITIAL_RELEASE);
    VD_LBP_ADD(ser, player_health, float, VERSION_INITIAL_RELEASE);
    return 1;
}

VD_LBP_SER_IMPL(LevelDataShield)
{
    VD_LBP_ADD(ser, num_orcs, int32_t, VERSION_INITIAL_RELEASE);
    VD_LBP_ADD(ser, player_health, float, VERSION_INITIAL_RELEASE);
    VD_LBP_ADD(ser, player_shield, float, VERSION_SHIELD);
    return 1;
}

VD_LBP_SER_IMPL(LevelDataGod)
{
    VD_LBP_ADD(ser, num_orcs, int32_t, VERSION_INITIAL_RELEASE);
    VD_LBP_REM(ser, player_health, float, VERSION_INITIAL_RELEASE, VERSION_GOD);
    VD_LBP_ADD(ser, player_shield, float, VERSION_SHIELD);

    if (ser->ver < VERSION_SHIELD) {
        printf("Since in the initial version we didn't have shields, but this level's player health is %f\n", player_health);
        printf("Then we'll just transfer the health to the shield\n");
        datum->player_shield = player_health;
    } else if (ser->ver < VERSION_GOD) {
        printf("Since we removed player health, but this level's player health is %f\n", player_health);
        printf("Then we'll just halve it and add it back to their shield\n");
        datum->player_shield += player_health * 0.5f;
    }
    return 1;
}

VD_LBP_SER_IMPL(Position)
{
    VD_LBP_SER(ser, x, int32_t);
    VD_LBP_SER(ser, y, int32_t);
    return 1;
}

VD_LBP_SER_IMPL(LevelDataOrcs)
{
    VD_LBP_ADD(ser, num_orcs, int32_t, VERSION_INITIAL_RELEASE);
    VD_LBP_REM(ser, player_health, float, VERSION_INITIAL_RELEASE, VERSION_GOD);
    VD_LBP_ADD(ser, player_shield, float, VERSION_SHIELD);

    if (ser->ver < VERSION_SHIELD) {
        printf("Since in the initial version we didn't have shields, but this level's player health is %f\n", player_health);
        printf("Then we'll just transfer the health to the shield\n");
        datum->player_shield = player_health;
    } else if (ser->ver < VERSION_GOD) {
        printf("Since we removed player health, but this level's player health is %f\n", player_health);
        printf("Then we'll just halve it and add it back to their shield\n");
        datum->player_shield += player_health * 0.5f;
    }

    if (ser->ver >= VERSION_ORCS) {
        if (!ser->writing) {
            datum->orc_positions = (Position*)malloc(sizeof(Position) * datum->num_orcs);
        }

        for (int32_t i = 0; i < datum->num_orcs; ++i) {
            VD_LBP_SER(ser, orc_positions[i], Position);
        }
    } else {
        if (!ser->writing) {
            datum->orc_positions = (Position*)malloc(sizeof(Position) * datum->num_orcs);

            printf("Since orc position did not exist before VERSION_ORCS (current version: %d),\n", ser->ver);
            printf("then we'll just initialize all the orcs to be at position 0,0\n");

            Position zero_position;
            zero_position.x = 0;
            zero_position.y = 0;
            for (int32_t i = 0; i < datum->num_orcs; ++i) {
                datum->orc_positions[i] = zero_position;
            }
        }
    }

    return 1;
}

int main(int argc, char const *argv[])
{
    {
        VdLbpSerializer ser = vd_lbp_stdio_serializer_make("level_data.lbp", 1, VERSION_INITIAL_RELEASE);

        LevelDataRelease level_data_release;
        level_data_release.num_orcs = 64;
        level_data_release.player_health = 100.f;
        VD_LBP_SERIALIZE(&ser, level_data_release, LevelDataRelease);

        ser.fin(&ser);
    }

    {
        VdLbpSerializer ser = vd_lbp_stdio_serializer_make("level_data_shield.lbp", 1, VERSION_SHIELD);

        LevelDataShield level_data_release;
        level_data_release.num_orcs = 64;
        level_data_release.player_health = 100.f;
        level_data_release.player_shield = 50.f;
        VD_LBP_SERIALIZE(&ser, level_data_release, LevelDataShield);

        ser.fin(&ser);
    }

    {

        VdLbpSerializer ser = vd_lbp_stdio_serializer_make("level_data.lbp", 0, VERSION_SHIELD);

        LevelDataShield level_data_shield;
        VD_LBP_SERIALIZE(&ser, level_data_shield, LevelDataShield);

        printf("Num Orcs = %d\n", level_data_shield.num_orcs);
        printf("Player Health = %f\n", level_data_shield.player_health);
        printf("Player Shield = %f\n", level_data_shield.player_shield);

        ser.fin(&ser);
    }

    {

        VdLbpSerializer ser = vd_lbp_stdio_serializer_make("level_data_shield.lbp", 0, VERSION_SHIELD);

        LevelDataShield level_data_shield;
        VD_LBP_SERIALIZE(&ser, level_data_shield, LevelDataShield);

        printf("Num Orcs = %d\n", level_data_shield.num_orcs);
        printf("Player Health = %f\n", level_data_shield.player_health);
        printf("Player Shield = %f\n", level_data_shield.player_shield);

        ser.fin(&ser);
    }

    {

        VdLbpSerializer ser = vd_lbp_stdio_serializer_make("level_data_shield.lbp", 0, VERSION_GOD);

        LevelDataGod level_data_god;
        VD_LBP_SERIALIZE(&ser, level_data_god, LevelDataGod);

        printf("Num Orcs = %d\n", level_data_god.num_orcs);
        printf("Player Shield = %f\n", level_data_god.player_shield);

        ser.fin(&ser);
    }

    {

        VdLbpSerializer ser = vd_lbp_stdio_serializer_make("level_data.lbp", 0, VERSION_GOD);

        LevelDataGod level_data_god;
        VD_LBP_SERIALIZE(&ser, level_data_god, LevelDataGod);

        printf("Num Orcs = %d\n", level_data_god.num_orcs);
        printf("Player Shield = %f\n", level_data_god.player_shield);

        ser.fin(&ser);
    }

    {
        VdLbpSerializer ser = vd_lbp_stdio_serializer_make("level_data_orcs.lbp", 1, VERSION_ORCS);

        LevelDataOrcs level_data_orcs;
        level_data_orcs.num_orcs = 10;
        level_data_orcs.player_shield = 45.f;
        level_data_orcs.orc_positions = (Position*)malloc(sizeof(Position) * level_data_orcs.num_orcs);
        for (int32_t i = 0; i < level_data_orcs.num_orcs; ++i) {
            level_data_orcs.orc_positions[i].x = i;
            level_data_orcs.orc_positions[i].y = i * 2;
        }

        VD_LBP_SERIALIZE(&ser, level_data_orcs, LevelDataOrcs);
        ser.fin(&ser);
    }

    {
        VdLbpSerializer ser = vd_lbp_stdio_serializer_make("level_data_orcs.lbp", 0, VERSION_ORCS);

        LevelDataOrcs level_data_orcs;
        VD_LBP_SERIALIZE(&ser, level_data_orcs, LevelDataOrcs);
        printf("Num Orcs = %d\n", level_data_orcs.num_orcs);
        printf("Player Shield = %f\n", level_data_orcs.player_shield);
        for (int32_t i = 0; i < level_data_orcs.num_orcs; ++i) {
            Position *orc_position = &level_data_orcs.orc_positions[i];
            printf("Orc[%d] is At (%d, %d)\n", i, orc_position->x, orc_position->y);
        }
        ser.fin(&ser);
    }

    {
        VdLbpSerializer ser = vd_lbp_stdio_serializer_make("level_data.lbp", 0, VERSION_ORCS);

        LevelDataOrcs level_data_orcs;
        VD_LBP_SERIALIZE(&ser, level_data_orcs, LevelDataOrcs);
        printf("Num Orcs = %d\n", level_data_orcs.num_orcs);
        printf("Player Shield = %f\n", level_data_orcs.player_shield);
        for (int32_t i = 0; i < level_data_orcs.num_orcs; ++i) {
            Position *orc_position = &level_data_orcs.orc_positions[i];
            printf("Orc[%d] is At (%d, %d)\n", i, orc_position->x, orc_position->y);
        }
        ser.fin(&ser);
    }


    return 0;
}