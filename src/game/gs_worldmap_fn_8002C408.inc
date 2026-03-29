    nofralloc
    stwu r1, -0x8e0(r1)
    mflr r0
    stw r0, 0x8e4(r1)
    stmw r15, 0x89c(r1)
    mr r31, r3
    mr r29, r4
    clrlwi r30, r29, 24
    li r25, 0x0
    cmpwi r30, 0x3
    beq @L_8002C4A4
    bge @L_8002C4B0
    cmpwi r30, 0x2
    bge @L_8002C440
    b @L_8002C4B0
    @L_8002C440:
    addi r15, r1, 0x128
    li r4, 0xeb
    mr r3, r15
    bl fn_80142A88
    addi r3, r15, 0x3ac
    li r4, 0xeb
    bl fn_80142A88
    li r0, 0x0
    li r3, 0x0
    stw r0, 0x880(r1)
    li r4, 0x3
    stw r0, 0x884(r1)
    stb r0, 0x888(r1)
    bl fn_80129280
    mr r4, r3
    lwz r3, lbl_8047A3DC(r13)
    li r5, 0x7198
    bl memcpy
    li r3, 0x0
    li r4, 0xd
    li r5, 0x0
    bl fn_8012A5B0
    stw r3, lbl_8047A3D8(r13)
    li r3, 0x1
    b @L_8002C4B4
    @L_8002C4A4:
    addi r3, r1, 0x128
    bl fn_80029CC0
    b @L_8002C4B4
    @L_8002C4B0:
    li r3, 0x1
    @L_8002C4B4:
    cmpwi r3, 0x0
    beq @L_8002CE58
    clrlwi r0, r29, 24
    cmplwi r0, 0x4
    bne @L_8002C600
    cmpwi r30, 0x3
    beq @L_8002C4F8
    bge @L_8002C564
    cmpwi r30, 0x2
    bge @L_8002C4E0
    b @L_8002C564
    @L_8002C4E0:
    li r3, 0x0
    li r4, 0xd
    li r5, 0x0
    bl fn_8012A5B0
    mr r15, r3
    b @L_8002C578
    @L_8002C4F8:
    addi r0, r1, 0x128
    cmplwi r0, 0x0
    beq @L_8002C55C
    lwz r0, lbl_8047A660(r13)
    cmpwi r0, 0x0
    ble @L_8002C538
    lwz r5, 0x880(r1)
    li r0, 0x0
    lwz r3, lbl_8047A660(r13)
    lwz r4, 0x884(r1)
    add r3, r5, r3
    stw r3, 0x880(r1)
    lwz r3, lbl_8047A660(r13)
    add r3, r4, r3
    stw r3, 0x884(r1)
    stw r0, lbl_8047A660(r13)
    @L_8002C538:
    lwz r0, lbl_8047A664(r13)
    cmpwi r0, 0x0
    ble @L_8002C554
    li r0, 0x0
    stw r0, 0x880(r1)
    stw r0, 0x884(r1)
    stw r0, lbl_8047A664(r13)
    @L_8002C554:
    lwz r15, 0x880(r1)
    b @L_8002C578
    @L_8002C55C:
    li r15, 0x0
    b @L_8002C578
    @L_8002C564:
    li r3, 0x0
    li r4, 0xd
    li r5, 0x0
    bl fn_8012A5B0
    mr r15, r3
    @L_8002C578:
    lwz r3, lbl_80478E54(r13)
    slwi r0, r31, 2
    lis r5, 0x99
    lwz r4, lbl_80478E44(r13)
    add r3, r3, r0
    lhz r0, 0x2(r3)
    subi r16, r5, 0x6981
    slwi r0, r0, 1
    add r17, r4, r0
    mr r3, r17
    b @L_8002C5A8
    @L_8002C5A4:
    addi r3, r3, 0x2
    @L_8002C5A8:
    lhz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_8002C5A4
    b @L_8002C5D4
    @L_8002C5B8:
    bl fn_801440A0
    bl fn_80143FE4
    clrlwi r0, r3, 16
    cmpw r0, r16
    bge @L_8002C5D0
    mr r16, r0
    @L_8002C5D0:
    addi r17, r17, 0x2
    @L_8002C5D4:
    lhz r3, 0x0(r17)
    cmplwi r3, 0x0
    bne @L_8002C5B8
    cmpw r15, r16
    bge @L_8002C600
    mr r3, r31
    li r4, 0x4
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_8002A1C4
    b @L_8002CE58
    @L_8002C600:
    clrlwi r15, r29, 24
    lis r3, lbl_80266E70@ha
    mulli r4, r15, 0x3
    slwi r27, r31, 2
    addi r0, r3, lbl_80266E70@l
    addi r28, r1, 0x128
    add r26, r0, r4
    addi r23, r26, 0x1
    addi r24, r26, 0x2
    b @L_8002CE40
    @L_8002C628:
    lwz r0, lbl_804788A8(r13)
    cmpwi r0, 0x0
    beq @L_8002C6B0
    lis r3, lbl_802E4F68@ha
    lis r5, lbl_802EF0A8@ha
    addi r3, r3, lbl_802E4F68@l
    li r4, 0x0
    lwz r6, 0x0(r3)
    addi r0, r5, lbl_802EF0A8@l
    lwz r8, 0x8(r3)
    lwz r7, 0x10(r3)
    mulli r9, r6, 0x1c
    lwz r6, 0x18(r3)
    lwz r5, 0x20(r3)
    mulli r8, r8, 0x1c
    stw r4, lbl_804788A8(r13)
    add r4, r0, r9
    lha r9, 0x4(r4)
    mulli r4, r7, 0x1c
    add r7, r0, r8
    sth r9, 0x4(r3)
    lha r7, 0x4(r7)
    mulli r6, r6, 0x1c
    add r4, r0, r4
    sth r7, 0xc(r3)
    lha r7, 0x4(r4)
    mulli r4, r5, 0x1c
    add r5, r0, r6
    sth r7, 0x14(r3)
    lha r5, 0x4(r5)
    add r4, r0, r4
    lha r0, 0x4(r4)
    sth r5, 0x1c(r3)
    sth r0, 0x24(r3)
    @L_8002C6B0:
    li r7, 0x0
    lwz r3, lbl_80478E54(r13)
    la r4, lbl_8047A3F8(r13)
    sth r7, lbl_8047A3F8(r13)
    addi r0, r3, 0x2
    lwz r3, lbl_80478E44(r13)
    stw r4, 0x108(r1)
    lhzx r0, r27, r0
    slwi r0, r0, 1
    add r8, r3, r0
    mr r3, r8
    b @L_8002C6E8
    @L_8002C6E0:
    addi r3, r3, 0x2
    addi r7, r7, 0x1
    @L_8002C6E8:
    lhz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_8002C6E0
    la r6, lbl_8047A3F4(r13)
    la r5, lbl_8047A3F0(r13)
    la r4, lbl_8047A3EC(r13)
    la r3, lbl_8047A3E8(r13)
    li r0, 0x0
    stw r7, 0x110(r1)
    stw r8, 0x10c(r1)
    stw r6, 0x114(r1)
    stw r5, 0x118(r1)
    stw r4, 0x11c(r1)
    stw r3, 0x120(r1)
    stb r29, 0x124(r1)
    stb r0, 0x125(r1)
    bl fn_801046B8
    mr r4, r3
    addi r9, r1, 0x108
    li r3, 0x60
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    cmpwi r3, -0x1
    bne @L_8002C760
    li r25, 0x0
    b @L_8002C768
    @L_8002C760:
    lwz r3, 0x108(r1)
    lhz r25, 0x0(r3)
    @L_8002C768:
    clrlwi r0, r25, 16
    cmplwi r0, 0x0
    beq @L_8002CB40
    mr r3, r25
    bl fn_801440A0
    bl fn_80143FE4
    clrlwi r16, r3, 16
    cmpwi r16, 0x0
    ble @L_8002C844
    cmpwi r30, 0x3
    beq @L_8002C7B8
    bge @L_8002C820
    cmpwi r30, 0x2
    bge @L_8002C7A4
    b @L_8002C820
    @L_8002C7A4:
    li r3, 0x0
    li r4, 0xd
    li r5, 0x0
    bl fn_8012A5B0
    b @L_8002C830
    @L_8002C7B8:
    cmplwi r28, 0x0
    beq @L_8002C818
    lwz r0, lbl_8047A660(r13)
    cmpwi r0, 0x0
    ble @L_8002C7F4
    lwz r5, 0x880(r1)
    li r0, 0x0
    lwz r3, lbl_8047A660(r13)
    lwz r4, 0x884(r1)
    add r3, r5, r3
    stw r3, 0x880(r1)
    lwz r3, lbl_8047A660(r13)
    add r3, r4, r3
    stw r3, 0x884(r1)
    stw r0, lbl_8047A660(r13)
    @L_8002C7F4:
    lwz r0, lbl_8047A664(r13)
    cmpwi r0, 0x0
    ble @L_8002C810
    li r0, 0x0
    stw r0, 0x880(r1)
    stw r0, 0x884(r1)
    stw r0, lbl_8047A664(r13)
    @L_8002C810:
    lwz r3, 0x880(r1)
    b @L_8002C830
    @L_8002C818:
    li r3, 0x0
    b @L_8002C830
    @L_8002C820:
    li r3, 0x0
    li r4, 0xd
    li r5, 0x0
    bl fn_8012A5B0
    @L_8002C830:
    divw r16, r3, r16
    cmpwi r16, 0x63
    ble @L_8002C848
    li r16, 0x63
    b @L_8002C848
    @L_8002C844:
    li r16, 0x63
    @L_8002C848:
    cmpwi r16, 0x0
    bgt @L_8002C868
    mr r3, r31
    li r4, 0x8
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_8002A1C4
    b @L_8002C628
    @L_8002C868:
    mr r3, r25
    bl fn_801440A0
    bl fn_80143FE4
    clrlwi r18, r25, 16
    clrlwi r17, r3, 16
    mr r4, r31
    addi r3, r1, 0x11
    mr r7, r18
    li r5, 0xc
    li r6, 0x2d
    li r8, -0x1
    crxor 6, 6, 6
    bl fn_80029FAC
    cmpwi r16, 0x1
    stw r3, lbl_8047A3E4(r13)
    bge @L_8002C8B0
    li r22, 0x0
    b @L_8002C940
    @L_8002C8B0:
    lbz r4, 0x0(r26)
    li r6, 0x1
    lbz r3, 0x0(r23)
    la r5, lbl_8047A3E0(r13)
    lbz r0, 0x0(r24)
    stw r6, 0x18(r1)
    stw r16, 0x1c(r1)
    stw r17, 0x20(r1)
    stw r6, lbl_8047A3E0(r13)
    stw r5, 0x24(r1)
    stb r4, 0x28(r1)
    stb r3, 0x29(r1)
    stb r0, 0x2a(r1)
    stw r6, 0x2c(r1)
    stw r6, 0x14(r1)
    bl fn_801046B8
    mr r4, r3
    addi r5, r1, 0x14
    addi r9, r1, 0x18
    li r3, 0x61
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    mr r16, r3
    li r3, 0x61
    bl fn_80102510
    li r3, 0x61
    li r4, 0x1
    bl fn_80102428
    cmpwi r16, -0x1
    bne @L_8002C93C
    li r22, -0x1
    b @L_8002C940
    @L_8002C93C:
    lwz r22, lbl_8047A3E0(r13)
    @L_8002C940:
    cmpwi r22, 0x0
    blt @L_8002C628
    mullw r21, r22, r17
    li r0, -0x1
    mr r4, r31
    mr r7, r18
    mr r9, r22
    addi r3, r1, 0x11
    stw r21, 0x8(r1)
    li r5, 0x5
    li r6, 0x2d
    li r8, 0x2f
    stw r0, 0xc(r1)
    li r10, 0x4b
    crxor 6, 6, 6
    bl fn_80029FAC
    lbz r7, 0x11(r1)
    mr r4, r3
    li r3, 0x2
    li r5, 0x1
    li r6, 0x0
    bl fn_80106ADC
    li r3, 0x0
    li r4, -0x1
    li r5, -0x1
    li r6, 0x0
    bl fn_8001E074
    extsb r16, r3
    li r3, 0x1
    bl fn_801069FC
    cmpwi r16, 0x1
    beq @L_8002C628
    cmpwi r16, -0x1
    beq @L_8002C628
    cmpwi r30, 0x3
    beq @L_8002C9F4
    bge @L_8002CA68
    cmpwi r30, 0x2
    bge @L_8002C9E0
    b @L_8002CA68
    @L_8002C9E0:
    mr r4, r25
    li r3, 0x0
    bl fn_80134420
    clrlwi r17, r3, 16
    b @L_8002CA78
    @L_8002C9F4:
    cmplwi r28, 0x0
    beq @L_8002CA60
    li r17, 0x0
    lhz r19, 0x890(r1)
    mr r16, r28
    mr r18, r17
    clrlwi r20, r25, 16
    b @L_8002CA54
    @L_8002CA14:
    mr r3, r16
    bl fn_80143C68
    clrlwi r0, r3, 16
    cmplw r0, r20
    bne @L_8002CA40
    mr r3, r16
    bl fn_80143C50
    subfic r0, r3, 0x3e7
    clrlwi r0, r0, 16
    add r17, r17, r0
    b @L_8002CA4C
    @L_8002CA40:
    cmplwi r0, 0x0
    bne @L_8002CA4C
    addi r17, r17, 0x3e7
    @L_8002CA4C:
    addi r18, r18, 0x1
    addi r16, r16, 0x4
    @L_8002CA54:
    cmpw r18, r19
    blt @L_8002CA14
    b @L_8002CA78
    @L_8002CA60:
    li r17, 0x0
    b @L_8002CA78
    @L_8002CA68:
    mr r4, r25
    li r3, 0x0
    bl fn_801298B8
    mr r17, r3
    @L_8002CA78:
    cmpw r17, r22
    bge @L_8002CA98
    mr r3, r31
    li r4, 0x9
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_8002A1C4
    b @L_8002C628
    @L_8002CA98:
    mr r3, r31
    li r4, 0x6
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_8002A1C4
    mr r3, r21
    mr r4, r25
    mr r5, r22
    mr r6, r29
    mr r7, r28
    bl fn_80029EF4
    li r3, 0x3cc
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    clrlwi r0, r29, 24
    cmplwi r0, 0x4
    bne @L_8002C628
    mr r4, r31
    addi r3, r1, 0x11
    li r5, 0x7
    li r6, -0x1
    crxor 6, 6, 6
    bl fn_80029FAC
    lbz r7, 0x11(r1)
    mr r4, r3
    li r3, 0x2
    li r5, 0x1
    li r6, 0x0
    bl fn_80106ADC
    li r3, 0x0
    li r4, -0x1
    li r5, -0x1
    li r6, 0x0
    bl fn_8001E074
    extsb r16, r3
    li r3, 0x1
    bl fn_801069FC
    cmpwi r16, -0x1
    beq @L_8002CB40
    cmpwi r16, 0x1
    bne @L_8002C628
    @L_8002CB40:
    cmplwi r15, 0x2
    beq @L_8002CB50
    cmplwi r15, 0x3
    bne @L_8002CE3C
    @L_8002CB50:
    lbz r0, 0x888(r1)
    cmplwi r0, 0x0
    bne @L_8002CBEC
    mr r4, r31
    addi r3, r1, 0x10
    li r5, 0xd
    li r6, -0x1
    crxor 6, 6, 6
    bl fn_80029FAC
    lbz r7, 0x10(r1)
    mr r4, r3
    li r3, 0x2
    li r5, 0x1
    li r6, 0x0
    bl fn_80106ADC
    li r3, 0x0
    li r4, -0x1
    li r5, -0x1
    li r6, 0x0
    bl fn_8001E074
    extsb r16, r3
    li r3, 0x1
    bl fn_801069FC
    cmpwi r16, 0x1
    beq @L_8002CBBC
    cmpwi r16, -0x1
    bne @L_8002CBC4
    @L_8002CBBC:
    li r0, 0x0
    b @L_8002CE34
    @L_8002CBC4:
    clrlwi r0, r29, 24
    cmplwi r0, 0x3
    bne @L_8002CBE4
    mr r3, r31
    li r4, 0xe
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_8002A1C4
    @L_8002CBE4:
    li r0, 0x1
    b @L_8002CE34
    @L_8002CBEC:
    mr r4, r31
    addi r3, r1, 0x10
    li r5, 0xf
    li r6, -0x1
    crxor 6, 6, 6
    bl fn_80029FAC
    lbz r7, 0x10(r1)
    mr r4, r3
    li r3, 0x2
    li r5, 0x1
    li r6, 0x0
    bl fn_80106ADC
    li r3, 0x0
    li r4, -0x1
    li r5, -0x1
    li r6, 0x0
    bl fn_8001E074
    extsb r16, r3
    li r3, 0x1
    bl fn_801069FC
    cmpwi r16, 0x1
    beq @L_8002CC4C
    cmpwi r16, -0x1
    bne @L_8002CC54
    @L_8002CC4C:
    li r0, 0x0
    b @L_8002CE34
    @L_8002CC54:
    clrlwi r0, r29, 24
    cmplwi r0, 0x3
    bne @L_8002CDF0
    lwz r0, lbl_80478E54(r13)
    li r3, 0x2
    lwz r7, lbl_80478E4C(r13)
    li r4, 0x3d83
    lbzx r0, r27, r0
    li r5, 0x0
    li r6, 0x0
    mulli r0, r0, 0x4c
    lbzx r16, r7, r0
    mr r7, r16
    bl fn_80106ADC
    addi r3, r1, 0x30
    li r4, 0x0
    li r5, 0xd8
    bl memset
    lwz r5, 0x880(r1)
    li r17, 0x0
    lwz r4, 0x884(r1)
    mr r20, r17
    lwz r3, 0x88c(r1)
    addi r19, r1, 0x30
    lhz r0, 0x890(r1)
    stw r5, 0x30(r1)
    stw r4, 0x34(r1)
    stw r3, 0x38(r1)
    sth r17, 0x3c(r1)
    sth r0, 0x3e(r1)
    b @L_8002CD48
    @L_8002CCD0:
    cmpwi r17, 0x0
    lhz r0, 0x890(r1)
    blt @L_8002CCE4
    cmpw r17, r0
    ble @L_8002CCEC
    @L_8002CCE4:
    li r21, 0x0
    b @L_8002CD14
    @L_8002CCEC:
    add r18, r28, r20
    mr r3, r18
    bl fn_80143C68
    mr r21, r3
    clrlwi r0, r21, 16
    cmplwi r0, 0x0
    beq @L_8002CD14
    mr r3, r18
    bl fn_80143C50
    sth r3, 0x12(r19)
    @L_8002CD14:
    sth r21, 0x10(r19)
    lhz r0, 0x10(r19)
    cmplwi r0, 0x0
    bne @L_8002CD3C
    slwi r4, r17, 2
    addi r3, r1, 0x30
    addi r0, r4, 0x12
    li r4, 0x0
    sthx r4, r3, r0
    b @L_8002CD54
    @L_8002CD3C:
    addi r19, r19, 0x4
    addi r17, r17, 0x1
    addi r20, r20, 0x4
    @L_8002CD48:
    lhz r0, 0x3e(r1)
    cmpw r17, r0
    blt @L_8002CCD0
    @L_8002CD54:
    li r3, 0x1
    bl fn_80093574
    addi r4, r1, 0x30
    li r3, 0x1
    li r5, 0x0
    bl fn_80092C90
    li r3, 0x1
    bl fn_80093574
    li r3, 0x1
    bl fn_80093610
    cmpwi r3, 0xc
    beq @L_8002CDB0
    li r3, 0x1
    bl fn_80093698
    mr r7, r16
    li r3, 0x2
    li r4, 0x3d85
    li r5, 0x1
    li r6, 0x0
    bl fn_80106ADC
    li r3, 0x1
    bl fn_801069FC
    b @L_8002CDD8
    @L_8002CDB0:
    li r3, 0x1
    bl fn_80093698
    mr r7, r16
    li r3, 0x2
    li r4, 0x3d84
    li r5, 0x1
    li r6, 0x0
    bl fn_80106ADC
    li r3, 0x1
    bl fn_801069FC
    @L_8002CDD8:
    mr r3, r31
    li r4, 0xe
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_8002A1C4
    b @L_8002CE30
    @L_8002CDF0:
    li r3, 0x4
    li r4, 0x2
    li r5, 0x0
    bl fn_801D0748
    cmpwi r3, 0x4
    beq @L_8002CE30
    li r3, 0x0
    li r4, 0x3
    bl fn_80129280
    lwz r4, lbl_8047A3DC(r13)
    li r5, 0x7198
    bl memcpy
    lwz r5, lbl_8047A3D8(r13)
    li r3, 0x0
    li r4, 0xd
    bl fn_8012A450
    @L_8002CE30:
    li r0, 0x1
    @L_8002CE34:
    mr r25, r0
    b @L_8002CE40
    @L_8002CE3C:
    li r25, 0x1
    @L_8002CE40:
    cmpwi r25, 0x0
    beq @L_8002C628
    li r3, 0x60
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    @L_8002CE58:
    lmw r15, 0x89c(r1)
    lwz r0, 0x8e4(r1)
    mtlr r0
    addi r1, r1, 0x8e0
    blr
