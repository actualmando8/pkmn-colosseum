    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    li r5, 0x5c
    stw r0, 0x44(r1)
    stmw r26, 0x28(r1)
    mr r30, r4
    mr r27, r3
    li r26, 0x0
    li r28, 0x1
    lwz r29, 0x4(r4)
    li r4, 0x0
    bl memset
    lwz r0, 0x0(r30)
    stb r0, 0x4c(r27)
    lwz r0, 0x8(r30)
    cmpwi r0, 0x3
    beq @L_801377DC
    bge @L_801377DC
    cmpwi r0, 0x1
    bge @L_801377D4
    b @L_801377DC
@L_801377D4:
    li r3, -0x4
    b @L_80137804
@L_801377DC:
    lwz r3, 0xc(r30)
    rlwinm r0, r3, 0, 29, 29
    cmpwi r0, 0x4
    bne @L_801377F0
    li r28, 0x0
@L_801377F0:
    rlwinm r0, r3, 0, 31, 31
    cmpwi r0, 0x1
    bne @L_80137800
    li r26, 0x1
@L_80137800:
    li r3, 0x0
@L_80137804:
    lwz r0, 0x0(r30)
    cmpwi r0, 0x0
    bne @L_80137818
    li r28, 0x1
    li r26, 0x1
@L_80137818:
    add r30, r3, r30
    stb r26, 0x4e(r27)
    addi r30, r30, 0x10
    rlwinm r31, r28, 0, 24, 31
    mr r28, r30
    rlwinm r26, r26, 0, 24, 31
    li r30, 0x0
    b @L_8013794C
@L_80137838:
    cmplwi r31, 0x0
    beq @L_80137874
    lwz r4, 0x0(r28)
    rlwinm r3, r4, 16, 24, 31
    rlwinm r0, r4, 24, 24, 31
    stb r3, 0xe(r1)
    stb r0, 0xd(r1)
    stb r4, 0xc(r1)
    lwz r4, 0x4(r28)
    rlwinm r3, r4, 16, 24, 31
    rlwinm r0, r4, 24, 24, 31
    stb r3, 0xa(r1)
    stb r0, 0x9(r1)
    stb r4, 0x8(r1)
    b @L_80137890
@L_80137874:
    li r0, 0x7f
    stb r0, 0xe(r1)
    stb r0, 0xd(r1)
    stb r0, 0xc(r1)
    stb r0, 0xa(r1)
    stb r0, 0x9(r1)
    stb r0, 0x8(r1)
@L_80137890:
    cmplwi r26, 0x0
    beq @L_801378B4
    lwz r0, 0x0(r28)
    rlwinm r0, r0, 8, 24, 31
    stb r0, 0xf(r1)
    lwz r0, 0x4(r28)
    rlwinm r0, r0, 8, 24, 31
    stb r0, 0xb(r1)
    b @L_801378C0
@L_801378B4:
    li r0, 0xff
    stb r0, 0xf(r1)
    stb r0, 0xb(r1)
@L_801378C0:
    lwz r0, 0x8(r28)
    cmpwi r0, 0x0
    bge @L_801378E4
    mr r3, r27
    addi r4, r1, 0xc
    addi r5, r1, 0x8
    li r6, -0x1
    bl fn_8013AB60
    b @L_80137944
@L_801378E4:
    bl fn_800D37CC
    lis r6, 0x4330
    lwz r0, 0x8(r28)
    xoris r3, r3, 0x8000
    stw r6, 0x10(r1)
    xoris r0, r0, 0x8000
    lfd f3, -25976(r2)
    stw r3, 0x14(r1)
    mr r3, r27
    lfs f0, -25992(r2)
    addi r4, r1, 0xc
    lfd f1, 0x10(r1)
    addi r5, r1, 0x8
    stw r0, 0x1c(r1)
    fsubs f2, f1, f3
    stw r6, 0x18(r1)
    lfd f1, 0x18(r1)
    fsubs f1, f1, f3
    fmuls f1, f1, f2
    fdivs f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x20(r1)
    lwz r6, 0x24(r1)
    bl fn_8013AB60
@L_80137944:
    addi r30, r30, 0x1
    addi r28, r28, 0x10
@L_8013794C:
    cmpw r30, r29
    blt @L_80137838
    mr r3, r28
    lmw r26, 0x28(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
