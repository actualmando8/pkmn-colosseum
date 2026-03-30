    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r20, 0x10(r1)
    mr r26, r3
    mr r27, r4
    mr r28, r5
    mr r29, r6
    mr r30, r7
    li r31, 0x0
    b @L_801B89A0
@L_801B84D0:
    rlwinm r0, r31, 2, 0, 29
    lwzx r0, r26, r0
    cmplw r0, r28
    bne @L_801B899C
    rlwinm r0, r31, 2, 0, 29
    lwzx r0, r27, r0
    cmpw r0, r30
    bge @L_801B89A8
    rlwinm r0, r31, 2, 0, 29
    li r31, 0x0
    stwx r30, r27, r0
    addi r30, r30, 0x1
    b @L_801B8990
@L_801B8504:
    rlwinm r0, r31, 3, 0, 28
    add r3, r28, r0
    lbz r0, 0x34(r3)
    cmplwi r0, 0x1
    bne @L_801B8748
    rlwinm r0, r31, 3, 0, 28
    mr r23, r30
    add r3, r28, r0
    li r25, 0x0
    lwz r24, 0x38(r3)
    b @L_801B8740
@L_801B8530:
    rlwinm r0, r25, 2, 0, 29
    lwzx r0, r26, r0
    cmplw r0, r24
    bne @L_801B873C
    rlwinm r0, r25, 2, 0, 29
    lwzx r0, r27, r0
    cmpw r0, r23
    bge @L_801B8748
    rlwinm r0, r25, 2, 0, 29
    li r25, 0x0
    stwx r23, r27, r0
    addi r23, r23, 0x1
    b @L_801B8730
@L_801B8564:
    rlwinm r0, r25, 3, 0, 28
    add r3, r24, r0
    lbz r0, 0x34(r3)
    cmplwi r0, 0x1
    bne @L_801B8648
    rlwinm r0, r25, 3, 0, 28
    mr r20, r23
    add r3, r24, r0
    li r22, 0x0
    lwz r21, 0x38(r3)
    b @L_801B8640
@L_801B8590:
    rlwinm r0, r22, 2, 0, 29
    lwzx r0, r26, r0
    cmplw r0, r21
    bne @L_801B863C
    rlwinm r0, r22, 2, 0, 29
    lwzx r0, r27, r0
    cmpw r0, r20
    bge @L_801B8648
    rlwinm r0, r22, 2, 0, 29
    li r22, 0x0
    stwx r20, r27, r0
    addi r20, r20, 0x1
    b @L_801B8630
@L_801B85C4:
    rlwinm r0, r22, 3, 0, 28
    add r3, r21, r0
    lbz r0, 0x34(r3)
    cmplwi r0, 0x1
    bne @L_801B85F8
    rlwinm r0, r22, 3, 0, 28
    mr r3, r26
    add r5, r21, r0
    mr r4, r27
    lwz r5, 0x38(r5)
    mr r6, r29
    mr r7, r20
    bl fn_801B84A4
@L_801B85F8:
    rlwinm r0, r22, 3, 0, 28
    add r3, r21, r0
    lbz r0, 0x54(r3)
    cmplwi r0, 0x1
    bne @L_801B862C
    rlwinm r0, r22, 3, 0, 28
    mr r3, r26
    add r5, r21, r0
    mr r4, r27
    lwz r5, 0x58(r5)
    mr r6, r29
    mr r7, r20
    bl fn_801B84A4
@L_801B862C:
    addi r22, r22, 0x1
@L_801B8630:
    cmpwi r22, 0x4
    blt @L_801B85C4
    b @L_801B8648
@L_801B863C:
    addi r22, r22, 0x1
@L_801B8640:
    cmpw r22, r29
    blt @L_801B8590
@L_801B8648:
    rlwinm r0, r25, 3, 0, 28
    add r3, r24, r0
    lbz r0, 0x54(r3)
    cmplwi r0, 0x1
    bne @L_801B872C
    rlwinm r0, r25, 3, 0, 28
    mr r22, r23
    add r3, r24, r0
    li r20, 0x0
    lwz r21, 0x58(r3)
    b @L_801B8724
