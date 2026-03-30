    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    stw r30, 0x8(r1)
    mr r30, r3
    lwz r3, 0x8(r3)
    bl fn_801A8458
    stw r3, 0x4(r30)
    lis r3, 0x4000
    addi r4, r3, 0x600f
    lwz r3, 0x8(r30)
    bl fn_801A8428
    clrlwi. r0, r31, 31
    li r4, 0x0
    beq @L_800DF598
    ori r4, r4, 0x1
    @L_800DF598:
    rlwinm. r0, r31, 0, 30, 30
    beq @L_800DF5A4
    ori r4, r4, 0x2
    @L_800DF5A4:
    rlwinm. r0, r31, 0, 29, 29
    beq @L_800DF5B0
    ori r4, r4, 0x4
    @L_800DF5B0:
    rlwinm. r0, r31, 0, 28, 28
    beq @L_800DF5BC
    ori r4, r4, 0x8
    @L_800DF5BC:
    rlwinm. r0, r31, 0, 27, 27
    beq @L_800DF5C8
    oris r4, r4, 0x4000
    @L_800DF5C8:
    rlwinm. r0, r31, 0, 26, 26
    beq @L_800DF5D4
    ori r4, r4, 0x2000
    @L_800DF5D4:
    rlwinm. r0, r31, 0, 25, 25
    beq @L_800DF5E0
    ori r4, r4, 0x4000
    @L_800DF5E0:
    lwz r3, 0x8(r30)
    bl fn_801A8440
    lwz r3, 0x8(r30)
    bl fn_801A6FF0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
