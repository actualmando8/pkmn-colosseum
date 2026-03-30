    nofralloc
    mflr r0
    lis r5, lbl_803FC5E0@ha
    stw r0, 0x4(r1)
    mulli r6, r3, 0xc
    stwu r1, -0x18(r1)
    addi r5, r5, lbl_803FC5E0@l
    stw r31, 0x14(r1)
    add r31, r5, r6
    lwz r0, lbl_80478A14(r13)
    rlwinm r4, r0, 0, 21, 23
    cmpwi r4, 0x400
    lis r0, 0x8000
    srw r0, r0, r3
    beq @L_800AA5FC
    bge @L_800AA540
    cmpwi r4, 0x200
    beq @L_800AA5CC
    bge @L_800AA534
    cmpwi r4, 0x100
    beq @L_800AA598
    bge @L_800AA5FC
    cmpwi r4, 0x0
    beq @L_800AA564
    b @L_800AA5FC
    @L_800AA534:
    cmpwi r4, 0x300
    beq @L_800AA5FC
    b @L_800AA5FC
    @L_800AA540:
    cmpwi r4, 0x600
    beq @L_800AA564
    bge @L_800AA558
    cmpwi r4, 0x500
    beq @L_800AA564
    b @L_800AA5FC
    @L_800AA558:
    cmpwi r4, 0x700
    beq @L_800AA564
    b @L_800AA5FC
    @L_800AA564:
    lbz r4, 0x6(r31)
    clrrwi r4, r4, 4
    stb r4, 0x6(r31)
    lbz r4, 0x7(r31)
    clrrwi r4, r4, 4
    stb r4, 0x7(r31)
    lbz r4, 0x8(r31)
    clrrwi r4, r4, 4
    stb r4, 0x8(r31)
    lbz r4, 0x9(r31)
    clrrwi r4, r4, 4
    stb r4, 0x9(r31)
    b @L_800AA5FC
    @L_800AA598:
    lbz r4, 0x4(r31)
    clrrwi r4, r4, 4
    stb r4, 0x4(r31)
    lbz r4, 0x5(r31)
    clrrwi r4, r4, 4
    stb r4, 0x5(r31)
    lbz r4, 0x8(r31)
    clrrwi r4, r4, 4
    stb r4, 0x8(r31)
    lbz r4, 0x9(r31)
    clrrwi r4, r4, 4
    stb r4, 0x9(r31)
    b @L_800AA5FC
    @L_800AA5CC:
    lbz r4, 0x4(r31)
    clrrwi r4, r4, 4
    stb r4, 0x4(r31)
    lbz r4, 0x5(r31)
    clrrwi r4, r4, 4
    stb r4, 0x5(r31)
    lbz r4, 0x6(r31)
    clrrwi r4, r4, 4
    stb r4, 0x6(r31)
    lbz r4, 0x7(r31)
    clrrwi r4, r4, 4
    stb r4, 0x7(r31)
    @L_800AA5FC:
    lbz r4, 0x2(r31)
    subi r4, r4, 0x80
    stb r4, 0x2(r31)
    lbz r4, 0x3(r31)
    subi r4, r4, 0x80
    stb r4, 0x3(r31)
    lbz r4, 0x4(r31)
    subi r4, r4, 0x80
    stb r4, 0x4(r31)
    lbz r4, 0x5(r31)
    subi r4, r4, 0x80
    stb r4, 0x5(r31)
    lwz r4, lbl_80478A10(r13)
    and. r0, r4, r0
    beq @L_800AA664
    lbz r0, 0x2(r31)
    extsb r0, r0
    cmpwi r0, 0x40
    ble @L_800AA664
    bl SIGetType
    clrrwi r3, r3, 16
    subis r0, r3, 0x900
    cmplwi r0, 0x0
    bne @L_800AA664
    li r0, 0x0
    stb r0, 0x2(r31)
    @L_800AA664:
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
