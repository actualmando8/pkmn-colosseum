    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, -32032(r13)
    lwz r5, 0x8(r4)
    cmplw r5, r3
    bne @L_800F7650
    lwz r0, 0x14(r5)
    stw r0, 0x8(r4)
    b @L_800F767C
    b @L_800F7650
@L_800F7638:
    cmplw r4, r3
    bne @L_800F764C
    lwz r0, 0x14(r4)
    stw r0, 0x14(r5)
    b @L_800F767C
@L_800F764C:
    mr r5, r4
@L_800F7650:
    lwz r4, 0x14(r5)
    cmplwi r4, 0x0
    bne @L_800F7638
    bne @L_800F767C
    lis r5, -32729
    mr r4, r3
    addi r3, r5, 4792
    crxor 6, 6, 6
    bl fn_800DD38C
    li r3, -0x1
    b @L_800F76D4
@L_800F767C:
    li r8, 0x0
    li r6, 0x0
    li r4, 0x3
    b @L_800F76C0
@L_800F768C:
    lwz r0, 0xc(r5)
    add r7, r0, r6
    lbz r0, 0x4(r7)
    cmplwi r0, 0x0
    beq @L_800F76B8
    lwz r0, 0x8(r7)
    lhz r5, 0x0(r3)
    rlwinm r0, r0, 16, 16, 31
    cmplw r5, r0
    bne @L_800F76B8
    stb r4, 0x4(r7)
@L_800F76B8:
    addi r6, r6, 0x16c
    addi r8, r8, 0x1
@L_800F76C0:
    lwz r5, -32032(r13)
    lhz r0, 0x0(r5)
    cmpw r8, r0
    blt @L_800F768C
    li r3, 0x0
@L_800F76D4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
