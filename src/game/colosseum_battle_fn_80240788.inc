    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r27, 0x2c(r1)
    mr r30, r6
    mr r28, r4
    mr r27, r3
    mr r29, r5
    mr r4, r30
    li r31, 0x0
    bl fn_80235AA0
    stb r3, 0x20(r1)
    mr r3, r27
    mr r4, r30
    bl fn_80235A3C
    stb r3, 0x21(r1)
    mr r3, r27
    mr r4, r30
    bl fn_802359D8
    stb r3, 0x22(r1)
    mr r3, r27
    mr r4, r30
    bl fn_80235974
    stb r3, 0x23(r1)
    mr r3, r27
    mr r4, r30
    bl fn_80235910
    stb r3, 0x24(r1)
    mr r3, r27
    mr r4, r30
    bl fn_802358AC
    stb r3, 0x25(r1)
    mr r3, r27
    mr r4, r30
    bl fn_802357CC
    stb r3, 0x26(r1)
    addi r3, r1, 0x20
    li r4, 0x0
    b @L_80240848
@L_80240824:
    clrlwi r0, r4, 24
    lbzx r0, r3, r0
    cmplwi r0, 0x8
    blt @L_80240844
    cmplwi r0, 0x9
    bgt @L_80240844
    li r0, 0x1
    b @L_80240858
@L_80240844:
    addi r4, r4, 0x1
@L_80240848:
    clrlwi r0, r4, 24
    cmplwi r0, 0x7
    blt @L_80240824
    li r0, 0x0
@L_80240858:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_802408AC
    mr r4, r27
    li r3, 0x0
    li r5, 0x1be
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1be
    bl fn_80239EE8
@L_802408AC:
    mr r3, r27
    mr r4, r30
    bl fn_80235AA0
    stb r3, 0x18(r1)
    mr r3, r27
    mr r4, r30
    bl fn_80235A3C
    stb r3, 0x19(r1)
    mr r3, r27
    mr r4, r30
    bl fn_802359D8
    stb r3, 0x1a(r1)
    mr r3, r27
    mr r4, r30
    bl fn_80235974
    stb r3, 0x1b(r1)
    mr r3, r27
    mr r4, r30
    bl fn_80235910
    stb r3, 0x1c(r1)
    mr r3, r27
    mr r4, r30
    bl fn_802358AC
    stb r3, 0x1d(r1)
    mr r3, r27
    mr r4, r30
    bl fn_802357CC
    stb r3, 0x1e(r1)
    addi r3, r1, 0x18
    li r4, 0x0
    b @L_8024094C
@L_80240928:
    clrlwi r0, r4, 24
    lbzx r0, r3, r0
    cmplwi r0, 0xa
    blt @L_80240948
    cmplwi r0, 0xc
    bgt @L_80240948
    li r0, 0x1
    b @L_8024095C
@L_80240948:
    addi r4, r4, 0x1
@L_8024094C:
    clrlwi r0, r4, 24
    cmplwi r0, 0x7
    blt @L_80240928
    li r0, 0x0
@L_8024095C:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_802409B0
    mr r3, r31
    mr r4, r27
    li r5, 0x1bf
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1bf
    bl fn_80239EE8
@L_802409B0:
    mr r3, r27
    mr r4, r30
    bl fn_80235AA0
    stb r3, 0x10(r1)
    mr r3, r27
    mr r4, r30
    bl fn_80235A3C
    stb r3, 0x11(r1)
    mr r3, r27
    mr r4, r30
    bl fn_802359D8
    stb r3, 0x12(r1)
    mr r3, r27
    mr r4, r30
    bl fn_80235974
    stb r3, 0x13(r1)
    mr r3, r27
    mr r4, r30
    bl fn_80235910
    stb r3, 0x14(r1)
    mr r3, r27
    mr r4, r30
    bl fn_802358AC
    stb r3, 0x15(r1)
    mr r3, r27
    mr r4, r30
    bl fn_802357CC
    stb r3, 0x16(r1)
    addi r3, r1, 0x10
    li r4, 0x0
    b @L_80240A50
@L_80240A2C:
    clrlwi r0, r4, 24
    lbzx r0, r3, r0
    cmplwi r0, 0x3
    blt @L_80240A4C
    cmplwi r0, 0x4
    bgt @L_80240A4C
    li r0, 0x1
    b @L_80240A60
@L_80240A4C:
    addi r4, r4, 0x1
@L_80240A50:
    clrlwi r0, r4, 24
    cmplwi r0, 0x7
    blt @L_80240A2C
    li r0, 0x0
@L_80240A60:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_80240AB4
    mr r3, r31
    mr r4, r27
    li r5, 0x1c0
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1c0
    bl fn_80239EE8
@L_80240AB4:
    mr r3, r27
    mr r4, r30
    bl fn_80235AA0
    stb r3, 0x8(r1)
    mr r3, r27
    mr r4, r30
    bl fn_80235A3C
    stb r3, 0x9(r1)
    mr r3, r27
    mr r4, r30
    bl fn_802359D8
    stb r3, 0xa(r1)
    mr r3, r27
    mr r4, r30
    bl fn_80235974
    stb r3, 0xb(r1)
    mr r3, r27
    mr r4, r30
    bl fn_80235910
    stb r3, 0xc(r1)
    mr r3, r27
    mr r4, r30
    bl fn_802358AC
    stb r3, 0xd(r1)
    mr r3, r27
    mr r4, r30
    bl fn_802357CC
    stb r3, 0xe(r1)
    addi r3, r1, 0x8
    li r4, 0x0
    b @L_80240B54
@L_80240B30:
    clrlwi r0, r4, 24
    lbzx r0, r3, r0
    cmplwi r0, 0x0
    blt @L_80240B50
    cmplwi r0, 0x2
    bgt @L_80240B50
    li r0, 0x1
    b @L_80240B64
@L_80240B50:
    addi r4, r4, 0x1
@L_80240B54:
    clrlwi r0, r4, 24
    cmplwi r0, 0x7
    blt @L_80240B30
    li r0, 0x0
@L_80240B64:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_80240BB8
    mr r3, r31
    mr r4, r27
    li r5, 0x1c1
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1c1
    bl fn_80239EE8
@L_80240BB8:
    mr r3, r31
    lmw r27, 0x2c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
