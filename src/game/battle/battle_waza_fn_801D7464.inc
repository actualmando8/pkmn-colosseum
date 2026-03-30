    nofralloc
    stwu r1, -0xe0(r1)
    mflr r0
    stw r0, 0xe4(r1)
    stfd f31, 0xd0(r1)
    psq_st f31, 0xd8(r1), 0, 0
    stmw r24, 0xb0(r1)
    lis r3, -32698
    li r26, 0x0
    addi r3, r3, 29688
    li r25, 0x0
    lwz r0, 0x624(r3)
    rlwinm r0, r0, 2, 0, 29
    add r3, r3, r0
    lwz r27, 0x61c(r3)
    cmplwi r27, 0x0
    beq @L_801D7B78
    lwz r3, 0x68(r27)
    cmplwi r3, 0x0
    beq @L_801D74B8
    lhz r26, 0x2c(r3)
    lhz r25, 0x2e(r3)
@L_801D74B8:
    lis r3, -32698
    lwz r0, 29688(r3)
    cmpwi r0, 0x0
    bne @L_801D7AE4
    bl fn_80177A38
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_801D79AC
    lis r3, -32728
    addi r5, r3, -27848
    lwz r4, 0x0(r5)
    lwz r3, 0x4(r5)
    lwz r0, 0x8(r5)
    stw r4, 0x6c(r1)
    stw r3, 0x70(r1)
    stw r0, 0x74(r1)
    bl fn_800D3088
    lis r0, 0x4330
    stw r3, 0xac(r1)
    lfd f2, -21368(r2)
    addi r3, r1, 0x9c
    stw r0, 0xa8(r1)
    lfs f0, -21384(r2)
    lfd f1, 0xa8(r1)
    fsubs f1, f1, f2
    fmuls f31, f0, f1
    bl fn_801778B4
    addi r3, r1, 0x90
    bl fn_80177908
    addi r3, r1, 0x84
    bl fn_8017795C
    addi r3, r1, 0x78
    addi r4, r1, 0x90
    addi r5, r1, 0x84
    bl fn_800E019C
    addi r3, r1, 0x78
    addi r5, r1, 0x9c
    mr r4, r3
    bl fn_800E0168
    addi r3, r1, 0x78
    mr r4, r3
    bl fn_800E0060
    addi r3, r1, 0x60
    addi r4, r1, 0x6c
    addi r5, r1, 0x78
    bl fn_800DFFCC
    addi r3, r1, 0x60
    mr r4, r3
    bl fn_800E0060
    addi r3, r1, 0x6c
    addi r4, r1, 0x78
    addi r5, r1, 0x60
    bl fn_800DFFCC
    addi r3, r1, 0x6c
    mr r4, r3
    bl fn_800E0060
    li r3, 0x1
    li r4, 0x1
    bl fn_800F7A7C
    mr r28, r3
    li r3, 0x1
    li r4, 0x1
    bl fn_800F7A08
    mr r29, r3
    li r3, 0x1
    li r4, 0x1
    bl fn_800F7994
    mr r30, r3
    li r3, 0x1
    li r4, 0x1
    bl fn_800F7920
    mr r31, r3
    li r3, 0x1
    bl fn_800F7AF0
    mr r24, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r24
    rlwinm. r0, r0, 0, 28, 28
    beq @L_801D7600
    li r0, 0x1
    stb r0, -21540(r13)
@L_801D7600:
    li r3, 0x1
    bl fn_800F7AF0
    mr r24, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r24
    rlwinm. r0, r0, 0, 29, 29
    beq @L_801D7628
    li r0, 0x1
    stb r0, -21539(r13)
@L_801D7628:
    li r3, 0x1
    bl fn_800F7AF0
    mr r24, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r24
    rlwinm. r0, r0, 0, 31, 31
    beq @L_801D7650
    li r0, 0x1
    stb r0, -21538(r13)
@L_801D7650:
    li r3, 0x1
    bl fn_800F7AF0
    mr r24, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r24
    rlwinm. r0, r0, 0, 30, 30
    beq @L_801D7678
    li r0, 0x1
    stb r0, -21537(r13)
@L_801D7678:
    lbz r0, -21540(r13)
    cmplwi r0, 0x0
    beq @L_801D76CC
    fmr f1, f31
    addi r3, r1, 0x54
    addi r4, r1, 0x6c
    bl fn_800E013C
    addi r3, r1, 0x84
    addi r5, r1, 0x6c
    mr r4, r3
    bl fn_800E019C
    li r3, 0x1
    bl fn_800F7AF0
    mr r24, r3
    li r3, 0x1
    bl fn_800F7B5C
    and r0, r3, r24
    rlwinm. r0, r0, 0, 28, 28
    beq @L_801D76CC
    li r0, 0x0
    stb r0, -21540(r13)
