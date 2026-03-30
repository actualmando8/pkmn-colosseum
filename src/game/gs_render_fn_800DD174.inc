    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    li r3, 0x0
    bl fn_801A4B00
    lwz r5, lbl_8047AAEC(r13)
    li r3, 0x0
    lwz r0, lbl_8047AAF0(r13)
    mr r4, r5
    mtctr r0
    cmplwi r0, 0x0
    ble @L_800DD1D4
    @L_800DD1AC:
    lbz r0, 0x0(r4)
    cmplwi r0, 0x1
    bne @L_800DD1C8
    lbz r0, 0x1(r4)
    cmplwi r0, 0x0
    beq @L_800DD1C8
    b @L_800DD1D8
    @L_800DD1C8:
    addi r4, r4, 0x74
    addi r3, r3, 0x1
    bdnz @L_800DD1AC
    @L_800DD1D4:
    li r3, -0x1
    @L_800DD1D8:
    cmpwi r3, -0x1
    beq @L_800DD254
    mulli r6, r3, 0x74
    addi r7, r3, 0x1
    mulli r4, r7, 0x74
    add r5, r5, r6
    b @L_800DD22C
    @L_800DD1F4:
    lwz r0, lbl_8047AAEC(r13)
    add r8, r0, r4
    lbz r0, 0x0(r8)
    cmplwi r0, 0x1
    bne @L_800DD224
    lbz r0, 0x1(r8)
    cmplwi r0, 0x0
    beq @L_800DD224
    lwz r3, 0xc(r5)
    mr r5, r8
    lwz r0, 0xc(r8)
    stw r0, 0xc(r3)
    @L_800DD224:
    addi r4, r4, 0x74
    addi r7, r7, 0x1
    @L_800DD22C:
    lwz r0, lbl_8047AAF0(r13)
    cmplw r7, r0
    blt @L_800DD1F4
    lwz r3, 0xc(r5)
    li r0, 0x0
    stw r0, 0xc(r3)
    lwz r0, lbl_8047AAEC(r13)
    add r3, r0, r6
    lwz r3, 0xc(r3)
    bl fn_801A4D20
    @L_800DD254:
    mr r3, r31
    bl fn_801A4F54
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
