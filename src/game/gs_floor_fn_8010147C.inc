    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr. r26, r3
    lis r3, -32729
    mr r27, r4
    mr r29, r5
    mr r28, r6
    addi r31, r3, 6128
    beq @L_801018FC
    cmplwi r27, 0x0
    bne @L_801014B4
    b @L_801018FC
@L_801014B4:
    addi r3, r31, 0x520
    crxor 6, 6, 6
    bl fn_800DD970
    lis r3, -32704
    li r0, 0x10
    addi r30, r3, 9496
    li r3, 0x0
    mtctr r0
@L_801014D4:
    lwz r0, 0x44(r30)
    cmpwi r0, 0x0
    beq @L_801014F0
    lwz r0, 0x40(r30)
    cmplw r0, r26
    bne @L_801014F0
    b @L_801015E0
@L_801014F0:
    lwz r0, 0x8c(r30)
    addi r30, r30, 0x48
    cmpwi r0, 0x0
    beq @L_80101510
    lwz r0, 0x40(r30)
    cmplw r0, r26
    bne @L_80101510
    b @L_801015E0
@L_80101510:
    lwz r0, 0x8c(r30)
    addi r30, r30, 0x48
    cmpwi r0, 0x0
    beq @L_80101530
    lwz r0, 0x40(r30)
    cmplw r0, r26
    bne @L_80101530
    b @L_801015E0
@L_80101530:
    lwz r0, 0x8c(r30)
    addi r30, r30, 0x48
    cmpwi r0, 0x0
    beq @L_80101550
    lwz r0, 0x40(r30)
    cmplw r0, r26
    bne @L_80101550
    b @L_801015E0
@L_80101550:
    lwz r0, 0x8c(r30)
    addi r30, r30, 0x48
    cmpwi r0, 0x0
    beq @L_80101570
    lwz r0, 0x40(r30)
    cmplw r0, r26
    bne @L_80101570
    b @L_801015E0
@L_80101570:
    lwz r0, 0x8c(r30)
    addi r30, r30, 0x48
    cmpwi r0, 0x0
    beq @L_80101590
    lwz r0, 0x40(r30)
    cmplw r0, r26
    bne @L_80101590
    b @L_801015E0
@L_80101590:
    lwz r0, 0x8c(r30)
    addi r30, r30, 0x48
    cmpwi r0, 0x0
    beq @L_801015B0
    lwz r0, 0x40(r30)
    cmplw r0, r26
    bne @L_801015B0
    b @L_801015E0
@L_801015B0:
    lwz r0, 0x8c(r30)
    addi r30, r30, 0x48
    cmpwi r0, 0x0
    beq @L_801015D0
    lwz r0, 0x40(r30)
    cmplw r0, r26
    bne @L_801015D0
    b @L_801015E0
@L_801015D0:
    addi r3, r3, 0x7
    addi r30, r30, 0x48
    bdnz @L_801014D4
    li r30, 0x0
@L_801015E0:
    cmplwi r30, 0x0
    bne @L_8010173C
    lis r3, -32752
    mr r5, r29
    addi r7, r3, 6416
    mr r6, r28
    li r3, 0x60
    li r4, 0x20
    bl fn_800F9418
    mr. r30, r3
    bne @L_80101620
    addi r3, r31, 0x540
    li r4, 0x44
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801018FC
@L_80101620:
    mr r4, r26
    mr r5, r27
    bl fn_80191F64
    lis r3, -32704
    li r0, 0x20
    addi r6, r3, 9496
    li r5, 0x0
    mtctr r0
@L_80101640:
    lwz r4, 0x44(r6)
    cmpwi r4, 0x0
    beq @L_80101668
    lwz r3, 0x40(r6)
    lwz r0, 0x40(r30)
    cmplw r3, r0
    bne @L_80101668
    addi r0, r4, 0x1
    stw r0, 0x44(r6)
    b @L_8010188C
@L_80101668:
    lwz r4, 0x8c(r6)
    addi r6, r6, 0x48
    cmpwi r4, 0x0
    beq @L_80101694
    lwz r3, 0x40(r6)
    lwz r0, 0x40(r30)
    cmplw r3, r0
    bne @L_80101694
    addi r0, r4, 0x1
    stw r0, 0x44(r6)
    b @L_8010188C
@L_80101694:
    lwz r4, 0x8c(r6)
    addi r6, r6, 0x48
    cmpwi r4, 0x0
    beq @L_801016C0
    lwz r3, 0x40(r6)
    lwz r0, 0x40(r30)
    cmplw r3, r0
    bne @L_801016C0
    addi r0, r4, 0x1
    stw r0, 0x44(r6)
    b @L_8010188C