@L_801D76CC:
    lbz r0, -21539(r13)
    cmplwi r0, 0x0
    beq @L_801D7720
    fmr f1, f31
    addi r3, r1, 0x54
    addi r4, r1, 0x6c
    bl fn_800E013C
    addi r3, r1, 0x84
    addi r5, r1, 0x54
    mr r4, r3
    bl fn_800E0168
    li r3, 0x1
    bl fn_800F7AF0
    mr r24, r3
    li r3, 0x1
    bl fn_800F7B5C
    and r0, r3, r24
    rlwinm. r0, r0, 0, 29, 29
    beq @L_801D7720
    li r0, 0x0
    stb r0, -21539(r13)
@L_801D7720:
    lbz r0, -21538(r13)
    cmplwi r0, 0x0
    beq @L_801D7774
    fmr f1, f31
    addi r3, r1, 0x54
    addi r4, r1, 0x60
    bl fn_800E013C
    addi r3, r1, 0x84
    addi r5, r1, 0x54
    mr r4, r3
    bl fn_800E019C
    li r3, 0x1
    bl fn_800F7AF0
    mr r24, r3
    li r3, 0x1
    bl fn_800F7B5C
    and r0, r3, r24
    rlwinm. r0, r0, 0, 31, 31
    beq @L_801D7774
    li r0, 0x0
    stb r0, -21538(r13)
@L_801D7774:
    lbz r0, -21537(r13)
    cmplwi r0, 0x0
    beq @L_801D77C8
    fmr f1, f31
    addi r3, r1, 0x54
    addi r4, r1, 0x60
    bl fn_800E013C
    addi r3, r1, 0x84
    addi r5, r1, 0x54
    mr r4, r3
    bl fn_800E0168
    li r3, 0x1
    bl fn_800F7AF0
    mr r24, r3
    li r3, 0x1
    bl fn_800F7B5C
    and r0, r3, r24
    rlwinm. r0, r0, 0, 30, 30
    beq @L_801D77C8
    li r0, 0x0
    stb r0, -21537(r13)
@L_801D77C8:
    extsb. r0, r28
    bne @L_801D77E8
    extsb. r0, r29
    bne @L_801D77E8
    extsb. r0, r30
    bne @L_801D77E8
    extsb. r0, r31
    beq @L_801D7954
@L_801D77E8:
    addi r3, r1, 0x2c
    addi r4, r1, 0x90
    addi r5, r1, 0x84
    bl fn_800E019C
    addi r3, r1, 0x9c
    addi r5, r1, 0x2c
    mr r4, r3
    bl fn_800E0168
    extsb. r0, r28
    beq @L_801D785C
    lfs f1, -21436(r2)
    addi r3, r1, 0x48
    lfs f2, -21396(r2)
    fmr f3, f1
    bl fn_800E01F4
    extsb. r0, r28
    lfs f1, -21380(r2)
    addi r3, r1, 0x38
    addi r4, r1, 0x48
    ble @L_801D7840
    fmr f0, f31
    b @L_801D7844
@L_801D7840:
    fneg f0, f31
@L_801D7844:
    fmuls f1, f1, f0
    bl fn_800E0718
    addi r3, r1, 0x9c
    addi r4, r1, 0x38
    mr r5, r3
    bl fn_800DFEEC
@L_801D785C:
    extsb. r0, r29
    beq @L_801D78F4
    lfs f1, -21436(r2)
    addi r3, r1, 0x48
    lfs f2, -21396(r2)
    fmr f3, f1
    bl fn_800E01F4
    addi r3, r1, 0x20
    addi r4, r1, 0x9c
    bl fn_800E0060
    addi r3, r1, 0x48
    addi r5, r1, 0x20
    mr r4, r3
    bl fn_800DFFCC
    extsb. r0, r29
    bgt @L_801D78B0
    addi r3, r1, 0x48
    bl fn_800E008C
    lfs f0, -21376(r2)
    fcmpo cr0, f1, f0
    ble @L_801D78F4
@L_801D78B0:
    addi r3, r1, 0x48
    mr r4, r3
    bl fn_800E0060
    extsb. r0, r29
    lfs f1, -21380(r2)
    addi r3, r1, 0x38
    addi r4, r1, 0x48
    ble @L_801D78D8
    fmr f0, f31
    b @L_801D78DC
@L_801D78D8:
    fneg f0, f31
@L_801D78DC:
    fmuls f1, f1, f0
    bl fn_800E0718
    addi r3, r1, 0x9c
    addi r4, r1, 0x38
    mr r5, r3
    bl fn_800DFEEC
@L_801D78F4:
    extsb. r0, r31
    beq @L_801D7944
    addi r3, r1, 0x14
    addi r4, r1, 0x9c
    bl fn_800E01D0
    addi r3, r1, 0x14
    mr r4, r3
    bl fn_800E0060
    extsb. r0, r31
    addi r3, r1, 0x14
    mr r4, r3
    ble @L_801D792C
    fmr f1, f31
    b @L_801D7930
