    nofralloc
    stwu r1, -0x38(r1)
    lfs f0, -29512(r2)
    fcmpo cr0, f3, f0
    bge @L_800BACB8
    fmr f3, f0
    b @L_800BACCC
@L_800BACB8:
    lfs f0, -29508(r2)
    fcmpo cr0, f3, f0
    cror eq, gt, eq
    bne @L_800BACCC
    lfs f3, -29504(r2)
@L_800BACCC:
    lfs f0, -29500(r2)
    cmpwi r5, 0x1
    lwz r0, 0x0(r3)
    fmuls f0, f0, f3
    rlwinm r5, r0, 0, 23, 14
    fctiwz f0, f0
    stfd f0, 0x30(r1)
    lwz r0, 0x34(r1)
    rlwimi r5, r0, 9, 15, 22
    stw r5, 0x0(r3)
    bne @L_800BAD00
    li r5, 0x1
    b @L_800BAD04
@L_800BAD00:
    li r5, 0x0
@L_800BAD04:
    lwz r9, 0x0(r3)
    rlwinm. r0, r7, 0, 24, 31
    rlwinm r5, r5, 4, 0, 27
    rlwinm r7, r9, 0, 28, 26
    or r5, r7, r5
    stw r5, 0x0(r3)
    addi r5, r13, -32112
    lbzx r0, r5, r4
    lwz r7, 0x0(r3)
    rlwinm r0, r0, 5, 0, 26
    rlwinm r4, r7, 0, 27, 23
    or r0, r4, r0
    stw r0, 0x0(r3)
    beq @L_800BAD44
    li r0, 0x0
    b @L_800BAD48
@L_800BAD44:
    li r0, 0x1
@L_800BAD48:
    lwz r4, 0x0(r3)
    rlwinm r0, r0, 8, 0, 23
    rlwinm r4, r4, 0, 24, 22
    or r0, r4, r0
    stw r0, 0x0(r3)
    rlwinm r4, r8, 19, 0, 12
    clrlslwi r0, r6, 24, 21
    lwz r5, 0x0(r3)
    rlwinm r5, r5, 0, 15, 13
    stw r5, 0x0(r3)
    lwz r5, 0x0(r3)
    rlwinm r5, r5, 0, 14, 12
    stw r5, 0x0(r3)
    lwz r5, 0x0(r3)
    rlwinm r5, r5, 0, 13, 10
    or r4, r5, r4
    stw r4, 0x0(r3)
    lwz r4, 0x0(r3)
    rlwinm r4, r4, 0, 11, 9
    or r0, r4, r0
    stw r0, 0x0(r3)
    lfs f0, -29496(r2)
    fcmpo cr0, f1, f0
    bge @L_800BADB0
    fmr f1, f0
    b @L_800BADC0
@L_800BADB0:
    lfs f0, -29492(r2)
    fcmpo cr0, f1, f0
    ble @L_800BADC0
    fmr f1, f0
@L_800BADC0:
    lfs f3, -29528(r2)
    lfs f0, -29496(r2)
    fmuls f1, f3, f1
    fcmpo cr0, f2, f0
    fctiwz f1, f1
    stfd f1, 0x30(r1)
    lwz r4, 0x34(r1)
    bge @L_800BADE8
    fmr f2, f0
    b @L_800BADF8
@L_800BADE8:
    lfs f0, -29492(r2)
    fcmpo cr0, f2, f0
    ble @L_800BADF8
    fmr f2, f0
@L_800BADF8:
    lwz r0, 0x4(r3)
    rlwinm r0, r0, 0, 0, 23
    rlwimi r0, r4, 0, 24, 31
    stw r0, 0x4(r3)
    lfs f0, -29528(r2)
    lwz r0, 0x4(r3)
    fmuls f0, f0, f2
    rlwinm r4, r0, 0, 24, 15
    fctiwz f0, f0
    stfd f0, 0x30(r1)
    lwz r0, 0x34(r1)
    rlwimi r4, r0, 8, 16, 23
    stw r4, 0x4(r3)
    addi r1, r1, 0x38
    blr
