    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stw r31, 0x3c(r1)
    mr r31, r3
    stw r30, 0x38(r1)
    lwz r4, 0x8(r3)
    lwz r3, 0x4(r3)
    lwz r0, 0xc(r31)
    subf r3, r4, r3
    cmplw r3, r0
    blt @L_8019A284
    li r3, 0x6
    b @L_8019B478
@L_8019A284:
    lbz r0, 0x11(r31)
    stb r0, 0x12(r31)
    lhz r0, 0x16(r31)
    cmplwi r0, 0x0
    bne @L_8019A2FC
    lwz r3, 0x4(r31)
    li r4, 0x3
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 0, 28, 31
    stb r0, 0x11(r31)
    lwz r3, 0x4(r31)
    addi r0, r3, 0x1
    stw r0, 0x4(r31)
    lbz r3, 0x0(r3)
    rlwinm. r0, r3, 0, 24, 24
    rlwinm r3, r3, 28, 29, 31
    addi r5, r3, 0x1
    bne @L_8019A2D0
    b @L_8019A2F8
@L_8019A2D0:
    lwz r3, 0x4(r31)
    addi r0, r3, 0x1
    stw r0, 0x4(r31)
    lbz r3, 0x0(r3)
    rlwinm. r0, r3, 0, 24, 24
    rlwinm r3, r3, 0, 25, 31
    slw r3, r3, r4
    addi r4, r4, 0x7
    add r5, r5, r3
    bne @L_8019A2D0
@L_8019A2F8:
    sth r5, 0x16(r31)
@L_8019A2FC:
    lhz r3, 0x16(r31)
    subi r0, r3, 0x1
    sth r0, 0x16(r31)
    lbz r0, 0x11(r31)
    cmpwi r0, 0x4
    beq @L_8019AAFC
    bge @L_8019A330
    cmpwi r0, 0x2
    beq @L_8019A5D8
    bge @L_8019A870
    cmpwi r0, 0x1
    bge @L_8019A340
    b @L_8019B474
@L_8019A330:
    cmpwi r0, 0x6
    beq @L_8019B1C0
    bge @L_8019B474
    b @L_8019AF60
@L_8019A340:
    cmplwi r31, 0x0
    bne @L_8019A350
    li r30, 0x0
    b @L_8019A358
@L_8019A350:
    lbz r0, 0x10(r31)
    rlwinm r30, r0, 0, 28, 31
@L_8019A358:
    subi r4, r30, 0x1
    li r3, 0x1
    subf r0, r4, r3
    li r5, 0x1
    orc r3, r3, r4
    rlwinm r0, r0, 31, 1, 31
    subf r0, r0, r3
    rlwinm. r0, r0, 1, 31, 31
    bne @L_8019A380
    li r5, 0x0
@L_8019A380:
    cmpwi r5, 0x0
    bne @L_8019A39C
    lis r4, -32729
    addi r3, r2, -23664
    addi r5, r4, 18300
    li r4, 0x17f
    bl fn_80196E10
@L_8019A39C:
    lfs f0, 0x24(r31)
    stfs f0, 0x20(r31)
    lbz r5, 0x14(r31)
    rlwinm. r0, r5, 0, 24, 31
    bne @L_8019A42C
    lwz r4, 0x4(r31)
    addi r0, r4, 0x1
    stw r0, 0x4(r31)
    lwz r3, 0x4(r31)
    lbz r5, 0x0(r4)
    addi r0, r3, 0x1
    stw r0, 0x4(r31)
    lwz r4, 0x4(r31)
    stw r5, 0x20(r1)
    lbz r3, 0x0(r3)
    addi r0, r4, 0x1
    lwz r5, 0x20(r1)
    stw r0, 0x4(r31)
    rlwinm r0, r3, 8, 0, 23
    or r0, r5, r0
    stw r0, 0x20(r1)
    lwz r3, 0x4(r31)
    lbz r4, 0x0(r4)
    addi r0, r3, 0x1
    lwz r5, 0x20(r1)
    rlwinm r4, r4, 16, 0, 15
    stw r0, 0x4(r31)
    or r0, r5, r4
    stw r0, 0x20(r1)
    lbz r0, 0x0(r3)
    lwz r3, 0x20(r1)
    rlwinm r0, r0, 24, 0, 7
    or r0, r3, r0
    stw r0, 0x20(r1)
    lfs f0, 0x20(r1)
    b @L_8019A580
@L_8019A42C:
    rlwinm r3, r5, 0, 24, 26
    li r4, 0x1
    cmpwi r3, 0x60
    rlwinm r0, r5, 0, 27, 31
    slw r0, r4, r0
    beq @L_8019A46C
    bge @L_8019A460
    cmpwi r3, 0x40
    beq @L_8019A518
    bge @L_8019A558
    cmpwi r3, 0x20
    beq @L_8019A4D4
    b @L_8019A558
@L_8019A460:
    cmpwi r3, 0x80
    beq @L_8019A4A4
    b @L_8019A558
@L_8019A46C:
    lwz r5, 0x4(r31)
    lis r4, 0x4330
    lwz r3, 0x4(r31)
    lbz r5, 0x0(r5)
    addi r3, r3, 0x1
    stw r4, 0x28(r1)
    extsb r4, r5
    lfd f1, -23624(r2)
    xoris r4, r4, 0x8000
    stw r3, 0x4(r31)
    stw r4, 0x2c(r1)
    lfd f0, 0x28(r1)
    fsubs f2, f0, f1
    b @L_8019A560
