    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r20, 0x10(r1)
    mr r20, r3
    mr r21, r4
    lha r0, 0x6(r21)
    lis r3, lbl_802EF0A8@ha
    addi r6, r3, lbl_802EF0A8@l
    lwz r27, 0x60(r20)
    mulli r4, r0, 0x1c
    lha r3, 0x492e(r6)
    lha r0, 0x4930(r6)
    li r24, 0x0
    lha r5, 0x4932(r6)
    li r23, 0xa
    add r4, r6, r4
    lha r6, 0x4934(r6)
    lha r7, 0x2(r4)
    li r22, 0x0
    lha r4, 0x4(r4)
    subf r3, r7, r3
    subf r4, r4, r0
    bl fn_800FE38C
    lbz r25, 0x94(r20)
    li r3, 0x50
    lwz r31, 0x8(r27)
    li r4, 0x270f
    extsb r25, r25
    bl fn_80132A38
    li r3, 0xdb
    bl fn_800FA444
    srwi r20, r3, 16
    li r3, 0x14f
    bl fn_800FA444
    lwz r4, 0xc(r27)
    srwi r3, r3, 16
    lha r0, 0x54(r21)
    lfs f0, lbl_8047B980(r2)
    lfs f1, 0x0(r4)
    subf r0, r20, r0
    subf r4, r3, r0
    fcmpu cr0, f0, f1
    beq @L_8002B284
    lwz r3, 0x14(r27)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_8002B284
    fcmpo cr0, f1, f0
    bge @L_8002B274
    subi r25, r25, 0x1
    li r24, -0x1
    b @L_8002B278
    @L_8002B274:
    li r23, 0xb
    @L_8002B278:
    fctiwz f0, f1
    stfd f0, 0x8(r1)
    lwz r22, 0xc(r1)
    @L_8002B284:
    mulli r30, r24, 0x1f
    mr r26, r24
    mr r28, r4
    slwi r29, r25, 1
    b @L_8002B3A8
    @L_8002B298:
    cmpwi r25, 0x0
    blt @L_8002B398
    subf r24, r22, r30
    blt @L_8002B2B4
    lwz r0, 0x8(r27)
    cmpw r25, r0
    blt @L_8002B2BC
    @L_8002B2B4:
    li r20, 0x0
    b @L_8002B2C4
    @L_8002B2BC:
    lwz r3, 0x4(r27)
    lhzx r20, r3, r29
    @L_8002B2C4:
    mr r3, r20
    bl fn_801440A0
    bl fn_80144088
    cmplwi r3, 0x0
    beq @L_8002B2EC
    mr r4, r24
    mr r6, r3
    li r3, 0x0
    li r5, -0x1
    bl fn_800FB680
    @L_8002B2EC:
    lbz r0, 0x1c(r27)
    cmplwi r0, 0x0
    beq @L_8002B300
    cmplwi r0, 0x1
    bne @L_8002B358
    @L_8002B300:
    mr r3, r28
    mr r4, r24
    li r5, -0x1
    li r6, 0x14f
    bl fn_800FB680
    mr r3, r20
    bl fn_801440A0
    bl fn_80143FFC
    mr r0, r3
    li r3, 0x50
    clrlwi r4, r0, 16
    bl fn_80132A38
    li r3, 0xdb
    bl fn_800FA444
    lha r0, 0x54(r21)
    srwi r3, r3, 16
    mr r4, r24
    li r5, -0x1
    subf r3, r3, r0
    li r6, 0xdb
    bl fn_800FB680
    b @L_8002B398
    @L_8002B358:
    mr r3, r20
    bl fn_801440A0
    bl fn_80143FE4
    mr r0, r3
    li r3, 0x50
    clrlwi r4, r0, 16
    bl fn_80132A38
    li r3, 0x153
    bl fn_800FA444
    lha r0, 0x54(r21)
    srwi r3, r3, 16
    mr r4, r24
    li r5, -0x1
    subf r3, r3, r0
    li r6, 0x153
    bl fn_800FB680
    @L_8002B398:
    addi r30, r30, 0x1f
    addi r26, r26, 0x1
    addi r29, r29, 0x2
    addi r25, r25, 0x1
    @L_8002B3A8:
    cmpw r26, r23
    bge @L_8002B3B8
    cmpw r25, r31
    blt @L_8002B298
    @L_8002B3B8:
    cmpw r26, r23
    bge @L_8002B3F0
    lbz r0, 0x1d(r27)
    mulli r4, r26, 0x1f
    li r3, 0x0
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    li r5, -0x1
    subf r4, r22, r4
    beq @L_8002B3E8
    li r6, 0x2b47
    b @L_8002B3EC
    @L_8002B3E8:
    li r6, 0x2b2c
    @L_8002B3EC:
    bl fn_800FB680
    @L_8002B3F0:
    bl fn_800FE35C
    li r3, 0x0
    lmw r20, 0x10(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
