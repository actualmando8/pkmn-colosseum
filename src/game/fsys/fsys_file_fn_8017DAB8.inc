    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r22, 0x8(r1)
    mr r30, r3
    li r27, 0x0
    li r29, 0x0
    b @L_8017DB44
    @L_8017DAD8:
    lwz r28, 0x40(r30)
    cmplwi r28, 0x0
    beq @L_8017DB04
    lwz r3, 0x18(r28)
    slwi r0, r29, 2
    add r24, r28, r3
    lwz r3, 0x0(r24)
    add r25, r28, r3
    lwzx r0, r25, r0
    add r26, r28, r0
    b @L_8017DB08
    @L_8017DB04:
    li r26, 0x0
    @L_8017DB08:
    mr r22, r26
    stw r29, 0xf4(r30)
    addi r31, r22, 0x28
    lwz r0, 0x28(r26)
    cmpwi r0, 0x4
    beq @L_8017DB40
    lwz r0, 0x0(r31)
    cmpwi r0, 0x6
    beq @L_8017DB40
    lwz r0, 0x0(r31)
    cmpwi r0, 0x7
    beq @L_8017DB40
    li r27, 0x1
    b @L_8017DB50
    @L_8017DB40:
    addi r29, r29, 0x1
    @L_8017DB44:
    lwz r0, 0xc(r30)
    cmplw r29, r0
    blt @L_8017DAD8
    @L_8017DB50:
    cmpwi r27, 0x0
    bne @L_8017DB60
    mr r3, r30
    bl fn_8017D960
    @L_8017DB60:
    lmw r22, 0x8(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
