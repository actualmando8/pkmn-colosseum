    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, lbl_8043D6F8@ha
    rlwinm r6, r3, 28, 20, 29
    stw r0, 0x14(r1)
    addi r4, r4, lbl_8043D6F8@l
    extrwi r0, r3, 10, 16
    lhzx r5, r4, r6
    stw r0, lbl_8047AF98(r13)
    cmplwi r5, 0x0
    beq @L_80152194
    add r4, r4, r6
    la r7, lbl_8047AF90(r13)
    lhz r8, 0x2(r4)
    lis r6, lbl_8043DEF8@ha
    sth r3, 0x4(r7)
    lis r4, fn_8015210C@ha
    addi r0, r6, lbl_8043DEF8@l
    slwi r3, r8, 3
    addi r7, r4, fn_8015210C@l
    stw r8, lbl_8047AF9C(r13)
    add r4, r0, r3
    la r3, lbl_8047AF90(r13)
    li r6, 0x8
    bl fn_80162118
    cmplwi r3, 0x0
    stw r3, lbl_8047AF8C(r13)
    beq @L_80152194
    lwz r3, 0x0(r3)
    b @L_80152198
    @L_80152194:
    li r3, 0x0
    @L_80152198:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
