    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r31, r3
    li r25, 0x0
    li r24, 0x0
    li r4, 0x0
    bl fn_801040D0
    clrlwi r26, r3, 24
    mr r3, r31
    bl fn_80103FE4
    mr r0, r3
    mr r3, r31
    mr r28, r0
    li r4, 0x2
    bl fn_801040D0
    extsb r0, r3
    lwz r3, 0x4(r31)
    mr r27, r0
    bl fn_8005D9E4
    clrlwi r0, r3, 24
    cmpw r27, r0
    ble @L_80012BF8
    mr r27, r0
    @L_80012BF8:
    mr r30, r28
    li r29, 0x0
    b @L_80012C30
    @L_80012C04:
    lwz r3, 0x0(r30)
    bl fn_800FA444
    srwi r0, r3, 16
    cmpw r24, r0
    bge @L_80012C1C
    mr r24, r0
    @L_80012C1C:
    clrlwi r0, r3, 16
    addi r30, r30, 0x4
    add r25, r0, r25
    addi r29, r29, 0x1
    addi r25, r25, 0x2
    @L_80012C30:
    cmpw r29, r27
    blt @L_80012C04
    cmpwi r26, 0x7f
    beq @L_80012C74
    bge @L_80012C88
    cmpwi r26, 0x2
    bge @L_80012C88
    cmpwi r26, 0x0
    bge @L_80012C58
    b @L_80012C88
    @L_80012C58:
    lbz r7, 0x8b(r31)
    mr r6, r25
    addi r5, r24, 0x20
    li r3, 0x0
    li r4, 0x0
    bl fn_8001E644
    b @L_80012C88
    @L_80012C74:
    mr r6, r25
    addi r5, r24, 0x20
    li r3, 0x0
    li r4, 0x0
    bl fn_8001EA98
    @L_80012C88:
    mr r30, r28
    li r24, 0x1
    li r28, 0x0
    b @L_80012D00
    @L_80012C98:
    lwz r3, 0x0(r30)
    cmplwi r3, 0x0
    beq @L_80012CC8
    bl fn_800FA444
    clrlwi r5, r3, 16
    lwz r6, 0x0(r30)
    mr r4, r24
    li r3, 0x20
    addi r29, r5, 0x2
    li r5, -0x1
    bl fn_800FB680
    b @L_80012CCC
    @L_80012CC8:
    li r29, 0x14
    @L_80012CCC:
    lbz r0, 0x95(r31)
    extsb r0, r0
    cmpw r0, r28
    bne @L_80012CF4
    mr r5, r31
    extsh r4, r24
    li r3, 0x20
    li r6, 0x157
    li r7, 0x0
    bl fn_801040F0
    @L_80012CF4:
    add r24, r24, r29
    addi r30, r30, 0x4
    addi r28, r28, 0x1
    @L_80012D00:
    cmpw r28, r27
    blt @L_80012C98
    li r3, 0x0
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
