    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r30, r3
    lwz r29, 0x0(r4)
    addi r31, r4, 0x10
    lwz r28, 0x4(r4)
    li r5, 0x0
    rlwinm r3, r29, 0, 16, 23
    lwz r0, 0x8(r4)
    rlwinm r10, r29, 0, 8, 15
    rlwinm r9, r29, 24, 0, 7
    rlwinm r7, r3, 8, 0, 23
    rlwinm r8, r28, 0, 16, 23
    lwz r3, 0xc(r4)
    rlwinm r11, r28, 0, 8, 15
    rlwinm r6, r0, 0, 16, 23
    rlwinm r12, r10, 24, 8, 31
    or r10, r9, r7
    rlwinm r9, r0, 0, 8, 15
    rlwinm r4, r3, 0, 16, 23
    rlwinm r7, r3, 0, 8, 15
    rlwinm r29, r29, 8, 24, 31
    or r10, r12, r10
    or r12, r29, r10
    rlwinm r10, r28, 24, 0, 7
    rlwinm r8, r8, 8, 0, 23
    rlwinm r11, r11, 24, 8, 31
    or r10, r10, r8
    stw r12, 0x0(r30)
    rlwinm r12, r28, 8, 24, 31
    rlwinm r8, r0, 24, 0, 7
    or r10, r11, r10
    rlwinm r6, r6, 8, 0, 23
    or r10, r12, r10
    rlwinm r9, r9, 24, 8, 31
    or r8, r8, r6
    rlwinm r6, r3, 24, 0, 7
    rlwinm r4, r4, 8, 0, 23
    stw r10, 0x4(r30)
    rlwinm r10, r0, 8, 24, 31
    or r0, r9, r8
    or r8, r10, r0
    rlwinm r7, r7, 24, 8, 31
    or r0, r6, r4
    rlwinm r3, r3, 8, 24, 31
    or r0, r7, r0
    stw r8, 0x8(r30)
    or r3, r3, r0
    rlwinm r0, r3, 16, 16, 31
    sth r0, 0xc(r30)
    rlwinm r0, r3, 0, 16, 31
    sth r0, 0xe(r30)
    lhz r0, 0xe(r30)
    cmplwi r0, 0x32
    beq @L_80089478
    cmplwi r0, 0x1e
    bne @L_8008947C
@L_80089478:
    li r5, 0x1
@L_8008947C:
    cmpwi r5, 0x0
    bne @L_8008949C
    lis r3, 0x8027
    lis r5, 0x8027
    addi r3, r3, 0xF568
    li r4, 0x6f
    addi r5, r5, 0xF574
    bl fn_80196E10
@L_8008949C:
    mr r6, r30
    li r7, 0x0
    b @L_800894EC
@L_800894A8:
    lwz r5, 0x0(r31)
    addi r31, r31, 0x4
    addi r7, r7, 0x1
    rlwinm r0, r5, 0, 16, 23
    rlwinm r4, r5, 0, 8, 15
    rlwinm r3, r5, 24, 0, 7
    rlwinm r5, r5, 8, 24, 31
    rlwinm r0, r0, 8, 0, 23
    rlwinm r4, r4, 24, 8, 31
    or r0, r3, r0
    or r0, r4, r0
    or r3, r5, r0
    rlwinm r0, r3, 0, 16, 31
    sth r0, 0x10(r6)
    rlwinm r0, r3, 16, 16, 31
    sth r0, 0x12(r6)
    addi r6, r6, 0x4
@L_800894EC:
    lhz r0, 0xe(r30)
    cmpw r7, r0
    blt @L_800894A8
    lwz r0, -25020(r13)
    cmpwi r0, 0x0
    beq @L_80089514
    li r0, 0x0
    stw r0, 0x0(r30)
    stw r0, 0x4(r30)
    stw r0, -25020(r13)
@L_80089514:
    lwz r0, -25024(r13)
    cmpwi r0, 0x0
    beq @L_80089548
    lwz r4, 0x0(r30)
    li r0, 0x0
    lwz r3, -25024(r13)
    add r3, r4, r3
    stw r3, 0x0(r30)
    lwz r4, 0x4(r30)
    lwz r3, -25024(r13)
    add r3, r4, r3
    stw r3, 0x4(r30)
    stw r0, -25024(r13)
@L_80089548:
    lwz r0, -25012(r13)
    cmpwi r0, 0x0
    beq @L_80089560
    li r0, 0x0
    stw r0, 0x8(r30)
    stw r0, -25012(r13)
@L_80089560:
    lwz r0, -25016(r13)
    cmpwi r0, 0x0
    beq @L_80089580
    lwz r3, 0x8(r30)
    li r0, 0x0
    ori r3, r3, 0x10
    stw r3, 0x8(r30)
    stw r0, -25016(r13)
@L_80089580:
    li r3, 0x1
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
