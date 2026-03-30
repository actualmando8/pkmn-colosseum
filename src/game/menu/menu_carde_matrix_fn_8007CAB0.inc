    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0xa6
    bl fn_80104704
    bl fn_801040A0
    lwz r5, 0x0(r3)
    cmplwi r5, 0x0
    beq @L_8007CB44
    lbz r0, 0xb6(r5)
    cmplwi r0, 0x0
    bne @L_8007CAF0
    lwz r0, 0xa4(r5)
    stw r0, 0xa0(r5)
    lbz r0, 0xb5(r5)
    stb r0, 0xb4(r5)
@L_8007CAF0:
    li r4, -0x1
    lwz r0, 0xa0(r5)
    cmpwi r0, 0x0
    bge @L_8007CB08
    sth r4, 0xa8(r5)
    b @L_8007CB1C
@L_8007CB08:
    lwz r3, 0xb0(r5)
    rlwinm r0, r0, 2, 0, 29
    lwzx r3, r3, r0
    lbz r0, 0x1a(r3)
    sth r0, 0xa8(r5)
@L_8007CB1C:
    lwz r0, 0xa4(r5)
    cmpwi r0, 0x0
    bge @L_8007CB30
    sth r4, 0xaa(r5)
    b @L_8007CB44
@L_8007CB30:
    lwz r3, 0xb0(r5)
    rlwinm r0, r0, 2, 0, 29
    lwzx r3, r3, r0
    lbz r0, 0x1a(r3)
    sth r0, 0xaa(r5)
@L_8007CB44:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
