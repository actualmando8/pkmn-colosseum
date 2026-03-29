    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r29, r3
    mr r30, r4
    mr r25, r6
    mulli r31, r5, 0x4c
    lis r3, lbl_80266918@ha
    addi r0, r3, lbl_80266918@l
    add r3, r0, r31
    lwz r0, 0x4(r3)
    cmpwi r0, 0x0
    blt @L_80017D10
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_80017D28
    @L_80017D10:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_80017D28:
    mr r26, r3
    li r28, -0x1
    li r27, 0x0
    b @L_80017D70
    @L_80017D38:
    mr r3, r26
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80017D68
    addi r28, r28, 0x1
    cmpw r28, r25
    blt @L_80017D68
    mr r3, r26
    bl fn_80143C68
    mr r28, r3
    b @L_80017D80
    @L_80017D68:
    addi r27, r27, 0x1
    addi r26, r26, 0x4
    @L_80017D70:
    lhz r0, 0x8(r1)
    cmpw r27, r0
    blt @L_80017D38
    li r28, 0x0
    @L_80017D80:
    mr r3, r28
    bl fn_801440A0
    bl fn_80143DCC
    cmplwi r3, 0x0
    beq @L_80017D9C
    li r6, 0x1
    b @L_80017DA0
    @L_80017D9C:
    li r6, 0x0
    @L_80017DA0:
    clrlwi r0, r28, 16
    cmplwi r0, 0x219
    bne @L_80017DB4
    li r7, 0x0
    b @L_80017DB8
    @L_80017DB4:
    li r7, 0x1
    @L_80017DB8:
    lwz r0, lbl_8047A2E0(r13)
    lis r3, lbl_80266918@ha
    addi r4, r3, lbl_80266918@l
    add r4, r4, r31
    slwi r0, r0, 3
    add r4, r4, r0
    li r3, 0x0
    lwz r0, 0x28(r4)
    lwz r5, 0x24(r4)
    mtctr r0
    cmpwi r0, 0x0
    ble @L_80017E78
    @L_80017DE8:
    lhz r4, 0x8(r5)
    clrlwi r0, r4, 31
    cmpwi r0, 0x0
    bne @L_80017E70
    rlwinm r0, r4, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_80017E0C
    cmpwi r6, 0x0
    beq @L_80017E70
    @L_80017E0C:
    rlwinm r0, r4, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_80017E20
    cmpwi r7, 0x0
    beq @L_80017E70
    @L_80017E20:
    lwz r4, 0x0(r5)
    cmpwi r6, 0x0
    li r0, 0x0
    stw r4, 0x0(r29)
    lwz r4, 0x4(r5)
    stw r4, 0x4(r29)
    sth r0, 0x8(r29)
    bne @L_80017E60
    lhz r0, 0x8(r5)
    rlwinm r0, r0, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_80017E60
    lhz r0, 0x8(r29)
    ori r0, r0, 0x1
    clrlwi r0, r0, 16
    sth r0, 0x8(r29)
    @L_80017E60:
    addi r3, r3, 0x1
    addi r29, r29, 0xc
    cmpw r3, r30
    bge @L_80017E78
    @L_80017E70:
    addi r5, r5, 0xc
    bdnz @L_80017DE8
    @L_80017E78:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
