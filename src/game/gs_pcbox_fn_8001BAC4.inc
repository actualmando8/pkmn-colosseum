    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    mr r29, r6
    mr r30, r7
    clrlwi r0, r28, 24
    lis r3, lbl_803A1D40@ha
    addi r31, r3, lbl_803A1D40@l
    li r6, 0x0
    li r3, 0x1
    cmpwi r0, 0x3
    stb r28, 0x0(r31)
    stb r4, 0x10(r31)
    stb r5, 0x11(r31)
    sth r29, 0x12(r31)
    stw r30, 0xc(r31)
    stb r8, 0x1(r31)
    stw r6, 0x1c(r31)
    stb r3, 0x15(r31)
    beq @L_8001BB54
    bge @L_8001BB38
    cmpwi r0, 0x2
    bge @L_8001BB40
    b @L_8001BB54
    @L_8001BB38:
    cmpwi r0, 0x5
    bge @L_8001BB54
    @L_8001BB40:
    lis r3, lbl_803A1D40@ha
    li r0, 0x1
    addi r3, r3, lbl_803A1D40@l
    stw r0, 0x8(r3)
    b @L_8001BB64
    @L_8001BB54:
    lis r3, lbl_803A1D40@ha
    li r0, 0x0
    addi r3, r3, lbl_803A1D40@l
    stw r0, 0x8(r3)
    @L_8001BB64:
    lis r3, lbl_803A1D40@ha
    addi r4, r3, lbl_803A1D40@l
    lbz r0, 0x0(r4)
    cmpwi r0, 0x2
    beq @L_8001BBBC
    bge @L_8001BB88
    cmpwi r0, 0x1
    bge @L_8001BBA8
    b @L_8001BBDC
    @L_8001BB88:
    cmpwi r0, 0x8
    bge @L_8001BBDC
    cmpwi r0, 0x6
    bge @L_8001BBDC
    li r0, 0x0
    stb r0, 0x2(r4)
    stb r0, 0x3(r4)
    b @L_8001BC20
    @L_8001BBA8:
    li r3, 0x1
    li r0, 0x0
    stb r3, 0x2(r4)
    stb r0, 0x3(r4)
    b @L_8001BC20
    @L_8001BBBC:
    clrlwi r0, r29, 16
    li r3, 0x1
    cmplwi r0, 0x0
    stb r3, 0x2(r4)
    stb r3, 0x3(r4)
    bne @L_8001BBDC
    li r0, 0x0
    stb r0, 0x15(r31)
    @L_8001BBDC:
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0xc(r1)
    lfd f2, lbl_8047B7B8(r2)
    stw r0, 0x8(r1)
    lfs f0, lbl_8047B7C4(r2)
    lfd f1, 0x8(r1)
    fsubs f1, f1, f2
    fdivs f1, f0, f1
    bl fn_8010206C
    lis r3, lbl_803A1D40@ha
    li r4, 0x1
    addi r3, r3, lbl_803A1D40@l
    li r0, 0x2
    stb r4, 0x2(r3)
    stb r0, 0x3(r3)
    @L_8001BC20:
    lis r3, lbl_803A1D40@ha
    addi r3, r3, lbl_803A1D40@l
    lbz r0, 0x2(r3)
    cmplwi r0, 0x1
    bne @L_8001BCA8
    li r3, 0x1
    li r4, 0x1
    bl fn_8019075C
    li r3, 0x38f
    bl fn_800FF730
    lis r3, lbl_803A1D40@ha
    addi r28, r3, lbl_803A1D40@l
    lbz r0, 0x3(r28)
    cmplwi r0, 0x1
    beq @L_8001BC68
    li r3, 0x0
    li r4, 0x0
    bl fn_8011288C
    @L_8001BC68:
    bl fn_800F0308
    lbz r0, 0x3(r28)
    cmplwi r0, 0x2
    bne @L_8001BCB8
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0xc(r1)
    lfd f2, lbl_8047B7B8(r2)
    stw r0, 0x8(r1)
    lfs f0, lbl_8047B7C4(r2)
    lfd f1, 0x8(r1)
    fsubs f1, f1, f2
    fdivs f1, f0, f1
    bl fn_80102038
    b @L_8001BCB8
    @L_8001BCA8:
    mr r3, r28
    mr r5, r30
    clrlwi r4, r29, 16
    bl fn_8001B1EC
    @L_8001BCB8:
    lis r3, lbl_803A1D40@ha
    li r0, 0x1
    addi r4, r3, lbl_803A1D40@l
    lwz r3, 0x1c(r31)
    stb r0, 0x4(r4)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
