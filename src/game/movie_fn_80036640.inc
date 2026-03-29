    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r0, 0x0
    lfs f1, lbl_8047BA30(r2)
    stb r0, lbl_8047A468(r13)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    lis r3, lbl_80267040@ha
    li r4, 0x1
    addi r3, r3, lbl_80267040@l
    bl fn_801E189C
    lis r3, 0x8000
    li r4, 0x0
    addi r3, r3, 0x1803
    li r5, 0x17fd
    bl memset
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
