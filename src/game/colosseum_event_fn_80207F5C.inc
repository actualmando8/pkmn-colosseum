    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr. r29, r3
    mr r30, r5
    lwz r31, 0x0(r5)
    bne @L_80207F84
    li r3, 0x0
    b @L_80207FB4
@L_80207F84:
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80207FA4
    li r3, 0x0
    b @L_80207FB4
@L_80207FA4:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80207FB4:
    cmplw r29, r31
    bne @L_80207FC4
    li r3, 0x1
    b @L_80208014
@L_80207FC4:
    li r4, 0x0
    li r5, 0x73
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 24, 31
    li r3, 0x0
    li r5, 0x10
    li r6, 0x0
    bl fn_80142CF4
    cmplwi r3, 0x0
    bne @L_80207FF8
    li r3, 0x1
    b @L_80208014
@L_80207FF8:
    lwz r0, 0x4(r30)
    cmplw r0, r3
    bne @L_80208010
    lwz r3, 0x8(r30)
    addi r0, r3, 0x1
    stw r0, 0x8(r30)
@L_80208010:
    li r3, 0x1
@L_80208014:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
