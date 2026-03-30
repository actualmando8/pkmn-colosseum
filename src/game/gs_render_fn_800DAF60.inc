    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    lwz r5, lbl_8047AA80(r13)
    lbz r0, 0x47e(r5)
    cmplwi r0, 0x1
    bne @L_800DAF98
    li r3, 0x0
    b @L_800DB07C
    @L_800DAF98:
    lbz r0, 0x49f(r5)
    cmplwi r0, 0x1
    bne @L_800DAFAC
    li r3, 0x0
    b @L_800DB07C
    @L_800DAFAC:
    lwz r0, lbl_8047AAD8(r13)
    lwz r31, lbl_8047AAD4(r13)
    mtctr r0
    cmplwi r0, 0x0
    ble @L_800DAFD8
    @L_800DAFC0:
    lbz r0, 0x0(r31)
    cmplwi r0, 0x0
    bne @L_800DAFD0
    b @L_800DAFDC
    @L_800DAFD0:
    addi r31, r31, 0x18
    bdnz @L_800DAFC0
    @L_800DAFD8:
    li r31, 0x0
    @L_800DAFDC:
    cmplwi r31, 0x0
    bne @L_800DAFEC
    li r3, 0x0
    b @L_800DB07C
    @L_800DAFEC:
    li r3, 0x1
    li r0, 0x0
    stb r3, 0x0(r31)
    mr r3, r30
    li r4, 0x20
    stb r0, 0x1(r31)
    stw r0, 0x10(r31)
    stw r0, 0x14(r31)
    bl fn_800E2C04
    sth r3, 0x2(r31)
    lhz r0, 0x2(r31)
    cmplwi r0, 0x0
    bne @L_800DB028
    li r3, 0x0
    b @L_800DB07C
    @L_800DB028:
    stw r30, 0x8(r31)
    lhz r3, 0x2(r31)
    bl fn_800E27B0
    stw r3, 0x4(r31)
    lwz r0, 0x4(r31)
    cmplwi r0, 0x0
    bne @L_800DB054
    lhz r3, 0x2(r31)
    bl fn_800E209C
    li r3, 0x0
    b @L_800DB07C
    @L_800DB054:
    stw r29, 0xc(r31)
    li r0, 0x1
    li r3, 0x1
    lwz r4, lbl_8047AA80(r13)
    stb r0, 0x47e(r4)
    lwz r4, lbl_8047AA80(r13)
    stw r31, 0x480(r4)
    lwz r0, 0x4(r31)
    lwz r4, lbl_8047AA80(r13)
    stw r0, 0x484(r4)
    @L_800DB07C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
