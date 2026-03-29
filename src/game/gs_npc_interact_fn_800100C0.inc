    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    bl fn_801040A0
    lbz r0, 0x1(r30)
    mr r31, r3
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8001010C
    mr r3, r30
    li r4, 0x0
    bl fn_801040D0
    mr r4, r3
    mr r3, r31
    li r5, 0x6
    bl memcpy
    @L_8001010C:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
