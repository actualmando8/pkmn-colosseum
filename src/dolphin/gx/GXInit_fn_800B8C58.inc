    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    stw r30, 0x10(r1)
    mr r30, r3
    bl OSDisableInterrupts
    li r6, 0x61
    lwz r4, -29632(r13)
    lis r5, 0xcc01
    rlwinm r0, r30, 0, 16, 31
    stb r6, -0x8000(r5)
    oris r0, r0, 0x4800
    stw r0, -0x8000(r5)
    rlwinm r0, r0, 0, 0, 15
    rlwimi r0, r30, 0, 16, 31
    rlwinm r0, r0, 0, 8, 31
    stb r6, -0x8000(r5)
    oris r0, r0, 0x4700
    stw r0, -0x8000(r5)
    mr r30, r3
    lwz r0, 0x4f4(r4)
    cmplwi r0, 0x0
    beq @L_800B8CBC
    bl fn_800B91EC
@L_800B8CBC:
    li r31, 0x0
    lis r3, 0xcc01
    stw r31, -0x8000(r3)
    stw r31, -0x8000(r3)
    stw r31, -0x8000(r3)
    stw r31, -0x8000(r3)
    stw r31, -0x8000(r3)
    stw r31, -0x8000(r3)
    stw r31, -0x8000(r3)
    stw r31, -0x8000(r3)
    bl fn_80098034
    mr r3, r30
    bl OSRestoreInterrupts
    lwz r3, -29632(r13)
    sth r31, 0x2(r3)
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