@L_8019A4A4:
    lwz r5, 0x4(r31)
    lis r4, 0x4330
    lwz r3, 0x4(r31)
    lbz r5, 0x0(r5)
    addi r3, r3, 0x1
    stw r4, 0x28(r1)
    lfd f1, -23632(r2)
    stw r5, 0x2c(r1)
    lfd f0, 0x28(r1)
    stw r3, 0x4(r31)
    fsubs f2, f0, f1
    b @L_8019A560
@L_8019A4D4:
    lwz r3, 0x4(r31)
    lis r4, 0x4330
    lwz r5, 0x4(r31)
    lbz r6, 0x1(r3)
    lwz r3, 0x4(r31)
    extsb r6, r6
    lbz r5, 0x0(r5)
    rlwimi r5, r6, 8, 0, 23
    stw r4, 0x28(r1)
    xoris r4, r5, 0x8000
    addi r3, r3, 0x2
    stw r4, 0x2c(r1)
    lfd f1, -23624(r2)
    lfd f0, 0x28(r1)
    stw r3, 0x4(r31)
    fsubs f2, f0, f1
    b @L_8019A560
@L_8019A518:
    lwz r5, 0x4(r31)
    lis r4, 0x4330
    lwz r3, 0x4(r31)
    lbz r6, 0x1(r5)
    lbz r5, 0x0(r3)
    rlwimi r5, r6, 8, 16, 23
    stw r4, 0x28(r1)
    xoris r4, r5, 0x8000
    lwz r3, 0x4(r31)
    stw r4, 0x2c(r1)
    addi r3, r3, 0x2
    lfd f1, -23624(r2)
    lfd f0, 0x28(r1)
    stw r3, 0x4(r31)
    fsubs f2, f0, f1
    b @L_8019A560
@L_8019A558:
    lfs f0, -23652(r2)
    b @L_8019A580