@L_801B8674:
    rlwinm r0, r20, 2, 0, 29
    lwzx r0, r26, r0
    cmplw r0, r21
    bne @L_801B8720
    rlwinm r0, r20, 2, 0, 29
    lwzx r0, r27, r0
    cmpw r0, r22
    bge @L_801B872C
    rlwinm r0, r20, 2, 0, 29
    li r20, 0x0
    stwx r22, r27, r0
    addi r22, r22, 0x1
    b @L_801B8714
@L_801B86A8:
    rlwinm r0, r20, 3, 0, 28
    add r3, r21, r0
    lbz r0, 0x34(r3)
    cmplwi r0, 0x1
    bne @L_801B86DC
    rlwinm r0, r20, 3, 0, 28
    mr r3, r26
    add r5, r21, r0
    mr r4, r27
    lwz r5, 0x38(r5)
    mr r6, r29
    mr r7, r22
    bl fn_801B84A4
@L_801B86DC:
    rlwinm r0, r20, 3, 0, 28
    add r3, r21, r0
    lbz r0, 0x54(r3)
    cmplwi r0, 0x1
    bne @L_801B8710
    rlwinm r0, r20, 3, 0, 28
    mr r3, r26
    add r5, r21, r0
    mr r4, r27
    lwz r5, 0x58(r5)
    mr r6, r29
    mr r7, r22
    bl fn_801B84A4
@L_801B8710:
    addi r20, r20, 0x1
@L_801B8714:
    cmpwi r20, 0x4
    blt @L_801B86A8
    b @L_801B872C
@L_801B8720:
    addi r20, r20, 0x1
@L_801B8724:
    cmpw r20, r29
    blt @L_801B8674
@L_801B872C:
    addi r25, r25, 0x1
@L_801B8730:
    cmpwi r25, 0x4
    blt @L_801B8564
    b @L_801B8748
@L_801B873C:
    addi r25, r25, 0x1
@L_801B8740:
    cmpw r25, r29
    blt @L_801B8530
@L_801B8748:
    rlwinm r0, r31, 3, 0, 28
    add r3, r28, r0
    lbz r0, 0x54(r3)
    cmplwi r0, 0x1
    bne @L_801B898C
    rlwinm r0, r31, 3, 0, 28
    mr r25, r30
    add r3, r28, r0
    li r23, 0x0
    lwz r24, 0x58(r3)
    b @L_801B8984
@L_801B8774:
    rlwinm r0, r23, 2, 0, 29
    lwzx r0, r26, r0
    cmplw r0, r24
    bne @L_801B8980
    rlwinm r0, r23, 2, 0, 29
    lwzx r0, r27, r0
    cmpw r0, r25
    bge @L_801B898C
    rlwinm r0, r23, 2, 0, 29
    li r23, 0x0
    stwx r25, r27, r0
    addi r25, r25, 0x1
    b @L_801B8974
@L_801B87A8:
    rlwinm r0, r23, 3, 0, 28
    add r3, r24, r0
    lbz r0, 0x34(r3)
    cmplwi r0, 0x1
    bne @L_801B888C
    rlwinm r0, r23, 3, 0, 28
    mr r22, r25
    add r3, r24, r0
    li r20, 0x0
    lwz r21, 0x38(r3)
    b @L_801B8884
@L_801B87D4:
    rlwinm r0, r20, 2, 0, 29
    lwzx r0, r26, r0
    cmplw r0, r21
    bne @L_801B8880
    rlwinm r0, r20, 2, 0, 29
    lwzx r0, r27, r0
    cmpw r0, r22
    bge @L_801B888C
    rlwinm r0, r20, 2, 0, 29
    li r20, 0x0
    stwx r22, r27, r0
    addi r22, r22, 0x1
    b @L_801B8874
