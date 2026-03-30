    nofralloc
    cmplwi r3, 0x0
    bne @L_801431BC
    li r3, 0x16
    blr
    @L_801431BC:
    bne @L_801431C8
    li r4, 0x0
    b @L_801431DC
    @L_801431C8:
    lbz r4, 0x0(r3)
    extrwi r5, r4, 1, 26
    neg r4, r5
    or r4, r4, r5
    srwi r4, r4, 31
    @L_801431DC:
    clrlwi. r4, r4, 24
    bne @L_801432B0
    cmplwi r3, 0x0
    bne @L_801431F4
    li r4, 0x0
    b @L_801431FC
    @L_801431F4:
    lbz r4, 0x0(r3)
    extrwi r4, r4, 4, 27
    @L_801431FC:
    clrlwi. r4, r4, 24
    bne @L_801432B0
    cmplwi r3, 0x0
    bne @L_80143214
    li r4, 0x0
    b @L_8014321C
    @L_80143214:
    lbz r4, 0x1(r3)
    extrwi r4, r4, 4, 24
    @L_8014321C:
    clrlwi. r4, r4, 24
    bne @L_801432B0
    cmplwi r3, 0x0
    bne @L_80143234
    li r4, 0x0
    b @L_8014323C
    @L_80143234:
    lbz r4, 0x1(r3)
    clrlwi r4, r4, 28
    @L_8014323C:
    clrlwi. r4, r4, 24
    bne @L_801432B0
    cmplwi r3, 0x0
    bne @L_80143254
    li r4, 0x0
    b @L_8014325C
    @L_80143254:
    lbz r4, 0x2(r3)
    extrwi r4, r4, 4, 24
    @L_8014325C:
    clrlwi. r4, r4, 24
    bne @L_801432B0
    cmplwi r3, 0x0
    bne @L_80143274
    li r4, 0x0
    b @L_8014327C
    @L_80143274:
    lbz r4, 0x2(r3)
    clrlwi r4, r4, 28
    @L_8014327C:
    clrlwi. r4, r4, 24
    bne @L_801432B0
    cmplwi r3, 0x0
    bne @L_80143294
    li r4, 0x0
    b @L_801432A8
    @L_80143294:
    lbz r4, 0x3(r3)
    extrwi r5, r4, 1, 24
    neg r4, r5
    or r4, r4, r5
    srwi r4, r4, 31
    @L_801432A8:
    clrlwi. r4, r4, 24
    beq @L_801432B8
    @L_801432B0:
    li r3, 0x0
    blr
    @L_801432B8:
    cmplwi r3, 0x0
    bne @L_801432C8
    li r4, 0x0
    b @L_801432DC
    @L_801432C8:
    lbz r4, 0x0(r3)
    extrwi r5, r4, 1, 25
    neg r4, r5
    or r4, r4, r5
    srwi r4, r4, 31
    @L_801432DC:
    clrlwi. r4, r4, 24
    beq @L_801432EC
    li r3, 0x1
    blr
    @L_801432EC:
    cmplwi r3, 0x0
    bne @L_801432FC
    li r4, 0x0
    b @L_80143310
    @L_801432FC:
    lbz r4, 0x3(r3)
    extrwi r5, r4, 1, 25
    neg r4, r5
    or r4, r4, r5
    srwi r4, r4, 31
    @L_80143310:
    clrlwi. r4, r4, 24
    beq @L_80143320
    li r3, 0x2
    blr
    @L_80143320:
    cmplwi r3, 0x0
    li r4, 0x0
    bne @L_80143334
    li r5, 0x0
    b @L_80143348
    @L_80143334:
    lbz r5, 0x3(r3)
    extrwi r6, r5, 1, 26
    neg r5, r6
    or r5, r5, r6
    srwi r5, r5, 31
    @L_80143348:
    clrlwi. r5, r5, 24
    beq @L_80143358
    li r0, 0x3
    li r4, 0x1
    @L_80143358:
    cmplwi r3, 0x0
    bne @L_80143368
    li r5, 0x0
    b @L_8014337C
    @L_80143368:
    lbz r5, 0x3(r3)
    extrwi r6, r5, 1, 27
    neg r5, r6
    or r5, r5, r6
    srwi r5, r5, 31
    @L_8014337C:
    clrlwi. r5, r5, 24
    beq @L_8014338C
    li r0, 0x4
    addi r4, r4, 0x1
    @L_8014338C:
    cmplwi r3, 0x0
    bne @L_8014339C
    li r5, 0x0
    b @L_801433B0
    @L_8014339C:
    lbz r5, 0x3(r3)
    extrwi r6, r5, 1, 28
    neg r5, r6
    or r5, r5, r6
    srwi r5, r5, 31
    @L_801433B0:
    clrlwi. r5, r5, 24
    beq @L_801433C0
    li r0, 0x5
    addi r4, r4, 0x1
    @L_801433C0:
    cmplwi r3, 0x0
    bne @L_801433D0
    li r5, 0x0
    b @L_801433E4
    @L_801433D0:
    lbz r5, 0x3(r3)
    extrwi r6, r5, 1, 29
    neg r5, r6
    or r5, r5, r6
    srwi r5, r5, 31
    @L_801433E4:
    clrlwi. r5, r5, 24
    beq @L_801433F4
    li r0, 0x6
    addi r4, r4, 0x1
    @L_801433F4:
    cmplwi r3, 0x0
    bne @L_80143404
    li r5, 0x0
    b @L_80143418
    @L_80143404:
    lbz r5, 0x3(r3)
    extrwi r6, r5, 1, 30
    neg r5, r6
    or r5, r5, r6
    srwi r5, r5, 31
    @L_80143418:
    clrlwi. r5, r5, 24
    beq @L_80143428
    li r0, 0x7
    addi r4, r4, 0x1
    @L_80143428:
    cmplwi r3, 0x0
    bne @L_80143438
    li r5, 0x0
    b @L_8014344C
    @L_80143438:
    lbz r5, 0x3(r3)
    clrlwi r6, r5, 31
    neg r5, r6
    or r5, r5, r6
    srwi r5, r5, 31
    @L_8014344C:
    clrlwi. r5, r5, 24
    beq @L_8014345C
    li r0, 0x8
    addi r4, r4, 0x1
    @L_8014345C:
    cmplwi r3, 0x0
    bne @L_8014346C
    li r5, 0x0
    b @L_80143480
    @L_8014346C:
    lbz r5, 0x0(r3)
    extrwi r6, r5, 1, 24
    neg r5, r6
    or r5, r5, r6
    srwi r5, r5, 31
    @L_80143480:
    clrlwi. r5, r5, 24
    beq @L_80143490
    li r0, 0x9
    addi r4, r4, 0x1
    @L_80143490:
    cmpwi r4, 0x0
    ble @L_801434AC
    cmpwi r4, 0x1
    mr r3, r0
    blelr
    li r3, 0xa
    blr
    @L_801434AC:
    cmplwi r3, 0x0
    bne @L_801434BC
    li r0, 0x0
    b @L_801434C0
    @L_801434BC:
    lbz r0, 0xa(r3)
    @L_801434C0:
    clrlwi. r0, r0, 24
    beq @L_801434D0
    li r3, 0xb
    blr
    @L_801434D0:
    cmplwi r3, 0x0
    bne @L_801434E0
    li r0, 0x0
    b @L_801434E4
    @L_801434E0:
    lbz r0, 0x9(r3)
    @L_801434E4:
    clrlwi. r0, r0, 24
    beq @L_801434F4
    li r3, 0xc
    blr
    @L_801434F4:
    cmplwi r3, 0x0
    bne @L_80143504
    li r0, 0x0
    b @L_80143508
    @L_80143504:
    lbz r0, 0x8(r3)
    @L_80143508:
    clrlwi. r0, r0, 24
    beq @L_80143518
    li r3, 0xd
    blr
    @L_80143518:
    cmplwi r3, 0x0
    bne @L_80143528
    li r0, 0x0
    b @L_8014352C
    @L_80143528:
    lbz r0, 0xf(r3)
    @L_8014352C:
    clrlwi. r0, r0, 24
    beq @L_8014353C
    li r3, 0xe
    blr
    @L_8014353C:
    cmplwi r3, 0x0
    bne @L_8014354C
    li r0, 0x0
    b @L_80143550
    @L_8014354C:
    lbz r0, 0xe(r3)
    @L_80143550:
    clrlwi. r0, r0, 24
    beq @L_80143560
    li r3, 0xf
    blr
    @L_80143560:
    cmplwi r3, 0x0
    bne @L_80143570
    li r0, 0x0
    b @L_80143574
    @L_80143570:
    lbz r0, 0xd(r3)
    @L_80143574:
    clrlwi. r0, r0, 24
    beq @L_80143584
    li r3, 0x10
    blr
    @L_80143584:
    cmplwi r3, 0x0
    bne @L_80143594
    li r0, 0x0
    b @L_80143598
    @L_80143594:
    lbz r0, 0xc(r3)
    @L_80143598:
    clrlwi. r0, r0, 24
    beq @L_801435A8
    li r3, 0x11
    blr
    @L_801435A8:
    cmplwi r3, 0x0
    bne @L_801435B8
    li r0, 0x0
    b @L_801435CC
    @L_801435B8:
    lbz r0, 0x4(r3)
    extrwi r4, r0, 1, 27
    neg r0, r4
    or r0, r0, r4
    srwi r0, r0, 31
    @L_801435CC:
    clrlwi. r0, r0, 24
    beq @L_801435DC
    li r3, 0x12
    blr
    @L_801435DC:
    cmplwi r3, 0x0
    bne @L_801435EC
    li r0, 0x0
    b @L_80143600
    @L_801435EC:
    lbz r0, 0x4(r3)
    extrwi r4, r0, 1, 24
    neg r0, r4
    or r0, r0, r4
    srwi r0, r0, 31
    @L_80143600:
    clrlwi. r0, r0, 24
    beq @L_80143610
    li r3, 0x13
    blr
    @L_80143610:
    cmplwi r3, 0x0
    bne @L_80143620
    li r0, 0x0
    b @L_80143634
    @L_80143620:
    lbz r0, 0x4(r3)
    extrwi r4, r0, 1, 28
    neg r0, r4
    or r0, r0, r4
    srwi r0, r0, 31
    @L_80143634:
    clrlwi. r0, r0, 24
    beq @L_80143644
    li r3, 0x14
    blr
    @L_80143644:
    cmplwi r3, 0x0
    bne @L_80143654
    li r0, 0x0
    b @L_80143668
    @L_80143654:
    lbz r0, 0x4(r3)
    extrwi r4, r0, 1, 26
    neg r0, r4
    or r0, r0, r4
    srwi r0, r0, 31
    @L_80143668:
    clrlwi. r0, r0, 24
    bne @L_8014368C
    cmplwi r3, 0x0
    bne @L_80143680
    li r0, 0x0
    b @L_80143684
    @L_80143680:
    lbz r0, 0xb(r3)
    @L_80143684:
    clrlwi. r0, r0, 24
    beq @L_80143694
    @L_8014368C:
    li r3, 0x15
    blr
    @L_80143694:
    li r3, 0x16
    blr
