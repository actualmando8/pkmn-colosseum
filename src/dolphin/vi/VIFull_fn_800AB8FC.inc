    nofralloc
    lwz r0, 0x0(r5)
    extrwi r0, r0, 14, 2
    sth r0, 0x0(r4)
    lwz r0, 0x0(r5)
    srwi r0, r0, 8
    extsb r0, r0
    stb r0, 0x2(r4)
    lwz r0, 0x0(r5)
    extsb r0, r0
    stb r0, 0x3(r4)
    lwz r0, lbl_80478A14(r13)
    rlwinm r0, r0, 0, 21, 23
    cmpwi r0, 0x400
    beq @L_800ABAC8
    bge @L_800AB968
    cmpwi r0, 0x200
    beq @L_800ABA34
    bge @L_800AB95C
    cmpwi r0, 0x100
    beq @L_800AB9E0
    bge @L_800ABB08
    cmpwi r0, 0x0
    beq @L_800AB98C
    b @L_800ABB08
    @L_800AB95C:
    cmpwi r0, 0x300
    beq @L_800ABA84
    b @L_800ABB08
    @L_800AB968:
    cmpwi r0, 0x600
    beq @L_800AB98C
    bge @L_800AB980
    cmpwi r0, 0x500
    beq @L_800AB98C
    b @L_800ABB08
    @L_800AB980:
    cmpwi r0, 0x700
    beq @L_800AB98C
    b @L_800ABB08
    @L_800AB98C:
    lwz r0, 0x4(r5)
    srwi r0, r0, 24
    extsb r0, r0
    stb r0, 0x4(r4)
    lwz r0, 0x4(r5)
    srwi r0, r0, 16
    extsb r0, r0
    stb r0, 0x5(r4)
    lwz r0, 0x4(r5)
    rlwinm r0, r0, 24, 24, 27
    stb r0, 0x6(r4)
    lwz r0, 0x4(r5)
    rlwinm r0, r0, 28, 24, 27
    stb r0, 0x7(r4)
    lwz r0, 0x4(r5)
    rlwinm r0, r0, 0, 24, 27
    stb r0, 0x8(r4)
    lwz r0, 0x4(r5)
    clrlslwi r0, r0, 28, 4
    stb r0, 0x9(r4)
    b @L_800ABB08
    @L_800AB9E0:
    lwz r0, 0x4(r5)
    rlwinm r0, r0, 8, 24, 27
    extsb r0, r0
    stb r0, 0x4(r4)
    lwz r0, 0x4(r5)
    rlwinm r0, r0, 12, 24, 27
    extsb r0, r0
    stb r0, 0x5(r4)
    lwz r0, 0x4(r5)
    extrwi r0, r0, 8, 8
    stb r0, 0x6(r4)
    lwz r0, 0x4(r5)
    extrwi r0, r0, 8, 16
    stb r0, 0x7(r4)
    lwz r0, 0x4(r5)
    rlwinm r0, r0, 0, 24, 27
    stb r0, 0x8(r4)
    lwz r0, 0x4(r5)
    clrlslwi r0, r0, 28, 4
    stb r0, 0x9(r4)
    b @L_800ABB08
    @L_800ABA34:
    lwz r0, 0x4(r5)
    rlwinm r0, r0, 8, 24, 27
    extsb r0, r0
    stb r0, 0x4(r4)
    lwz r0, 0x4(r5)
    rlwinm r0, r0, 12, 24, 27
    extsb r0, r0
    stb r0, 0x5(r4)
    lwz r0, 0x4(r5)
    rlwinm r0, r0, 16, 24, 27
    stb r0, 0x6(r4)
    lwz r0, 0x4(r5)
    rlwinm r0, r0, 20, 24, 27
    stb r0, 0x7(r4)
    lwz r0, 0x4(r5)
    extrwi r0, r0, 8, 16
    stb r0, 0x8(r4)
    lwz r0, 0x4(r5)
    stb r0, 0x9(r4)
    b @L_800ABB08
    @L_800ABA84:
    lwz r6, 0x4(r5)
    li r0, 0x0
    srwi r6, r6, 24
    extsb r6, r6
    stb r6, 0x4(r4)
    lwz r6, 0x4(r5)
    srwi r6, r6, 16
    extsb r6, r6
    stb r6, 0x5(r4)
    lwz r6, 0x4(r5)
    extrwi r6, r6, 8, 16
    stb r6, 0x6(r4)
    lwz r5, 0x4(r5)
    stb r5, 0x7(r4)
    stb r0, 0x8(r4)
    stb r0, 0x9(r4)
    b @L_800ABB08
    @L_800ABAC8:
    lwz r6, 0x4(r5)
    li r0, 0x0
    srwi r6, r6, 24
    extsb r6, r6
    stb r6, 0x4(r4)
    lwz r6, 0x4(r5)
    srwi r6, r6, 16
    extsb r6, r6
    stb r6, 0x5(r4)
    stb r0, 0x6(r4)
    stb r0, 0x7(r4)
    lwz r0, 0x4(r5)
    extrwi r0, r0, 8, 16
    stb r0, 0x8(r4)
    lwz r0, 0x4(r5)
    stb r0, 0x9(r4)
    @L_800ABB08:
    lbz r6, 0x2(r4)
    lis r5, lbl_803FC5E0@ha
    mulli r3, r3, 0xc
    subi r0, r6, 0x80
    stb r0, 0x2(r4)
    addi r0, r5, lbl_803FC5E0@l
    add r3, r0, r3
    lbz r5, 0x3(r4)
    subi r0, r5, 0x80
    stb r0, 0x3(r4)
    lbz r5, 0x4(r4)
    subi r0, r5, 0x80
    stb r0, 0x4(r4)
    lbz r5, 0x5(r4)
    subi r0, r5, 0x80
    stb r0, 0x5(r4)
    lbz r7, 0x2(r3)
    lbz r6, 0x2(r4)
    extsb. r0, r7
    ble @L_800ABB78
    extsb r5, r7
    subi r0, r5, 0x80
    extsb r5, r0
    extsb r0, r6
    cmpw r0, r5
    bge @L_800ABB9C
    mr r6, r5
    b @L_800ABB9C
    @L_800ABB78:
    extsb. r0, r7
    bge @L_800ABB9C
    extsb r5, r7
    addi r0, r5, 0x7f
    extsb r5, r0
    extsb r0, r6
    cmpw r5, r0
    bge @L_800ABB9C
    mr r6, r5
    @L_800ABB9C:
    subf r6, r7, r6
    stb r6, 0x2(r4)
    lbz r7, 0x3(r3)
    lbz r6, 0x3(r4)
    extsb. r0, r7
    ble @L_800ABBD4
    extsb r5, r7
    subi r0, r5, 0x80
    extsb r5, r0
    extsb r0, r6
    cmpw r0, r5
    bge @L_800ABBF8
    mr r6, r5
    b @L_800ABBF8
    @L_800ABBD4:
    extsb. r0, r7
    bge @L_800ABBF8
    extsb r5, r7
    addi r0, r5, 0x7f
    extsb r5, r0
    extsb r0, r6
    cmpw r5, r0
    bge @L_800ABBF8
    mr r6, r5
    @L_800ABBF8:
    subf r6, r7, r6
    stb r6, 0x3(r4)
    lbz r7, 0x4(r3)
    lbz r6, 0x4(r4)
    extsb. r0, r7
    ble @L_800ABC30
    extsb r5, r7
    subi r0, r5, 0x80
    extsb r5, r0
    extsb r0, r6
    cmpw r0, r5
    bge @L_800ABC54
    mr r6, r5
    b @L_800ABC54
    @L_800ABC30:
    extsb. r0, r7
    bge @L_800ABC54
    extsb r5, r7
    addi r0, r5, 0x7f
    extsb r5, r0
    extsb r0, r6
    cmpw r5, r0
    bge @L_800ABC54
    mr r6, r5
    @L_800ABC54:
    subf r6, r7, r6
    stb r6, 0x4(r4)
    lbz r7, 0x5(r3)
    lbz r6, 0x5(r4)
    extsb. r0, r7
    ble @L_800ABC8C
    extsb r5, r7
    subi r0, r5, 0x80
    extsb r5, r0
    extsb r0, r6
    cmpw r0, r5
    bge @L_800ABCB0
    mr r6, r5
    b @L_800ABCB0
    @L_800ABC8C:
    extsb. r0, r7
    bge @L_800ABCB0
    extsb r5, r7
    addi r0, r5, 0x7f
    extsb r5, r0
    extsb r0, r6
    cmpw r5, r0
    bge @L_800ABCB0
    mr r6, r5
    @L_800ABCB0:
    subf r6, r7, r6
    stb r6, 0x5(r4)
    lbz r0, 0x6(r3)
    lbz r5, 0x6(r4)
    cmplw r5, r0
    bge @L_800ABCCC
    mr r5, r0
    @L_800ABCCC:
    subf r5, r0, r5
    stb r5, 0x6(r4)
    lbz r0, 0x7(r3)
    lbz r3, 0x7(r4)
    cmplw r3, r0
    bge @L_800ABCE8
    mr r3, r0
    @L_800ABCE8:
    subf r3, r0, r3
    stb r3, 0x7(r4)
    blr
