    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r0, 0x0
    stw r3, lbl_8047A2E0(r13)
    stw r5, lbl_8047A2F8(r13)
    stw r0, lbl_8047A2E4(r13)
    stw r4, lbl_8047A2BC(r13)
    bl fn_80018A68
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
