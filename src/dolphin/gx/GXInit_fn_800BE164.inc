    nofralloc
    lwz r10, -24228(r13)
    addi r8, r10, 0x42
    lhz r6, 0x42(r10)
    addi r7, r10, 0x40
@L_800BE174:
    mr r0, r6
    lhz r5, 0x0(r7)
    lhz r6, 0x0(r8)
    cmplw r6, r0
    bne @L_800BE174
    rlwinm r0, r6, 16, 0, 15
    or r6, r0, r5
    addi r9, r10, 0x46
    lhz r7, 0x46(r10)
    addi r8, r10, 0x44
@L_800BE19C:
    mr r0, r7
    lhz r5, 0x0(r8)
    lhz r7, 0x0(r9)
    cmplw r7, r0
    bne @L_800BE19C
    rlwinm r0, r7, 16, 0, 15
    or r7, r0, r5
    addi r11, r10, 0x4a
    lhz r8, 0x4a(r10)
    addi r9, r10, 0x48
@L_800BE1C4:
    mr r0, r8
    lhz r5, 0x0(r9)
    lhz r8, 0x0(r11)
    cmplw r8, r0
    bne @L_800BE1C4
    rlwinm r0, r8, 16, 0, 15
    or r8, r0, r5
    addi r11, r10, 0x4e
    lhz r9, 0x4e(r10)
    addi r10, r10, 0x4c
@L_800BE1EC:
    mr r0, r9
    lhz r5, 0x0(r10)
    lhz r9, 0x0(r11)
    cmplw r9, r0
    bne @L_800BE1EC
    rlwinm r0, r9, 16, 0, 15
    or r9, r0, r5
    lwz r5, -29632(r13)
    lwz r0, 0x4e4(r5)
    cmpwi r0, 0xa
    beq @L_800BE238
    bge @L_800BE228
    cmpwi r0, 0x0
    bge @L_800BE248
    b @L_800BE25C
@L_800BE228:
    cmpwi r0, 0x23
    beq @L_800BE250
    bge @L_800BE25C
    b @L_800BE248
@L_800BE238:
    mulli r0, r7, 0x3e8
    divwu r0, r0, r6
    stw r0, 0x0(r3)
    b @L_800BE264
@L_800BE248:
    stw r6, 0x0(r3)
    b @L_800BE264
@L_800BE250:
    li r0, 0x0
    stw r0, 0x0(r3)
    b @L_800BE264
@L_800BE25C:
    li r0, 0x0
    stw r0, 0x0(r3)
@L_800BE264:
    lwz r3, -29632(r13)
    lwz r0, 0x4e8(r3)
    cmplwi r0, 0x16
    bgt @L_800BE300
    lis r3, -32719
    addi r3, r3, 14100
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r3, r0
    mtctr r0
    bctr
    rlwinm r0, r9, 2, 0, 29
    stw r0, 0x0(r4)
    blr
    rlwinm r0, r9, 1, 0, 30
    add r0, r8, r0
    stw r0, 0x0(r4)
    blr
    mulli r3, r8, 0x3
    rlwinm r0, r9, 2, 0, 29
    add r0, r3, r0
    stw r0, 0x0(r4)
    blr
    mulli r3, r8, 0x5
    mulli r0, r9, 0x6
    add r0, r3, r0
    stw r0, 0x0(r4)
    blr
    mulli r3, r8, 0x7
    rlwinm r0, r9, 3, 0, 28
    add r0, r3, r0
    stw r0, 0x0(r4)
    blr
    stw r9, 0x0(r4)
    blr
    stw r8, 0x0(r4)
    blr
    li r0, 0x0
    stw r0, 0x0(r4)
    blr
@L_800BE300:
    li r0, 0x0
    stw r0, 0x0(r4)
    blr
