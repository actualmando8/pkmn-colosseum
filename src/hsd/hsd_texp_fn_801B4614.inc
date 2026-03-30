    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stw r31, 0x5c(r1)
    mr r31, r3
    stw r30, 0x58(r1)
    li r30, 0x0
    b @L_801B4A84
@L_801B4634:
    lwz r0, 0x0(r31)
    cmpwi r0, 0x4
    beq @L_801B4654
    lis r4, -32729
    addi r3, r2, -22576
    addi r5, r4, 21440
    li r4, 0x5ad
    bl fn_80196E10
@L_801B4654:
    lbz r0, 0xe(r31)
    cmplwi r0, 0x8
    bge @L_801B4A80
    lbz r0, 0xc(r31)
    li r4, 0x1
    lbz r3, 0xe(r31)
    cmplwi r0, 0x1
    slw r0, r4, r3
    or r30, r30, r0
    bne @L_801B48B0
    lbz r0, 0xd(r31)
    cmpwi r0, 0x1
    beq @L_801B475C
    bge @L_801B4698
    cmpwi r0, 0x0
    bge @L_801B46A4
    b @L_801B475C
@L_801B4698:
    cmpwi r0, 0x3
    bge @L_801B475C
    b @L_801B46DC
@L_801B46A4:
    lwz r5, 0x8(r31)
    addi r4, r1, 0x17
    lbz r0, 0xe(r31)
    addi r3, r1, 0x14
    lwz r6, 0x0(r5)
    rlwinm r5, r0, 2, 0, 29
    lbz r0, 0xe(r31)
    stw r6, 0x10(r1)
    lbzx r5, r4, r5
    rlwinm r4, r0, 2, 0, 29
    stb r5, 0x13(r1)
    lwz r0, 0x10(r1)
    stwx r0, r3, r4
    b @L_801B4A80
@L_801B46DC:
    lwz r6, 0x8(r31)
    lwz r0, 0x0(r6)
    cmplwi r0, 0x100
    bge @L_801B46F4
    lwz r5, 0x0(r6)
    b @L_801B46F8
@L_801B46F4:
    li r5, 0xff
@L_801B46F8:
    lbz r4, 0xe(r31)
    addi r3, r1, 0x14
    lwz r0, 0x4(r6)
    rlwinm r4, r4, 2, 0, 29
    cmplwi r0, 0x100
    stbx r5, r3, r4
    bge @L_801B471C
    lwz r5, 0x4(r6)
    b @L_801B4720
@L_801B471C:
    li r5, 0xff
@L_801B4720:
    lbz r4, 0xe(r31)
    addi r3, r1, 0x15
    lwz r0, 0x8(r6)
    rlwinm r4, r4, 2, 0, 29
    cmplwi r0, 0x100
    stbx r5, r3, r4
    bge @L_801B4744
    lwz r4, 0x8(r6)
    b @L_801B4748
@L_801B4744:
    li r4, 0xff
@L_801B4748:
    lbz r0, 0xe(r31)
    addi r3, r1, 0x16
    rlwinm r0, r0, 2, 0, 29
    stbx r4, r3, r0
    b @L_801B4A80
@L_801B475C:
    lbz r0, 0xd(r31)
    cmpwi r0, 0x2
    beq @L_801B47E8
    bge @L_801B4778
    cmpwi r0, 0x1
    bge @L_801B4784
    b @L_801B47E8
@L_801B4778:
    cmpwi r0, 0x4
    bge @L_801B47E8
    b @L_801B4798
@L_801B4784:
    lwz r4, 0x8(r31)
    lhz r3, 0x0(r4)
    lhz r5, 0x2(r4)
    lhz r6, 0x4(r4)
    b @L_801B4834
@L_801B4798:
    lwz r3, 0x8(r31)
    lfs f1, -22560(r2)
    lfs f0, 0x0(r3)
    lfs f3, -22560(r2)
    lfs f2, 0x4(r3)
    fmuls f4, f1, f0
    lfs f1, -22560(r2)
    lfs f0, 0x8(r3)
    fmuls f2, f3, f2
    fctiwz f3, f4
    fmuls f0, f1, f0
    fctiwz f1, f2
    stfd f3, 0x38(r1)
    fctiwz f0, f0
    stfd f1, 0x40(r1)
    lwz r3, 0x3c(r1)
    stfd f0, 0x48(r1)
    lwz r5, 0x44(r1)
    lwz r6, 0x4c(r1)
    b @L_801B4834
