    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r22, 0x8(r1)
    mr. r25, r3
    mr r24, r4
    mr r23, r5
    mr r26, r6
    mr r22, r7
    bne @L_80142398
    li r3, 0x0
    b @L_801425D4
    @L_80142398:
    mr r4, r23
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_801423BC
    li r0, 0x0
    b @L_801423D8
    @L_801423BC:
    lwz r0, lbl_80478BD8(r13)
    clrlwi r3, r23, 16
    cmplw r3, r0
    blt @L_801423D4
    li r0, 0x0
    b @L_801423D8
    @L_801423D4:
    li r0, 0x1
    @L_801423D8:
    clrlwi. r0, r0, 24
    bne @L_801423E8
    li r3, 0x0
    b @L_801425D4
    @L_801423E8:
    clrlwi r30, r23, 16
    clrlwi r29, r22, 16
    clrlwi r31, r26, 24
    clrlwi r24, r24, 16
    li r27, 0x0
    b @L_801425C4
    @L_80142400:
    cmplwi r31, 0x2
    bne @L_801424A4
    clrlslwi r0, r27, 16, 2
    clrlwi r23, r27, 16
    add. r3, r25, r0
    bne @L_80142420
    li r0, 0x0
    b @L_80142490
    @L_80142420:
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r28, r3, 16
    bne @L_80142440
    li r0, 0x0
    b @L_80142490
    @L_80142440:
    mr r4, r28
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80142464
    li r0, 0x0
    b @L_8014247C
    @L_80142464:
    lwz r0, lbl_80478BD8(r13)
    cmplw r28, r0
    blt @L_80142478
    li r0, 0x0
    b @L_8014247C
    @L_80142478:
    li r0, 0x1
    @L_8014247C:
    clrlwi. r0, r0, 24
    bne @L_8014248C
    li r0, 0x0
    b @L_80142490
    @L_8014248C:
    li r0, 0x1
    @L_80142490:
    clrlwi. r0, r0, 24
    bne @L_801425C0
    slwi r0, r23, 2
    add r3, r25, r0
    b @L_801425D4
    @L_801424A4:
    clrlslwi r0, r27, 16, 2
    clrlwi r28, r27, 16
    add. r22, r25, r0
    bne @L_801424BC
    li r0, 0x0
    b @L_80142578
    @L_801424BC:
    cmplwi r22, 0x0
    bne @L_801424CC
    li r0, 0x0
    b @L_80142540
    @L_801424CC:
    mr r3, r22
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r23, r3, 16
    bne @L_801424F0
    li r0, 0x0
    b @L_80142540
    @L_801424F0:
    mr r4, r23
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80142514
    li r0, 0x0
    b @L_8014252C
    @L_80142514:
    lwz r0, lbl_80478BD8(r13)
    cmplw r23, r0
    blt @L_80142528
    li r0, 0x0
    b @L_8014252C
    @L_80142528:
    li r0, 0x1
    @L_8014252C:
    clrlwi. r0, r0, 24
    bne @L_8014253C
    li r0, 0x0
    b @L_80142540
    @L_8014253C:
    li r0, 0x1
    @L_80142540:
    clrlwi. r0, r0, 24
    bne @L_80142550
    li r0, 0x0
    b @L_80142578
    @L_80142550:
    mr r3, r22
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    cmpw r30, r3
    bne @L_80142574
    li r0, 0x1
    b @L_80142578
    @L_80142574:
    li r0, 0x0
    @L_80142578:
    clrlwi. r0, r0, 24
    beq @L_801425C0
    clrlwi r0, r26, 24
    cmplwi r0, 0x1
    bne @L_801425B4
    mr r3, r22
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_80142CF4
    cmpw r3, r29
    bge @L_801425C0
    slwi r0, r28, 2
    add r3, r25, r0
    b @L_801425D4
    @L_801425B4:
    slwi r0, r28, 2
    add r3, r25, r0
    b @L_801425D4
    @L_801425C0:
    addi r27, r27, 0x1
    @L_801425C4:
    clrlwi r0, r27, 16
    cmplw r0, r24
    blt @L_80142400
    li r3, 0x0
    @L_801425D4:
    lmw r22, 0x8(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
