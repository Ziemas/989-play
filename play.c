#include <debug.h>
#include <iopcontrol.h>
#include <loadfile.h>
#include <sifrpc.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define SND_BIND_ID 0x123456

#define SONG_PATH "host0:mus/BEACH.MUS"

enum SND_FUNC {
    snd_StartSoundSystem = 0,
    snd_StopSoundSystem = 1,
    snd_LoadBankByName = 2,
    snd_ResolveBankXREFS = 8,
    snd_PlaySoundVolPanPMPB = 18,
    snd_GetLastLoadError = 82,
};

struct LoadBank_Args {
    int file_offset;
    char name[0x16];
};

struct PlaySound_Args {
    int bank;
    int sound;
    int volume;
    int pan;
    int pitchmod;
    int pitchbend;
};

SifRpcClientData_t snd_bd;

char rcv[64] __attribute__((aligned(64))) = {};
char snd[64] __attribute__((aligned(64))) = {};

void bla(void* end_param) {}

void print_bytes(int* buf)
{
    for (int i = 0; i < 16; i++) {
        scr_printf(" %x", buf[i]);
        if (i % 8 == 0)
            scr_printf("\n");
    }
    scr_printf("\n");
}

int main()
{
    init_scr();
    scr_printf("989play\n");

    SifIopReboot("host0:iop/IOPRP221.IMG");

    while (!SifIopSync())
        ;

    SifInitRpc(0);

    scr_printf("Iop synced, loading modules\n");
    SifLoadModule("host0:iop/LIBSD.IRX", 0, NULL);
    SifLoadModule("host0:iop/989SND.IRX", 0, NULL);

    SifBindRpc(&snd_bd, SND_BIND_ID, 0);
    scr_printf("starting sound system\n");

    SifCallRpc(&snd_bd, snd_StartSoundSystem, 0, NULL, 0, rcv, sizeof(rcv), NULL, NULL);

    struct LoadBank_Args* args = (struct LoadBank_Args*)snd;
    args->file_offset = 0;
    strcpy(args->name, SONG_PATH);

    scr_printf("loading sound bank\n");
    SifCallRpc(&snd_bd, snd_LoadBankByName, 0, snd, sizeof(snd), rcv, sizeof(rcv), NULL, NULL);

    int bank = ((int*)rcv)[1];
    scr_printf("opened bank %x\n", bank);

    SifCallRpc(&snd_bd, snd_ResolveBankXREFS, 0, NULL, 0, NULL, 0, NULL, NULL);

    struct PlaySound_Args* pargs = (struct PlaySound_Args*)snd;
    pargs->bank = bank;
    pargs->sound = 0;
    pargs->volume = 0x400;
    pargs->pan = 0;
    pargs->pitchmod = 0;
    pargs->pitchbend = 0;

    scr_printf("playing sound\n");
    SifCallRpc(&snd_bd, snd_PlaySoundVolPanPMPB, 0, snd, sizeof(snd), rcv, sizeof(rcv), NULL, NULL);

    while (true)
        ;
}
