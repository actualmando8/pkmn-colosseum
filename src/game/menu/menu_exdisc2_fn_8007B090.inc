    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_800A7BCC
    mr r0, r3
    la r3, lbl_8047A640(r13)
    mr r4, r0
    li r5, 0x4
    bl memcpy
    mr r3, r31
    bl fn_8007B114
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
