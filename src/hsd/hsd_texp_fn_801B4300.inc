    nofralloc
    stwu r1, -0x5e0(r1)
    mflr r0
    stw r0, 0x5e4(r1)
    li r0, 0x1
    stmw r27, 0x5cc(r1)
    mr. r28, r4
    li r4, 0x1
    mr r27, r3
    mr r29, r5
    stw r4, 0xc(r1)
    stw r0, 0x8(r1)
    bne @L_801B4340
    addi r3, r2, -22576
    li r4, 0x677
    addi r5, r2, -22568
    bl fn_80196E10
@L_801B4340:
    cmplwi r29, 0x0
    bne @L_801B435C
    lis r4, -32729
    addi r3, r2, -22576
    addi r5, r4, 21416
    li r4, 0x678
    bl fn_80196E10
@L_801B435C:
    addi r3, r1, 0x10
    li r4, 0x0
    li r5, 0x2c
    bl memset
    cmplwi r27, 0x0
    bne @L_801B437C
    li r0, 0x0
    b @L_801B43A8
@L_801B437C:
    addis r0, r27, 0x1
    cmplwi r0, 0xffff
    bne @L_801B4390
    li r0, 0x2
    b @L_801B43A8
@L_801B4390:
    addis r0, r27, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B43A4
    li r0, 0x3
    b @L_801B43A8
@L_801B43A4:
    lwz r0, 0x0(r27)
@L_801B43A8:
    cmpwi r0, 0x4
    beq @L_801B43D0
    bge @L_801B43DC
    cmpwi r0, 0x1
    beq @L_801B43C0
    b @L_801B43DC
@L_801B43C0:
    lwz r3, 0x8(r27)
    addi r0, r3, 0x1
    stw r0, 0x8(r27)
    b @L_801B43DC
@L_801B43D0:
    lbz r3, 0x10(r27)
    addi r0, r3, 0x1
    stb r0, 0x10(r27)
@L_801B43DC:
    cmplwi r27, 0x0
    bne @L_801B43EC
    li r0, 0x0
    b @L_801B4418
@L_801B43EC:
    addis r0, r27, 0x1
    cmplwi r0, 0xffff
    bne @L_801B4400
    li r0, 0x2
    b @L_801B4418
@L_801B4400:
    addis r0, r27, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B4414
    li r0, 0x3
    b @L_801B4418
@L_801B4414:
    lwz r0, 0x0(r27)
@L_801B4418:
    cmpwi r0, 0x4
    beq @L_801B4440
    bge @L_801B444C
    cmpwi r0, 0x1
    beq @L_801B4430
    b @L_801B444C
@L_801B4430:
    lwz r3, 0x14(r27)
    addi r0, r3, 0x1
    stw r0, 0x14(r27)
    b @L_801B444C
@L_801B4440:
    lbz r3, 0x10(r27)
    addi r0, r3, 0x1
    stb r0, 0x10(r27)
@L_801B444C:
    mr r3, r27
    bl fn_801B8FB8
    mr r3, r27
    addi r4, r1, 0xbc
    bl fn_801B8024
    addi r4, r1, 0xbc
    mr r31, r3
    addi r5, r1, 0x3c
    addi r6, r1, 0x10
    bl fn_801B7CA0
    li r30, 0x0
    b @L_801B44B0
@L_801B447C:
    rlwinm r0, r30, 2, 0, 29
    addi r3, r1, 0x3c
    lwzx r3, r3, r0
    addi r4, r1, 0x10
    bl fn_801B50C0
    cmpwi r3, 0x0
    bge @L_801B44AC
    lis r4, -32729
    addi r3, r2, -22576
    addi r5, r4, 21428
    li r4, 0x698
    bl fn_80196E10
@L_801B44AC:
    addi r30, r30, 0x1
@L_801B44B0:
    cmpw r30, r31
    blt @L_801B447C
    subi r30, r31, 0x1
    b @L_801B44D4
@L_801B44C0:
    rlwinm r0, r30, 2, 0, 29
    addi r3, r1, 0x3c
    lwzx r3, r3, r0
    bl fn_801B8D5C
    subi r30, r30, 0x1
@L_801B44D4:
    cmpwi r30, 0x0
    bge @L_801B44C0
    mr r3, r27
    addi r4, r1, 0xbc
    bl fn_801B8024
    addi r4, r1, 0xbc
    mr r31, r3
    addi r5, r1, 0x3c
    addi r6, r1, 0x10
    bl fn_801B7CA0
    li r0, 0x0
    li r30, 0x0
    stw r0, 0x0(r28)
    b @L_801B4558
@L_801B450C:
    li r3, 0x88
    bl fn_80193B10
    mr r0, r3
    mr r3, r30
    mr r27, r0
    bl fn_801B3338
    subf r0, r30, r31
    stw r3, 0x8(r27)
    rlwinm r0, r0, 2, 0, 29
    addi r3, r1, 0x38
    lwzx r3, r3, r0
    mr r4, r27
    addi r5, r1, 0xc
    addi r6, r1, 0x8
    bl fn_801B4B5C
    lwz r0, 0x0(r28)
    addi r30, r30, 0x1
    stw r0, 0x0(r27)
    stw r27, 0x0(r28)
@L_801B4558:
    cmpw r30, r31
    blt @L_801B450C
    lwz r3, 0x0(r29)
    li r4, 0x1
    li r5, 0x1
    bl fn_801B7178
    stw r3, 0x0(r29)
    li r4, 0x4
    li r5, 0x0
    lwz r3, 0x0(r29)
    bl fn_801B7178
    mr r0, r3
    mr r3, r31
    stw r0, 0x0(r29)
    lmw r27, 0x5cc(r1)
    lwz r0, 0x5e4(r1)
    mtlr r0
    addi r1, r1, 0x5e0
    blr
