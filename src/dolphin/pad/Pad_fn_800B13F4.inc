    nofralloc
    mflr r0
    lis r4, lbl_803FC620@ha
    stw r0, 0x4(r1)
    addi r4, r4, lbl_803FC620@l
    addi r0, r4, 0x30
    stwu r1, -0x18(r1)
    cmplw r0, r3
    stw r31, 0x14(r1)
    beq @L_800B1424
    addi r0, r4, 0x140
    cmplw r0, r3
    addi r4, r4, 0x110
    @L_800B1424:
    lwz r31, 0x80(r4)
    lis r3, 0xff00
    bl fn_800AE7CC
    @L_800B1430:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800B1430
    mr r3, r31
    bl fn_800AE7CC
    @L_800B1444:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800B1444
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
