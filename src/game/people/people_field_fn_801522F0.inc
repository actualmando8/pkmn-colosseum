    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, lbl_80438CF8@ha
    lis r6, fn_801522E0@ha
    stw r0, 0x14(r1)
    la r5, lbl_8047AF7C(r13)
    addi r7, r6, fn_801522E0@l
    addi r4, r4, lbl_80438CF8@l
    sth r3, 0x4(r5)
    la r3, lbl_8047AF7C(r13)
    lhz r5, lbl_8047AFA8(r13)
    li r6, 0x8
    bl fn_80162118
    cmplwi r3, 0x0
    stw r3, lbl_8047AF78(r13)
    beq @L_80152338
    lwz r3, 0x0(r3)
    b @L_8015233C
    @L_80152338:
    li r3, 0x0
    @L_8015233C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
