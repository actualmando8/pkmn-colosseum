    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lwz r31, lbl_8047AC08(r13)
    b @L_800F0580
    @L_800F04E4:
    lwz r0, 0xc(r31)
    cmplw r0, r30
    bne @L_800F057C
    lwz r0, lbl_8047AC00(r13)
    cmplw r31, r0
    beq @L_800F0508
    lwz r0, lbl_8047AC04(r13)
    cmplw r31, r0
    bne @L_800F0524
    @L_800F0508:
    li r3, 0x1
    stb r3, 0x15(r31)
    lwz r0, lbl_8047AC04(r13)
    cmplw r31, r0
    bne @L_800F057C
    stb r3, lbl_8047AC0C(r13)
    b @L_800F057C
    @L_800F0524:
    li r0, 0x0
    stb r0, 0x14(r31)
    stb r0, 0x8(r31)
    lwz r3, 0x0(r31)
    cmplwi r3, 0x0
    beq @L_800F0544
    lwz r0, 0x4(r31)
    stw r0, 0x4(r3)
    @L_800F0544:
    lwz r3, 0x4(r31)
    cmplwi r3, 0x0
    beq @L_800F0558
    lwz r0, 0x0(r31)
    stw r0, 0x0(r3)
    @L_800F0558:
    lwz r0, lbl_8047AC08(r13)
    cmplw r0, r31
    bne @L_800F056C
    lwz r0, 0x4(r31)
    stw r0, lbl_8047AC08(r13)
    @L_800F056C:
    lhz r3, 0x20(r31)
    bl fn_800E209C
    lhz r3, 0x22(r31)
    bl fn_800E209C
    @L_800F057C:
    lwz r31, 0x4(r31)
    @L_800F0580:
    cmplwi r31, 0x0
    bne @L_800F04E4
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
