    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r3, lbl_8047A450(r13)
    li r0, -0x1
    cmplw r3, r0
    bge @L_800325B4
    addi r0, r3, 0x1
    stw r0, lbl_8047A450(r13)
    @L_800325B4:
    lwz r4, lbl_8047A450(r13)
    lis r3, 0xaaab
    subi r0, r3, 0x5555
    addi r3, r4, 0x1
    mulhwu r0, r0, r3
    srwi r0, r0, 2
    mulli r0, r0, 0x6
    subf r0, r0, r3
    cmplwi r0, 0x0
    bne @L_800325E8
    li r0, 0xe
    stw r0, lbl_8047A458(r13)
    b @L_800327E8
    @L_800325E8:
    lbz r0, lbl_8047A44A(r13)
    cmplwi r0, 0x0
    beq @L_800327E0
    li r0, 0x0
    cmplwi r4, 0x1e
    stb r0, lbl_8047A44A(r13)
    bge @L_80032694
    subi r0, r4, 0x6
    cmplwi r0, 0x12
    bgt @L_80032648
    lis r3, jumptable_802E4FE0@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_802E4FE0@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r31, 0x258
    b @L_8003264C
    li r31, 0x4b0
    b @L_8003264C
    li r31, 0x960
    b @L_8003264C
    li r31, 0x12c0
    b @L_8003264C
    @L_80032648:
    li r31, 0x258
    @L_8003264C:
    li r3, 0x3cc
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    mr r4, r31
    li r3, 0x0
    bl fn_801293FC
    mr r4, r31
    li r3, 0x2f
    bl fn_80132A38
    li r3, 0x8
    li r4, 0x3b60
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    b @L_80032794
    @L_80032694:
    cmpwi r4, 0x5a
    beq @L_80032738
    bge @L_800326B8
    cmpwi r4, 0x3c
    beq @L_80032718
    bge @L_80032794
    cmpwi r4, 0x1e
    beq @L_800326D0
    b @L_80032794
    @L_800326B8:
    cmpwi r4, 0x96
    beq @L_80032778
    bge @L_80032794
    cmpwi r4, 0x78
    beq @L_80032758
    b @L_80032794
    @L_800326D0:
    li r3, 0x3cc
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    li r3, 0x0
    li r4, 0x2580
    bl fn_801293FC
    li r3, 0x2f
    li r4, 0x2580
    bl fn_80132A38
    li r3, 0x8
    li r4, 0x3b8c
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    b @L_80032794
    @L_80032718:
    li r3, 0x8
    li r4, 0x3b8f
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    b @L_80032794
    @L_80032738:
    li r3, 0x8
    li r4, 0x3b53
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    b @L_80032794
    @L_80032758:
    li r3, 0x8
    li r4, 0x3b56
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    b @L_80032794
    @L_80032778:
    li r3, 0x8
    li r4, 0x3b59
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    @L_80032794:
    lis r3, lbl_803A3278@ha
    li r0, 0x0
    addi r3, r3, lbl_803A3278@l
    stw r0, lbl_8047A43C(r13)
    stb r0, 0x0(r3)
    stb r0, 0x1(r3)
    stb r0, 0x2(r3)
    stb r0, 0x3(r3)
    stb r0, 0x4(r3)
    stb r0, 0x5(r3)
    stb r0, 0x6(r3)
    stb r0, 0x7(r3)
    stb r0, 0x8(r3)
    stb r0, 0x9(r3)
    stb r0, 0xa(r3)
    stb r0, 0xb(r3)
    stb r0, 0xc(r3)
    stb r0, 0xd(r3)
    stb r0, 0xe(r3)
    @L_800327E0:
    li r0, 0x5
    stw r0, lbl_8047A458(r13)
    @L_800327E8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
