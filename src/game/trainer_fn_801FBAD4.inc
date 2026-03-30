    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r4
    mr r31, r5
    li r4, 0x0
    li r5, 0x4c
    bl fn_801FB1C0
    mr. r30, r3
    beq @L_801FBC0C
    mr r4, r29
    li r3, 0x0
    li r5, 0x12
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r0, r31, 24
    mr r31, r3
    bne @L_801FBB3C
    mr r3, r30
    clrlwi r4, r31, 16
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    b @L_801FBC0C
    @L_801FBB3C:
    cmplwi r0, 0x1
    bne @L_801FBB74
    mr r3, r30
    clrlwi r4, r31, 16
    li r5, 0x4
    bl fn_801DA9E8
    @L_801FBB54:
    mr r3, r30
    clrlwi r4, r31, 16
    li r5, 0x4
    bl fn_801DA94C
    clrlwi. r0, r3, 24
    beq @L_801FBC0C
    bl fn_800F0308
    b @L_801FBB54
    @L_801FBB74:
    cmplwi r0, 0x2
    bne @L_801FBB90
    mr r3, r30
    clrlwi r4, r31, 16
    li r5, 0x4
    bl fn_801DA8C4
    b @L_801FBC0C
    @L_801FBB90:
    cmplwi r0, 0x3
    bne @L_801FBBB0
    mr r3, r30
    li r4, 0x57
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    b @L_801FBC0C
    @L_801FBBB0:
    cmplwi r0, 0x4
    bne @L_801FBBCC
    mr r3, r30
    li r4, 0x57
    li r5, 0x4
    bl fn_801DA9E8
    b @L_801FBC0C
    @L_801FBBCC:
    cmplwi r0, 0x5
    bne @L_801FBBF4
    @L_801FBBD4:
    mr r3, r30
    li r4, 0x57
    li r5, 0x4
    bl fn_801DA94C
    clrlwi. r0, r3, 24
    beq @L_801FBC0C
    bl fn_800F0308
    b @L_801FBBD4
    @L_801FBBF4:
    cmplwi r0, 0x6
    bne @L_801FBC0C
    mr r3, r30
    li r4, 0x57
    li r5, 0x4
    bl fn_801DA8C4
    @L_801FBC0C:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
