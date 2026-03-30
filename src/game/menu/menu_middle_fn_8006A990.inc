    nofralloc
    stwu r1, -0xb30(r1)
    mflr r0
    stw r0, 0xb34(r1)
    stw r31, 0xb2c(r1)
    stw r30, 0xb28(r1)
    stw r29, 0xb24(r1)
    stw r28, 0xb20(r1)
    mr r28, r3
    mr r29, r5
    addi r3, r1, 0x8
    bl fn_8012AC64
    addi r3, r1, 0x8
    li r4, 0x0
    bl fn_8012A7B4
    li r30, 0x0
    b @L_8006A9FC
    @L_8006A9D0:
    mr r4, r30
    addi r3, r1, 0x8
    bl fn_8012AC08
    mr r31, r3
    bl fn_80077A5C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8006A9F8
    mr r3, r31
    bl fn_8012086C
    @L_8006A9F8:
    addi r30, r30, 0x1
    @L_8006A9FC:
    clrlwi r0, r30, 16
    cmplwi r0, 0x6
    blt @L_8006A9D0
    lhz r31, 0x2(r28)
    mr r3, r28
    li r4, 0x0
    li r5, 0x1660
    bl memset
    sth r31, 0x2(r28)
    addi r3, r28, 0x2c
    addi r4, r1, 0x8
    bl fn_8012AC64
    addi r3, r28, 0xb44
    addi r4, r1, 0x8
    bl fn_8012AC64
    sth r29, 0x0(r28)
    clrlwi r4, r29, 16
    lwz r3, lbl_80478F20(r13)
    lwz r0, 0x0(r3)
    cmplw r0, r4
    bgt @L_8006AA58
    li r0, -0x1
    b @L_8006AA98
    @L_8006AA58:
    cmpwi r4, 0x9
    bge @L_8006AA70
    cmpwi r4, 0x1
    beq @L_8006AA94
    bge @L_8006AA84
    b @L_8006AA94
    @L_8006AA70:
    cmpwi r4, 0x30a
    bge @L_8006AA94
    cmpwi r4, 0x308
    bge @L_8006AA8C
    b @L_8006AA94
    @L_8006AA84:
    li r0, 0x1
    b @L_8006AA98
    @L_8006AA8C:
    li r0, 0x2
    b @L_8006AA98
    @L_8006AA94:
    li r0, 0x0
    @L_8006AA98:
    stw r0, 0x4(r28)
    lwz r0, 0xb34(r1)
    lwz r31, 0xb2c(r1)
    lwz r30, 0xb28(r1)
    lwz r29, 0xb24(r1)
    lwz r28, 0xb20(r1)
    mtlr r0
    addi r1, r1, 0xb30
    blr
