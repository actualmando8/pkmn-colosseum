    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r31, lbl_8047AC00(r13)
    stw r3, 0x18(r31)
    lbz r0, 0x17(r31)
    cmplwi r0, 0x1
    bne @L_800F1000
    lwz r0, lbl_8047AC00(r13)
    cmplw r31, r0
    beq @L_800F0F88
    lwz r0, lbl_8047AC04(r13)
    cmplw r31, r0
    bne @L_800F0FA4
    @L_800F0F88:
    li r3, 0x1
    stb r3, 0x15(r31)
    lwz r0, lbl_8047AC04(r13)
    cmplw r31, r0
    bne @L_800F1054
    stb r3, lbl_8047AC0C(r13)
    b @L_800F1054
    @L_800F0FA4:
    li r0, 0x0
    stb r0, 0x14(r31)
    stb r0, 0x8(r31)
    lwz r3, 0x0(r31)
    cmplwi r3, 0x0
    beq @L_800F0FC4
    lwz r0, 0x4(r31)
    stw r0, 0x4(r3)
    @L_800F0FC4:
    lwz r3, 0x4(r31)
    cmplwi r3, 0x0
    beq @L_800F0FD8
    lwz r0, 0x0(r31)
    stw r0, 0x0(r3)
    @L_800F0FD8:
    lwz r0, lbl_8047AC08(r13)
    cmplw r0, r31
    bne @L_800F0FEC
    lwz r0, 0x4(r31)
    stw r0, lbl_8047AC08(r13)
    @L_800F0FEC:
    lhz r3, 0x20(r31)
    bl fn_800E209C
    lhz r3, 0x22(r31)
    bl fn_800E209C
    b @L_800F1054
    @L_800F1000:
    li r3, 0x1
    li r0, 0x0
    stb r3, 0x15(r31)
    stb r0, 0x14(r31)
    lwz r3, 0x0(r31)
    cmplwi r3, 0x0
    beq @L_800F1024
    lwz r0, 0x4(r31)
    stw r0, 0x4(r3)
    @L_800F1024:
    lwz r3, 0x4(r31)
    cmplwi r3, 0x0
    beq @L_800F1038
    lwz r0, 0x0(r31)
    stw r0, 0x0(r3)
    @L_800F1038:
    lwz r0, lbl_8047AC08(r13)
    cmplw r0, r31
    bne @L_800F104C
    lwz r0, 0x4(r31)
    stw r0, lbl_8047AC08(r13)
    @L_800F104C:
    li r0, 0x1
    stb r0, lbl_8047AC0C(r13)
    @L_800F1054:
    bl fn_800F0308
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
