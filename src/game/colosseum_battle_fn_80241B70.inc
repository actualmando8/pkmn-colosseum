    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    mr r28, r3
    mr r26, r6
    mr r29, r4
    mr r30, r5
    li r31, 0x0
    mr r4, r26
    li r3, 0x2
    bl fn_801F025C
    li r4, 0x4a
    mr r27, r3
    bl fn_801F6E98
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80241BCC
    mr r3, r27
    li r4, 0x4a
    bl fn_801F6D9C
    mr r27, r3
    b @L_80241BD0
@L_80241BCC:
    li r27, 0x0
@L_80241BD0:
    mr r3, r28
    mr r4, r26
    bl fn_80235AA0
    stb r3, 0x8(r1)
    mr r3, r28
    mr r4, r26
    bl fn_80235A3C
    stb r3, 0x9(r1)
    mr r3, r28
    mr r4, r26
    bl fn_802359D8
    stb r3, 0xa(r1)
    mr r3, r28
    mr r4, r26
    bl fn_80235974
    stb r3, 0xb(r1)
    mr r3, r28
    mr r4, r26
    bl fn_80235910
    stb r3, 0xc(r1)
    mr r3, r28
    mr r4, r26
    bl fn_802358AC
    stb r3, 0xd(r1)
    mr r3, r28
    mr r4, r26
    bl fn_802357CC
    stb r3, 0xe(r1)
    addi r3, r1, 0x8
    li r4, 0x0
    b @L_80241C70
@L_80241C4C:
    clrlwi r0, r4, 24
    lbzx r0, r3, r0
    cmplwi r0, 0x8
    blt @L_80241C6C
    cmplwi r0, 0xc
    bgt @L_80241C6C
    li r0, 0x1
    b @L_80241C80
@L_80241C6C:
    addi r4, r4, 0x1
@L_80241C70:
    clrlwi r0, r4, 24
    cmplwi r0, 0x7
    blt @L_80241C4C
    li r0, 0x0
@L_80241C80:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_80241CD4
    mr r4, r28
    li r3, 0x0
    li r5, 0x1ac
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1ac
    bl fn_80239EE8
@L_80241CD4:
    extsh r0, r27
    cmpwi r0, 0x1
    bne @L_80241D2C
    mr r3, r31
    mr r4, r28
    li r5, 0x1ad
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1ad
    bl fn_80239EE8
    b @L_80241DD0
@L_80241D2C:
    cmpwi r0, 0x2
    bne @L_80241D80
    mr r3, r31
    mr r4, r28
    li r5, 0x1ae
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1ae
    bl fn_80239EE8
    b @L_80241DD0
@L_80241D80:
    cmpwi r0, 0x3
    bne @L_80241DD0
    mr r3, r31
    mr r4, r28
    li r5, 0x1af
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1af
    bl fn_80239EE8
@L_80241DD0:
    mr r3, r31
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
