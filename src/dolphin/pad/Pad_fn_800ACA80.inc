    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    addi r30, r4, 0x0
    stw r29, 0x14(r1)
    addi r29, r3, 0x0
    lwz r0, lbl_8047A924(r13)
    cmpwi r0, 0x1
    bne @L_800ACAB4
    li r3, 0x4000
    b @L_800ACB28
    @L_800ACAB4:
    lwz r3, lbl_80478A30(r13)
    bl OSRegisterVersion
    bl OSDisableInterrupts
    li r0, 0x0
    lis r4, fn_800ACB54@ha
    stw r0, lbl_8047A908(r13)
    addi r31, r3, 0x0
    addi r4, r4, fn_800ACB54@l
    li r3, 0x6
    bl __OSSetInterruptHandler
    lis r3, 0x200
    bl __OSUnmaskInterrupts
    li r0, 0x4000
    stw r30, lbl_8047A91C(r13)
    lis r3, 0xcc00
    stw r0, lbl_8047A918(r13)
    addi r4, r3, 0x5000
    stw r29, lbl_8047A920(r13)
    lhz r0, 0x1a(r4)
    lhz r3, 0x501a(r3)
    clrrwi r0, r0, 8
    rlwimi r0, r3, 0, 24, 31
    sth r0, 0x1a(r4)
    bl fn_800ACBFC
    li r0, 0x1
    stw r0, lbl_8047A924(r13)
    mr r3, r31
    bl OSRestoreInterrupts
    lwz r3, lbl_8047A918(r13)
    @L_800ACB28:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