@L_801B8808:
    rlwinm r0, r20, 3, 0, 28
    add r3, r21, r0
    lbz r0, 0x34(r3)
    cmplwi r0, 0x1
    bne @L_801B883C
    rlwinm r0, r20, 3, 0, 28
    mr r3, r26
    add r5, r21, r0
    mr r4, r27
    lwz r5, 0x38(r5)
    mr r6, r29
    mr r7, r22
    bl fn_801B84A4
@L_801B883C:
    rlwinm r0, r20, 3, 0, 28
    add r3, r21, r0
    lbz r0, 0x54(r3)
    cmplwi r0, 0x1
    bne @L_801B8870
    rlwinm r0, r20, 3, 0, 28
    mr r3, r26
    add r5, r21, r0
    mr r4, r27
    lwz r5, 0x58(r5)
    mr r6, r29
    mr r7, r22
    bl fn_801B84A4
@L_801B8870:
    addi r20, r20, 0x1
@L_801B8874:
    cmpwi r20, 0x4
    blt @L_801B8808
    b @L_801B888C
@L_801B8880:
    addi r20, r20, 0x1
@L_801B8884:
    cmpw r20, r29
    blt @L_801B87D4
@L_801B888C:
    rlwinm r0, r23, 3, 0, 28
    add r3, r24, r0
    lbz r0, 0x54(r3)
    cmplwi r0, 0x1
    bne @L_801B8970
    rlwinm r0, r23, 3, 0, 28
    mr r22, r25
    add r3, r24, r0
    li r20, 0x0
    lwz r21, 0x58(r3)
    b @L_801B8968
@L_801B88B8:
    rlwinm r0, r20, 2, 0, 29
    lwzx r0, r26, r0
    cmplw r0, r21
    bne @L_801B8964
    rlwinm r0, r20, 2, 0, 29
    lwzx r0, r27, r0
    cmpw r0, r22
    bge @L_801B8970
    rlwinm r0, r20, 2, 0, 29
    li r20, 0x0
    stwx r22, r27, r0
    addi r22, r22, 0x1
    b @L_801B8958
@L_801B88EC:
    rlwinm r0, r20, 3, 0, 28
    add r3, r21, r0
    lbz r0, 0x34(r3)
    cmplwi r0, 0x1
    bne @L_801B8920
    rlwinm r0, r20, 3, 0, 28
    mr r3, r26
    add r5, r21, r0
    mr r4, r27
    lwz r5, 0x38(r5)
    mr r6, r29
    mr r7, r22
    bl fn_801B84A4
@L_801B8920:
    rlwinm r0, r20, 3, 0, 28
    add r3, r21, r0
    lbz r0, 0x54(r3)
    cmplwi r0, 0x1
    bne @L_801B8954
    rlwinm r0, r20, 3, 0, 28
    mr r3, r26
    add r5, r21, r0
    mr r4, r27
    lwz r5, 0x58(r5)
    mr r6, r29
    mr r7, r22
    bl fn_801B84A4
@L_801B8954:
    addi r20, r20, 0x1
@L_801B8958:
    cmpwi r20, 0x4
    blt @L_801B88EC
    b @L_801B8970
@L_801B8964:
    addi r20, r20, 0x1
@L_801B8968:
    cmpw r20, r29
    blt @L_801B88B8
@L_801B8970:
    addi r23, r23, 0x1
@L_801B8974:
    cmpwi r23, 0x4
    blt @L_801B87A8
    b @L_801B898C
@L_801B8980:
    addi r23, r23, 0x1
@L_801B8984:
    cmpw r23, r29
    blt @L_801B8774
@L_801B898C:
    addi r31, r31, 0x1
@L_801B8990:
    cmpwi r31, 0x4
    blt @L_801B8504
    b @L_801B89A8
@L_801B899C:
    addi r31, r31, 0x1
@L_801B89A0:
    cmpw r31, r29
    blt @L_801B84D0
@L_801B89A8:
    lmw r20, 0x10(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
