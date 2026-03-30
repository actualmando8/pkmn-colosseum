    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmpwi r7, 0x1
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r26, r3
    mr r30, r4
    mr r27, r5
    mr r28, r6
    mr r29, r8
    bne @L_8025DE8C
    lfs f1, -20528(r2)
    li r3, 0x2
    bl fn_801C41C8
@L_8025DE8C:
    lwz r3, 0x0(r26)
    li r4, 0x1
    bl fn_801DA4E8
    mr r31, r30
    li r30, 0x0
    b @L_8025DF1C
@L_8025DEA4:
    cmpwi r28, 0x0
    bne @L_8025DEBC
    lwz r3, 0x0(r26)
    lhz r4, 0x0(r31)
    lhz r5, 0x2(r31)
    bl fn_801DA914
@L_8025DEBC:
    lwz r3, 0x0(r26)
    lhz r4, 0x0(r31)
    lhz r5, 0x2(r31)
    bl fn_801DA9E8
    b @L_8025DED8
@L_8025DED0:
    bl fn_801DB088
    bl fn_800F0308
@L_8025DED8:
    lwz r3, 0x0(r26)
    lhz r4, 0x0(r31)
    lhz r5, 0x2(r31)
    bl fn_801DA94C
    clrlwi. r0, r3, 24
    bne @L_8025DED0
    cmpwi r29, 0x1
    bne @L_8025DF04
    lwz r3, 0x0(r26)
    li r4, 0x0
    bl fn_801DA4E8
@L_8025DF04:
    lwz r3, 0x0(r26)
    lhz r4, 0x0(r31)
    lhz r5, 0x2(r31)
    bl fn_801DA8C4
    addi r31, r31, 0x4
    addi r30, r30, 0x1
@L_8025DF1C:
    cmpw r30, r27
    blt @L_8025DEA4
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
