    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r3, lbl_803A9768@ha
    addi r3, r3, lbl_803A9768@l
    lwz r0, 0x278(r3)
    mulli r0, r0, 0x138
    add r3, r3, r0
    addi r3, r3, 0x8
    bl fn_80124A60
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
