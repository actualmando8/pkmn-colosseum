    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lbz r0, 0x1(r31)
    extsb r0, r0
    cmpwi r0, 0x3
    beq @L_80056678
    bge @L_8005669C
    cmpwi r0, 0x0
    beq @L_80056644
    b @L_8005669C
    @L_80056644:
    lbz r0, 0x2(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8005669C
    lwz r0, lbl_8047A568(r13)
    cmpwi r0, 0x0
    beq @L_8005666C
    lwz r3, 0x4(r31)
    li r4, 0x107
    bl fn_801080CC
    @L_8005666C:
    li r0, 0x1
    stb r0, 0x2(r31)
    b @L_8005669C
    @L_80056678:
    lbz r0, 0x2(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8005669C
    lwz r3, 0x4(r31)
    li r4, 0x10b
    bl fn_801080CC
    li r0, 0x1
    stb r0, 0x2(r31)
    @L_8005669C:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