@L_801016C0:
    lwz r4, 0x8c(r6)
    addi r6, r6, 0x48
    cmpwi r4, 0x0
    beq @L_801016EC
    lwz r3, 0x40(r6)
    lwz r0, 0x40(r30)
    cmplw r3, r0
    bne @L_801016EC
    addi r0, r4, 0x1
    stw r0, 0x44(r6)
    b @L_8010188C
@L_801016EC:
    addi r5, r5, 0x3
    addi r6, r6, 0x48
    bdnz @L_80101640
    lis r3, -32704
    li r0, 0x80
    addi r29, r3, 9496
    mtctr r0
@L_80101708:
    lwz r0, 0x44(r29)
    cmpwi r0, 0x0
    bne @L_80101730
    mr r3, r29
    mr r4, r30
    li r5, 0x44
    bl memcpy
    li r0, 0x1
    stw r0, 0x44(r29)
    b @L_8010188C
@L_80101730:
    addi r29, r29, 0x48
    bdnz @L_80101708
    b @L_8010188C
@L_8010173C:
    lis r3, -32752
    mr r5, r29
    addi r7, r3, 6416
    mr r6, r28
    li r3, 0x60
    li r4, 0x20
    bl fn_800F9418
    mr. r28, r3
    bne @L_80101774
    addi r3, r31, 0x540
    li r4, 0x44
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801018FC
@L_80101774:
    mr r4, r30
    li r5, 0x44
    bl memcpy
    lis r3, -32704
    li r0, 0x20
    addi r6, r3, 9496
    li r5, 0x0
    mtctr r0
@L_80101794:
    lwz r4, 0x44(r6)
    cmpwi r4, 0x0
    beq @L_801017BC
    lwz r3, 0x40(r6)
    lwz r0, 0x40(r28)
    cmplw r3, r0
    bne @L_801017BC
    addi r0, r4, 0x1
    stw r0, 0x44(r6)
    b @L_8010188C
@L_801017BC:
    lwz r4, 0x8c(r6)
    addi r6, r6, 0x48
    cmpwi r4, 0x0
    beq @L_801017E8
    lwz r3, 0x40(r6)
    lwz r0, 0x40(r28)
    cmplw r3, r0
    bne @L_801017E8
    addi r0, r4, 0x1
    stw r0, 0x44(r6)
    b @L_8010188C
@L_801017E8:
    lwz r4, 0x8c(r6)
    addi r6, r6, 0x48
    cmpwi r4, 0x0
    beq @L_80101814
    lwz r3, 0x40(r6)
    lwz r0, 0x40(r28)
    cmplw r3, r0
    bne @L_80101814
    addi r0, r4, 0x1
    stw r0, 0x44(r6)
    b @L_8010188C
@L_80101814:
    lwz r4, 0x8c(r6)
    addi r6, r6, 0x48
    cmpwi r4, 0x0
    beq @L_80101840
    lwz r3, 0x40(r6)
    lwz r0, 0x40(r28)
    cmplw r3, r0
    bne @L_80101840
    addi r0, r4, 0x1
    stw r0, 0x44(r6)
    b @L_8010188C
@L_80101840:
    addi r5, r5, 0x3
    addi r6, r6, 0x48
    bdnz @L_80101794
    lis r3, -32704
    li r0, 0x80
    addi r29, r3, 9496
    mtctr r0
@L_8010185C:
    lwz r0, 0x44(r29)
    cmpwi r0, 0x0
    bne @L_80101884
    mr r3, r29
    mr r4, r28
    li r5, 0x44
    bl memcpy
    li r0, 0x1
    stw r0, 0x44(r29)
    b @L_8010188C
@L_80101884:
    addi r29, r29, 0x48
    bdnz @L_8010185C
@L_8010188C:
    mr r3, r30
    addi r4, r31, 0x2c8
    li r28, 0x0
    li r29, 0x0
    bl fn_80191ECC
    lwz r4, 0x0(r3)
    cmplwi r4, 0x0
    beq @L_801018C4
    b @L_801018B8
@L_801018B0:
    addi r4, r4, 0x4
    addi r28, r28, 0x1
@L_801018B8:
    lwz r0, 0x0(r4)
    cmplwi r0, 0x0
    bne @L_801018B0
@L_801018C4:
    lwz r3, 0x8(r3)
    cmplwi r3, 0x0
    beq @L_801018E8
    b @L_801018DC
@L_801018D4:
    addi r3, r3, 0x4
    addi r29, r29, 0x1
@L_801018DC:
    lwz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_801018D4
@L_801018E8:
    mr r4, r28
    mr r5, r29
    addi r3, r31, 0x574
    crxor 6, 6, 6
    bl fn_800DD970
@L_801018FC:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
