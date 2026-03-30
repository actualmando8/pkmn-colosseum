    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r20, 0x10(r1)
    mr r26, r3
    mr r27, r4
    mr r28, r5
    li r23, 0x0
    lhz r0, 0xe(r3)
    lwz r31, 0x10(r3)
    rlwinm r22, r0, 5, 0, 26
    b @L_801AC1D8
@L_801ABE04:
    lbz r0, 0x0(r31)
    li r30, 0x3
    lbz r4, 0x1(r31)
    rlwinm. r0, r0, 0, 24, 28
    lbz r24, 0x2(r31)
    rlwimi r24, r4, 8, 16, 23
    beq @L_801AC1E4
    lbz r3, 0x0(r31)
    rlwinm r5, r24, 0, 16, 31
    lbz r0, 0x0(r31)
    rlwinm r3, r3, 0, 24, 28
    rlwinm r4, r0, 0, 29, 31
    bl fn_800B928C
    li r25, 0x0
    b @L_801AC1C8
@L_801ABE40:
    li r29, 0x0
@L_801ABE44:
    mulli r0, r29, 0x18
    lwz r3, 0x8(r26)
    add r5, r3, r0
    lwz r0, 0x0(r5)
    cmpwi r0, 0xff
    beq @L_801AC1C4
    lwz r3, 0x0(r5)
    mr r0, r30
    lbzx r0, r31, r0
    addi r30, r30, 0x1
    cmplwi r3, 0x19
    bgt @L_801AC198
    lis r4, jumptable_8036CD1C@ha
    rlwinm r3, r3, 2, 0, 29
    addi r4, r4, jumptable_8036CD1C@l
    lwzx r4, r4, r3
    mtctr r4
    bctr
    lis r3, 0xcc01
    stb r0, -0x8000(r3)
    b @L_801AC1BC
    lwz r3, 0x4(r5)
    cmpwi r3, 0x3
    bne @L_801ABEB8
    mr r3, r30
    addi r30, r30, 0x1
    lbzx r3, r31, r3
    rlwimi r3, r0, 8, 8, 23
    rlwinm r0, r3, 0, 16, 31
@L_801ABEB8:
    rlwinm r4, r0, 0, 16, 31
    rlwinm r3, r0, 0, 16, 31
    mulli r7, r4, 0xc
    rlwinm r0, r0, 0, 16, 31
    lis r5, 0xcc01
    lis r4, 0xcc01
    mulli r6, r3, 0xc
    lis r3, 0xcc01
    add r7, r27, r7
    lfs f2, 0x8(r7)
    mulli r0, r0, 0xc
    add r6, r27, r6
    lfs f1, 0x4(r6)
    lfsx f0, r27, r0
    stfs f0, -0x8000(r5)
    stfs f1, -0x8000(r4)
    stfs f2, -0x8000(r3)
    b @L_801AC1BC
    lwz r3, 0x4(r5)
    cmpwi r3, 0x3
    bne @L_801ABF20
    mr r3, r30
    addi r30, r30, 0x1
    lbzx r3, r31, r3
    rlwimi r3, r0, 8, 8, 23
    rlwinm r0, r3, 0, 16, 31
@L_801ABF20:
    rlwinm r4, r0, 0, 16, 31
    rlwinm r3, r0, 0, 16, 31
    mulli r7, r4, 0xc
    rlwinm r0, r0, 0, 16, 31
    lis r5, 0xcc01
    lis r4, 0xcc01
    mulli r6, r3, 0xc
    lis r3, 0xcc01
    add r7, r28, r7
    lfs f2, 0x8(r7)
    mulli r0, r0, 0xc
    add r6, r28, r6
    lfs f1, 0x4(r6)
    lfsx f0, r28, r0
    stfs f0, -0x8000(r5)
    stfs f1, -0x8000(r4)
    stfs f2, -0x8000(r3)
    b @L_801AC1BC
    lwz r3, 0x4(r5)
    cmpwi r3, 0x3
    bne @L_801ABF88
    mr r3, r30
    addi r30, r30, 0x1
    lbzx r3, r31, r3
    rlwimi r3, r0, 8, 8, 23
    rlwinm r0, r3, 0, 16, 31
