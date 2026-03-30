    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    stw r30, 0x8(r1)
    mr r30, r3
    lwz r3, 0x3c(r3)
    addis r0, r3, 0x102
    cmplwi r0, 0xfefe
    bne @L_800DF4D4
    lwz r3, 0x8(r30)
    lwz r0, 0x10(r3)
    stw r0, 0x3c(r30)
    b @L_800DF4E4
    @L_800DF4D4:
    lis r3, lbl_80270528@ha
    addi r3, r3, lbl_80270528@l
    crxor 6, 6, 6
    bl fn_800DD970
    @L_800DF4E4:
    lwz r3, 0x8(r30)
    stw r31, 0x10(r3)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
