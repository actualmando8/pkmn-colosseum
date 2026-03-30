    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, lbl_804378F8@ha
    lis r6, fn_801522E0@ha
    stw r0, 0x14(r1)
    la r5, lbl_8047AF70(r13)
    addi r7, r6, fn_801522E0@l
    addi r4, r4, lbl_804378F8@l
    sth r3, 0x4(r5)
    la r3, lbl_8047AF70(r13)
    lhz r5, lbl_8047AFA6(r13)
    li r6, 0x8
    bl fn_80162118
    cmplwi r3, 0x0
    stw r3, lbl_8047AF6C(r13)
    beq @L_80152394
    lwz r3, 0x0(r3)
    b @L_80152398
    @L_80152394:
    li r3, 0x0
    @L_80152398:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
