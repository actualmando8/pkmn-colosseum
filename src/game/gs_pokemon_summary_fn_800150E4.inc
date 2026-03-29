    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r19, 0x1c(r1)
    mr r27, r3
    mr r25, r5
    mulli r23, r4, 0x4c
    lis r3, lbl_80266918@ha
    li r24, 0x0
    addi r0, r3, lbl_80266918@l
    add r3, r0, r23
    li r22, 0x8
    lwz r0, 0x4(r3)
    li r28, 0x0
    cmpwi r0, 0x0
    blt @L_80015144
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0xa
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_8001515C
    @L_80015144:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0xa
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_8001515C:
    li r26, 0x0
    mr r30, r3
    mr r29, r26
    b @L_8001518C
    @L_8001516C:
    mr r3, r30
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80015184
    addi r26, r26, 0x1
    @L_80015184:
    addi r29, r29, 0x1
    addi r30, r30, 0x4
    @L_8001518C:
    lhz r0, 0xa(r1)
    cmpw r29, r0
    blt @L_8001516C
    lfs f0, lbl_8047B748(r2)
    lfs f1, lbl_8047A2D0(r13)
    lbz r30, 0x0(r25)
    fcmpu cr0, f0, f1
    extsb r30, r30
    beq @L_800151E0
    lwz r0, lbl_8047A2C8(r13)
    cmpwi r0, 0x0
    beq @L_800151E0
    fcmpo cr0, f1, f0
    bge @L_800151D0
    subi r30, r30, 0x1
    li r24, -0x1
    b @L_800151D4
    @L_800151D0:
    li r22, 0x9
    @L_800151D4:
    fctiwz f0, f1
    stfd f0, 0x10(r1)
    lwz r28, 0x14(r1)
    @L_800151E0:
    lis r3, lbl_80266918@ha
    mr r31, r24
    addi r0, r3, lbl_80266918@l
    add r3, r0, r23
    mulli r25, r24, 0x1f
    addi r24, r3, 0x4
    b @L_8001532C
    @L_800151FC:
    cmpwi r30, 0x0
    blt @L_80015320
    addi r0, r30, 0x1
    mr r29, r27
    cmpwi r0, 0xa
    subf r19, r28, r25
    bge @L_80015234
    li r3, 0x34
    li r4, 0x0
    bl fn_80132A38
    li r3, 0xca
    bl fn_800FA444
    srwi r0, r3, 16
    add r29, r27, r0
    @L_80015234:
    addi r4, r30, 0x1
    li r3, 0x34
    bl fn_80132A38
    mr r3, r29
    mr r23, r19
    mr r4, r19
    li r5, -0x1
    li r6, 0xca
    bl fn_800FB680
    li r3, 0xca
    bl fn_800FA444
    lwz r4, 0x0(r24)
    srwi r0, r3, 16
    add r29, r29, r0
    cmpwi r4, 0x0
    blt @L_80015294
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r4, 24
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_800152AC
    @L_80015294:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_800152AC:
    mr r19, r3
    li r21, -0x1
    li r20, 0x0
    b @L_800152F0
    @L_800152BC:
    mr r3, r19
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800152E8
    addi r21, r21, 0x1
    cmpw r21, r30
    blt @L_800152E8
    mr r3, r19
    bl fn_80143C68
    b @L_80015300
    @L_800152E8:
    addi r20, r20, 0x1
    addi r19, r19, 0x4
    @L_800152F0:
    lhz r0, 0x8(r1)
    cmpw r20, r0
    blt @L_800152BC
    li r3, 0x0
    @L_80015300:
    clrlwi r3, r3, 16
    bl fn_801440A0
    bl fn_80144088
    mr r6, r3
    mr r4, r23
    addi r3, r29, 0xa
    li r5, -0x1
    bl fn_800FB680
    @L_80015320:
    addi r25, r25, 0x1f
    addi r31, r31, 0x1
    addi r30, r30, 0x1
    @L_8001532C:
    cmpw r31, r22
    bge @L_8001533C
    cmpw r30, r26
    blt @L_800151FC
    @L_8001533C:
    cmpwi r31, 0x8
    bge @L_8001535C
    mulli r0, r31, 0x1f
    mr r3, r27
    li r5, -0x1
    li r6, 0x2afe
    subf r4, r28, r0
    bl fn_800FB680
    @L_8001535C:
    li r3, 0x0
    lmw r19, 0x1c(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
