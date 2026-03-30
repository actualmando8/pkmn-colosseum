    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x0
    stw r0, 0x14(r1)
    li r0, -0x1
    stw r31, 0xc(r1)
    mr r31, r3
    stw r4, lbl_8047ABB0(r13)
    lis r4, fn_800EE20C@ha
    addi r4, r4, fn_800EE20C@l
    stw r0, lbl_8047ABAC(r13)
    lwz r3, 0x8(r3)
    bl fn_801A3918
    lwz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800EE138
    lwz r3, lbl_8047ABB0(r13)
    subi r0, r3, 0x1
    stw r0, lbl_8047ABB0(r13)
    @L_800EE138:
    lwz r0, 0x14(r1)
    lwz r3, lbl_8047ABB0(r13)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
