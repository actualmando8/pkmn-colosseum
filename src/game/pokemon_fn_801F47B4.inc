    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r6, r4
    li r4, 0x0
    stw r0, 0x14(r1)
    li r5, 0x35
    stw r31, 0xc(r1)
    bl fn_801F54A4
    mr r31, r3
    bl fn_801F7404
    clrlwi. r0, r3, 24
    bne @L_801F47EC
    li r3, 0x0
    b @L_801F47F0
    @L_801F47EC:
    mr r3, r31
    @L_801F47F0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
