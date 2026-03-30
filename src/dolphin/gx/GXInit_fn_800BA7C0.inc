    nofralloc
    stwu r1, -0x28(r1)
    cmplwi r5, 0x3c
    stw r31, 0x24(r1)
    bgt @L_800BA80C
    lis r8, -32719
    addi r8, r8, 13160
    rlwinm r0, r5, 2, 0, 29
    lwzx r0, r8, r0
    mtctr r0
    bctr
    li r0, 0x3
    li r8, 0x3
    b @L_800BA814
    li r0, 0x3
    li r8, 0x2
    b @L_800BA814
    li r0, 0x2
    li r8, 0x2
    b @L_800BA814
@L_800BA80C:
    li r8, 0x0
    li r0, 0x0
@L_800BA814:
    cmplwi r5, 0x6
    beq @L_800BA824
    cmplwi r5, 0x16
    bne @L_800BA82C
@L_800BA824:
    li r5, 0x40
    b @L_800BA830
@L_800BA82C:
    li r5, 0x20
@L_800BA830:
    rlwinm r6, r6, 0, 24, 31
    cmplwi r6, 0x1
    bne @L_800BA8D8
    rlwinm r9, r7, 0, 24, 31
    li r6, 0x1
    mtctr r9
    slw r7, r6, r8
    slw r6, r6, r0
    cmplwi r9, 0x0
    subi r10, r6, 0x1
    subi r7, r7, 0x1
    li r31, 0x0
    ble @L_800BA90C
@L_800BA864:
    rlwinm r11, r3, 0, 16, 31
    add r6, r11, r10
    rlwinm r12, r4, 0, 16, 31
    sraw r9, r6, r0
    add r6, r12, r7
    sraw r6, r6, r8
    mullw r6, r9, r6
    mullw r6, r5, r6
    cmplwi r11, 0x1
    add r31, r31, r6
    bne @L_800BA898
    cmplwi r12, 0x1
    beq @L_800BA90C
@L_800BA898:
    rlwinm r3, r3, 0, 16, 31
    cmplwi r3, 0x1
    ble @L_800BA8AC
    srawi r6, r11, 1
    b @L_800BA8B0
@L_800BA8AC:
    li r6, 0x1
@L_800BA8B0:
    rlwinm r3, r4, 0, 16, 31
    cmplwi r3, 0x1
    rlwinm r3, r6, 0, 16, 31
    ble @L_800BA8C8
    srawi r4, r12, 1
    b @L_800BA8CC
@L_800BA8C8:
    li r4, 0x1
@L_800BA8CC:
    rlwinm r4, r4, 0, 16, 31
    bdnz @L_800BA864
    b @L_800BA90C
@L_800BA8D8:
    li r6, 0x1
    slw r7, r6, r0
    slw r6, r6, r8
    rlwinm r9, r3, 0, 16, 31
    subi r3, r7, 0x1
    add r7, r9, r3
    rlwinm r4, r4, 0, 16, 31
    subi r3, r6, 0x1
    sraw r6, r7, r0
    add r0, r4, r3
    sraw r0, r0, r8
    mullw r0, r6, r0
    mullw r31, r5, r0
@L_800BA90C:
    mr r3, r31
    lwz r31, 0x24(r1)
    addi r1, r1, 0x28
    blr
