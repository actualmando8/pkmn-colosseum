    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r3, 0x8(r3)
    bl fn_801A8458
    clrlwi. r0, r3, 31
    li r4, 0x0
    beq @L_800DF414
    ori r4, r4, 0x1
    @L_800DF414:
    rlwinm. r0, r3, 0, 30, 30
    beq @L_800DF420
    ori r4, r4, 0x2
    @L_800DF420:
    rlwinm. r0, r3, 0, 29, 29
    beq @L_800DF42C
    ori r4, r4, 0x4
    @L_800DF42C:
    rlwinm. r0, r3, 0, 28, 28
    beq @L_800DF438
    ori r4, r4, 0x8
    @L_800DF438:
    rlwinm. r0, r3, 0, 1, 1
    beq @L_800DF444
    ori r4, r4, 0x10
    @L_800DF444:
    rlwinm. r0, r3, 0, 18, 18
    beq @L_800DF450
    ori r4, r4, 0x20
    @L_800DF450:
    rlwinm. r0, r3, 0, 17, 17
    beq @L_800DF45C
    ori r4, r4, 0x40
    @L_800DF45C:
    lwz r0, 0x14(r1)
    mr r3, r4
    mtlr r0
    addi r1, r1, 0x10
    blr
