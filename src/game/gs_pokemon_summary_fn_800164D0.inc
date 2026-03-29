    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r27, r3
    mr r31, r4
    lbz r0, 0x95(r27)
    lis r3, lbl_80266918@ha
    addi r3, r3, lbl_80266918@l
    extsb r0, r0
    mulli r0, r0, 0x4c
    add r3, r3, r0
    lwz r0, 0x1c(r3)
    clrlwi r3, r0, 16
    bl fn_80103E68
    lbz r0, 0x95(r27)
    lis r4, lbl_80266918@ha
    srwi r3, r3, 16
    extsb r0, r0
    sth r3, 0xc(r1)
    mulli r0, r0, 0x4c
    addi r3, r4, lbl_80266918@l
    lbz r4, 0xc(r1)
    add r3, r3, r0
    extsb r4, r4
    lwz r0, 0x4(r3)
    addi r27, r4, 0x8
    cmpwi r0, 0x0
    blt @L_80016564
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_8001657C
    @L_80016564:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_8001657C:
    li r30, 0x0
    mr r28, r3
    mr r29, r30
    b @L_800165AC
    @L_8001658C:
    mr r3, r28
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800165A4
    addi r30, r30, 0x1
    @L_800165A4:
    addi r29, r29, 0x1
    addi r28, r28, 0x4
    @L_800165AC:
    lhz r0, 0x8(r1)
    cmpw r29, r0
    blt @L_8001658C
    addi r0, r30, 0x1
    cmpw r27, r0
    bge @L_800165F8
    lwz r0, lbl_8047A2D8(r13)
    cmpwi r0, -0x1
    bne @L_800165F8
    lfs f1, lbl_8047B744(r2)
    lfs f0, lbl_8047A2C4(r13)
    lfs f2, lbl_8047B740(r2)
    fsubs f0, f1, f0
    fmuls f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    stb r0, 0x67(r31)
    b @L_80016600
    @L_800165F8:
    li r0, 0x0
    stb r0, 0x67(r31)
    @L_80016600:
    li r3, 0x0
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
