    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    lbz r0, 0x1(r28)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80019A40
    li r4, 0x48
    bl fn_80103FFC
    cmplwi r3, 0x0
    beq @L_80019A40
    lwz r4, 0x60(r28)
    li r5, 0x48
    bl memcpy
    @L_80019A40:
    mr r3, r28
    bl fn_80103FE4
    li r29, 0x0
    la r30, lbl_80478870(r13)
    mr r31, r3
    @L_80019A54:
    lwz r0, 0x4(r31)
    cmplwi r0, 0x0
    beq @L_80019A70
    lhz r3, 0x0(r30)
    li r4, 0x1
    bl fn_8005D8F8
    b @L_80019A7C
    @L_80019A70:
    lhz r3, 0x0(r30)
    li r4, 0x0
    bl fn_8005D8F8
    @L_80019A7C:
    addi r31, r31, 0xc
    addi r30, r30, 0x2
    addi r29, r29, 0x1
    cmpwi r29, 0x4
    blt @L_80019A54
    lwz r3, 0x4(r28)
    bl fn_801022B8
    cmpwi r3, 0xe95
    beq @L_80019AD8
    bge @L_80019AB4
    cmpwi r3, 0xe93
    beq @L_80019AC0
    bge @L_80019ACC
    b @L_80019AF0
    @L_80019AB4:
    cmpwi r3, 0xe97
    bge @L_80019AF0
    b @L_80019AE4
    @L_80019AC0:
    li r0, 0x0
    stw r0, 0x80(r28)
    b @L_80019AF8
    @L_80019ACC:
    li r0, 0x1
    stw r0, 0x80(r28)
    b @L_80019AF8
    @L_80019AD8:
    li r0, 0x2
    stw r0, 0x80(r28)
    b @L_80019AF8
    @L_80019AE4:
    li r0, 0x3
    stw r0, 0x80(r28)
    b @L_80019AF8
    @L_80019AF0:
    li r0, -0x1
    stw r0, 0x80(r28)
    @L_80019AF8:
    li r3, 0x0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
