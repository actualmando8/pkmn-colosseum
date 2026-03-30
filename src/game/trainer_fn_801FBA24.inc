    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0x4c
    li r6, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r4
    li r4, 0x0
    bl fn_801FB1C0
    mr. r31, r3
    beq @L_801FBAC0
    clrlwi. r0, r30, 24
    bne @L_801FBA6C
    li r4, 0x74
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    b @L_801FBAC0
    @L_801FBA6C:
    cmplwi r0, 0x1
    bne @L_801FBA84
    li r4, 0x74
    li r5, 0x4
    bl fn_801DA9E8
    b @L_801FBAC0
    @L_801FBA84:
    cmplwi r0, 0x2
    bne @L_801FBAAC
    @L_801FBA8C:
    mr r3, r31
    li r4, 0x74
    li r5, 0x4
    bl fn_801DA94C
    clrlwi. r0, r3, 24
    beq @L_801FBAC0
    bl fn_800F0308
    b @L_801FBA8C
    @L_801FBAAC:
    cmplwi r0, 0x3
    bne @L_801FBAC0
    li r4, 0x74
    li r5, 0x4
    bl fn_801DA8C4
    @L_801FBAC0:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
