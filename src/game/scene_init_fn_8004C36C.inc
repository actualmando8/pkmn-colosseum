    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r5, r3
    mr r30, r4
    lbz r0, 0x94(r5)
    li r3, 0x34
    lbz r5, 0x8b(r5)
    extsb r4, r0
    oris r31, r5, 0xe66e
    addi r4, r4, 0x1
    bl fn_80132A38
    li r3, 0xca
    bl fn_800FA444
    lha r0, 0x54(r30)
    srwi r3, r3, 16
    mr r5, r31
    li r4, 0x0
    subf r3, r3, r0
    li r6, 0xca
    bl fn_800FB680
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
