    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lfs f1, lbl_8047BA30(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    lis r3, lbl_80267014@ha
    li r4, 0x0
    addi r3, r3, lbl_80267014@l
    bl fn_801E189C
    li r3, 0x494
    li r4, 0x0
    li r5, 0x7f
    bl fn_80165A20
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
