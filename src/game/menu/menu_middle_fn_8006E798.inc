    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r30, r3
    lbz r0, 0x2(r30)
    lis r3, lbl_80267EA8@ha
    addi r31, r3, lbl_80267EA8@l
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8006E858
    lbz r0, 0x1(r30)
    extsb r0, r0
    cmpwi r0, 0x3
    beq @L_8006E820
    bge @L_8006E858
    cmpwi r0, 0x0
    beq @L_8006E7E4
    b @L_8006E858
    @L_8006E7E4:
    addi r28, r31, 0x5f4
    addi r3, r31, 0x0
    li r27, 0x0
    lhz r29, 0x18(r3)
    @L_8006E7F4:
    lhz r4, 0x0(r28)
    mr r3, r30
    bl fn_801046C8
    mr r4, r29
    addi r3, r3, 0xc
    bl fn_80108518
    addi r28, r28, 0x2
    addi r27, r27, 0x1
    cmplwi r27, 0x7
    blt @L_8006E7F4
    b @L_8006E858
    @L_8006E820:
    addi r28, r31, 0x5f4
    addi r3, r31, 0x0
    li r27, 0x0
    lhz r29, 0x1a(r3)
    @L_8006E830:
    lhz r4, 0x0(r28)
    mr r3, r30
    bl fn_801046C8
    mr r4, r29
    addi r3, r3, 0xc
    bl fn_80108518
    addi r28, r28, 0x2
    addi r27, r27, 0x1
    cmplwi r27, 0x7
    blt @L_8006E830
    @L_8006E858:
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    lwz r0, 0x4(r3)
    cmpwi r0, 0x2
    beq @L_8006E888
    bge @L_8006E888
    cmpwi r0, 0x0
    bge @L_8006E880
    b @L_8006E888
    @L_8006E880:
    li r27, 0x0
    b @L_8006E88C
    @L_8006E888:
    li r27, 0x2
    @L_8006E88C:
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    addi r3, r3, 0x59a8
    bl fn_8006A7E8
    cmpwi r3, 0x0
    beq @L_8006E8B0
    li r0, 0x1
    b @L_8006E8B4
    @L_8006E8B0:
    li r0, 0x0
    @L_8006E8B4:
    mr r3, r30
    add r27, r27, r0
    li r4, 0x99b
    bl fn_801046C8
    slwi r0, r27, 3
    addi r28, r31, 0x5d4
    li r29, 0x0
    mr r27, r3
    add r28, r28, r0
    @L_8006E8D8:
    lbz r4, 0x0(r28)
    mr r3, r27
    bl fn_80109220
    lwz r27, 0x0(r27)
    addi r28, r28, 0x1
    addi r29, r29, 0x1
    cmpwi r29, 0x8
    blt @L_8006E8D8
    mr r3, r30
    li r4, 0x9a7
    bl fn_801046C8
    li r0, 0x3d2c
    li r4, 0x9a9
    stw r0, 0x4c(r3)
    mr r3, r30
    bl fn_801046C8
    li r0, 0x3d26
    li r4, 0x9a6
    stw r0, 0x4c(r3)
    mr r3, r30
    bl fn_801046C8
    mr r29, r3
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    lwz r0, 0x8(r3)
    addi r5, r31, 0x604
    mr r3, r30
    li r4, 0x9a8
    slwi r0, r0, 2
    lwzx r0, r5, r0
    stw r0, 0x4c(r29)
    bl fn_801046C8
    mr r29, r3
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    lwz r0, 0x4(r3)
    addi r6, r31, 0x61c
    mr r3, r30
    li r4, 0x0
    slwi r0, r0, 2
    li r5, 0x0
    lwzx r0, r6, r0
    stw r0, 0x4c(r29)
    bl fn_80070D84
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
