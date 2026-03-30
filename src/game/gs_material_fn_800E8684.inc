    nofralloc
    stwu r1, -0xe0(r1)
    mflr r0
    lis r3, lbl_80401490@ha
    stw r0, 0xe4(r1)
    li r0, 0x3
    stmw r23, 0xbc(r1)
    addi r29, r3, lbl_80401490@l
    li r3, 0x0
    mr r5, r29
    mtctr r0
    @L_800E86AC:
    li r30, 0x0
    stb r30, 0x50(r5)
    stw r30, 0x0(r5)
    stw r30, 0x8(r5)
    stw r30, 0xc(r5)
    stw r30, 0x10(r5)
    stw r30, 0x14(r5)
    stw r30, 0x18(r5)
    stw r30, 0x1c(r5)
    stw r30, 0x20(r5)
    stw r30, 0x24(r5)
    stw r30, 0x28(r5)
    stw r30, 0x2c(r5)
    stw r30, 0x30(r5)
    stw r30, 0x34(r5)
    stw r30, 0x38(r5)
    stw r30, 0x3c(r5)
    stw r30, 0x40(r5)
    stw r30, 0x44(r5)
    stb r30, 0xa8(r5)
    stwu r30, 0x58(r5)
    stw r30, 0x8(r5)
    addi r3, r3, 0x1
    stw r30, 0xc(r5)
    stw r30, 0x10(r5)
    stw r30, 0x14(r5)
    stw r30, 0x18(r5)
    stw r30, 0x1c(r5)
    stw r30, 0x20(r5)
    stw r30, 0x24(r5)
    stw r30, 0x28(r5)
    stw r30, 0x2c(r5)
    stw r30, 0x30(r5)
    stw r30, 0x34(r5)
    stw r30, 0x38(r5)
    stw r30, 0x3c(r5)
    stw r30, 0x40(r5)
    stw r30, 0x44(r5)
    addi r5, r5, 0x58
    bdnz @L_800E86AC
    li r28, 0x0
    b @L_800E8A74
    @L_800E8754:
    lwz r0, lbl_8047AB74(r13)
    li r5, 0x0
    add r26, r0, r30
    lwz r3, 0x0(r26)
    clrlwi. r0, r3, 31
    beq @L_800E8A6C
    rlwinm. r4, r3, 0, 30, 30
    beq @L_800E8788
    rlwinm. r0, r3, 0, 9, 9
    beq @L_800E8788
    rlwinm. r0, r3, 0, 3, 3
    beq @L_800E8788
    li r5, 0x1
    @L_800E8788:
    rlwinm. r0, r3, 0, 2, 2
    beq @L_800E8794
    li r5, 0x1
    @L_800E8794:
    rlwinm. r0, r3, 0, 21, 21
    beq @L_800E87A8
    cmplwi r4, 0x0
    beq @L_800E87A8
    li r5, 0x1
    @L_800E87A8:
    clrlwi. r0, r5, 24
    beq @L_800E8A6C
    li r27, 0x0
    li r31, 0x0
    b @L_800E8A60
    @L_800E87BC:
    lwz r23, 0x160(r26)
    lwz r3, 0x15c(r26)
    cmplwi r23, 0x0
    lwzx r24, r3, r31
    beq @L_800E8A58
    lfs f1, 0x30(r26)
    lfs f0, 0x34(r26)
    lfs f3, 0x38(r26)
    fadds f2, f1, f0
    lfs f1, lbl_8047CBC0(r2)
    lfs f0, lbl_8047CBC4(r2)
    fadds f2, f3, f2
    fdivs f1, f2, f1
    fcmpo cr0, f1, f0
    blt @L_800E8A58
    mr r3, r24
    mr r4, r26
    mr r5, r23
    addi r6, r26, 0x4c
    bl fn_800E9998
    mr. r25, r3
    beq @L_800E8A44
    li r0, 0x2
    mr r4, r25
    li r3, 0x0
    mtctr r0
    @L_800E8824:
    lwz r0, 0x8(r4)
    cmplw r0, r26
    bne @L_800E8834
    b @L_800E88D0
    @L_800E8834:
    lwz r0, 0xc(r4)
    addi r3, r3, 0x1
    cmplw r0, r26
    bne @L_800E8848
    b @L_800E88D0
    @L_800E8848:
    lwz r0, 0x10(r4)
    addi r3, r3, 0x1
    cmplw r0, r26
    bne @L_800E885C
    b @L_800E88D0
    @L_800E885C:
    lwz r0, 0x14(r4)
    addi r3, r3, 0x1
    cmplw r0, r26
    bne @L_800E8870
    b @L_800E88D0
    @L_800E8870:
    lwz r0, 0x18(r4)
    addi r3, r3, 0x1
    cmplw r0, r26
    bne @L_800E8884
    b @L_800E88D0
    @L_800E8884:
    lwz r0, 0x1c(r4)
    addi r3, r3, 0x1
    cmplw r0, r26
    bne @L_800E8898
    b @L_800E88D0
    @L_800E8898:
    lwz r0, 0x20(r4)
    addi r3, r3, 0x1
    cmplw r0, r26
    bne @L_800E88AC
    b @L_800E88D0
    @L_800E88AC:
    lwz r0, 0x24(r4)
    addi r3, r3, 0x1
    cmplw r0, r26
    bne @L_800E88C0
    b @L_800E88D0
    @L_800E88C0:
    addi r4, r4, 0x20
    addi r3, r3, 0x1
    bdnz @L_800E8824
    li r3, -0x1
    @L_800E88D0:
    cmpwi r3, -0x1
    bne @L_800E8A58
    li r0, 0x2
    mr r4, r25
    li r3, 0x0
    mtctr r0
    @L_800E88E8:
    lwz r0, 0x8(r4)
    cmplwi r0, 0x0
    bne @L_800E88F8
    b @L_800E8994
    @L_800E88F8:
    lwz r0, 0xc(r4)
    addi r3, r3, 0x1
    cmplwi r0, 0x0
    bne @L_800E890C
    b @L_800E8994
    @L_800E890C:
    lwz r0, 0x10(r4)
    addi r3, r3, 0x1
    cmplwi r0, 0x0
    bne @L_800E8920
    b @L_800E8994
    @L_800E8920:
    lwz r0, 0x14(r4)
    addi r3, r3, 0x1
    cmplwi r0, 0x0
    bne @L_800E8934
    b @L_800E8994
    @L_800E8934:
    lwz r0, 0x18(r4)
    addi r3, r3, 0x1
    cmplwi r0, 0x0
    bne @L_800E8948
    b @L_800E8994
    @L_800E8948:
    lwz r0, 0x1c(r4)
    addi r3, r3, 0x1
    cmplwi r0, 0x0
    bne @L_800E895C
    b @L_800E8994
    @L_800E895C:
    lwz r0, 0x20(r4)
    addi r3, r3, 0x1
    cmplwi r0, 0x0
    bne @L_800E8970
    b @L_800E8994
    @L_800E8970:
    lwz r0, 0x24(r4)
    addi r3, r3, 0x1
    cmplwi r0, 0x0
    bne @L_800E8984
    b @L_800E8994
    @L_800E8984:
    addi r4, r4, 0x20
    addi r3, r3, 0x1
    bdnz @L_800E88E8
    li r3, -0x1
    @L_800E8994:
    cmpwi r3, -0x1
    bne @L_800E89B4
    mr r3, r24
    mr r4, r26
    mr r5, r23
    addi r6, r26, 0x4c
    bl fn_800E93B8
    b @L_800E8A58
    @L_800E89B4:
    slwi r4, r3, 2
    addi r3, r26, 0x4c
    addi r0, r4, 0x8
    addi r4, r1, 0x8
    stwx r26, r25, r0
    bl fn_80190E34
    lfs f1, 0x8(r1)
    lfs f0, 0xc(r1)
    fcmpo cr0, f0, f1
    ble @L_800E89E0
    fmr f1, f0
    @L_800E89E0:
    lfs f0, 0x10(r1)
    fcmpo cr0, f0, f1
    ble @L_800E89F0
    fmr f1, f0
    @L_800E89F0:
    lwz r3, 0x58(r26)
    lfs f0, lbl_8047CBC0(r2)
    lfs f3, 0x0(r3)
    lfs f2, 0x4(r3)
    lfs f4, 0x8(r3)
    fadds f2, f3, f2
    fadds f2, f4, f2
    fdivs f0, f2, f0
    fmuls f1, f1, f0
    bl fn_800CDA74
    frsp f1, f1
    bl fn_800C46B0
    lwz r0, 0x48(r25)
    cmplw r3, r0
    bge @L_800E8A30
    stw r3, 0x48(r25)
    @L_800E8A30:
    lwz r0, 0x4c(r25)
    cmplw r3, r0
    ble @L_800E8A58
    stw r3, 0x4c(r25)
    b @L_800E8A58
    @L_800E8A44:
    mr r3, r24
    mr r4, r26
    mr r5, r23
    addi r6, r26, 0x4c
    bl fn_800E93B8
    @L_800E8A58:
    addi r31, r31, 0x4
    addi r27, r27, 0x1
    @L_800E8A60:
    lwz r0, 0x158(r26)
    cmplw r27, r0
    blt @L_800E87BC
    @L_800E8A6C:
    addi r30, r30, 0x170
    addi r28, r28, 0x1
    @L_800E8A74:
    lwz r0, lbl_8047AB78(r13)
    cmplw r28, r0
    blt @L_800E8754
    li r30, 0x0
    @L_800E8A84:
    lwz r3, 0x0(r29)
    cmplwi r3, 0x0
    bne @L_800E8AA4
    lwz r3, 0x54(r29)
    bl fn_801B06DC
    li r0, 0x0
    stb r0, 0x50(r29)
    b @L_800E8ED8
    @L_800E8AA4:
    lwz r0, 0x0(r3)
    clrlwi. r0, r0, 31
    beq @L_800E8ED8
    lfs f1, lbl_8047CBC8(r2)
    addi r3, r1, 0x44
    li r26, 0x0
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    li r25, 0x0
    mr r23, r29
    @L_800E8AD0:
    lwz r24, 0x8(r23)
    cmplwi r24, 0x0
    beq @L_800E8B1C
    lwz r0, 0x0(r24)
    clrlwi. r0, r0, 31
    beq @L_800E8B1C
    mr r3, r24
    bl fn_800E5188
    mr r4, r3
    lwz r3, 0x54(r29)
    bl fn_801B07D4
    mr r3, r24
    addi r4, r1, 0x38
    bl fn_800E3D14
    addi r3, r1, 0x44
    addi r5, r1, 0x38
    mr r4, r3
    bl fn_800E019C
    addi r26, r26, 0x1
    @L_800E8B1C:
    addi r25, r25, 0x1
    addi r23, r23, 0x4
    cmplwi r25, 0x10
    blt @L_800E8AD0
    cmplwi r26, 0x0
    bne @L_800E8B40
    li r0, 0x0
    stb r0, 0x50(r29)
    b @L_800E8ED8
    @L_800E8B40:
    lwz r3, 0x4(r29)
    lwz r3, 0xc(r3)
    lhz r0, 0x8(r3)
    clrlwi r0, r0, 30
    cmpwi r0, 0x1
    beq @L_800E8B68
    blt @L_800E8B8C
    cmpwi r0, 0x4
    bge @L_800E8B8C
    b @L_800E8B7C
    @L_800E8B68:
    lwz r3, 0x54(r29)
    li r4, 0x3
    lwz r3, 0x4(r3)
    bl fn_801943BC
    b @L_800E8B8C
    @L_800E8B7C:
    lwz r3, 0x54(r29)
    li r4, 0x2
    lwz r3, 0x4(r3)
    bl fn_801943BC
    @L_800E8B8C:
    lis r0, 0x4330
    addi r3, r1, 0x44
    stw r26, 0xac(r1)
    mr r4, r3
    lfd f1, lbl_8047CBE0(r2)
    stw r0, 0xa8(r1)
    lfd f0, 0xa8(r1)
    fsubs f1, f0, f1
    bl fn_800E00AC
    lwz r3, 0x54(r29)
    addi r4, r1, 0x44
    lwz r3, 0x4(r3)
    bl fn_80195970
    lwz r4, 0x4(r29)
    lwz r3, 0x54(r29)
    lwz r4, 0xc(r4)
    lfs f1, lbl_8047CBCC(r2)
    bl fn_801B0A98
    lwz r3, 0x54(r29)
    lfs f1, lbl_8047CBD0(r2)
    lwz r3, 0x4(r3)
    bl fn_801944E8
    lwz r3, 0x54(r29)
    lfs f1, lbl_8047CBD4(r2)
    lwz r3, 0x4(r3)
    bl fn_801944C0
    lwz r3, 0x54(r29)
    lwz r3, 0x4(r3)
    bl fn_80195794
    lis r3, lbl_80478AC0@ha
    lfs f0, lbl_80478AC0@l(r3)
    fcmpu cr0, f1, f0
    beq @L_800E8C1C
    lfs f0, lbl_8047CBD0(r2)
    fcmpo cr0, f1, f0
    bge @L_800E8C2C
    @L_800E8C1C:
    lwz r3, 0x54(r29)
    li r4, 0x0
    bl fn_801B073C
    b @L_800E8EE8
    @L_800E8C2C:
    lwz r3, 0x54(r29)
    addi r4, r1, 0x38
    lwz r3, 0x4(r3)
    bl fn_80195904
    lwz r3, 0x54(r29)
    addi r4, r1, 0x2c
    lwz r3, 0x4(r3)
    bl fn_801950D0
    lwz r6, 0x4(r29)
    addi r3, r1, 0x50
    addi r4, r1, 0x38
    addi r5, r1, 0x44
    lwz r7, 0xc(r6)
    addi r6, r1, 0x2c
    lhz r0, 0x8(r7)
    clrlwi r8, r0, 30
    subfic r7, r8, 0x1
    subi r0, r8, 0x1
    or r0, r7, r0
    srwi r7, r0, 31
    bl fn_801B0408
    lwz r3, 0x54(r29)
    lwz r23, 0x0(r3)
    b @L_800E8DDC
    @L_800E8C8C:
    lwz r3, 0x4(r23)
    bl fn_800E3B6C
    mr. r27, r3
    beq @L_800E8DD8
    addi r4, r1, 0x38
    bl fn_800E3D14
    cmplwi r26, 0x1
    bne @L_800E8D30
    addi r3, r27, 0x4c
    addi r4, r1, 0x20
    bl fn_80190E34
    lfs f1, 0x20(r1)
    lfs f0, 0x24(r1)
    fcmpo cr0, f0, f1
    ble @L_800E8CCC
    fmr f1, f0
    @L_800E8CCC:
    lfs f0, 0x28(r1)
    fcmpo cr0, f0, f1
    ble @L_800E8CDC
    fmr f1, f0
    @L_800E8CDC:
    lwz r3, 0x58(r27)
    lfs f0, lbl_8047CBC0(r2)
    lfs f3, 0x0(r3)
    lfs f2, 0x4(r3)
    lfs f4, 0x8(r3)
    fadds f2, f3, f2
    fadds f2, f4, f2
    fdivs f0, f2, f0
    fmuls f1, f1, f0
    bl fn_800CDA74
    frsp f1, f1
    bl fn_800C46B0
    lwz r4, lbl_8047AB80(r13)
    lis r0, 0x4330
    stw r0, 0xa8(r1)
    add r0, r4, r3
    lfd f1, lbl_8047CBE0(r2)
    stw r0, 0xac(r1)
    lfd f0, 0xa8(r1)
    fsubs f1, f0, f1
    b @L_800E8DB0
    @L_800E8D30:
    addi r3, r27, 0x4c
    addi r4, r1, 0x14
    bl fn_80190E34
    lfs f1, 0x14(r1)
    lfs f0, 0x18(r1)
    fcmpo cr0, f0, f1
    ble @L_800E8D50
    fmr f1, f0
    @L_800E8D50:
    lfs f0, 0x1c(r1)
    fcmpo cr0, f0, f1
    ble @L_800E8D60
    fmr f1, f0
    @L_800E8D60:
    lwz r3, 0x58(r27)
    lfs f0, lbl_8047CBC0(r2)
    lfs f3, 0x0(r3)
    lfs f2, 0x4(r3)
    lfs f4, 0x8(r3)
    fadds f2, f3, f2
    fadds f2, f4, f2
    fdivs f0, f2, f0
    fmuls f1, f1, f0
    bl fn_800CDA74
    frsp f1, f1
    bl fn_800C46B0
    lwz r4, lbl_8047AB84(r13)
    lis r0, 0x4330
    stw r0, 0xa8(r1)
    add r0, r4, r3
    lfd f1, lbl_8047CBE0(r2)
    stw r0, 0xac(r1)
    lfd f0, 0xa8(r1)
    fsubs f1, f0, f1
    @L_800E8DB0:
    lfs f0, lbl_8047CBD0(r2)
    fcmpo cr0, f1, f0
    bge @L_800E8DC0
    fmr f1, f0
    @L_800E8DC0:
    fneg f2, f1
    addi r3, r1, 0x50
    fmr f4, f1
    addi r4, r1, 0x38
    fmr f3, f2
    bl fn_801B019C
    @L_800E8DD8:
    lwz r23, 0x0(r23)
    @L_800E8DDC:
    cmplwi r23, 0x0
    bne @L_800E8C8C
    addi r3, r1, 0x50
    bl fn_801B03A0
    cmpwi r3, 0x0
    beq @L_800E8E10
    lwz r3, 0x54(r29)
    lfs f1, 0x90(r1)
    lfs f2, 0x94(r1)
    lfs f3, 0x98(r1)
    lfs f4, 0x9c(r1)
    bl fn_801B04E0
    b @L_800E8E58
    @L_800E8E10:
    lwz r4, 0x4c(r29)
    lis r0, 0x4330
    lwz r3, lbl_8047AB84(r13)
    stw r0, 0xa8(r1)
    add r0, r4, r3
    lfd f2, lbl_8047CBE0(r2)
    stw r0, 0xac(r1)
    lfs f0, lbl_8047CBD0(r2)
    lfd f1, 0xa8(r1)
    fsubs f1, f1, f2
    fcmpo cr0, f1, f0
    bge @L_800E8E44
    fmr f1, f0
    @L_800E8E44:
    fneg f2, f1
    lwz r3, 0x54(r29)
    fmr f4, f1
    fmr f3, f2
    bl fn_801B04E0
    @L_800E8E58:
    lwz r23, 0x54(r29)
    lbz r0, lbl_8047AB94(r13)
    cmplwi r23, 0x0
    subfic r0, r0, 0xff
    clrlwi r24, r0, 24
    bne @L_800E8E84
    lis r3, lbl_80270E98@ha
    li r4, 0x6b
    addi r3, r3, lbl_80270E98@l
    la r5, lbl_8047CBD8(r2)
    bl fn_80196E10
    @L_800E8E84:
    stb r24, 0x20(r23)
    lwz r4, lbl_8047AB90(r13)
    lwz r0, lbl_8047AB8C(r13)
    lwz r3, 0x54(r29)
    clrlwi r4, r4, 16
    clrlwi r5, r0, 16
    bl fn_801B1524
    li r0, 0x1
    li r3, 0x3
    stb r0, 0x50(r29)
    bl fn_8019C708
    lwz r3, 0x54(r29)
    bl fn_801B16C0
    lwz r3, 0x54(r29)
    bl fn_801B0EB8
    lwz r3, 0x54(r29)
    bl fn_801B0BD8
    bl fn_8019C6FC
    lwz r3, 0x54(r29)
    li r4, 0x0
    bl fn_801B073C
    @L_800E8ED8:
    addi r30, r30, 0x1
    addi r29, r29, 0x58
    cmplwi r30, 0x6
    blt @L_800E8A84
    @L_800E8EE8:
    lmw r23, 0xbc(r1)
    lwz r0, 0xe4(r1)
    mtlr r0
    addi r1, r1, 0xe0
    blr