@L_801D792C:
    fneg f1, f31
@L_801D7930:
    bl fn_800E013C
    addi r3, r1, 0x9c
    addi r5, r1, 0x14
    mr r4, r3
    bl fn_800E019C
@L_801D7944:
    addi r3, r1, 0x9c
    addi r5, r1, 0x2c
    mr r4, r3
    bl fn_800E019C
@L_801D7954:
    addi r3, r1, 0x9c
    bl fn_801778DC
    addi r3, r1, 0x90
    bl fn_80177930
    addi r3, r1, 0x84
    bl fn_80177984
    bl fn_800D2584
    mr. r24, r3
    beq @L_801D79AC
    addi r3, r1, 0x8
    addi r4, r1, 0x90
    addi r5, r1, 0x84
    bl fn_800E019C
    mr r3, r24
    addi r4, r1, 0x9c
    bl fn_800D207C
    lis r4, -32713
    mr r3, r24
    addi r4, r4, 9632
    addi r5, r1, 0x8
    bl fn_800D1F04
    bl fn_800D2248
@L_801D79AC:
    lis r3, -32698
    addi r31, r3, 29688
    lwz r0, 0x4(r31)
    cmpwi r0, 0x1
    bne @L_801D7A40
    lwz r0, 0x68(r27)
    cmplwi r0, 0x0
    bne @L_801D7A40
    li r3, 0x1
    bl fn_800F7AF0
    mr r28, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r28
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801D79F8
    lhz r3, 0x32(r27)
    subi r0, r3, 0x1
    sth r0, 0x32(r27)
@L_801D79F8:
    li r3, 0x1
    bl fn_800F7AF0
    mr r28, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r28
    rlwinm. r0, r0, 0, 26, 26
    beq @L_801D7A40
    lhz r3, 0x32(r27)
    addi r0, r3, 0x1
    sth r0, 0x32(r27)
    lhz r3, 0x14(r27)
    lhz r4, 0x32(r27)
    subi r0, r3, 0x1
    cmpw r4, r0
    ble @L_801D7A40
    li r0, 0x0
    sth r0, 0x32(r27)
@L_801D7A40:
    lis r3, -32698
    addi r3, r3, 29688
    lbz r0, 0x8(r3)
    rlwinm r0, r0, 24, 0, 1
    srawi r0, r0, 31
    extsb. r0, r0
    beq @L_801D7AA0
    li r3, 0x1
    bl fn_800F7AF0
    mr r28, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r28
    rlwinm. r0, r0, 0, 20, 20
    beq @L_801D7AA0
    lis r3, -32698
    addi r4, r3, 29688
    lwz r3, 0x18(r4)
    addi r0, r3, 0x1
    cmpwi r0, 0x10
    stw r0, 0x18(r4)
    blt @L_801D7AA0
    li r0, -0x1
    stw r0, 0x18(r4)
@L_801D7AA0:
    li r3, 0x1
    bl fn_800F7AF0
    mr r28, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r28
    rlwinm. r0, r0, 0, 23, 23
    beq @L_801D7AE4
    lwz r0, 0x4(r31)
    cmpwi r0, 0x1
    bne @L_801D7AE4
    lwz r0, 0x68(r27)
    cmplwi r0, 0x0
    bne @L_801D7AE4
    mr r3, r27
    li r4, 0x0
    bl fn_801D9C1C
@L_801D7AE4:
    li r3, 0x1
    bl fn_800F7AF0
    mr r28, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r28
    rlwinm. r0, r0, 0, 23, 23
    beq @L_801D7B78
    lis r3, -32698
    lwzu r0, 29688(r3)
    cmpwi r0, 0x1
    bne @L_801D7B34
    lwz r0, 0x63c(r3)
    cmpwi r0, 0x0
    beq @L_801D7B34
    lwz r4, 0x638(r3)
    mr r3, r27
    li r5, 0x0
    bl fn_801DF070
    b @L_801D7B78
@L_801D7B34:
    mr r3, r27
    mr r4, r26
    mr r5, r25
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801D7B78
    mr r3, r27
    li r4, 0x1
    bl fn_801DA4E8
    mr r3, r27
    mr r4, r26
    mr r5, r25
    bl fn_801DA9E8
    lis r3, -32698
    li r0, 0x1
    addi r3, r3, 29688
    stw r0, 0x62c(r3)
@L_801D7B78:
    psq_l f31, 0xd8(r1), 0, 0
    lfd f31, 0xd0(r1)
    lmw r24, 0xb0(r1)
    lwz r0, 0xe4(r1)
    mtlr r0
    addi r1, r1, 0xe0
    blr
