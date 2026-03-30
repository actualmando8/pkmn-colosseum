    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr r25, r3
    li r26, 0x0
    b @L_8007775C
@L_80077700:
    mr r3, r25
    mr r4, r26
    bl fn_8012AC08
    li r27, 0x0
    mr r24, r3
@L_80077714:
    mr r3, r24
    mr r4, r27
    bl fn_80076398
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80077734
    li r0, 0x0
    b @L_80077744
@L_80077734:
    addi r27, r27, 0x1
    cmpwi r27, 0x6
    blt @L_80077714
    li r0, 0x1
@L_80077744:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80077758
    li r0, 0x0
    b @L_8007776C
@L_80077758:
    addi r26, r26, 0x1
@L_8007775C:
    rlwinm r0, r26, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_80077700
    li r0, 0x1
@L_8007776C:
    rlwinm r3, r0, 0, 24, 31
    neg r0, r3
    or r0, r0, r3
    rlwinm r0, r0, 1, 31, 31
    cmpwi r0, 0x0
    beq @L_80077A44
    bl fn_8006B420
    li r29, 0x0
    mr r27, r3
    mr r24, r29
@L_80077794:
    mr r3, r25
    mr r4, r27
    mr r5, r24
    bl fn_80076F2C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800777B8
    li r0, 0x0
    b @L_800777C8
@L_800777B8:
    addi r24, r24, 0x1
    cmpwi r24, 0x4
    blt @L_80077794
    li r0, 0x1
@L_800777C8:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800777DC
    li r0, 0x0
    b @L_80077A34
@L_800777DC:
    li r28, 0x0
@L_800777E0:
    mr r3, r25
    rlwinm r4, r28, 0, 16, 31
    bl fn_8012AC08
    mr r30, r3
    cmplwi r30, 0x0
    beq @L_80077A1C
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80077A1C
    cntlzw r0, r30
    li r26, 0x0
    rlwinm r31, r0, 27, 5, 31
@L_80077814:
    cmpwi r31, 0x0
    li r24, 0x0
    bne @L_8007783C
    mr r3, r30
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_80077840
@L_8007783C:
    li r24, 0x1
@L_80077840:
    cmpwi r24, 0x0
    beq @L_80077850
    li r0, 0x1
    b @L_800779E0
@L_80077850:
    cmpwi r26, 0x1
    beq @L_8007789C
    bge @L_80077868
    cmpwi r26, 0x0
    bge @L_80077874
    b @L_800779C4
@L_80077868:
    cmpwi r26, 0x3
    bge @L_800779C4
    b @L_800778C4
@L_80077874:
    mr r3, r30
    bl fn_8011F4A8
    rlwinm r5, r3, 0, 24, 31
    lha r0, 0x0(r27)
    srawi r4, r5, 31
    rlwinm r3, r0, 1, 31, 31
    subfc r0, r0, r5
    adde r0, r4, r3
    rlwinm r0, r0, 0, 24, 31
    b @L_800779E0
@L_8007789C:
    mr r3, r30
    bl fn_8011F4A8
    lha r0, 0x2(r27)
    rlwinm r5, r3, 0, 24, 31
    rlwinm r3, r5, 1, 31, 31
    srawi r4, r0, 31
    subfc r0, r5, r0
    adde r0, r4, r3
    rlwinm r0, r0, 0, 24, 31
    b @L_800779E0
@L_800778C4:
    mr r3, r30
    bl fn_8011F1A0
    mr r23, r3
    bl fn_8006B420
    rlwinm r0, r23, 0, 16, 31
    mr r24, r3
    cmpwi r0, 0xaf
    beq @L_800778FC
    bge @L_80077904
    cmpwi r0, 0x0
    beq @L_800778F4
    b @L_80077904
@L_800778F4:
    li r3, 0x1
    b @L_8007790C
@L_800778FC:
    li r3, 0x0
    b @L_8007790C
@L_80077904:
    mr r3, r23
    bl fn_80142984
@L_8007790C:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80077920
    li r0, 0x0
    b @L_800779E0
@L_80077920:
    lwz r0, 0x8(r24)
    cmpwi r0, 0x1
    beq @L_80077950
    bge @L_8007793C
    cmpwi r0, 0x0
    bge @L_80077948
    b @L_800779BC
@L_8007793C:
    cmpwi r0, 0x3
    bge @L_800779BC
    b @L_80077964
@L_80077948:
    li r0, 0x1
    b @L_800779E0
@L_80077950:
    rlwinm r0, r23, 0, 16, 31
    cntlzw r0, r0
    rlwinm r0, r0, 27, 5, 31
    rlwinm r0, r0, 0, 24, 31
    b @L_800779E0
@L_80077964:
    lis r3, -32721
    lwz r0, -32504(r13)
    addi r5, r3, -7080
    li r4, 0x0
    rlwinm r3, r23, 0, 16, 31
    mtctr r0
    cmplwi r0, 0x0
    ble @L_800779B4
@L_80077984:
    lhz r0, 0x0(r5)
    cmplw r3, r0
    bne @L_800779A8
    addi r0, r4, 0x18
    lbzx r0, r24, r0
    cntlzw r0, r0
    rlwinm r0, r0, 27, 5, 31
    rlwinm r0, r0, 0, 24, 31
    b @L_800779E0
@L_800779A8:
    addi r5, r5, 0x2
    addi r4, r4, 0x1
    bdnz @L_80077984
@L_800779B4:
    li r0, 0x1
    b @L_800779E0
@L_800779BC:
    li r0, 0x0
    b @L_800779E0
@L_800779C4:
    lis r3, -32729
    lis r5, -32729
    addi r3, r3, -30136
    li r4, 0xfb
    addi r5, r5, -30120
    bl fn_80196E10
    li r0, 0x0
@L_800779E0:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800779F4
    li r0, 0x0
    b @L_80077A04
@L_800779F4:
    addi r26, r26, 0x1
    cmpwi r26, 0x3
    blt @L_80077814
    li r0, 0x1
@L_80077A04:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80077A18
    li r0, 0x0
    b @L_80077A34
@L_80077A18:
    addi r29, r29, 0x1
@L_80077A1C:
    addi r28, r28, 0x1
    cmpwi r28, 0x6
    blt @L_800777E0
    neg r0, r29
    andc r0, r0, r29
    rlwinm r0, r0, 1, 31, 31
@L_80077A34:
    rlwinm r3, r0, 0, 24, 31
    neg r0, r3
    or r0, r0, r3
    rlwinm r0, r0, 1, 31, 31
@L_80077A44:
    rlwinm r3, r0, 0, 24, 31
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
