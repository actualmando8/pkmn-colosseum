    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stmw r17, 0x24(r1)
    mr r29, r3
    mr r18, r5
    mulli r21, r4, 0x4c
    lis r3, lbl_80266918@ha
    li r19, 0x0
    addi r0, r3, lbl_80266918@l
    add r3, r0, r21
    li r23, 0x8
    lwz r0, 0x4(r3)
    li r25, 0x0
    cmpwi r0, 0x0
    blt @L_80015A1C
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0xe
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_80015A34
    @L_80015A1C:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0xe
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_80015A34:
    li r27, 0x0
    mr r22, r3
    mr r20, r27
    b @L_80015A64
    @L_80015A44:
    mr r3, r22
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80015A5C
    addi r27, r27, 0x1
    @L_80015A5C:
    addi r20, r20, 0x1
    addi r22, r22, 0x4
    @L_80015A64:
    lhz r0, 0xe(r1)
    cmpw r20, r0
    blt @L_80015A44
    lfs f0, lbl_8047B748(r2)
    lfs f1, lbl_8047A2D0(r13)
    lbz r31, 0x0(r18)
    fcmpu cr0, f0, f1
    extsb r31, r31
    beq @L_80015AB8
    lwz r0, lbl_8047A2C8(r13)
    cmpwi r0, 0x0
    beq @L_80015AB8
    fcmpo cr0, f1, f0
    bge @L_80015AA8
    subi r31, r31, 0x1
    li r19, -0x1
    b @L_80015AAC
    @L_80015AA8:
    li r23, 0x9
    @L_80015AAC:
    fctiwz f0, f1
    stfd f0, 0x10(r1)
    lwz r25, 0x14(r1)
    @L_80015AB8:
    lis r3, lbl_80266918@ha
    mr r28, r19
    addi r0, r3, lbl_80266918@l
    mr r22, r29
    add r3, r0, r21
    mulli r26, r19, 0x1f
    addi r30, r3, 0x4
    b @L_80015DF4
    @L_80015AD8:
    cmpwi r31, 0x0
    blt @L_80015DE8
    lwz r0, 0x0(r30)
    subf r24, r25, r26
    cmpwi r0, 0x0
    blt @L_80015B10
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0xc
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_80015B28
    @L_80015B10:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0xc
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_80015B28:
    mr r20, r3
    li r18, -0x1
    li r19, 0x0
    b @L_80015B6C
    @L_80015B38:
    mr r3, r20
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80015B64
    addi r18, r18, 0x1
    cmpw r18, r31
    blt @L_80015B64
    mr r3, r20
    bl fn_80143C68
    b @L_80015B7C
    @L_80015B64:
    addi r19, r19, 0x1
    addi r20, r20, 0x4
    @L_80015B6C:
    lhz r0, 0xc(r1)
    cmpw r19, r0
    blt @L_80015B38
    li r3, 0x0
    @L_80015B7C:
    bl fn_801440A0
    mr r17, r3
    bl fn_80143F24
    clrlwi r20, r3, 24
    mr r3, r17
    addi r19, r20, 0x1
    bl fn_80143E88
    mr r21, r3
    clrlwi r0, r21, 24
    cmplwi r0, 0xff
    addi r0, r21, 0x1
    clrlwi r18, r0, 24
    beq @L_80015BB8
    li r17, 0x2b00
    b @L_80015BBC
    @L_80015BB8:
    li r17, 0x2aff
    @L_80015BBC:
    mr r3, r29
    mr r4, r24
    mr r6, r17
    li r5, -0x1
    bl fn_800FB680
    mr r3, r17
    bl fn_800FA444
    clrlwi r0, r21, 24
    srwi r3, r3, 16
    cmplwi r0, 0xff
    add r21, r29, r3
    beq @L_80015C20
    mr r4, r18
    li r3, 0x34
    bl fn_80132A38
    mr r3, r21
    mr r4, r24
    li r5, -0x1
    li r6, 0xca
    bl fn_800FB680
    li r3, 0xca
    bl fn_800FA444
    srwi r0, r3, 16
    add r21, r21, r0
    b @L_80015C88
    @L_80015C20:
    cmpwi r19, 0xa
    bge @L_80015C58
    li r3, 0x34
    li r4, 0x0
    bl fn_80132A38
    mr r3, r21
    mr r4, r24
    li r5, -0x1
    li r6, 0xca
    bl fn_800FB680
    li r3, 0xca
    bl fn_800FA444
    srwi r0, r3, 16
    add r21, r21, r0
    @L_80015C58:
    mr r4, r19
    li r3, 0x34
    bl fn_80132A38
    mr r3, r21
    mr r4, r24
    li r5, -0x1
    li r6, 0xca
    bl fn_800FB680
    li r3, 0xca
    bl fn_800FA444
    srwi r0, r3, 16
    add r21, r21, r0
    @L_80015C88:
    mr r3, r20
    bl fn_80143EF0
    clrlwi r3, r3, 16
    bl fn_8011CA34
    bl fn_8011CA1C
    mr r6, r3
    mr r4, r24
    addi r3, r21, 0x9
    li r5, -0x1
    bl fn_800FB680
    lwz r0, 0x0(r30)
    addi r6, r1, 0xa
    lwz r3, lbl_8047A2F8(r13)
    li r5, 0x0
    clrlwi r4, r0, 24
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    lhz r4, 0xa(r1)
    li r3, 0x34
    bl fn_80132A38
    li r3, 0xca
    bl fn_800FA444
    srwi r0, r3, 16
    li r3, 0x12e
    extsh r17, r0
    bl fn_800FA444
    srwi r3, r3, 16
    addi r0, r29, 0x11a
    extsh r3, r3
    mr r4, r24
    add r17, r17, r3
    li r5, -0x1
    extsh r3, r17
    li r6, 0x12e
    subf r3, r3, r0
    bl fn_800FB680
    lwz r0, 0x0(r30)
    cmpwi r0, 0x0
    blt @L_80015D48
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_80015D60
    @L_80015D48:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_80015D60:
    mr r20, r3
    li r18, -0x1
    li r19, 0x0
    b @L_80015DA4
    @L_80015D70:
    mr r3, r20
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80015D9C
    addi r18, r18, 0x1
    cmpw r18, r31
    blt @L_80015D9C
    mr r3, r20
    bl fn_80143C50
    b @L_80015DB4
    @L_80015D9C:
    addi r19, r19, 0x1
    addi r20, r20, 0x4
    @L_80015DA4:
    lhz r0, 0x8(r1)
    cmpw r19, r0
    blt @L_80015D70
    li r3, 0x0
    @L_80015DB4:
    clrlwi r4, r3, 16
    li r3, 0x34
    bl fn_80132A38
    li r3, 0xca
    bl fn_800FA444
    srwi r3, r3, 16
    addi r0, r29, 0x11a
    extsh r3, r3
    mr r4, r24
    subf r3, r3, r0
    li r5, -0x1
    li r6, 0xca
    bl fn_800FB680
    @L_80015DE8:
    addi r26, r26, 0x1f
    addi r28, r28, 0x1
    addi r31, r31, 0x1
    @L_80015DF4:
    cmpw r28, r23
    bge @L_80015E04
    cmpw r31, r27
    blt @L_80015AD8
    @L_80015E04:
    cmpwi r28, 0x8
    bge @L_80015E24
    mulli r0, r28, 0x1f
    mr r3, r22
    li r5, -0x1
    li r6, 0x2afe
    subf r4, r25, r0
    bl fn_800FB680
    @L_80015E24:
    li r3, 0x0
    lmw r17, 0x24(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
