    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, lbl_804653A8@ha
    li r4, 0x0
    stw r0, 0x14(r1)
    addi r3, r3, lbl_804653A8@l
    li r5, 0x194
    bl memset
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
