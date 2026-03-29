    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r31, r3
    lis r3, lbl_803A6748@ha
    lfs f1, lbl_8047BAC0(r2)
    addi r3, r3, lbl_803A6748@l
    li r4, 0x0
    lfs f0, 0x44(r3)
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x8b(r31)
    lwz r0, 0x0(r3)
    lwz r3, lbl_8047A4D4(r13)
    slwi r0, r0, 2
    add r3, r3, r0
    lhz r28, 0x2(r3)
    mr r3, r28
    bl fn_801EEFAC
    clrlwi r3, r3, 16
    cmplwi r3, 0x9
    bne @L_8003B764
    mr r3, r28
    bl fn_801EE248
    bl fn_8011E778
    cmplwi r3, 0x0
    beq @L_8003B7B8
    bl fn_8011E760
    bl fn_800FA280
    mr r30, r3
    b @L_8003B7B8
    @L_8003B764:
    bl fn_801FCCC4
    bl fn_801FCC3C
    bl fn_801FCA2C
    mr r29, r3
    bl fn_801FCA14
    bl fn_800FA280
    mr r30, r3
    @L_8003B780:
    mr r3, r29
    bl fn_801FC964
    clrlwi r0, r3, 24
    cmpw r28, r0
    bne @L_8003B7B0
    mr r3, r29
    bl fn_801FCA14
    cmplwi r3, 0x0
    beq @L_8003B7B8
    bl fn_800FA280
    mr r30, r3
    b @L_8003B7B8
    @L_8003B7B0:
    addi r29, r29, 0x50
    b @L_8003B780
    @L_8003B7B8:
    cmplwi r30, 0x0
    mr r3, r30
    bne @L_8003B7CC
    li r3, 0x1
    bl fn_800FA280
    @L_8003B7CC:
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    lbz r5, 0x8b(r31)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0xcf
    bl fn_800FB680
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
