    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    lwz r3, lbl_8047B64C(r13)
    mr r30, r3
    bl fn_80203E7C
    mr r0, r3
    mr r3, r30
    mr r29, r0
    bl fn_80205BE8
    lwz r4, lbl_8047B610(r13)
    mr r28, r3
    lbz r0, 0x9(r4)
    lwz r31, 0x1(r4)
    cmplwi r0, 0x0
    lwz r30, 0x5(r4)
    beq @L_8021EC54
    mr r4, r29
    bl fn_80123584
    stb r3, lbl_8047B642(r13)
@L_8021EC54:
    mr r3, r28
    mr r4, r29
    li r5, 0x0
    la r6, lbl_8047B642(r13)
    bl fn_8012361C
    extsb r6, r3
    cmpwi r6, -0x1
    bne @L_8021EC98
    mr r3, r28
    mr r4, r29
    la r5, lbl_8047B642(r13)
    bl fn_801236F8
    lwz r4, lbl_8047B610(r13)
    sth r3, lbl_8047B61C(r13)
    addi r0, r4, 0xa
    stw r0, lbl_8047B610(r13)
    b @L_8021ECE4
@L_8021EC98:
    cmpwi r6, -0x2
    bne @L_8021ECB0
    lbz r3, lbl_8047B642(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B642(r13)
    b @L_8021EC54
@L_8021ECB0:
    cmpwi r6, -0x3
    bne @L_8021ECC0
    stw r30, lbl_8047B610(r13)
    b @L_8021ECE4
@L_8021ECC0:
    lbz r5, lbl_8047B642(r13)
    mr r3, r28
    li r4, 0x0
    addi r0, r5, 0x1
    li r5, 0x7f
    stb r0, lbl_8047B642(r13)
    bl fn_8012640C
    sth r3, lbl_8047B61C(r13)
    stw r31, lbl_8047B610(r13)
@L_8021ECE4:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
