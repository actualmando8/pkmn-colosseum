    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    mr r28, r3
    mr r29, r4
    bl fn_80103FE4
    mr r31, r3
    mr r3, r28
    mr r4, r29
    bl fn_8001D834
    mr r0, r3
    lwz r3, 0x4(r28)
    mr r30, r0
    bl fn_80104530
    srwi r0, r3, 16
    li r3, 0x197
    sth r0, 0x8(r1)
    bl fn_800FA444
    srwi r3, r3, 16
    lha r0, 0x54(r29)
    extsh r3, r3
    mr r5, r30
    subf r3, r3, r0
    li r4, 0x0
    srwi r0, r3, 31
    li r6, 0x197
    add r0, r0, r3
    srawi r0, r0, 1
    extsh r26, r0
    mr r3, r26
    bl fn_800FB680
    lbz r0, 0x9(r1)
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_80010468
    cmpwi r0, 0x4
    bge @L_80010468
    mulli r27, r0, 0xc
    add r3, r31, r27
    lwz r0, 0x4(r3)
    cmplwi r0, 0x0
    beq @L_80010468
    lwz r3, 0x40(r31)
    bl fn_80205B8C
    cmplwi r3, 0x0
    beq @L_8001036C
    lbz r6, 0x95(r28)
    li r4, 0x0
    li r5, 0x7f
    extsb r6, r6
    bl fn_8012640C
    clrlwi r0, r3, 16
    b @L_80010370
    @L_8001036C:
    li r0, 0x0
    @L_80010370:
    clrlwi r0, r0, 16
    cmpwi r0, 0x164
    beq @L_80010398
    bge @L_8001038C
    cmpwi r0, 0x0
    beq @L_80010398
    b @L_80010410
    @L_8001038C:
    cmpwi r0, 0x166
    bge @L_80010410
    b @L_800103D4
    @L_80010398:
    lha r6, 0x56(r29)
    mr r5, r26
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r8, 0x2be1
    bl fn_800FB8C8
    lha r5, 0x54(r29)
    mr r7, r30
    lha r6, 0x56(r29)
    li r3, 0x0
    li r4, 0x0
    li r8, 0x2be1
    bl fn_800FB8C8
    b @L_80010468
    @L_800103D4:
    lha r6, 0x56(r29)
    mr r5, r26
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r8, 0x2b6d
    bl fn_800FB8C8
    lha r5, 0x54(r29)
    mr r7, r30
    lha r6, 0x56(r29)
    li r3, 0x0
    li r4, 0x0
    li r8, 0x2b6d
    bl fn_800FB8C8
    b @L_80010468
    @L_80010410:
    add r4, r31, r27
    li r3, 0x34
    lbz r4, 0xf(r4)
    bl fn_80132A38
    lha r6, 0x56(r29)
    mr r5, r26
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r8, 0xd2
    bl fn_800FB8C8
    add r4, r31, r27
    li r3, 0x34
    lbz r4, 0xe(r4)
    bl fn_80132A38
    lha r5, 0x54(r29)
    mr r7, r30
    lha r6, 0x56(r29)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xd2
    bl fn_800FB8C8
    @L_80010468:
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
