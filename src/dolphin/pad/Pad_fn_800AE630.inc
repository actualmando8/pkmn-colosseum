    nofralloc
    mflr r0
    cmplwi r10, 0x0
    stw r0, 0x4(r1)
    li r0, 0x0
    stwu r1, -0x38(r1)
    stw r31, 0x34(r1)
    stw r30, 0x30(r1)
    addi r30, r6, 0x0
    stw r29, 0x2c(r1)
    addi r29, r3, 0x0
    stw r0, 0x0(r3)
    stw r4, 0x4(r3)
    stw r5, 0x8(r3)
    stw r7, 0x10(r3)
    stw r8, 0x14(r3)
    stw r9, 0x18(r3)
    beq @L_800AE67C
    stw r10, 0x1c(r29)
    b @L_800AE688
    @L_800AE67C:
    lis r3, fn_800AE4F0@ha
    addi r0, r3, fn_800AE4F0@l
    stw r0, 0x1c(r29)
    @L_800AE688:
    bl OSDisableInterrupts
    cmpwi r30, 0x1
    addi r31, r3, 0x0
    beq @L_800AE6CC
    bge @L_800AE6EC
    cmpwi r30, 0x0
    bge @L_800AE6A8
    b @L_800AE6EC
    @L_800AE6A8:
    lwz r0, lbl_8047A930(r13)
    cmplwi r0, 0x0
    beq @L_800AE6C0
    lwz r3, lbl_8047A934(r13)
    stw r29, 0x0(r3)
    b @L_800AE6C4
    @L_800AE6C0:
    stw r29, lbl_8047A930(r13)
    @L_800AE6C4:
    stw r29, lbl_8047A934(r13)
    b @L_800AE6EC
    @L_800AE6CC:
    lwz r0, lbl_8047A928(r13)
    cmplwi r0, 0x0
    beq @L_800AE6E4
    lwz r3, lbl_8047A92C(r13)
    stw r29, 0x0(r3)
    b @L_800AE6E8
    @L_800AE6E4:
    stw r29, lbl_8047A928(r13)
    @L_800AE6E8:
    stw r29, lbl_8047A92C(r13)
    @L_800AE6EC:
    lwz r0, lbl_8047A938(r13)
    cmplwi r0, 0x0
    bne @L_800AE768
    lwz r0, lbl_8047A93C(r13)
    cmplwi r0, 0x0
    bne @L_800AE768
    lwz r6, lbl_8047A928(r13)
    cmplwi r6, 0x0
    beq @L_800AE758
    lwz r3, 0x8(r6)
    cmplwi r3, 0x0
    bne @L_800AE730
    lwz r4, 0x10(r6)
    lwz r5, 0x14(r6)
    lwz r6, 0x18(r6)
    bl fn_800AC990
    b @L_800AE740
    @L_800AE730:
    lwz r4, 0x14(r6)
    lwz r5, 0x10(r6)
    lwz r6, 0x18(r6)
    bl fn_800AC990
    @L_800AE740:
    lwz r3, lbl_8047A928(r13)
    lwz r0, 0x1c(r3)
    stw r0, lbl_8047A940(r13)
    stw r3, lbl_8047A938(r13)
    lwz r0, 0x0(r3)
    stw r0, lbl_8047A928(r13)
    @L_800AE758:
    lwz r0, lbl_8047A938(r13)
    cmplwi r0, 0x0
    bne @L_800AE768
    bl fn_800AE3F0
    @L_800AE768:
    mr r3, r31
    bl OSRestoreInterrupts
    lwz r0, 0x3c(r1)
    lwz r31, 0x34(r1)
    lwz r30, 0x30(r1)
    lwz r29, 0x2c(r1)
    addi r1, r1, 0x38
    mtlr r0
    blr
