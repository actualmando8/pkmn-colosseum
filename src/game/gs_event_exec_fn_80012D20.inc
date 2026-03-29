    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    lbz r0, 0x1(r29)
    extsb r0, r0
    cmpwi r0, 0x3
    beq @L_80012DD4
    bge @L_80012D5C
    cmpwi r0, 0x0
    beq @L_80012D68
    b @L_80012DF8
    @L_80012D5C:
    cmpwi r0, 0x5
    beq @L_80012DF8
    b @L_80012DF8
    @L_80012D68:
    lbz r0, 0x2(r29)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80012DF8
    li r4, 0x1
    bl fn_801040D0
    mr r0, r3
    mr r3, r29
    mr r30, r0
    li r4, 0x2
    bl fn_801040D0
    mr r0, r3
    mr r3, r29
    slwi r31, r0, 2
    mr r4, r31
    bl fn_80103FFC
    cmplwi r3, 0x0
    beq @L_80012DBC
    mr r4, r30
    mr r5, r31
    bl memcpy
    @L_80012DBC:
    lwz r3, 0x4(r29)
    li r4, 0x26
    bl fn_801080CC
    li r0, 0x1
    stb r0, 0x2(r29)
    b @L_80012DF8
    @L_80012DD4:
    lbz r0, 0x2(r29)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80012DF8
    lwz r3, 0x4(r29)
    li r4, 0x2a
    bl fn_801080CC
    li r0, 0x1
    stb r0, 0x2(r29)
    @L_80012DF8:
    li r3, 0x0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
