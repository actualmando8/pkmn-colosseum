    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr. r31, r3
    stw r30, 0x18(r1)
    mr r30, r5
    beq @L_801BEE50
    cmplwi r4, 0x18
    bgt @L_801BEE50
    lis r3, jumptable_8036D734@ha
    rlwinm r0, r4, 2, 0, 29
    addi r3, r3, jumptable_8036D734@l
    lwzx r3, r3, r0
    mtctr r3
    bctr
    lwz r0, 0x68(r31)
    cmplwi r0, 0x0
    bne @L_801BE8BC
    lis r4, lbl_802756E8@ha
    addi r3, r2, -22512
    addi r5, r4, lbl_802756E8@l
    li r4, 0x116
    bl fn_80196E10
@L_801BE8BC:
    lfs f0, 0x0(r30)
    lwz r3, 0x68(r31)
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r4, 0xc(r1)
    rlwinm r0, r4, 2, 0, 29
    lwzx r0, r3, r0
    cmplwi r0, 0x0
    beq @L_801BEE50
    lwz r3, 0x68(r31)
    rlwinm r0, r4, 2, 0, 29
    lwzx r0, r3, r0
    stw r0, 0x58(r31)
    b @L_801BEE50
    lwz r0, 0x6c(r31)
    cmplwi r0, 0x0
    beq @L_801BEE50
    lfs f0, 0x0(r30)
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x70(r31)
    b @L_801BEE50
    lfs f1, 0x0(r30)
    lfs f0, -22464(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801BE934
    lfs f1, -22464(r2)
    b @L_801BE948
@L_801BE934:
    lfs f0, -22460(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801BE948
    lfs f1, -22460(r2)
@L_801BE948:
    stfs f1, 0x50(r31)
    b @L_801BEE50
    lfs f0, 0x0(r30)
    stfs f0, 0x18(r31)
    b @L_801BE9A0
    lfs f0, 0x0(r30)
    stfs f0, 0x1c(r31)
    b @L_801BE9A0
    lfs f0, 0x0(r30)
    stfs f0, 0x20(r31)
    b @L_801BE9A0
    lfs f0, 0x0(r30)
    stfs f0, 0x34(r31)
    b @L_801BE9A0
    lfs f0, 0x0(r30)
    stfs f0, 0x38(r31)
    b @L_801BE9A0
    lfs f0, 0x0(r30)
    stfs f0, 0x28(r31)
    b @L_801BE9A0
    lfs f0, 0x0(r30)
    stfs f0, 0x2c(r31)
@L_801BE9A0:
    lwz r0, 0x4c(r31)
    oris r0, r0, 0x8000
    stw r0, 0x4c(r31)
    b @L_801BEE50
    lwz r0, 0x60(r31)
    cmplwi r0, 0x0
    beq @L_801BEE50
    lfs f0, 0x0(r30)
    lwz r3, 0x60(r31)
    stfs f0, 0x4(r3)
    b @L_801BEE50
    lwz r0, 0xa8(r31)
    cmplwi r0, 0x0
    beq @L_801BEE50
    lfs f1, 0x0(r30)
    lfs f0, -22464(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801BE9F4
    lfs f1, -22464(r2)
    b @L_801BEA08
@L_801BE9F4:
    lfs f0, -22460(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801BEA08
    lfs f1, -22460(r2)
@L_801BEA08:
    lfs f0, -22408(r2)
    lwz r3, 0xa8(r31)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x10(r3)
    b @L_801BEE50
    lwz r0, 0xa8(r31)
    cmplwi r0, 0x0
    beq @L_801BEE50
    lfs f1, 0x0(r30)
    lfs f0, -22464(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801BEA50
    lfs f1, -22464(r2)
    b @L_801BEA64
@L_801BEA50:
    lfs f0, -22460(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801BEA64
    lfs f1, -22460(r2)
@L_801BEA64:
    lfs f0, -22408(r2)
    lwz r3, 0xa8(r31)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x11(r3)
    b @L_801BEE50
    lwz r0, 0xa8(r31)
    cmplwi r0, 0x0
    beq @L_801BEE50
    lfs f1, 0x0(r30)
    lfs f0, -22464(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801BEAAC
    lfs f1, -22464(r2)
    b @L_801BEAC0
@L_801BEAAC:
    lfs f0, -22460(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801BEAC0
    lfs f1, -22460(r2)
@L_801BEAC0:
    lfs f0, -22408(r2)
    lwz r3, 0xa8(r31)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x12(r3)
    b @L_801BEE50
    lwz r0, 0xa8(r31)
    cmplwi r0, 0x0
    beq @L_801BEE50
    lfs f1, 0x0(r30)
    lfs f0, -22464(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801BEB08
    lfs f1, -22464(r2)
    b @L_801BEB1C
@L_801BEB08:
    lfs f0, -22460(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801BEB1C
    lfs f1, -22460(r2)
@L_801BEB1C:
    lfs f0, -22408(r2)
    lwz r3, 0xa8(r31)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x13(r3)
    b @L_801BEE50
    lwz r0, 0xa8(r31)
    cmplwi r0, 0x0
    beq @L_801BEE50
    lfs f1, 0x0(r30)
    lfs f0, -22464(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801BEB64
    lfs f1, -22464(r2)
    b @L_801BEB78
@L_801BEB64:
    lfs f0, -22460(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801BEB78
    lfs f1, -22460(r2)
@L_801BEB78:
    lfs f0, -22408(r2)
    lwz r3, 0xa8(r31)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x14(r3)
    b @L_801BEE50
    lwz r0, 0xa8(r31)
    cmplwi r0, 0x0
    beq @L_801BEE50
    lfs f1, 0x0(r30)
    lfs f0, -22464(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801BEBC0
    lfs f1, -22464(r2)
    b @L_801BEBD4
@L_801BEBC0:
    lfs f0, -22460(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801BEBD4
    lfs f1, -22460(r2)
@L_801BEBD4:
    lfs f0, -22408(r2)
    lwz r3, 0xa8(r31)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x15(r3)
    b @L_801BEE50
    lwz r0, 0xa8(r31)
    cmplwi r0, 0x0
    beq @L_801BEE50
    lfs f1, 0x0(r30)
    lfs f0, -22464(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801BEC1C
    lfs f1, -22464(r2)
    b @L_801BEC30
@L_801BEC1C:
    lfs f0, -22460(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801BEC30
    lfs f1, -22460(r2)
@L_801BEC30:
    lfs f0, -22408(r2)
    lwz r3, 0xa8(r31)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x16(r3)
    b @L_801BEE50
    lwz r0, 0xa8(r31)
    cmplwi r0, 0x0
    beq @L_801BEE50
    lfs f1, 0x0(r30)
    lfs f0, -22464(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801BEC78
    lfs f1, -22464(r2)
    b @L_801BEC8C
@L_801BEC78:
    lfs f0, -22460(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801BEC8C
    lfs f1, -22460(r2)
@L_801BEC8C:
    lfs f0, -22408(r2)
    lwz r3, 0xa8(r31)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x17(r3)
    b @L_801BEE50
    lwz r0, 0xa8(r31)
    cmplwi r0, 0x0
    beq @L_801BEE50
    lfs f1, 0x0(r30)
    lfs f0, -22464(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801BECD4
    lfs f1, -22464(r2)
    b @L_801BECE8
@L_801BECD4:
    lfs f0, -22460(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801BECE8
    lfs f1, -22460(r2)
@L_801BECE8:
    lfs f0, -22408(r2)
    lwz r3, 0xa8(r31)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x18(r3)
    b @L_801BEE50
    lwz r0, 0xa8(r31)
    cmplwi r0, 0x0
    beq @L_801BEE50
    lfs f1, 0x0(r30)
    lfs f0, -22464(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801BED30
    lfs f1, -22464(r2)
    b @L_801BED44
@L_801BED30:
    lfs f0, -22460(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801BED44
    lfs f1, -22460(r2)
@L_801BED44:
    lfs f0, -22408(r2)
    lwz r3, 0xa8(r31)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x19(r3)
    b @L_801BEE50
    lwz r0, 0xa8(r31)
    cmplwi r0, 0x0
    beq @L_801BEE50
    lfs f1, 0x0(r30)
    lfs f0, -22464(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801BED8C
    lfs f1, -22464(r2)
    b @L_801BEDA0
@L_801BED8C:
    lfs f0, -22460(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801BEDA0
    lfs f1, -22460(r2)
@L_801BEDA0:
    lfs f0, -22408(r2)
    lwz r3, 0xa8(r31)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x1a(r3)
    b @L_801BEE50
    lwz r0, 0xa8(r31)
    cmplwi r0, 0x0
    beq @L_801BEE50
    lfs f1, 0x0(r30)
    lfs f0, -22464(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801BEDE8
    lfs f1, -22464(r2)
    b @L_801BEDFC
@L_801BEDE8:
    lfs f0, -22460(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801BEDFC
    lfs f1, -22460(r2)
@L_801BEDFC:
    lfs f0, -22408(r2)
    lwz r3, 0xa8(r31)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x1b(r3)
    b @L_801BEE50
    lfs f1, 0x0(r30)
    lfs f0, -22464(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801BEE38
    lfs f1, -22464(r2)
    b @L_801BEE4C
@L_801BEE38:
    lfs f0, -22460(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801BEE4C
    lfs f1, -22460(r2)
@L_801BEE4C:
    stfs f1, 0x50(r31)
@L_801BEE50:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

