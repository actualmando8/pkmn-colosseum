    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r4, fn_8000857C@ha
    clrlwi r3, r3, 16
    addi r5, r4, fn_8000857C@l
    addi r4, r1, 0x8
    bl fn_8001E304
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8000854C
    li r3, -0x1
    b @L_8000856C
    @L_8000854C:
    lwz r0, 0x8(r1)
    lwz r3, lbl_80478B38(r13)
    cmplw r0, r3
    blt @L_80008564
    subi r0, r3, 0x1
    stw r0, 0x8(r1)
    @L_80008564:
    bl fn_8001E200
    lwz r3, 0x8(r1)
    @L_8000856C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
