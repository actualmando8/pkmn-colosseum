    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r19, 0x1c(r1)
    mr r26, r3
    mr r22, r5
    mulli r24, r4, 0x4c
    lis r3, lbl_80266918@ha
    li r27, 0x0
    addi r0, r3, lbl_80266918@l
    add r3, r0, r24
    li r23, 0x8
    lwz r0, 0x4(r3)
    li r25, 0x0
    cmpwi r0, 0x0
    blt @L_80015E9C
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0xe
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_80015EB4
    @L_80015E9C:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0xe
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_80015EB4:
    li r28, 0x0
    mr r30, r3
    mr r29, r28
    b @L_80015EE4
    @L_80015EC4:
    mr r3, r30
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80015EDC
    addi r28, r28, 0x1
    @L_80015EDC:
    addi r29, r29, 0x1
    addi r30, r30, 0x4
    @L_80015EE4:
    lhz r0, 0xe(r1)
    cmpw r29, r0
    blt @L_80015EC4
    lfs f0, lbl_8047B748(r2)
    lfs f1, lbl_8047A2D0(r13)
    lbz r30, 0x0(r22)
    fcmpu cr0, f0, f1
    extsb r30, r30
    beq @L_80015F38
    lwz r0, lbl_8047A2C8(r13)
    cmpwi r0, 0x0
    beq @L_80015F38
    fcmpo cr0, f1, f0
    bge @L_80015F28
    subi r30, r30, 0x1
    li r27, -0x1
    b @L_80015F2C
    @L_80015F28:
    li r23, 0x9
    @L_80015F2C:
    fctiwz f0, f1
    stfd f0, 0x10(r1)
    lwz r25, 0x14(r1)
    @L_80015F38:
    lis r3, lbl_80266918@ha
    mr r29, r27
    addi r0, r3, lbl_80266918@l
    mr r22, r26
    add r3, r0, r24
    mulli r27, r27, 0x1f
    addi r31, r3, 0x4
    b @L_80016168
    @L_80015F58:
    cmpwi r30, 0x0
    blt @L_8001615C
    lwz r0, 0x0(r31)
    subf r24, r25, r27
    cmpwi r0, 0x0
    blt @L_80015F90
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0xc
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_80015FA8
    @L_80015F90:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0xc
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_80015FA8:
    mr r19, r3
    li r21, -0x1
    li r20, 0x0
    b @L_80015FEC
    @L_80015FB8:
    mr r3, r19
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80015FE4
    addi r21, r21, 0x1
    cmpw r21, r30
    blt @L_80015FE4
    mr r3, r19
    bl fn_80143C68
    b @L_80015FFC
    @L_80015FE4:
    addi r20, r20, 0x1
    addi r19, r19, 0x4
    @L_80015FEC:
    lhz r0, 0xc(r1)
    cmpw r20, r0
    blt @L_80015FB8
    li r3, 0x0
    @L_80015FFC:
    clrlwi r3, r3, 16
    bl fn_801440A0
    bl fn_80144088
    mr r6, r3
    cmplwi r6, 0x0
    beq @L_80016024
    mr r3, r26
    mr r4, r24
    li r5, -0x1
    bl fn_800FB680
    @L_80016024:
    lwz r0, 0x0(r31)
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
    extsh r19, r0
    bl fn_800FA444
    srwi r3, r3, 16
    addi r0, r26, 0x11a
    extsh r3, r3
    mr r4, r24
    add r19, r19, r3
    li r5, -0x1
    extsh r3, r19
    li r6, 0x12e
    subf r3, r3, r0
    bl fn_800FB680
    lwz r0, 0x0(r31)
    cmpwi r0, 0x0
    blt @L_800160BC
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_800160D4
    @L_800160BC:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_800160D4:
    mr r21, r3
    li r19, -0x1
    li r20, 0x0
    b @L_80016118
    @L_800160E4:
    mr r3, r21
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80016110
    addi r19, r19, 0x1
    cmpw r19, r30
    blt @L_80016110
    mr r3, r21
    bl fn_80143C50
    b @L_80016128
    @L_80016110:
    addi r20, r20, 0x1
    addi r21, r21, 0x4
    @L_80016118:
    lhz r0, 0x8(r1)
    cmpw r20, r0
    blt @L_800160E4
    li r3, 0x0
    @L_80016128:
    clrlwi r4, r3, 16
    li r3, 0x34
    bl fn_80132A38
    li r3, 0xca
    bl fn_800FA444
    srwi r3, r3, 16
    addi r0, r26, 0x11a
    extsh r3, r3
    mr r4, r24
    subf r3, r3, r0
    li r5, -0x1
    li r6, 0xca
    bl fn_800FB680
    @L_8001615C:
    addi r27, r27, 0x1f
    addi r29, r29, 0x1
    addi r30, r30, 0x1
    @L_80016168:
    cmpw r29, r23
    bge @L_80016178
    cmpw r30, r28
    blt @L_80015F58
    @L_80016178:
    cmpw r29, r23
    bge @L_80016198
    mulli r0, r29, 0x1f
    mr r3, r22
    li r5, -0x1
    li r6, 0x2afe
    subf r4, r25, r0
    bl fn_800FB680
    @L_80016198:
    li r3, 0x0
    lmw r19, 0x1c(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
