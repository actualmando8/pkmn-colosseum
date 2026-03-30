    nofralloc
    rlwinm. r0, r4, 0, 16, 31
    li r5, 0x0
    bne @L_801DDC24
    li r3, 0x0
    blr
@L_801DDC24:
    cmplwi r0, 0x4
    bne @L_801DDC78
    rlwinm. r3, r3, 0, 16, 31
    beq @L_801DDC40
    lwz r0, -31552(r13)
    cmplw r3, r0
    blt @L_801DDC48
@L_801DDC40:
    li r3, 0x0
    blr
@L_801DDC48:
    mulli r0, r3, 0xc
    lis r3, -32713
    addi r3, r3, 10184
    lwzx r4, r3, r0
    add r3, r3, r0
    lwz r0, 0x4(r3)
    cmplwi r4, 0x0
    beq @L_801DDD20
    cmplwi r0, 0x0
    beq @L_801DDD20
    li r5, 0x1
    b @L_801DDD20
@L_801DDC78:
    rlwinm. r6, r3, 0, 16, 31
    lis r3, -32713
    subi r4, r4, 0x1
    addi r0, r3, 12816
    mr r8, r0
    rlwinm r4, r4, 0, 16, 31
    beq @L_801DDCA0
    lwz r0, -31584(r13)
    cmplw r6, r0
    blt @L_801DDCA8
@L_801DDCA0:
    li r3, 0x0
    blr
@L_801DDCA8:
    mulli r7, r6, 0x1c
    lis r3, -32713
    clrlslwi r4, r4, 16, 3
    addi r0, r3, -7856
    add r0, r0, r7
    add r3, r0, r4
    lwz r0, 0x4(r3)
    lwz r3, 0x8(r3)
    cmplwi r0, 0x0
    beq @L_801DDCDC
    cmplwi r3, 0x0
    beq @L_801DDCDC
    li r5, 0x1
@L_801DDCDC:
    lwz r0, -31544(r13)
    mtctr r0
    cmplwi r0, 0x0
    ble @L_801DDD20
@L_801DDCEC:
    lwz r0, 0x0(r8)
    cmplw r0, r6
    bne @L_801DDD18
    add r3, r4, r8
    lwz r0, 0x8(r3)
    lwz r3, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_801DDD18
    cmplwi r3, 0x0
    beq @L_801DDD18
    addi r5, r5, 0x1
@L_801DDD18:
    addi r8, r8, 0x20
    bdnz @L_801DDCEC
@L_801DDD20:
    mr r3, r5
    blr
