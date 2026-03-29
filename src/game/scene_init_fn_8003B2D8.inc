    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r31, r3
    lis r3, lbl_803A6748@ha
    lfs f1, lbl_8047BAC0(r2)
    addi r4, r3, lbl_803A6748@l
    li r30, 0x0
    lfs f0, 0x44(r4)
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x8b(r31)
    lwz r0, lbl_8047A4D0(r13)
    lwz r3, 0x0(r4)
    cmplwi r0, 0x0
    clrlwi r0, r3, 16
    beq @L_8003B3D8
    lwz r3, lbl_8047A4D4(r13)
    slwi r0, r0, 2
    addi r28, r4, 0x94
    add r3, r3, r0
    lhz r27, 0x2(r3)
    mr r4, r28
    mr r3, r27
    bl fn_801EE544
    lbz r0, 0x0(r28)
    mr r28, r3
    cmpwi r0, 0x0
    beq @L_8003B368
    blt @L_8003B3B8
    cmpwi r0, 0x3
    bge @L_8003B3B8
    b @L_8003B370
    @L_8003B368:
    li r28, 0x371f
    b @L_8003B3B8
    @L_8003B370:
    cmplwi r27, 0x43
    bne @L_8003B380
    li r28, 0x12b0
    b @L_8003B3B8
    @L_8003B380:
    mr r3, r27
    li r4, 0x0
    bl fn_801EEFAC
    clrlwi r3, r3, 16
    cmplwi r3, 0x9
    bne @L_8003B3AC
    mr r3, r27
    bl fn_801EE328
    li r30, 0x1
    mr r29, r3
    b @L_8003B3B8
    @L_8003B3AC:
    bl fn_801FCCC4
    bl fn_801FCC7C
    mr r28, r3
    @L_8003B3B8:
    clrlwi r0, r30, 24
    cmplwi r0, 0x0
    beq @L_8003B3C8
    b @L_8003B3DC
    @L_8003B3C8:
    mr r3, r28
    bl fn_800FA280
    mr r29, r3
    b @L_8003B3DC
    @L_8003B3D8:
    mr r29, r30
    @L_8003B3DC:
    cmplwi r29, 0x0
    mr r27, r29
    bne @L_8003B3F4
    li r3, 0x1
    bl fn_800FA280
    mr r27, r3
    @L_8003B3F4:
    lis r3, lbl_803A6748@ha
    addi r3, r3, lbl_803A6748@l
    lwz r3, 0x0(r3)
    bl fn_8003CE1C
    cmplwi r3, 0x0
    bne @L_8003B414
    li r3, 0x1
    bl fn_800FA280
    @L_8003B414:
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    lbz r5, 0x8b(r31)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0xe7
    bl fn_800FB680
    mr r4, r27
    li r3, 0x37
    bl fn_80132A38
    lbz r5, 0x8b(r31)
    li r0, -0x100
    li r3, 0xb4
    li r4, 0x0
    or r5, r5, r0
    li r6, 0xe7
    bl fn_800FB680
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
