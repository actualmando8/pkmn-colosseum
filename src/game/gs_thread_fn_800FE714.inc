    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    subi r0, r3, 0x1
    mulli r0, r0, 0x18
    stw r31, 0xc(r1)
    lwz r3, lbl_8047AC7C(r13)
    add r31, r3, r0
    bl OSDisableInterrupts
    lwz r4, 0x0(r31)
    cmplwi r4, 0x0
    beq @L_800FE74C
    lwz r0, 0x4(r31)
    stw r0, 0x4(r4)
@L_800FE74C:
    lwz r4, 0x4(r31)
    cmplwi r4, 0x0
    beq @L_800FE760
    lwz r0, 0x0(r31)
    stw r0, 0x0(r4)
@L_800FE760:
    lwz r0, lbl_8047AC98(r13)
    cmplw r0, r31
    bne @L_800FE774
    lwz r0, 0x4(r31)
    stw r0, lbl_8047AC98(r13)
@L_800FE774:
    li r0, 0x0
    stw r0, 0x0(r31)
    stw r0, 0x4(r31)
    bl OSRestoreInterrupts
    li r0, 0x0
    stw r0, 0x8(r31)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
