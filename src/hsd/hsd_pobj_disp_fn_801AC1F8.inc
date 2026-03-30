    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r7, lbl_80274EE0@ha
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r29, r3
    addi r31, r7, lbl_80274EE0@l
    mr r27, r5
    mr r30, r6
    lwz r3, 0x14(r3)
    lwz r7, 0x18(r29)
    lwz r0, 0x0(r3)
    rlwinm r3, r4, 2, 0, 29
    lwzx r28, r7, r3
    cmpwi r0, 0x19
    beq @L_801AC248
    addi r5, r31, 0x130
    addi r3, r2, -23016
    li r4, 0x4b1
    bl fn_80196E10
@L_801AC248:
    lwz r3, 0x14(r29)
    lwz r0, 0x4(r3)
    cmpwi r0, 0x3
    bne @L_801AC278
    rlwinm r4, r27, 1, 0, 30
    rlwinm r0, r27, 1, 0, 30
    add r3, r28, r0
    lbzx r28, r28, r4
    lbz r0, 0x1(r3)
    rlwinm r3, r28, 8, 0, 23
    add r28, r3, r0
    b @L_801AC27C
@L_801AC278:
    lbzx r28, r28, r27
@L_801AC27C:
    lwz r3, 0x14(r29)
    lwz r0, 0x8(r3)
    cmpwi r0, 0x0
    beq @L_801AC29C
    addi r5, r31, 0x15c
    addi r3, r2, -23016
    li r4, 0x4ba
    bl fn_80196E10
@L_801AC29C:
    lwz r4, 0x14(r29)
    lwz r3, 0x14(r29)
    lhz r0, 0x12(r4)
    lwz r5, 0x14(r29)
    mullw r4, r28, r0
    lwz r0, 0xc(r3)
    lwz r3, 0x14(r5)
    cmpwi r0, 0x4
    add r4, r3, r4
    bne @L_801AC2D4
    mr r3, r30
    li r5, 0x24
    bl memcpy
    b @L_801AC4A8
@L_801AC2D4:
    lwz r3, 0x14(r29)
    li r6, 0x1
    lwz r5, 0x14(r29)
    lwz r3, 0xc(r3)
    lbz r0, 0x10(r5)
    cmpwi r3, 0x2
    slw r0, r6, r0
    beq @L_801AC3D4
    bge @L_801AC308
    cmpwi r3, 0x0
    beq @L_801AC314
    bge @L_801AC370
    b @L_801AC498
@L_801AC308:
    cmpwi r3, 0x4
    bge @L_801AC498
    b @L_801AC434
@L_801AC314:
    li r3, 0x0
    b @L_801AC364
@L_801AC31C:
    lbzx r5, r3, r4
    lis r8, 0x4330
    xoris r7, r0, 0x8000
    lis r6, 0x4330
    stw r5, 0xc(r1)
    rlwinm r5, r3, 2, 0, 29
    lfd f2, -22936(r2)
    addi r3, r3, 0x1
    stw r8, 0x8(r1)
    lfd f1, -22944(r2)
    lfd f0, 0x8(r1)
    stw r7, 0x14(r1)
    fsubs f2, f0, f2
    stw r6, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
    stfsx f0, r30, r5
@L_801AC364:
    cmpwi r3, 0x9
    blt @L_801AC31C
    b @L_801AC4A8
@L_801AC370:
    li r3, 0x0
    b @L_801AC3C8
@L_801AC378:
    lbzx r5, r3, r4
    lis r8, 0x4330
    xoris r7, r0, 0x8000
    lis r6, 0x4330
    extsb r9, r5
    rlwinm r5, r3, 2, 0, 29
    xoris r9, r9, 0x8000
    stw r8, 0x10(r1)
    lfd f2, -22944(r2)
    addi r3, r3, 0x1
    stw r9, 0x14(r1)
    lfd f1, -22944(r2)
    lfd f0, 0x10(r1)
    stw r7, 0xc(r1)
    fsubs f2, f0, f2
    stw r6, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
    stfsx f0, r30, r5
@L_801AC3C8:
    cmpwi r3, 0x9
    blt @L_801AC378
    b @L_801AC4A8
@L_801AC3D4:
    li r3, 0x0
    b @L_801AC428
@L_801AC3DC:
    rlwinm r5, r3, 1, 0, 30
    lis r8, 0x4330
    lhzx r9, r4, r5
    xoris r7, r0, 0x8000
    lis r6, 0x4330
    rlwinm r5, r3, 2, 0, 29
    stw r9, 0x14(r1)
    addi r3, r3, 0x1
    lfd f2, -22936(r2)
    stw r8, 0x10(r1)
    lfd f1, -22944(r2)
    lfd f0, 0x10(r1)
    stw r7, 0xc(r1)
    fsubs f2, f0, f2
    stw r6, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
    stfsx f0, r30, r5
@L_801AC428:
    cmpwi r3, 0x9
    blt @L_801AC3DC
    b @L_801AC4A8
@L_801AC434:
    li r3, 0x0
    b @L_801AC48C
@L_801AC43C:
    rlwinm r5, r3, 1, 0, 30
    lis r8, 0x4330
    lhax r9, r4, r5
    xoris r7, r0, 0x8000
    lis r6, 0x4330
    rlwinm r5, r3, 2, 0, 29
    xoris r9, r9, 0x8000
    stw r8, 0x10(r1)
    lfd f2, -22944(r2)
    addi r3, r3, 0x1
    stw r9, 0x14(r1)
    lfd f1, -22944(r2)
    lfd f0, 0x10(r1)
    stw r7, 0xc(r1)
    fsubs f2, f0, f2
    stw r6, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
    stfsx f0, r30, r5
@L_801AC48C:
    cmpwi r3, 0x9
    blt @L_801AC43C
    b @L_801AC4A8
@L_801AC498:
    addi r5, r31, 0x18c
    addi r3, r2, -23016
    li r4, 0x4d9
    bl fn_80196D78
@L_801AC4A8:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