@L_801ABF88:
    mulli r0, r0, 0x3
    lis r5, 0xcc01
    lis r21, 0xcc01
    lis r20, 0xcc01
    rlwinm r0, r0, 0, 16, 31
    lis r8, 0xcc01
    mr r3, r0
    lis r10, 0xcc01
    mulli r7, r0, 0xc
    mr r6, r0
    mr r4, r0
    mulli r11, r3, 0xc
    mr r3, r0
    add r7, r28, r7
    lfs f2, 0x8(r7)
    mulli r9, r6, 0xc
    mr r7, r0
    add r6, r28, r11
    lfs f1, 0x4(r6)
    lfsx f0, r28, r9
    mulli r12, r4, 0xc
    mr r6, r0
    stfs f0, -0x8000(r5)
    lis r9, 0xcc01
    mulli r11, r0, 0xc
    lis r5, 0xcc01
    stfs f1, -0x8000(r21)
    add r12, r28, r12
    lis r4, 0xcc01
    add r21, r28, r11
    mulli r11, r3, 0xc
    stfs f2, -0x8000(r20)
    lis r3, 0xcc01
    lfs f2, 0x14(r21)
    add r11, r28, r11
    lfs f1, 0x10(r12)
    lfs f0, 0xc(r11)
    mulli r7, r7, 0xc
    stfs f0, -0x8000(r8)
    mulli r6, r6, 0xc
    stfs f1, -0x8000(r10)
    add r8, r28, r7
    stfs f2, -0x8000(r9)
    add r7, r28, r6
    mulli r0, r0, 0xc
    lfs f2, 0x20(r8)
    lfs f1, 0x1c(r7)
    add r6, r28, r0
    lfs f0, 0x18(r6)
    stfs f0, -0x8000(r5)
    stfs f1, -0x8000(r4)
    stfs f2, -0x8000(r3)
    b @L_801AC1BC
    lwz r3, 0x4(r5)
    cmpwi r3, 0x3
    bne @L_801AC084
    mr r3, r30
    addi r30, r30, 0x1
    lbzx r4, r31, r3
    lis r3, 0xcc01
    rlwimi r4, r0, 8, 8, 23
    sth r4, -0x8000(r3)
    b @L_801AC1BC
@L_801AC084:
    lis r3, 0xcc01
    stb r0, -0x8000(r3)
    b @L_801AC1BC
    lwz r3, 0x4(r5)
    cmpwi r3, 0x3
    bne @L_801AC0B8
    mr r3, r30
    addi r30, r30, 0x1
    lbzx r4, r31, r3
    lis r3, 0xcc01
    rlwimi r4, r0, 8, 8, 23
    sth r4, -0x8000(r3)
    b @L_801AC1BC
@L_801AC0B8:
    lwz r3, 0x4(r5)
    cmpwi r3, 0x2
    bne @L_801AC0D0
    lis r3, 0xcc01
    stb r0, -0x8000(r3)
    b @L_801AC1BC
@L_801AC0D0:
    lwz r3, 0xc(r5)
    cmpwi r3, 0x3
    beq @L_801AC108
    bge @L_801AC0F8
    cmpwi r3, 0x1
    beq @L_801AC124
    bge @L_801AC154
    cmpwi r3, 0x0
    bge @L_801AC108
    b @L_801AC1BC
@L_801AC0F8:
    cmpwi r3, 0x5
    beq @L_801AC154
    bge @L_801AC1BC
    b @L_801AC124
@L_801AC108:
    mr r3, r30
    addi r30, r30, 0x1
    lbzx r4, r31, r3
    lis r3, 0xcc01
    rlwimi r4, r0, 8, 8, 23
    sth r4, -0x8000(r3)
    b @L_801AC1BC
@L_801AC124:
    add r3, r31, r30
    lbzx r4, r31, r30
    lbz r5, 0x1(r3)
    lis r3, 0xcc01
    addi r30, r30, 0x2
    stb r0, -0x8000(r3)
    lis r3, 0xcc01
    rlwinm r0, r5, 0, 24, 31
    stb r4, -0x8000(r3)
    lis r3, 0xcc01
    stb r0, -0x8000(r3)
    b @L_801AC1BC
@L_801AC154:
    add r4, r31, r30
    add r5, r31, r30
    lbzx r7, r31, r30
    lis r3, 0xcc01
    lbz r6, 0x2(r4)
    lis r4, 0xcc01
    lbz r5, 0x1(r5)
    addi r30, r30, 0x3
    stb r0, -0x8000(r4)
    rlwinm r0, r5, 0, 24, 31
    lis r4, 0xcc01
    rlwinm r5, r6, 0, 24, 31
    stb r7, -0x8000(r3)
    lis r3, 0xcc01
    stb r0, -0x8000(r4)
    stb r5, -0x8000(r3)
    b @L_801AC1BC
@L_801AC198:
    lwz r3, 0x4(r5)
    cmpwi r3, 0x3
    bne @L_801AC1A8
    addi r30, r30, 0x1
@L_801AC1A8:
    lis r3, lbl_80274FE4@ha
    lwz r4, 0x0(r5)
    addi r3, r3, lbl_80274FE4@l
    crxor 6, 6, 6
    bl OSReport
@L_801AC1BC:
    addi r29, r29, 0x1
    b @L_801ABE44
@L_801AC1C4:
    addi r25, r25, 0x1
@L_801AC1C8:
    cmpw r25, r24
    blt @L_801ABE40
    add r23, r23, r30
    add r31, r31, r30
@L_801AC1D8:
    addi r0, r23, 0x3
    cmpw r0, r22
    blt @L_801ABE04
@L_801AC1E4:
    lmw r20, 0x10(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
