    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r22, 0x18(r1)
    mr r22, r3
    lis r3, lbl_80267EA8@ha
    addi r25, r3, lbl_80267EA8@l
    bl fn_80105624
    lbz r0, 0xa(r22)
    mr r26, r3
    cmplwi r0, 0x0
    bne @L_8006C004
    lhz r5, 0x6(r26)
    li r3, 0x0
    rlwinm r4, r5, 0, 29, 29
    clrlwi r7, r5, 31
    neg r0, r4
    rlwinm r5, r5, 0, 28, 28
    or r0, r0, r4
    neg r6, r7
    srwi r28, r0, 31
    neg r4, r5
    clrlwi r0, r28, 24
    or r6, r6, r7
    cmplwi r0, 0x0
    or r0, r4, r5
    srwi r27, r6, 31
    srwi r29, r0, 31
    bne @L_8006BBB4
    clrlwi r0, r29, 24
    cmplwi r0, 0x0
    beq @L_8006BBB8
    @L_8006BBB4:
    li r3, 0x1
    @L_8006BBB8:
    clrlwi r0, r28, 24
    clrlwi r24, r3, 24
    cmplwi r0, 0x0
    li r4, 0x0
    bne @L_8006BBD8
    clrlwi r0, r29, 24
    cmplwi r0, 0x0
    bne @L_8006BBDC
    @L_8006BBD8:
    li r4, 0x1
    @L_8006BBDC:
    lwz r3, 0x4(r22)
    clrlwi r23, r4, 24
    bl fn_801022B8
    mr r30, r3
    bl fn_80077BD0
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8006BC3C
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_8006C004
    lwz r3, 0x4(r22)
    li r4, 0xe35
    bl fn_80102138
    extsb r3, r3
    li r0, 0x0
    stb r3, 0x11(r1)
    addi r4, r1, 0xc
    stb r0, 0x10(r1)
    lhz r0, 0x10(r1)
    sth r0, 0xc(r1)
    lwz r3, 0x4(r22)
    bl fn_801044D0
    b @L_8006C004
    @L_8006BC3C:
    mr r3, r22
    li r4, 0x0
    bl fn_801040D0
    lhz r4, 0x6(r26)
    mr r31, r3
    li r3, 0x0
    rlwinm r0, r4, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_8006BC68
    li r3, -0x1
    b @L_8006BC78
    @L_8006BC68:
    clrlwi r0, r4, 31
    cmpwi r0, 0x0
    beq @L_8006BC78
    li r3, 0x1
    @L_8006BC78:
    cmpwi r30, 0xe33
    beq @L_8006BCE8
    bge @L_8006BC9C
    cmpwi r30, 0xa0d
    beq @L_8006BCAC
    bge @L_8006BD20
    cmpwi r30, 0xa0c
    bge @L_8006BCA8
    b @L_8006BD20
    @L_8006BC9C:
    cmpwi r30, 0xe35
    bge @L_8006BD20
    b @L_8006BCE4
    @L_8006BCA8:
    mulli r3, r3, 0xa
    @L_8006BCAC:
    lha r0, 0x14(r31)
    cmpwi r0, 0x0
    blt @L_8006BD24
    add r0, r0, r3
    cmpwi r0, 0x1
    bge @L_8006BCCC
    li r0, 0x1
    b @L_8006BCD8
    @L_8006BCCC:
    cmpwi r0, 0x63
    ble @L_8006BCD8
    li r0, 0x63
    @L_8006BCD8:
    extsh r0, r0
    sth r0, 0x14(r31)
    b @L_8006BD24
    @L_8006BCE4:
    mulli r3, r3, 0xa
    @L_8006BCE8:
    lha r0, 0x16(r31)
    cmpwi r0, 0x0
    blt @L_8006BD24
    add r0, r0, r3
    cmpwi r0, 0x1
    bge @L_8006BD08
    li r0, 0x1
    b @L_8006BD14
    @L_8006BD08:
    cmpwi r0, 0x63
    ble @L_8006BD14
    li r0, 0x63
    @L_8006BD14:
    extsh r0, r0
    sth r0, 0x16(r31)
    b @L_8006BD24
    @L_8006BD20:
    li r3, 0x0
    @L_8006BD24:
    cmpwi r3, 0x0
    bne @L_8006C004
    lhz r0, 0x94(r22)
    cmpwi r30, 0x9fd
    sth r0, 0x14(r1)
    beq @L_8006BFB8
    bge @L_8006BD6C
    cmpwi r30, 0x9f9
    beq @L_8006BDE4
    bge @L_8006BD5C
    cmpwi r30, 0x9f7
    beq @L_8006BD9C
    bge @L_8006BDC0
    b @L_8006BFFC
    @L_8006BD5C:
    cmpwi r30, 0x9fb
    beq @L_8006BE8C
    bge @L_8006BFFC
    b @L_8006BE08
    @L_8006BD6C:
    cmpwi r30, 0xe33
    beq @L_8006BF70
    bge @L_8006BD90
    cmpwi r30, 0xa0d
    beq @L_8006BF1C
    bge @L_8006BFFC
    cmpwi r30, 0xa0c
    bge @L_8006BF10
    b @L_8006BFFC
    @L_8006BD90:
    cmpwi r30, 0xe35
    bge @L_8006BFFC
    b @L_8006BF64
    @L_8006BD9C:
    cmplwi r24, 0x0
    beq @L_8006BFFC
    lbz r0, 0x11(r31)
    cmplw r0, r23
    beq @L_8006BDB8
    li r3, 0x24
    bl fn_80166A28
    @L_8006BDB8:
    stb r23, 0x11(r31)
    b @L_8006C004
    @L_8006BDC0:
    cmplwi r24, 0x0
    beq @L_8006BFFC
    lbz r0, 0x12(r31)
    cmplw r0, r23
    beq @L_8006BDDC
    li r3, 0x24
    bl fn_80166A28
    @L_8006BDDC:
    stb r23, 0x12(r31)
    b @L_8006C004
    @L_8006BDE4:
    cmplwi r24, 0x0
    beq @L_8006BFFC
    lbz r0, 0x13(r31)
    cmplw r0, r23
    beq @L_8006BE00
    li r3, 0x24
    bl fn_80166A28
    @L_8006BE00:
    stb r23, 0x13(r31)
    b @L_8006C004
    @L_8006BE08:
    cmplwi r24, 0x0
    beq @L_8006BE54
    lha r4, 0x14(r31)
    srwi r0, r4, 31
    srawi r3, r4, 31
    cmpw r0, r23
    xor r22, r3, r4
    subf r22, r3, r22
    bne @L_8006BE34
    li r3, 0x24
    bl fn_80166A28
    @L_8006BE34:
    cmplwi r23, 0x0
    beq @L_8006BE44
    mr r0, r22
    b @L_8006BE48
    @L_8006BE44:
    neg r0, r22
    @L_8006BE48:
    extsh r0, r0
    sth r0, 0x14(r31)
    b @L_8006C004
    @L_8006BE54:
    lha r0, 0x14(r31)
    cmpwi r0, 0x0
    blt @L_8006BFFC
    lhz r0, 0x4(r26)
    rlwinm r0, r0, 0, 27, 27
    cmpwi r0, 0x0
    beq @L_8006BFFC
    lwz r3, 0x4(r22)
    li r4, 0xa0c
    bl fn_80102138
    mr r4, r3
    lwz r3, 0x4(r22)
    bl fn_80102398
    b @L_8006C004
    @L_8006BE8C:
    cmplwi r24, 0x0
    beq @L_8006BED8
    lha r4, 0x16(r31)
    srwi r0, r4, 31
    srawi r3, r4, 31
    cmpw r0, r23
    xor r22, r3, r4
    subf r22, r3, r22
    bne @L_8006BEB8
    li r3, 0x24
    bl fn_80166A28
    @L_8006BEB8:
    cmplwi r23, 0x0
    beq @L_8006BEC8
    mr r0, r22
    b @L_8006BECC
    @L_8006BEC8:
    neg r0, r22
    @L_8006BECC:
    extsh r0, r0
    sth r0, 0x16(r31)
    b @L_8006C004
    @L_8006BED8:
    lha r0, 0x16(r31)
    cmpwi r0, 0x0
    blt @L_8006BFFC
    lhz r0, 0x4(r26)
    rlwinm r0, r0, 0, 27, 27
    cmpwi r0, 0x0
    beq @L_8006BFFC
    lwz r3, 0x4(r22)
    li r4, 0xe34
    bl fn_80102138
    mr r4, r3
    lwz r3, 0x4(r22)
    bl fn_80102398
    b @L_8006C004
    @L_8006BF10:
    clrlwi r0, r28, 24
    cmplwi r0, 0x0
    bne @L_8006C004
    @L_8006BF1C:
    lha r0, 0x14(r31)
    cmpwi r0, 0x0
    bge @L_8006BF38
    addi r3, r25, 0x7d8
    addi r5, r25, 0x7e8
    li r4, 0xe73
    bl fn_80196E10
    @L_8006BF38:
    lhz r0, 0x4(r26)
    rlwinm r0, r0, 0, 26, 27
    cmpwi r0, 0x0
    beq @L_8006BFFC
    lwz r3, 0x4(r22)
    li r4, 0x9fa
    bl fn_80102138
    mr r4, r3
    lwz r3, 0x4(r22)
    bl fn_80102398
    b @L_8006C004
    @L_8006BF64:
    clrlwi r0, r28, 24
    cmplwi r0, 0x0
    bne @L_8006C004
    @L_8006BF70:
    lha r0, 0x16(r31)
    cmpwi r0, 0x0
    bge @L_8006BF8C
    addi r3, r25, 0x7d8
    addi r5, r25, 0x808
    li r4, 0xe7f
    bl fn_80196E10
    @L_8006BF8C:
    lhz r0, 0x4(r26)
    rlwinm r0, r0, 0, 26, 27
    cmpwi r0, 0x0
    beq @L_8006BFFC
    lwz r3, 0x4(r22)
    li r4, 0x9fb
    bl fn_80102138
    mr r4, r3
    lwz r3, 0x4(r22)
    bl fn_80102398
    b @L_8006C004
    @L_8006BFB8:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_8006BFF0
    lwz r3, 0x4(r22)
    li r4, 0x9fb
    bl fn_80102138
    extsb r0, r3
    addi r4, r1, 0x8
    stb r0, 0x15(r1)
    lhz r0, 0x14(r1)
    sth r0, 0x8(r1)
    lwz r3, 0x4(r22)
    bl fn_801044D0
    b @L_8006C004
    @L_8006BFF0:
    clrlwi r0, r29, 24
    cmplwi r0, 0x0
    bne @L_8006C004
    @L_8006BFFC:
    mr r3, r22
    bl fn_80102F38
    @L_8006C004:
    lmw r22, 0x18(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
