    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0x1
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r31, r4
    mr r30, r3
    li r4, 0x0
    bl fn_80235B04
    clrlwi. r0, r3, 24
    bne @L_80251DF4
    mr r3, r30
    mr r4, r31
    bl fn_80237664
    extrwi r0, r3, 15, 16
    b @L_80251E2C
@L_80251DF4:
    cmplwi r0, 0x1
    bne @L_80251E1C
    mr r3, r30
    mr r4, r31
    bl fn_80237664
    clrlwi r3, r3, 16
    li r0, 0x1e
    mulli r3, r3, 0x14
    divw r0, r3, r0
    b @L_80251E2C
@L_80251E1C:
    mr r3, r30
    mr r4, r31
    bl fn_80237664
    extrwi r0, r3, 14, 16
@L_80251E2C:
    neg r3, r0
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
