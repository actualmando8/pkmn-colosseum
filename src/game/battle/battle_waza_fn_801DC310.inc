    nofralloc
    lwz r6, 0x8(r4)
    lwz r8, 0x24(r3)
    cmpwi r6, 0x0
    mr r5, r8
    beq @L_801DC370
    b @L_801DC338
@L_801DC328:
    lwz r0, 0x0(r8)
    cmpw r0, r6
    beq @L_801DC340
    lwz r8, 0xa8(r8)
@L_801DC338:
    cmplwi r8, 0x0
    bne @L_801DC328
@L_801DC340:
    cmplwi r8, 0x0
    beq @L_801DC370
    lwz r0, 0x70(r8)
    stw r0, 0x70(r4)
    lwz r0, 0x10(r4)
    lwz r7, 0x70(r4)
    rlwinm r0, r0, 2, 0, 29
    add r6, r8, r0
    lwz r0, 0x2c(r6)
    add r0, r7, r0
    stw r0, 0x70(r4)
    b @L_801DC398
@L_801DC370:
    lwz r0, 0xc(r3)
    lwz r7, 0x3c(r3)
    mulli r6, r0, 0xd4
    lwz r0, 0x10(r4)
    lwz r7, 0x2c(r7)
    rlwinm r0, r0, 2, 0, 29
    addi r6, r6, 0xc
    add r0, r6, r0
    lwzx r0, r7, r0
    stw r0, 0x70(r4)
@L_801DC398:
    lwz r6, 0xc(r4)
    cmplwi r5, 0x0
    lwz r0, 0x70(r4)
    rlwinm r6, r6, 2, 0, 29
    add r6, r4, r6
    lwz r6, 0x2c(r6)
    subf r0, r6, r0
    stw r0, 0x70(r4)
    lwz r0, 0x70(r4)
    stw r0, 0x74(r4)
    beq @L_801DC454
    lwz r0, 0x70(r5)
    lwz r6, 0x70(r4)
    cmpw r0, r6
    bgt @L_801DC3E8
    lwz r8, 0x4(r4)
    cmpwi r8, 0x6
    bne @L_801DC428
    cmpw r0, r6
    bne @L_801DC428
@L_801DC3E8:
    stw r5, 0xa8(r4)
    li r0, 0x0
    stw r0, 0xac(r4)
    stw r4, 0xac(r5)
    stw r4, 0x24(r3)
    b @L_801DC464
    b @L_801DC428
@L_801DC404:
    lwz r0, 0x70(r7)
    cmpw r0, r6
    bgt @L_801DC434
    cmpwi r8, 0x6
    bne @L_801DC424
    lwz r0, 0x70(r5)
    cmpw r0, r6
    beq @L_801DC434
@L_801DC424:
    mr r5, r7
@L_801DC428:
    lwz r7, 0xa8(r5)
    cmplwi r7, 0x0
    bne @L_801DC404
@L_801DC434:
    stw r7, 0xa8(r4)
    lwz r6, 0xa8(r4)
    cmplwi r6, 0x0
    beq @L_801DC448
    stw r4, 0xac(r6)
@L_801DC448:
    stw r4, 0xa8(r5)
    stw r5, 0xac(r4)
    b @L_801DC464
@L_801DC454:
    stw r4, 0x24(r3)
    li r0, 0x0
    stw r0, 0xa8(r4)
    stw r0, 0xac(r4)
@L_801DC464:
    stw r3, 0xb0(r4)
    blr