@L_8019A560:
    xoris r3, r0, 0x8000
    lis r0, 0x4330
    stw r3, 0x2c(r1)
    lfd f1, -23624(r2)
    stw r0, 0x28(r1)
    lfd f0, 0x28(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
@L_8019A580:
    stfs f0, 0x24(r31)
    lbz r0, 0x12(r31)
    cmplwi r0, 0x5
    beq @L_8019A5A0
    lfs f1, 0x2c(r31)
    lfs f0, -23652(r2)
    stfs f1, 0x28(r31)
    stfs f0, 0x2c(r31)
@L_8019A5A0:
    li r0, 0x1
    cmplwi r31, 0x0
    subf r4, r0, r30
    li r3, 0x4
    subf r0, r30, r0
    nor r0, r4, r0
    srawi r0, r0, 31
    add r3, r0, r3
    beq @L_8019B478
    lbz r0, 0x10(r31)
    rlwinm r0, r0, 0, 24, 27
    rlwimi r0, r3, 0, 28, 31
    stb r0, 0x10(r31)
    b @L_8019B478
@L_8019A5D8:
    cmplwi r31, 0x0
    bne @L_8019A5E8
    li r30, 0x0
    b @L_8019A5F0
@L_8019A5E8:
    lbz r0, 0x10(r31)
    rlwinm r30, r0, 0, 28, 31
@L_8019A5F0:
    subi r4, r30, 0x1
    li r3, 0x1
    subf r0, r4, r3
    li r5, 0x1
    orc r3, r3, r4
    rlwinm r0, r0, 31, 1, 31
    subf r0, r0, r3
    rlwinm. r0, r0, 1, 31, 31
    bne @L_8019A618
    li r5, 0x0
@L_8019A618:
    cmpwi r5, 0x0
    bne @L_8019A634
    lis r4, -32729
    addi r3, r2, -23664
    addi r5, r4, 18300
    li r4, 0x193
    bl fn_80196E10
@L_8019A634:
    lfs f0, 0x24(r31)
    stfs f0, 0x20(r31)
    lbz r5, 0x14(r31)
    rlwinm. r0, r5, 0, 24, 31
    bne @L_8019A6C4
    lwz r4, 0x4(r31)
    addi r0, r4, 0x1
    stw r0, 0x4(r31)
    lwz r3, 0x4(r31)
    lbz r5, 0x0(r4)
    addi r0, r3, 0x1
    stw r0, 0x4(r31)
    lwz r4, 0x4(r31)
    stw r5, 0x1c(r1)
    lbz r3, 0x0(r3)
    addi r0, r4, 0x1
    lwz r5, 0x1c(r1)
    stw r0, 0x4(r31)
    rlwinm r0, r3, 8, 0, 23
    or r0, r5, r0
    stw r0, 0x1c(r1)
    lwz r3, 0x4(r31)
    lbz r4, 0x0(r4)
    addi r0, r3, 0x1
    lwz r5, 0x1c(r1)
    rlwinm r4, r4, 16, 0, 15
    stw r0, 0x4(r31)
    or r0, r5, r4
    stw r0, 0x1c(r1)
    lbz r0, 0x0(r3)
    lwz r3, 0x1c(r1)
    rlwinm r0, r0, 24, 0, 7
    or r0, r3, r0
    stw r0, 0x1c(r1)
    lfs f0, 0x1c(r1)
    b @L_8019A818
@L_8019A6C4:
    rlwinm r3, r5, 0, 24, 26
    li r4, 0x1
    cmpwi r3, 0x60
    rlwinm r0, r5, 0, 27, 31
    slw r0, r4, r0
    beq @L_8019A704
    bge @L_8019A6F8
    cmpwi r3, 0x40
    beq @L_8019A7B0
    bge @L_8019A7F0
    cmpwi r3, 0x20
    beq @L_8019A76C
    b @L_8019A7F0
@L_8019A6F8:
    cmpwi r3, 0x80
    beq @L_8019A73C
    b @L_8019A7F0
@L_8019A704:
    lwz r5, 0x4(r31)
    lis r4, 0x4330
    lwz r3, 0x4(r31)
    lbz r5, 0x0(r5)
    addi r3, r3, 0x1
    stw r4, 0x28(r1)
    extsb r4, r5
    lfd f1, -23624(r2)
    xoris r4, r4, 0x8000
    stw r3, 0x4(r31)
    stw r4, 0x2c(r1)
    lfd f0, 0x28(r1)
    fsubs f2, f0, f1
    b @L_8019A7F8
@L_8019A73C:
    lwz r5, 0x4(r31)
    lis r4, 0x4330
    lwz r3, 0x4(r31)
    lbz r5, 0x0(r5)
    addi r3, r3, 0x1
    stw r4, 0x28(r1)
    lfd f1, -23632(r2)
    stw r5, 0x2c(r1)
    lfd f0, 0x28(r1)
    stw r3, 0x4(r31)
    fsubs f2, f0, f1
    b @L_8019A7F8
@L_8019A76C:
    lwz r3, 0x4(r31)
    lis r4, 0x4330
    lwz r5, 0x4(r31)
    lbz r6, 0x1(r3)
    lwz r3, 0x4(r31)
    extsb r6, r6
    lbz r5, 0x0(r5)
    rlwimi r5, r6, 8, 0, 23
    stw r4, 0x28(r1)
    xoris r4, r5, 0x8000
    addi r3, r3, 0x2
    stw r4, 0x2c(r1)
    lfd f1, -23624(r2)
    lfd f0, 0x28(r1)
    stw r3, 0x4(r31)
    fsubs f2, f0, f1
    b @L_8019A7F8
@L_8019A7B0:
    lwz r5, 0x4(r31)
    lis r4, 0x4330
    lwz r3, 0x4(r31)
    lbz r6, 0x1(r5)
    lbz r5, 0x0(r3)
    rlwimi r5, r6, 8, 16, 23
    stw r4, 0x28(r1)
    xoris r4, r5, 0x8000
    lwz r3, 0x4(r31)
    stw r4, 0x2c(r1)
    addi r3, r3, 0x2
    lfd f1, -23624(r2)
    lfd f0, 0x28(r1)
    stw r3, 0x4(r31)
    fsubs f2, f0, f1
    b @L_8019A7F8
@L_8019A7F0:
    lfs f0, -23652(r2)
    b @L_8019A818
@L_8019A7F8:
    xoris r3, r0, 0x8000
    lis r0, 0x4330
    stw r3, 0x2c(r1)
    lfd f1, -23624(r2)
    stw r0, 0x28(r1)
    lfd f0, 0x28(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
@L_8019A818:
    stfs f0, 0x24(r31)
    lbz r0, 0x12(r31)
    cmplwi r0, 0x5
    beq @L_8019A838
    lfs f1, 0x2c(r31)
    lfs f0, -23652(r2)
    stfs f1, 0x28(r31)
    stfs f0, 0x2c(r31)
@L_8019A838:
    li r0, 0x1
    cmplwi r31, 0x0
    subf r4, r0, r30
    li r3, 0x4
    subf r0, r30, r0
    nor r0, r4, r0
    srawi r0, r0, 31
    add r3, r0, r3
    beq @L_8019B478
    lbz r0, 0x10(r31)
    rlwinm r0, r0, 0, 24, 27
    rlwimi r0, r3, 0, 28, 31
    stb r0, 0x10(r31)
    b @L_8019B478
@L_8019A870:
    cmplwi r31, 0x0
    bne @L_8019A880
    li r30, 0x0
    b @L_8019A888
@L_8019A880:
    lbz r0, 0x10(r31)
    rlwinm r30, r0, 0, 28, 31
@L_8019A888:
    subi r4, r30, 0x1
    li r3, 0x1
    subf r0, r4, r3
    li r5, 0x1
    orc r3, r3, r4
    rlwinm r0, r0, 31, 1, 31
    subf r0, r0, r3
    rlwinm. r0, r0, 1, 31, 31
    bne @L_8019A8B0
    li r5, 0x0
@L_8019A8B0:
    cmpwi r5, 0x0
    bne @L_8019A8CC
    lis r4, -32729
    addi r3, r2, -23664
    addi r5, r4, 18300
    li r4, 0x1a7
    bl fn_80196E10
@L_8019A8CC:
    lfs f0, 0x24(r31)
    stfs f0, 0x20(r31)
    lfs f0, 0x2c(r31)
    stfs f0, 0x28(r31)
    lbz r5, 0x14(r31)
    rlwinm. r0, r5, 0, 24, 31
    bne @L_8019A964
    lwz r4, 0x4(r31)
    addi r0, r4, 0x1
    stw r0, 0x4(r31)
    lwz r3, 0x4(r31)
    lbz r5, 0x0(r4)
    addi r0, r3, 0x1
    stw r0, 0x4(r31)
    lwz r4, 0x4(r31)
    stw r5, 0x18(r1)
    lbz r3, 0x0(r3)
    addi r0, r4, 0x1
    lwz r5, 0x18(r1)
    stw r0, 0x4(r31)
    rlwinm r0, r3, 8, 0, 23
    or r0, r5, r0
    stw r0, 0x18(r1)
    lwz r3, 0x4(r31)
    lbz r4, 0x0(r4)
    addi r0, r3, 0x1
    lwz r5, 0x18(r1)
    rlwinm r4, r4, 16, 0, 15
    stw r0, 0x4(r31)
    or r0, r5, r4
    stw r0, 0x18(r1)
    lbz r0, 0x0(r3)
    lwz r3, 0x18(r1)
    rlwinm r0, r0, 24, 0, 7
    or r0, r3, r0
    stw r0, 0x18(r1)
    lfs f0, 0x18(r1)
    b @L_8019AAB8
@L_8019A964:
    rlwinm r3, r5, 0, 24, 26
    li r4, 0x1
    cmpwi r3, 0x60
    rlwinm r0, r5, 0, 27, 31
    slw r0, r4, r0
    beq @L_8019A9A4
    bge @L_8019A998
    cmpwi r3, 0x40
    beq @L_8019AA50
    bge @L_8019AA90
    cmpwi r3, 0x20
    beq @L_8019AA0C
    b @L_8019AA90
@L_8019A998:
    cmpwi r3, 0x80
    beq @L_8019A9DC
    b @L_8019AA90
@L_8019A9A4:
    lwz r5, 0x4(r31)
    lis r4, 0x4330
    lwz r3, 0x4(r31)
    lbz r5, 0x0(r5)
    addi r3, r3, 0x1
    stw r4, 0x28(r1)
    extsb r4, r5
    lfd f1, -23624(r2)
    xoris r4, r4, 0x8000
    stw r3, 0x4(r31)
    stw r4, 0x2c(r1)
    lfd f0, 0x28(r1)
    fsubs f2, f0, f1
    b @L_8019AA98
@L_8019A9DC:
    lwz r5, 0x4(r31)
    lis r4, 0x4330
    lwz r3, 0x4(r31)
    lbz r5, 0x0(r5)
    addi r3, r3, 0x1
    stw r4, 0x28(r1)
    lfd f1, -23632(r2)
    stw r5, 0x2c(r1)
    lfd f0, 0x28(r1)
    stw r3, 0x4(r31)
    fsubs f2, f0, f1
    b @L_8019AA98
@L_8019AA0C:
    lwz r3, 0x4(r31)
    lis r4, 0x4330
    lwz r5, 0x4(r31)
    lbz r6, 0x1(r3)
    lwz r3, 0x4(r31)
    extsb r6, r6
    lbz r5, 0x0(r5)
    rlwimi r5, r6, 8, 0, 23
    stw r4, 0x28(r1)
    xoris r4, r5, 0x8000
    addi r3, r3, 0x2
    stw r4, 0x2c(r1)
    lfd f1, -23624(r2)
    lfd f0, 0x28(r1)
    stw r3, 0x4(r31)
    fsubs f2, f0, f1
    b @L_8019AA98
@L_8019AA50:
    lwz r5, 0x4(r31)
    lis r4, 0x4330
    lwz r3, 0x4(r31)
    lbz r6, 0x1(r5)
    lbz r5, 0x0(r3)
    rlwimi r5, r6, 8, 16, 23
    stw r4, 0x28(r1)
    xoris r4, r5, 0x8000
    lwz r3, 0x4(r31)
    stw r4, 0x2c(r1)
    addi r3, r3, 0x2
    lfd f1, -23624(r2)
    lfd f0, 0x28(r1)
    stw r3, 0x4(r31)
    fsubs f2, f0, f1
    b @L_8019AA98
@L_8019AA90:
    lfs f0, -23652(r2)
    b @L_8019AAB8
@L_8019AA98:
    xoris r3, r0, 0x8000
    lis r0, 0x4330
    stw r3, 0x2c(r1)
    lfd f1, -23624(r2)
    stw r0, 0x28(r1)
    lfd f0, 0x28(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
@L_8019AAB8:
    li r0, 0x1
    stfs f0, 0x24(r31)
    subf r3, r0, r30
    lfs f0, -23652(r2)
    subf r0, r30, r0
    cmplwi r31, 0x0
    nor r0, r3, r0
    li r3, 0x4
    srawi r0, r0, 31
    stfs f0, 0x2c(r31)
    add r3, r0, r3
    beq @L_8019B478
    lbz r0, 0x10(r31)
    rlwinm r0, r0, 0, 24, 27
    rlwimi r0, r3, 0, 28, 31
    stb r0, 0x10(r31)
    b @L_8019B478
@L_8019AAFC:
    cmplwi r31, 0x0
    bne @L_8019AB0C
    li r30, 0x0
    b @L_8019AB14
@L_8019AB0C:
    lbz r0, 0x10(r31)
    rlwinm r30, r0, 0, 28, 31
@L_8019AB14:
    subi r4, r30, 0x1
    li r3, 0x1
    subf r0, r4, r3
    li r5, 0x1
    orc r3, r3, r4
    rlwinm r0, r0, 31, 1, 31
    subf r0, r0, r3
    rlwinm. r0, r0, 1, 31, 31
    bne @L_8019AB3C
    li r5, 0x0
@L_8019AB3C:
    cmpwi r5, 0x0
    bne @L_8019AB58
    lis r4, -32729
    addi r3, r2, -23664
    addi r5, r4, 18300
    li r4, 0x1b9
    bl fn_80196E10
@L_8019AB58:
    lfs f0, 0x24(r31)
    stfs f0, 0x20(r31)
    lbz r5, 0x14(r31)
    rlwinm. r0, r5, 0, 24, 31
    bne @L_8019ABE8
    lwz r4, 0x4(r31)
    addi r0, r4, 0x1
    stw r0, 0x4(r31)
    lwz r3, 0x4(r31)
    lbz r5, 0x0(r4)
    addi r0, r3, 0x1
    stw r0, 0x4(r31)
    lwz r4, 0x4(r31)
    stw r5, 0x14(r1)
    lbz r3, 0x0(r3)
    addi r0, r4, 0x1
    lwz r5, 0x14(r1)
    stw r0, 0x4(r31)
    rlwinm r0, r3, 8, 0, 23
    or r0, r5, r0
    stw r0, 0x14(r1)
    lwz r3, 0x4(r31)
    lbz r4, 0x0(r4)
    addi r0, r3, 0x1
    lwz r5, 0x14(r1)
    rlwinm r4, r4, 16, 0, 15
    stw r0, 0x4(r31)
    or r0, r5, r4
    stw r0, 0x14(r1)
    lbz r0, 0x0(r3)
    lwz r3, 0x14(r1)
    rlwinm r0, r0, 24, 0, 7
    or r0, r3, r0
    stw r0, 0x14(r1)
    lfs f0, 0x14(r1)
    b @L_8019AD3C
@L_8019ABE8:
    rlwinm r3, r5, 0, 24, 26
    li r4, 0x1
    cmpwi r3, 0x60
    rlwinm r0, r5, 0, 27, 31
    slw r0, r4, r0
    beq @L_8019AC28
    bge @L_8019AC1C
    cmpwi r3, 0x40
    beq @L_8019ACD4
    bge @L_8019AD14
    cmpwi r3, 0x20
    beq @L_8019AC90
    b @L_8019AD14
@L_8019AC1C:
    cmpwi r3, 0x80
    beq @L_8019AC60
    b @L_8019AD14
@L_8019AC28:
    lwz r5, 0x4(r31)
    lis r4, 0x4330
    lwz r3, 0x4(r31)
    lbz r5, 0x0(r5)
    addi r3, r3, 0x1
    stw r4, 0x28(r1)
    extsb r4, r5
    lfd f1, -23624(r2)
    xoris r4, r4, 0x8000
    stw r3, 0x4(r31)
    stw r4, 0x2c(r1)
    lfd f0, 0x28(r1)
    fsubs f2, f0, f1
    b @L_8019AD1C
@L_8019AC60:
    lwz r5, 0x4(r31)
    lis r4, 0x4330
    lwz r3, 0x4(r31)
    lbz r5, 0x0(r5)
    addi r3, r3, 0x1
    stw r4, 0x28(r1)
    lfd f1, -23632(r2)
    stw r5, 0x2c(r1)
    lfd f0, 0x28(r1)
    stw r3, 0x4(r31)
    fsubs f2, f0, f1
    b @L_8019AD1C
@L_8019AC90:
    lwz r3, 0x4(r31)
    lis r4, 0x4330
    lwz r5, 0x4(r31)
    lbz r6, 0x1(r3)
    lwz r3, 0x4(r31)
    extsb r6, r6
    lbz r5, 0x0(r5)
    rlwimi r5, r6, 8, 0, 23
    stw r4, 0x28(r1)
    xoris r4, r5, 0x8000
    addi r3, r3, 0x2
    stw r4, 0x2c(r1)
    lfd f1, -23624(r2)
    lfd f0, 0x28(r1)
    stw r3, 0x4(r31)
    fsubs f2, f0, f1
    b @L_8019AD1C
@L_8019ACD4:
    lwz r5, 0x4(r31)
    lis r4, 0x4330
    lwz r3, 0x4(r31)
    lbz r6, 0x1(r5)
    lbz r5, 0x0(r3)
    rlwimi r5, r6, 8, 16, 23
    stw r4, 0x28(r1)
    xoris r4, r5, 0x8000
    lwz r3, 0x4(r31)
    stw r4, 0x2c(r1)
    addi r3, r3, 0x2
    lfd f1, -23624(r2)
    lfd f0, 0x28(r1)
    stw r3, 0x4(r31)
    fsubs f2, f0, f1
    b @L_8019AD1C
@L_8019AD14:
    lfs f0, -23652(r2)
    b @L_8019AD3C
@L_8019AD1C:
    xoris r3, r0, 0x8000
    lis r0, 0x4330
    stw r3, 0x2c(r1)
    lfd f1, -23624(r2)
    stw r0, 0x28(r1)
    lfd f0, 0x28(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
@L_8019AD3C:
    stfs f0, 0x24(r31)
    lfs f0, 0x2c(r31)
    stfs f0, 0x28(r31)
    lbz r5, 0x15(r31)
    rlwinm. r0, r5, 0, 24, 31
    bne @L_8019ADD0
    lwz r4, 0x4(r31)
    addi r0, r4, 0x1
    stw r0, 0x4(r31)
    lwz r3, 0x4(r31)
    lbz r5, 0x0(r4)
    addi r0, r3, 0x1
    stw r0, 0x4(r31)
    lwz r4, 0x4(r31)
    stw r5, 0x10(r1)
    lbz r3, 0x0(r3)
    addi r0, r4, 0x1
    lwz r5, 0x10(r1)
    stw r0, 0x4(r31)
    rlwinm r0, r3, 8, 0, 23
    or r0, r5, r0
    stw r0, 0x10(r1)
    lwz r3, 0x4(r31)
    lbz r4, 0x0(r4)
    addi r0, r3, 0x1
    lwz r5, 0x10(r1)
    rlwinm r4, r4, 16, 0, 15
    stw r0, 0x4(r31)
    or r0, r5, r4
    stw r0, 0x10(r1)
    lbz r0, 0x0(r3)
    lwz r3, 0x10(r1)
    rlwinm r0, r0, 24, 0, 7
    or r0, r3, r0
    stw r0, 0x10(r1)
    lfs f0, 0x10(r1)
    b @L_8019AF24
@L_8019ADD0:
    rlwinm r3, r5, 0, 24, 26
    li r4, 0x1
    cmpwi r3, 0x60
    rlwinm r0, r5, 0, 27, 31
    slw r0, r4, r0
    beq @L_8019AE10
    bge @L_8019AE04
    cmpwi r3, 0x40
    beq @L_8019AEBC
    bge @L_8019AEFC
    cmpwi r3, 0x20
    beq @L_8019AE78
    b @L_8019AEFC
@L_8019AE04:
    cmpwi r3, 0x80
    beq @L_8019AE48
    b @L_8019AEFC
@L_8019AE10:
    lwz r5, 0x4(r31)
    lis r4, 0x4330
    lwz r3, 0x4(r31)
    lbz r5, 0x0(r5)
    addi r3, r3, 0x1
    stw r4, 0x28(r1)
    extsb r4, r5
    lfd f1, -23624(r2)
    xoris r4, r4, 0x8000
    stw r3, 0x4(r31)
    stw r4, 0x2c(r1)
    lfd f0, 0x28(r1)
    fsubs f2, f0, f1
    b @L_8019AF04
@L_8019AE48:
    lwz r5, 0x4(r31)
    lis r4, 0x4330
    lwz r3, 0x4(r31)
    lbz r5, 0x0(r5)
    addi r3, r3, 0x1
    stw r4, 0x28(r1)
    lfd f1, -23632(r2)
    stw r5, 0x2c(r1)
    lfd f0, 0x28(r1)
    stw r3, 0x4(r31)
    fsubs f2, f0, f1
    b @L_8019AF04
@L_8019AE78:
    lwz r3, 0x4(r31)
    lis r4, 0x4330
    lwz r5, 0x4(r31)
    lbz r6, 0x1(r3)
    lwz r3, 0x4(r31)
    extsb r6, r6
    lbz r5, 0x0(r5)
    rlwimi r5, r6, 8, 0, 23
    stw r4, 0x28(r1)
    xoris r4, r5, 0x8000
    addi r3, r3, 0x2
    stw r4, 0x2c(r1)
    lfd f1, -23624(r2)
    lfd f0, 0x28(r1)
    stw r3, 0x4(r31)
    fsubs f2, f0, f1
    b @L_8019AF04
@L_8019AEBC:
    lwz r5, 0x4(r31)
    lis r4, 0x4330
    lwz r3, 0x4(r31)
    lbz r6, 0x1(r5)
    lbz r5, 0x0(r3)
    rlwimi r5, r6, 8, 16, 23
    stw r4, 0x28(r1)
    xoris r4, r5, 0x8000
    lwz r3, 0x4(r31)
    stw r4, 0x2c(r1)
    addi r3, r3, 0x2
    lfd f1, -23624(r2)
    lfd f0, 0x28(r1)
    stw r3, 0x4(r31)
    fsubs f2, f0, f1
    b @L_8019AF04
@L_8019AEFC:
    lfs f0, -23652(r2)
    b @L_8019AF24
@L_8019AF04:
    xoris r3, r0, 0x8000
    lis r0, 0x4330
    stw r3, 0x2c(r1)
    lfd f1, -23624(r2)
    stw r0, 0x28(r1)
    lfd f0, 0x28(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
@L_8019AF24:
    li r0, 0x1
    cmplwi r31, 0x0
    subf r4, r0, r30
    li r3, 0x4
    subf r0, r30, r0
    stfs f0, 0x2c(r31)
    nor r0, r4, r0
    srawi r0, r0, 31
    add r3, r0, r3
    beq @L_8019B478
    lbz r0, 0x10(r31)
    rlwinm r0, r0, 0, 24, 27
    rlwimi r0, r3, 0, 28, 31
    stb r0, 0x10(r31)
    b @L_8019B478
@L_8019AF60:
    cmplwi r31, 0x0
    bne @L_8019AF70
    li r3, 0x0
    b @L_8019AF78
@L_8019AF70:
    lbz r0, 0x10(r31)
    rlwinm r3, r0, 0, 28, 31
@L_8019AF78:
    subi r4, r3, 0x1
    li r3, 0x1
    subf r0, r4, r3
    li r5, 0x1
    orc r3, r3, r4
    rlwinm r0, r0, 31, 1, 31
    subf r0, r0, r3
    rlwinm. r0, r0, 1, 31, 31
    bne @L_8019AFA0
    li r5, 0x0
@L_8019AFA0:
    cmpwi r5, 0x0
    bne @L_8019AFBC
    lis r4, -32729
    addi r3, r2, -23664
    addi r5, r4, 18300
    li r4, 0x1cc
    bl fn_80196E10
@L_8019AFBC:
    lfs f0, 0x2c(r31)
    stfs f0, 0x28(r31)
    lbz r5, 0x15(r31)
    rlwinm. r0, r5, 0, 24, 31
    bne @L_8019B04C
    lwz r4, 0x4(r31)
    addi r0, r4, 0x1
    stw r0, 0x4(r31)
    lwz r3, 0x4(r31)
    lbz r5, 0x0(r4)
    addi r0, r3, 0x1
    stw r0, 0x4(r31)
    lwz r4, 0x4(r31)
    stw r5, 0xc(r1)
    lbz r3, 0x0(r3)
    addi r0, r4, 0x1
    lwz r5, 0xc(r1)
    stw r0, 0x4(r31)
    rlwinm r0, r3, 8, 0, 23
    or r0, r5, r0
    stw r0, 0xc(r1)
    lwz r3, 0x4(r31)
    lbz r4, 0x0(r4)
    addi r0, r3, 0x1
    lwz r5, 0xc(r1)
    rlwinm r4, r4, 16, 0, 15
    stw r0, 0x4(r31)
    or r0, r5, r4
    stw r0, 0xc(r1)
    lbz r0, 0x0(r3)
    lwz r3, 0xc(r1)
    rlwinm r0, r0, 24, 0, 7
    or r0, r3, r0
    stw r0, 0xc(r1)
    lfs f0, 0xc(r1)
    b @L_8019B1A0
@L_8019B04C:
    rlwinm r3, r5, 0, 24, 26
    li r4, 0x1
    cmpwi r3, 0x60
    rlwinm r0, r5, 0, 27, 31
    slw r0, r4, r0
    beq @L_8019B08C
    bge @L_8019B080
    cmpwi r3, 0x40
    beq @L_8019B138
    bge @L_8019B178
    cmpwi r3, 0x20
    beq @L_8019B0F4
    b @L_8019B178
@L_8019B080:
    cmpwi r3, 0x80
    beq @L_8019B0C4
    b @L_8019B178
@L_8019B08C:
    lwz r5, 0x4(r31)
    lis r4, 0x4330
    lwz r3, 0x4(r31)
    lbz r5, 0x0(r5)
    addi r3, r3, 0x1
    stw r4, 0x28(r1)
    extsb r4, r5
    lfd f1, -23624(r2)
    xoris r4, r4, 0x8000
    stw r3, 0x4(r31)
    stw r4, 0x2c(r1)
    lfd f0, 0x28(r1)
    fsubs f2, f0, f1
    b @L_8019B180
@L_8019B0C4:
    lwz r5, 0x4(r31)
    lis r4, 0x4330
    lwz r3, 0x4(r31)
    lbz r5, 0x0(r5)
    addi r3, r3, 0x1
    stw r4, 0x28(r1)
    lfd f1, -23632(r2)
    stw r5, 0x2c(r1)
    lfd f0, 0x28(r1)
    stw r3, 0x4(r31)
    fsubs f2, f0, f1
    b @L_8019B180
@L_8019B0F4:
    lwz r3, 0x4(r31)
    lis r4, 0x4330
    lwz r5, 0x4(r31)
    lbz r6, 0x1(r3)
    lwz r3, 0x4(r31)
    extsb r6, r6
    lbz r5, 0x0(r5)
    rlwimi r5, r6, 8, 0, 23
    stw r4, 0x28(r1)
    xoris r4, r5, 0x8000
    addi r3, r3, 0x2
    stw r4, 0x2c(r1)
    lfd f1, -23624(r2)
    lfd f0, 0x28(r1)
    stw r3, 0x4(r31)
    fsubs f2, f0, f1
    b @L_8019B180
@L_8019B138:
    lwz r5, 0x4(r31)
    lis r4, 0x4330
    lwz r3, 0x4(r31)
    lbz r6, 0x1(r5)
    lbz r5, 0x0(r3)
    rlwimi r5, r6, 8, 16, 23
    stw r4, 0x28(r1)
    xoris r4, r5, 0x8000
    lwz r3, 0x4(r31)
    stw r4, 0x2c(r1)
    addi r3, r3, 0x2
    lfd f1, -23624(r2)
    lfd f0, 0x28(r1)
    stw r3, 0x4(r31)
    fsubs f2, f0, f1
    b @L_8019B180
@L_8019B178:
    lfs f0, -23652(r2)
    b @L_8019B1A0
@L_8019B180:
    xoris r3, r0, 0x8000
    lis r0, 0x4330
    stw r3, 0x2c(r1)
    lfd f1, -23624(r2)
    stw r0, 0x28(r1)
    lfd f0, 0x28(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
@L_8019B1A0:
    cmplwi r31, 0x0
    stfs f0, 0x2c(r31)
    bne @L_8019B1B4
    li r3, 0x0
    b @L_8019B478
@L_8019B1B4:
    lbz r0, 0x10(r31)
    rlwinm r3, r0, 0, 28, 31
    b @L_8019B478
@L_8019B1C0:
    cmplwi r31, 0x0
    bne @L_8019B1D0
    li r30, 0x0
    b @L_8019B1D8
@L_8019B1D0:
    lbz r0, 0x10(r31)
    rlwinm r30, r0, 0, 28, 31
@L_8019B1D8:
    subi r4, r30, 0x1
    li r3, 0x1
    subf r0, r4, r3
    li r5, 0x1
    orc r3, r3, r4
    rlwinm r0, r0, 31, 1, 31
    subf r0, r0, r3
    rlwinm. r0, r0, 1, 31, 31
    bne @L_8019B200
    li r5, 0x0
@L_8019B200:
    cmpwi r5, 0x0
    bne @L_8019B21C
    lis r4, -32729
    addi r3, r2, -23664
    addi r5, r4, 18300
    li r4, 0x1e9
    bl fn_80196E10
@L_8019B21C:
    lbz r0, 0x10(r31)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_8019B250
    lbz r0, 0x11(r31)
    stb r0, 0x12(r31)
    lbz r0, 0x10(r31)
    rlwinm r0, r0, 0, 26, 24
    stb r0, 0x10(r31)
    lbz r0, 0x10(r31)
    ori r0, r0, 0x80
    stb r0, 0x10(r31)
    lfs f0, 0x24(r31)
    stfs f0, 0x20(r31)
@L_8019B250:
    lbz r5, 0x14(r31)
    rlwinm. r0, r5, 0, 24, 31
    bne @L_8019B2D8
    lwz r4, 0x4(r31)
    addi r0, r4, 0x1
    stw r0, 0x4(r31)
    lwz r3, 0x4(r31)
    lbz r5, 0x0(r4)
    addi r0, r3, 0x1
    stw r0, 0x4(r31)
    lwz r4, 0x4(r31)
    stw r5, 0x8(r1)
    lbz r3, 0x0(r3)
    addi r0, r4, 0x1
    lwz r5, 0x8(r1)
    stw r0, 0x4(r31)
    rlwinm r0, r3, 8, 0, 23
    or r0, r5, r0
    stw r0, 0x8(r1)
    lwz r3, 0x4(r31)
    lbz r4, 0x0(r4)
    addi r0, r3, 0x1
    lwz r5, 0x8(r1)
    rlwinm r4, r4, 16, 0, 15
    stw r0, 0x4(r31)
    or r0, r5, r4
    stw r0, 0x8(r1)
    lbz r0, 0x0(r3)
    lwz r3, 0x8(r1)
    rlwinm r0, r0, 24, 0, 7
    or r0, r3, r0
    stw r0, 0x8(r1)
    lfs f0, 0x8(r1)
    b @L_8019B42C
@L_8019B2D8:
    rlwinm r3, r5, 0, 24, 26
    li r4, 0x1
    cmpwi r3, 0x60
    rlwinm r0, r5, 0, 27, 31
    slw r0, r4, r0
    beq @L_8019B318
    bge @L_8019B30C
    cmpwi r3, 0x40
    beq @L_8019B3C4
    bge @L_8019B404
    cmpwi r3, 0x20
    beq @L_8019B380
    b @L_8019B404
@L_8019B30C:
    cmpwi r3, 0x80
    beq @L_8019B350
    b @L_8019B404
@L_8019B318:
    lwz r5, 0x4(r31)
    lis r4, 0x4330
    lwz r3, 0x4(r31)
    lbz r5, 0x0(r5)
    addi r3, r3, 0x1
    stw r4, 0x28(r1)
    extsb r4, r5
    lfd f1, -23624(r2)
    xoris r4, r4, 0x8000
    stw r3, 0x4(r31)
    stw r4, 0x2c(r1)
    lfd f0, 0x28(r1)
    fsubs f2, f0, f1
    b @L_8019B40C
@L_8019B350:
    lwz r5, 0x4(r31)
    lis r4, 0x4330
    lwz r3, 0x4(r31)
    lbz r5, 0x0(r5)
    addi r3, r3, 0x1
    stw r4, 0x28(r1)
    lfd f1, -23632(r2)
    stw r5, 0x2c(r1)
    lfd f0, 0x28(r1)
    stw r3, 0x4(r31)
    fsubs f2, f0, f1
    b @L_8019B40C
@L_8019B380:
    lwz r3, 0x4(r31)
    lis r4, 0x4330
    lwz r5, 0x4(r31)
    lbz r6, 0x1(r3)
    lwz r3, 0x4(r31)
    extsb r6, r6
    lbz r5, 0x0(r5)
    rlwimi r5, r6, 8, 0, 23
    stw r4, 0x28(r1)
    xoris r4, r5, 0x8000
    addi r3, r3, 0x2
    stw r4, 0x2c(r1)
    lfd f1, -23624(r2)
    lfd f0, 0x28(r1)
    stw r3, 0x4(r31)
    fsubs f2, f0, f1
    b @L_8019B40C
@L_8019B3C4:
    lwz r5, 0x4(r31)
    lis r4, 0x4330
    lwz r3, 0x4(r31)
    lbz r6, 0x1(r5)
    lbz r5, 0x0(r3)
    rlwimi r5, r6, 8, 16, 23
    stw r4, 0x28(r1)
    xoris r4, r5, 0x8000
    lwz r3, 0x4(r31)
    stw r4, 0x2c(r1)
    addi r3, r3, 0x2
    lfd f1, -23624(r2)
    lfd f0, 0x28(r1)
    stw r3, 0x4(r31)
    fsubs f2, f0, f1
    b @L_8019B40C
@L_8019B404:
    lfs f0, -23652(r2)
    b @L_8019B42C
@L_8019B40C:
    xoris r3, r0, 0x8000
    lis r0, 0x4330
    stw r3, 0x2c(r1)
    lfd f1, -23624(r2)
    stw r0, 0x28(r1)
    lfd f0, 0x28(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
@L_8019B42C:
    stfs f0, 0x24(r31)
    li r0, 0x1
    subf r3, r0, r30
    cmplwi r31, 0x0
    lbz r4, 0x10(r31)
    subf r0, r30, r0
    nor r0, r3, r0
    li r3, 0x4
    ori r4, r4, 0x40
    srawi r0, r0, 31
    stb r4, 0x10(r31)
    add r3, r0, r3
    beq @L_8019B478
    lbz r0, 0x10(r31)
    rlwinm r0, r0, 0, 24, 27
    rlwimi r0, r3, 0, 28, 31
    stb r0, 0x10(r31)
    b @L_8019B478
@L_8019B474:
    li r3, 0x0
@L_8019B478:
    lwz r0, 0x44(r1)
    lwz r31, 0x3c(r1)
    lwz r30, 0x38(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
