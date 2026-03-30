    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r22, 0x8(r1)
    mr r27, r3
    mr r28, r4
    cmplwi r28, 0x0
    lis r5, lbl_80272088@ha
    addi r25, r5, lbl_80272088@l
    li r30, 0x0
    li r29, 0x0
    beq @L_80113764
    bl fn_800F9318
    addi r4, r25, 0x28
    bl fn_80191ECC
    mr r31, r3
    cmplwi r31, 0x0
    beq @L_80113764
    lwz r0, 0x0(r31)
    cmplwi r0, 0x0
    beq @L_80113698
    mr r3, r28
    bl fn_80115274
    mr r26, r3
    li r24, 0x0
    b @L_80113688
@L_8011354C:
    or r23, r26, r30
    mr r3, r23
    bl fn_80113D58
    mr r22, r3
    cmplwi r22, 0x0
    bne @L_8011358C
    lis r3, lbl_8035B878@ha
    mr r5, r30
    addi r4, r3, lbl_8035B878@l
    addi r3, r25, 0x64
    crxor 6, 6, 6
    bl fn_800DD970
    addi r3, r25, 0x88
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_80113680
@L_8011358C:
    mr r3, r27
    mr r4, r23
    bl fn_800F9210
    lis r4, -32751
    mr r3, r22
    addi r6, r4, 15632
    mr r5, r23
    mr r4, r27
    bl fn_800F9378
    mr r3, r22
    li r4, 0x0
    bl fn_800E4014
    bl fn_800057A0
    cmpwi r3, 0x1
    bne @L_80113680
    bl fn_800FF56C
    cmpwi r3, 0x75
    beq @L_80113654
    bge @L_80113624
    cmpwi r3, 0x15
    beq @L_80113654
    bge @L_80113604
    cmpwi r3, 0xb
    bge @L_801135F8
    cmpwi r3, 0x3
    beq @L_80113654
    b @L_80113680
@L_801135F8:
    cmpwi r3, 0xf
    bge @L_80113680
    b @L_80113654
@L_80113604:
    cmpwi r3, 0x72
    beq @L_80113654
    bge @L_80113680
    cmpwi r3, 0x27
    bge @L_80113680
    cmpwi r3, 0x24
    bge @L_80113654
    b @L_80113680
@L_80113624:
    cmpwi r3, 0xd0
    beq @L_80113654
    bge @L_80113648
    cmpwi r3, 0xca
    beq @L_80113654
    bge @L_80113680
    cmpwi r3, 0x7b
    beq @L_80113654
    b @L_80113680
@L_80113648:
    cmpwi r3, 0xde
    beq @L_80113654
    b @L_80113680
@L_80113654:
    mr r3, r22
    bl fn_800EC1BC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80113680
    mr r3, r22
    li r4, 0x0
    bl fn_800EC188
    lfs f1, -26380(r2)
    mr r3, r22
    bl fn_800EC308
@L_80113680:
    addi r24, r24, 0x4
    addi r30, r30, 0x1
@L_80113688:
    lwz r3, 0x0(r31)
    lwzx r0, r3, r24
    cmplwi r0, 0x0
    bne @L_8011354C
@L_80113698:
    lwz r0, 0x8(r31)
    cmplwi r0, 0x0
    beq @L_80113724
    mr r3, r28
    bl fn_80115268
    mr r30, r3
    li r24, 0x0
    b @L_80113714
@L_801136B8:
    mr r3, r27
    or r4, r30, r29
    bl fn_800F9318
    mr r22, r3
    cmplwi r22, 0x0
    beq @L_8011370C
    li r4, 0x1
    bl fn_800DCC34
    mr r3, r22
    bl fn_800DCC2C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8011370C
    mr r3, r22
    li r4, 0x0
    bl fn_800DCB78
    lfs f1, -26376(r2)
    mr r3, r22
    bl fn_800DCAF0
    mr r3, r22
    bl fn_800DCAB0
@L_8011370C:
    addi r24, r24, 0x4
    addi r29, r29, 0x1
@L_80113714:
    lwz r3, 0x8(r31)
    lwzx r0, r3, r24
    cmplwi r0, 0x0
    bne @L_801136B8
@L_80113724:
    mr r3, r28
    bl fn_8011525C
    mr r0, r3
    mr r3, r27
    mr r4, r0
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80113748
    bl fn_80179404
@L_80113748:
    mr r3, r28
    bl fn_80115250
    mr r0, r3
    mr r3, r27
    mr r4, r0
    bl fn_800F9318
    bl fn_800D2B90
@L_80113764:
    lmw r22, 0x8(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

