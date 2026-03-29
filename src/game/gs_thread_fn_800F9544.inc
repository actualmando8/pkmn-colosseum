    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r6
    stw r29, 0x14(r1)
    mr r29, r5
    stw r28, 0x10(r1)
    mr r28, r4
    lwz r31, lbl_8047AC5C(r13)
    lwz r7, lbl_8047AC60(r13)
    mr r4, r31
    mtctr r7
    cmplwi r7, 0x0
    beq @L_800F95B4
@L_800F9584:
    lwz r0, 0x4(r4)
    cmplwi r0, 0x0
    beq @L_800F95AC
    lwz r0, 0x8(r4)
    cmplw r0, r28
    bne @L_800F95AC
    lwz r0, 0xc(r4)
    cmplw r0, r29
    bne @L_800F95AC
    b @L_800F95B8
@L_800F95AC:
    addi r4, r4, 0x14
    bdnz @L_800F9584
@L_800F95B4:
    li r4, 0x0
@L_800F95B8:
    cmplwi r4, 0x0
    beq @L_800F95C8
    li r3, 0x0
    b @L_800F9650
@L_800F95C8:
    mtctr r7
    cmplwi r7, 0x0
    ble @L_800F95EC
@L_800F95D4:
    lwz r0, 0x4(r31)
    cmplwi r0, 0x0
    bne @L_800F95E4
    b @L_800F95F0
@L_800F95E4:
    addi r31, r31, 0x14
    bdnz @L_800F95D4
@L_800F95EC:
    li r31, 0x0
@L_800F95F0:
    cmplwi r31, 0x0
    bne @L_800F9600
    li r3, 0x0
    b @L_800F9650
@L_800F9600:
    bl fn_800E3534
    sth r3, 0x0(r31)
    lhz r3, 0x0(r31)
    cmplwi r3, 0x0
    bne @L_800F961C
    li r3, 0x0
    b @L_800F9650
@L_800F961C:
    bl fn_800E27B0
    stw r3, 0x4(r31)
    lwz r0, 0x4(r31)
    cmplwi r0, 0x0
    bne @L_800F9640
    lhz r3, 0x0(r31)
    bl fn_800E209C
    li r3, 0x0
    b @L_800F9650
@L_800F9640:
    stw r28, 0x8(r31)
    stw r29, 0xc(r31)
    stw r30, 0x10(r31)
    lwz r3, 0x4(r31)
@L_800F9650:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
