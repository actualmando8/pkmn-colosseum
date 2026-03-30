    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r5, lbl_802704A0@ha
    stw r0, 0x14(r1)
    li r0, 0x0
    stw r31, 0xc(r1)
    addi r31, r5, lbl_802704A0@l
    stw r30, 0x8(r1)
    mr. r30, r3
    stb r0, lbl_8047AB10(r13)
    stw r30, lbl_8047AB04(r13)
    stb r4, lbl_8047AB11(r13)
    bne @L_800DDF9C
    addi r3, r31, 0x2c
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x1
    b @L_800DE084
    @L_800DDF9C:
    bl fn_800E3534
    clrlwi. r0, r3, 16
    sth r3, lbl_8047AAF8(r13)
    bne @L_800DDFC0
    addi r3, r31, 0x48
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_800DE084
    @L_800DDFC0:
    srwi r0, r30, 7
    rlwinm r3, r30, 26, 6, 30
    stw r0, lbl_8047AB0C(r13)
    bl fn_800E3534
    clrlwi. r0, r3, 16
    sth r3, lbl_8047AAFA(r13)
    bne @L_800DDFF8
    lhz r3, lbl_8047AAF8(r13)
    bl fn_800E209C
    addi r3, r31, 0x48
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_800DE084
    @L_800DDFF8:
    lhz r3, lbl_8047AAF8(r13)
    bl fn_800E27B0
    cmplwi r3, 0x0
    stw r3, lbl_8047AAFC(r13)
    bne @L_800DE030
    lhz r3, lbl_8047AAF8(r13)
    bl fn_800E209C
    lhz r3, lbl_8047AAFA(r13)
    bl fn_800E209C
    addi r3, r31, 0x48
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_800DE084
    @L_800DE030:
    lhz r3, lbl_8047AAFA(r13)
    bl fn_800E27B0
    cmplwi r3, 0x0
    stw r3, lbl_8047AB00(r13)
    bne @L_800DE070
    lhz r3, lbl_8047AAF8(r13)
    bl fn_800E24B0
    lhz r3, lbl_8047AAF8(r13)
    bl fn_800E209C
    lhz r3, lbl_8047AAFA(r13)
    bl fn_800E209C
    addi r3, r31, 0x48
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_800DE084
    @L_800DE070:
    lwz r4, lbl_8047AB04(r13)
    addi r3, r31, 0x5c
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x1
    @L_800DE084:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
