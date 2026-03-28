    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r29, r3
    mr r30, r4
    mr r31, r5
    lbz r0, -21504(r13)
    cmplwi r0, 0x0
    bne @L_801E0FE4
    li r27, 0x1
    b @L_801E112C
@L_801E0FE4:
    lwz r0, -21496(r13)
    cmpwi r0, 0x2
    beq @L_801E105C
    bge @L_801E1004
    cmpwi r0, 0x0
    beq @L_801E1010
    bge @L_801E1018
    b @L_801E10A0
@L_801E1004:
    cmpwi r0, 0x4
    bge @L_801E10A0
    b @L_801E1064
@L_801E1010:
    li r27, 0x1
    b @L_801E10A0
@L_801E1018:
    li r0, 0x2
    lis r3, -32698
    stw r0, -21496(r13)
    addi r28, r3, 31992
    li r27, 0x0
    b @L_801E1048
@L_801E1030:
    lwz r3, 0x0(r28)
    cmplwi r3, 0x0
    beq @L_801E1040
    bl fn_800FE6F8
@L_801E1040:
    addi r27, r27, 0x1
    addi r28, r28, 0x4
@L_801E1048:
    lwz r0, -21492(r13)
    cmplw r27, r0
    blt @L_801E1030
    li r27, 0x1
    b @L_801E10A0
@L_801E105C:
    li r27, 0x0
    b @L_801E10A0
@L_801E1064:
    li r28, 0x0
    lis r3, -32698
    stw r28, -21496(r13)
    addi r27, r3, 31992
    b @L_801E1090
@L_801E1078:
    lwz r3, 0x0(r27)
    cmplwi r3, 0x0
    beq @L_801E1088
    bl fn_800FE6DC
@L_801E1088:
    addi r28, r28, 0x1
    addi r27, r27, 0x4
@L_801E1090:
    lwz r0, -21492(r13)
    cmplw r28, r0
    blt @L_801E1078
    li r27, 0x0
@L_801E10A0:
    lwz r0, -21500(r13)
    cmpwi r0, 0x3
    beq @L_801E10C0
    bge @L_801E10B4
    b @L_801E1114
@L_801E10B4:
    cmpwi r0, 0x5
    bge @L_801E1114
    b @L_801E10D8
@L_801E10C0:
    lwz r0, -21496(r13)
    cmpwi r0, 0x2
    beq @L_801E1114
    li r0, 0x1
    stw r0, -21496(r13)
    b @L_801E1114
@L_801E10D8:
    lwz r0, -21496(r13)
    li r4, 0x2
    cmpwi r0, 0x0
    bne @L_801E10EC
    li r4, 0x1
@L_801E10EC:
    lwz r3, -21488(r13)
    stw r4, -21496(r13)
    addi r0, r3, 0x1
    cmplwi r0, 0x5
    stw r0, -21488(r13)
    blt @L_801E1114
    li r3, 0x3
    li r0, 0x0
    stw r3, -21496(r13)
    stw r0, -21488(r13)
@L_801E1114:
    lwz r0, -21496(r13)
    cmpwi r0, 0x2
    bne @L_801E112C
    lis r3, 0xe390
    subi r3, r3, 0x6ef5
    bl fn_800F0470
@L_801E112C:
    lbz r0, -21476(r13)
    cmplwi r0, 0x0
    beq @L_801E113C
    li r27, 0x0
@L_801E113C:
    mr r4, r27
    li r3, 0x0
    bl fn_800D3410
    mr r3, r29
    mr r4, r30
    mr r5, r31
    bl fn_800D3FA4
    bl fn_800D3190
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
