    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, 0xaaab
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    subi r31, r3, 0x5555
    stw r30, 0x8(r1)
@L_800FEA90:
    lwz r3, lbl_8047AC98(r13)
    b @L_800FEAE0
@L_800FEA98:
    lwz r0, 0x8(r3)
    lwz r30, 0x4(r3)
    cmpwi r0, 0x2
    bne @L_800FEADC
    lbz r0, 0xd(r3)
    cmplwi r0, 0x0
    bne @L_800FEADC
    lwz r0, lbl_8047AC7C(r13)
    stw r3, lbl_8047AC94(r13)
    subf r0, r0, r3
    mulhwu r0, r31, r0
    lwz r12, 0x14(r3)
    lwz r4, 0x10(r3)
    srwi r3, r0, 4
    addi r3, r3, 0x1
    mtctr r12
    bctrl
@L_800FEADC:
    mr r3, r30
@L_800FEAE0:
    cmplwi r3, 0x0
    bne @L_800FEA98
    li r0, 0x0
    stw r0, lbl_8047AC94(r13)
    bl OSDisableInterrupts
    lwz r7, lbl_8047AC9C(r13)
    b @L_800FEB88
@L_800FEAFC:
    lwz r8, 0x4(r7)
    lwz r5, lbl_8047AC98(r13)
    b @L_800FEB0C
@L_800FEB08:
    mr r5, r6
@L_800FEB0C:
    lwz r6, 0x4(r5)
    cmplwi r6, 0x0
    beq @L_800FEB28
    lbz r4, 0xc(r5)
    lbz r0, 0xc(r7)
    cmplw r4, r0
    blt @L_800FEB08
@L_800FEB28:
    cmplwi r6, 0x0
    bne @L_800FEB54
    lbz r4, 0xc(r5)
    lbz r0, 0xc(r7)
    cmplw r4, r0
    bge @L_800FEB54
    stw r5, 0x0(r7)
    li r0, 0x0
    stw r0, 0x4(r7)
    stw r7, 0x4(r5)
    b @L_800FEB84
@L_800FEB54:
    lwz r4, 0x0(r5)
    cmplwi r4, 0x0
    beq @L_800FEB64
    stw r7, 0x4(r4)
@L_800FEB64:
    lwz r0, 0x0(r5)
    stw r0, 0x0(r7)
    stw r5, 0x4(r7)
    stw r7, 0x0(r5)
    lwz r0, lbl_8047AC98(r13)
    cmplw r0, r5
    bne @L_800FEB84
    stw r7, lbl_8047AC98(r13)
@L_800FEB84:
    mr r7, r8
@L_800FEB88:
    cmplwi r7, 0x0
    bne @L_800FEAFC
    li r0, 0x0
    stw r0, lbl_8047AC9C(r13)
    bl OSRestoreInterrupts
    b @L_800FEA90
