    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r19, 0x1c(r1)
    mr r28, r4
    li r21, 0x0
    li r30, 0x8
    li r29, 0x0
    li r27, 0x0
    li r3, 0x0
    bl fn_801347D0
    clrlwi r20, r3, 16
    li r23, 0x0
    b @L_800396A0
    @L_8003967C:
    extsh r4, r23
    li r3, 0x0
    bl fn_80134768
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8003969C
    addi r27, r27, 0x1
    @L_8003969C:
    addi r23, r23, 0x1
    @L_800396A0:
    cmpw r23, r20
    blt @L_8003967C
    lwz r31, lbl_8047A4A8(r13)
    li r3, 0x34
    li r4, 0x3e7
    bl fn_80132A38
    li r3, 0xca
    bl fn_800FA444
    srwi r0, r3, 16
    li r3, 0x12e
    extsh r19, r0
    bl fn_800FA444
    lfs f0, lbl_8047BAB0(r2)
    srwi r0, r3, 16
    lfs f1, lbl_8047A4C0(r13)
    extsh r0, r0
    add r3, r19, r0
    fcmpu cr0, f0, f1
    beq @L_8003971C
    lwz r0, lbl_8047A4BC(r13)
    cmpwi r0, 0x0
    beq @L_8003971C
    fcmpo cr0, f1, f0
    bge @L_8003970C
    subi r31, r31, 0x1
    li r21, -0x1
    b @L_80039710
    @L_8003970C:
    li r30, 0x9
    @L_80039710:
    fctiwz f0, f1
    stfd f0, 0x8(r1)
    lwz r29, 0xc(r1)
    @L_8003971C:
    mulli r26, r21, 0x1f
    mr r23, r21
    subfic r25, r3, 0x11a
    b @L_8003988C
    @L_8003972C:
    cmpwi r31, 0x0
    blt @L_80039880
    subf r24, r29, r26
    li r3, 0x0
    bl fn_801347D0
    clrlwi r19, r3, 16
    li r22, -0x1
    li r21, 0x0
    b @L_80039790
    @L_80039750:
    extsh r4, r21
    li r3, 0x0
    bl fn_80134768
    mr r20, r3
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8003978C
    addi r22, r22, 0x1
    cmpw r22, r31
    blt @L_8003978C
    mr r3, r20
    bl fn_80143C68
    mr r22, r3
    b @L_8003979C
    @L_8003978C:
    addi r21, r21, 0x1
    @L_80039790:
    cmpw r21, r19
    blt @L_80039750
    li r22, 0x0
    @L_8003979C:
    clrlwi r3, r22, 16
    bl fn_801440A0
    bl fn_80144088
    cmplwi r3, 0x0
    beq @L_800397C4
    mr r4, r24
    mr r5, r28
    mr r6, r3
    li r3, 0x0
    bl fn_800FB680
    @L_800397C4:
    mr r3, r22
    bl fn_801440A0
    bl fn_80144014
    clrlwi r0, r3, 24
    cmplwi r0, 0x5
    beq @L_80039880
    mr r3, r25
    mr r4, r24
    mr r5, r28
    li r6, 0x12e
    bl fn_800FB680
    li r3, 0x0
    bl fn_801347D0
    clrlwi r19, r3, 16
    li r20, -0x1
    li r21, 0x0
    b @L_80039844
    @L_80039808:
    extsh r4, r21
    li r3, 0x0
    bl fn_80134768
    mr r22, r3
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80039840
    addi r20, r20, 0x1
    cmpw r20, r31
    blt @L_80039840
    mr r3, r22
    bl fn_80143C50
    b @L_80039850
    @L_80039840:
    addi r21, r21, 0x1
    @L_80039844:
    cmpw r21, r19
    blt @L_80039808
    li r3, 0x0
    @L_80039850:
    clrlwi r4, r3, 16
    li r3, 0x34
    bl fn_80132A38
    li r3, 0xca
    bl fn_800FA444
    srwi r0, r3, 16
    mr r4, r24
    extsh r0, r0
    mr r5, r28
    subfic r3, r0, 0x11a
    li r6, 0xca
    bl fn_800FB680
    @L_80039880:
    addi r26, r26, 0x1f
    addi r23, r23, 0x1
    addi r31, r31, 0x1
    @L_8003988C:
    cmpw r23, r30
    bge @L_8003989C
    cmpw r31, r27
    blt @L_8003972C
    @L_8003989C:
    cmpw r23, r30
    bge @L_800398BC
    mulli r0, r23, 0x1f
    mr r5, r28
    li r3, 0x0
    li r6, 0x1b67
    subf r4, r29, r0
    bl fn_800FB680
    @L_800398BC:
    li r3, 0x0
    lmw r19, 0x1c(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
