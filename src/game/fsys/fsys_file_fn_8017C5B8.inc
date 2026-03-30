    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r18, 0x18(r1)
    mr r31, r3
    lwz r28, 0x40(r3)
    lwz r18, 0xf4(r3)
    cmplwi r28, 0x0
    beq @L_8017C5FC
    lwz r3, 0x18(r28)
    slwi r0, r18, 2
    add r21, r28, r3
    lwz r3, 0x0(r21)
    add r22, r28, r3
    lwzx r0, r22, r0
    add r23, r28, r0
    b @L_8017C600
    @L_8017C5FC:
    li r23, 0x0
    @L_8017C600:
    mr r30, r23
    lwz r25, 0xfc(r31)
    lwz r3, 0xf8(r31)
    lwz r4, 0x20(r23)
    lwz r5, 0x0(r23)
    bl fn_8017F794
    mr. r29, r3
    bne @L_8017C6B8
    lwz r24, 0x14(r30)
    lwz r4, 0xf8(r31)
    lwz r5, 0x20(r30)
    mr r3, r24
    lwz r6, 0x0(r30)
    bl fn_8017F928
    mr. r29, r3
    bne @L_8017C64C
    li r0, 0x98
    stw r0, 0x48(r31)
    b @L_8017C6C0
    @L_8017C64C:
    lwz r0, 0x4c(r31)
    cmpwi r0, 0x3
    bne @L_8017C690
    mr r3, r31
    bl fn_8017D68C
    lwz r27, lbl_8047B1B8(r13)
    li r26, 0x0
    b @L_8017C684
    @L_8017C66C:
    lwz r3, 0x0(r27)
    lwz r0, 0x8(r31)
    cmpw r3, r0
    beq @L_8017C690
    addi r27, r27, 0x8
    addi r26, r26, 0x1
    @L_8017C684:
    lwz r0, lbl_8047B1BC(r13)
    cmpw r26, r0
    blt @L_8017C66C
    @L_8017C690:
    li r0, 0x97
    lis r3, fn_8017F25C@ha
    stw r0, 0x48(r31)
    addi r6, r3, fn_8017F25C@l
    mr r4, r29
    mr r5, r24
    lwz r3, 0x4(r25)
    mr r7, r31
    bl fn_80180694
    b @L_8017C6C0
    @L_8017C6B8:
    li r0, 0x98
    stw r0, 0x48(r31)
    @L_8017C6C0:
    mr r19, r29
    li r3, 0x1
    stw r19, 0x8(r1)
    lmw r18, 0x18(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
