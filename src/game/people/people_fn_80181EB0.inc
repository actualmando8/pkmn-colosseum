    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r27, r3
    mr r28, r4
    li r29, 0x0
    bl fn_80113F48
    lis r4, 0xf85
    mr r31, r3
    addi r3, r4, 0x400
    bl fn_801CBA0C
    li r26, 0x0
    mr r30, r3
    b @L_80181F24
    @L_80181EEC:
    mr r3, r26
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80181F20
    lwz r0, 0x28(r3)
    cmplw r0, r27
    bne @L_80181F20
    lwz r0, 0x2c(r3)
    cmplw r0, r28
    bne @L_80181F20
    lwz r26, 0x4(r3)
    b @L_80181F90
    @L_80181F20:
    addi r26, r26, 0x1
    @L_80181F24:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_80181EEC
    li r25, 0x0
    b @L_80181F80
    @L_80181F38:
    mr r3, r25
    bl fn_8018FD88
    mr r26, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80181F7C
    lwz r0, 0x2c(r26)
    cmplw r0, r28
    bne @L_80181F7C
    lis r3, lbl_80273FD8@ha
    mr r4, r27
    addi r3, r3, lbl_80273FD8@l
    mr r5, r28
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r26, 0x4(r26)
    b @L_80181F90
    @L_80181F7C:
    addi r25, r25, 0x1
    @L_80181F80:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_80181F38
    li r26, 0x0
    @L_80181F90:
    li r25, 0x0
    b @L_80181FC0
    @L_80181F98:
    mr r3, r25
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80181FBC
    lwz r0, 0x4(r3)
    cmplw r0, r26
    bne @L_80181FBC
    b @L_80181FD0
    @L_80181FBC:
    addi r25, r25, 0x1
    @L_80181FC0:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_80181F98
    li r3, 0x0
    @L_80181FD0:
    cmplwi r3, 0x0
    beq @L_80181FF0
    lwz r3, 0x30(r3)
    bl fn_8018F6F4
    cmplwi r3, 0x0
    beq @L_80181FF0
    bl fn_8018F698
    mr r29, r3
    @L_80181FF0:
    extsb. r0, r29
    blt @L_80182014
    mr r3, r31
    mr r4, r30
    mr r5, r27
    mr r6, r28
    extsb r7, r29
    bl fn_801845E4
    b @L_8018202C
    @L_80182014:
    mr r3, r31
    mr r4, r30
    mr r5, r27
    mr r6, r28
    li r7, 0x0
    bl fn_801845E4
    @L_8018202C:
    mr r3, r31
    mr r4, r30
    bl fn_800F9318
    mr. r25, r3
    beq @L_80182054
    li r4, 0x1
    bl fn_800E3CC8
    mr r3, r25
    li r4, 0x1
    bl fn_800E90C8
    @L_80182054:
    mr r3, r30
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    li r3, 0x47
    bl fn_80166A28
    li r25, 0x0
    b @L_801820B0
    @L_80182078:
    mr r3, r25
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801820AC
    lwz r0, 0x28(r3)
    cmplw r0, r31
    bne @L_801820AC
    lwz r0, 0x2c(r3)
    cmplw r0, r30
    bne @L_801820AC
    lwz r25, 0x4(r3)
    b @L_8018211C
    @L_801820AC:
    addi r25, r25, 0x1
    @L_801820B0:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_80182078
    li r25, 0x0
    b @L_8018210C
    @L_801820C4:
    mr r3, r25
    bl fn_8018FD88
    mr r27, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182108
    lwz r0, 0x2c(r27)
    cmplw r0, r30
    bne @L_80182108
    lis r3, lbl_80273FD8@ha
    mr r4, r31
    addi r3, r3, lbl_80273FD8@l
    mr r5, r30
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r25, 0x4(r27)
    b @L_8018211C
    @L_80182108:
    addi r25, r25, 0x1
    @L_8018210C:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_801820C4
    li r25, 0x0
    @L_8018211C:
    li r26, 0x0
    b @L_80182150
    @L_80182124:
    mr r3, r26
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018214C
    lwz r0, 0x4(r3)
    cmplw r0, r25
    bne @L_8018214C
    mr r25, r3
    b @L_80182160
    @L_8018214C:
    addi r26, r26, 0x1
    @L_80182150:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_80182124
    li r25, 0x0
    @L_80182160:
    cmplwi r25, 0x0
    beq @L_801821A4
    li r0, 0x0
    lis r3, 0x596
    stw r0, 0x8(r1)
    mr r9, r31
    mr r10, r30
    addi r4, r3, 0xe
    stw r0, 0xc(r1)
    li r3, 0xf
    li r5, 0x1000
    li r6, 0x1
    li r7, 0x0
    li r8, 0x4
    crxor 6, 6, 6
    bl fn_800F7318
    sth r3, 0x4c(r25)
    @L_801821A4:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
