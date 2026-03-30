    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    stw r30, 0x10(r1)
    mr r30, r3
    bl OSDisableInterrupts
    addi r31, r3, 0x0
    addi r3, r30, 0x0
    bl fn_800AF14C
    li r0, 0x0
    stw r0, 0x0(r30)
    li r0, 0x1
    addi r3, r31, 0x0
    stw r0, 0x8(r30)
    bl OSRestoreInterrupts
    lwz r0, lbl_8047A968(r13)
    cmplw r30, r0
    bne @L_800AE990
    mr r3, r30
    bl fn_800AEFC0
    @L_800AE990:
    mr r3, r30
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
