    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    bl fn_801ADCD8
    mr r31, r3
    bl fn_801ADCD8
    slwi r0, r3, 16
    or r3, r0, r31
    divwu r0, r3, r30
    mullw r0, r0, r30
    subf r3, r0, r3
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