@L_801B47E8:
    lwz r3, 0x8(r31)
    lfd f1, -22552(r2)
    lfd f0, 0x0(r3)
    lfd f3, -22552(r2)
    lfd f2, 0x8(r3)
    fmul f4, f1, f0
    lfd f1, -22552(r2)
    lfd f0, 0x10(r3)
    fmul f2, f3, f2
    fmul f0, f1, f0
    fctiwz f3, f4
    fctiwz f1, f2
    fctiwz f0, f0
    stfd f3, 0x48(r1)
    stfd f1, 0x40(r1)
    lwz r3, 0x4c(r1)
    stfd f0, 0x38(r1)
    lwz r5, 0x44(r1)
    lwz r6, 0x3c(r1)
@L_801B4834:
    cmpwi r3, 0xff
    ble @L_801B4844
    li r4, 0xff
    b @L_801B484C
@L_801B4844:
    srawi r0, r3, 31
    andc r4, r3, r0
@L_801B484C:
    lbz r0, 0xe(r31)
    cmpwi r5, 0xff
    addi r3, r1, 0x14
    rlwinm r0, r0, 2, 0, 29
    stbx r4, r3, r0
    ble @L_801B486C
    li r4, 0xff
    b @L_801B4874
@L_801B486C:
    srawi r0, r5, 31
    andc r4, r5, r0
@L_801B4874:
    lbz r0, 0xe(r31)
    cmpwi r6, 0xff
    addi r3, r1, 0x15
    rlwinm r0, r0, 2, 0, 29
    stbx r4, r3, r0
    ble @L_801B4894
    li r4, 0xff
    b @L_801B489C
@L_801B4894:
    srawi r0, r6, 31
    andc r4, r6, r0
@L_801B489C:
    lbz r0, 0xe(r31)
    addi r3, r1, 0x16
    rlwinm r0, r0, 2, 0, 29
    stbx r4, r3, r0
    b @L_801B4A80
@L_801B48B0:
    lbz r0, 0xd(r31)
    cmpwi r0, 0x2
    beq @L_801B4910
    bge @L_801B48D0
    cmpwi r0, 0x0
    beq @L_801B48DC
    bge @L_801B48E8
    b @L_801B4974
@L_801B48D0:
    cmpwi r0, 0x4
    bge @L_801B4974
    b @L_801B4938
@L_801B48DC:
    lwz r3, 0x8(r31)
    lbz r0, 0x0(r3)
    b @L_801B49AC
@L_801B48E8:
    lwz r3, 0x8(r31)
    lhz r3, 0x0(r3)
    cmpwi r3, 0xff
    ble @L_801B4900
    li r0, 0xff
    b @L_801B4908
@L_801B4900:
    srawi r0, r3, 31
    andc r0, r3, r0
@L_801B4908:
    rlwinm r0, r0, 0, 24, 31
    b @L_801B49AC
@L_801B4910:
    lwz r3, 0x8(r31)
    lwz r3, 0x0(r3)
    cmpwi r3, 0xff
    ble @L_801B4928
    li r0, 0xff
    b @L_801B4930
@L_801B4928:
    srawi r0, r3, 31
    andc r0, r3, r0
@L_801B4930:
    rlwinm r0, r0, 0, 24, 31
    b @L_801B49AC
@L_801B4938:
    lwz r3, 0x8(r31)
    lfs f1, -22560(r2)
    lfs f0, 0x0(r3)
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x48(r1)
    lwz r3, 0x4c(r1)
    cmpwi r3, 0xff
    ble @L_801B4964
    li r0, 0xff
    b @L_801B496C
@L_801B4964:
    srawi r0, r3, 31
    andc r0, r3, r0
@L_801B496C:
    rlwinm r0, r0, 0, 24, 31
    b @L_801B49AC
@L_801B4974:
    lwz r3, 0x8(r31)
    lfd f1, -22552(r2)
    lfd f0, 0x0(r3)
    fmul f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x48(r1)
    lwz r3, 0x4c(r1)
    cmpwi r3, 0xff
    ble @L_801B49A0
    li r0, 0xff
    b @L_801B49A8
