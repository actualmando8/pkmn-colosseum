    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r4
    lwz r7, 0x60(r3)
    lis r3, lbl_80266E80@ha
    addi r6, r3, lbl_80266E80@l
    lbz r0, 0x1d(r7)
    lwz r5, 0x0(r6)
    clrlwi r0, r0, 31
    lwz r4, 0x4(r6)
    lwz r3, 0x8(r6)
    cmpwi r0, 0x0
    lwz r0, 0xc(r6)
    stw r5, 0x8(r1)
    stw r4, 0xc(r1)
    stw r3, 0x10(r1)
    stw r0, 0x14(r1)
    beq @L_8002ABA0
    li r0, 0x0
    li r3, 0x0
    stb r0, 0x67(r31)
    b @L_8002ACA4
    @L_8002ABA0:
    lbz r0, 0x1c(r7)
    cmplwi r0, 0x0
    beq @L_8002ABB4
    cmplwi r0, 0x1
    bne @L_8002ABC4
    @L_8002ABB4:
    li r0, 0x0
    li r3, 0x0
    stb r0, 0x67(r31)
    b @L_8002ACA4
    @L_8002ABC4:
    cmpwi r0, 0x3
    beq @L_8002ABF0
    bge @L_8002AC0C
    cmpwi r0, 0x2
    bge @L_8002ABDC
    b @L_8002AC0C
    @L_8002ABDC:
    li r3, 0x0
    li r4, 0xe
    li r5, 0x0
    bl fn_8012A5B0
    b @L_8002AC1C
    @L_8002ABF0:
    addi r0, r7, 0x20
    cmplwi r0, 0x0
    beq @L_8002AC04
    lwz r3, 0x77c(r7)
    b @L_8002AC1C
    @L_8002AC04:
    li r3, 0x0
    b @L_8002AC1C
    @L_8002AC0C:
    li r3, 0x0
    li r4, 0xe
    li r5, 0x0
    bl fn_8012A5B0
    @L_8002AC1C:
    lwz r5, lbl_804788F0(r13)
    lis r4, lbl_802E61D8@ha
    addi r0, r4, lbl_802E61D8@l
    subi r5, r5, 0x1
    slwi r4, r5, 2
    add r4, r0, r4
    addi r0, r5, 0x1
    mtctr r0
    cmpwi r5, 0x0
    blt @L_8002AC5C
    @L_8002AC44:
    lwz r0, 0x0(r4)
    cmplw r0, r3
    ble @L_8002AC5C
    subi r4, r4, 0x4
    subi r5, r5, 0x1
    bdnz @L_8002AC44
    @L_8002AC5C:
    cmpwi r5, 0x0
    bge @L_8002AC68
    li r5, 0x0
    @L_8002AC68:
    cmpwi r5, 0x4
    blt @L_8002AC74
    li r5, 0x3
    @L_8002AC74:
    slwi r0, r5, 2
    addi r3, r1, 0x8
    lha r4, 0x6(r31)
    lwzx r0, r3, r0
    cmpw r4, r0
    bne @L_8002AC98
    li r0, 0xff
    stb r0, 0x67(r31)
    b @L_8002ACA0
    @L_8002AC98:
    li r0, 0x0
    stb r0, 0x67(r31)
    @L_8002ACA0:
    li r3, 0x0
    @L_8002ACA4:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