@L_801B49A0:
    srawi r0, r3, 31
    andc r0, r3, r0
@L_801B49A8:
    rlwinm r0, r0, 0, 24, 31
@L_801B49AC:
    lbz r3, 0xe(r31)
    cmplwi r3, 0x4
    bge @L_801B4A30
    lbz r3, 0xf(r31)
    cmpwi r3, 0x1
    beq @L_801B49F4
    bge @L_801B49D4
    cmpwi r3, 0x0
    bge @L_801B49E0
    b @L_801B4A1C
@L_801B49D4:
    cmpwi r3, 0x3
    bge @L_801B4A1C
    b @L_801B4A08
@L_801B49E0:
    lbz r4, 0xe(r31)
    addi r3, r1, 0x14
    rlwinm r4, r4, 2, 0, 29
    stbx r0, r3, r4
    b @L_801B4A80
@L_801B49F4:
    lbz r4, 0xe(r31)
    addi r3, r1, 0x15
    rlwinm r4, r4, 2, 0, 29
    stbx r0, r3, r4
    b @L_801B4A80
@L_801B4A08:
    lbz r4, 0xe(r31)
    addi r3, r1, 0x16
    rlwinm r4, r4, 2, 0, 29
    stbx r0, r3, r4
    b @L_801B4A80
@L_801B4A1C:
    lbz r4, 0xe(r31)
    addi r3, r1, 0x17
    rlwinm r4, r4, 2, 0, 29
    stbx r0, r3, r4
    b @L_801B4A80
@L_801B4A30:
    lbz r3, 0xf(r31)
    cmplwi r3, 0x3
    bne @L_801B4A50
    lbz r4, 0xe(r31)
    addi r3, r1, 0x17
    rlwinm r4, r4, 2, 0, 29
    stbx r0, r3, r4
    b @L_801B4A80
@L_801B4A50:
    lbz r3, 0xe(r31)
    addi r6, r1, 0x14
    lbz r4, 0xe(r31)
    addi r5, r1, 0x15
    rlwinm r7, r3, 2, 0, 29
    lbz r3, 0xe(r31)
    stbx r0, r6, r7
    rlwinm r6, r4, 2, 0, 29
    rlwinm r4, r3, 2, 0, 29
    addi r3, r1, 0x16
    stbx r0, r5, r6
    stbx r0, r3, r4
@L_801B4A80:
    lwz r31, 0x4(r31)
@L_801B4A84:
    cmplwi r31, 0x0
    bne @L_801B4634
    cmplwi r30, 0x0
    beq @L_801B4B44
    bl fn_800B8E74
    li r31, 0x0
    b @L_801B4AE0
@L_801B4AA0:
    li r0, 0x1
    slw r0, r0, r31
    and. r0, r30, r0
    beq @L_801B4ADC
    lis r3, -32713
    rlwinm r6, r31, 2, 0, 29
    addi r5, r1, 0x14
    rlwinm r4, r31, 2, 0, 29
    addi r0, r3, -11480
    lwzx r5, r5, r6
    add r3, r0, r4
    addi r4, r1, 0xc
    stw r5, 0xc(r1)
    lwz r3, 0x0(r3)
    bl fn_800BC3E0
@L_801B4ADC:
    addi r31, r31, 0x1
@L_801B4AE0:
    cmpwi r31, 0x4
    blt @L_801B4AA0
    li r31, 0x4
    b @L_801B4B30
@L_801B4AF0:
    li r0, 0x1
    slw r0, r0, r31
    and. r0, r30, r0
    beq @L_801B4B2C
    lis r3, -32713
    rlwinm r5, r31, 2, 0, 29
    addi r4, r1, 0x14
    rlwinm r0, r31, 2, 0, 29
    lwzx r5, r4, r5
    addi r3, r3, -11464
    add r3, r3, r0
    addi r4, r1, 0x8
    stw r5, 0x8(r1)
    lwz r3, -0x10(r3)
    bl fn_800BC2F8
@L_801B4B2C:
    addi r31, r31, 0x1
@L_801B4B30:
    cmpwi r31, 0x7
    blt @L_801B4AF0
    li r3, 0x10
    bl fn_801B25C4
    bl fn_800B8E74
@L_801B4B44:
    lwz r0, 0x64(r1)
    lwz r31, 0x5c(r1)
    lwz r30, 0x58(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
