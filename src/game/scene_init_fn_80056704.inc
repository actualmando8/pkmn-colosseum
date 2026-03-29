    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    lwz r3, lbl_8047A584(r13)
    subi r0, r3, 0x1
    stw r3, lbl_8047A580(r13)
    cmpwi r0, 0x0
    stw r0, lbl_8047A584(r13)
    bge @L_80056730
    li r0, 0x2
    stw r0, lbl_8047A584(r13)
    @L_80056730:
    lis r3, lbl_8026768C@ha
    lwz r6, lbl_8047A584(r13)
    addi r5, r3, lbl_8026768C@l
    lwz r4, 0x0(r5)
    cmpwi r6, 0x0
    lwz r3, 0x4(r5)
    lwz r0, 0x8(r5)
    stw r4, 0x8(r1)
    stw r3, 0xc(r1)
    stw r0, 0x10(r1)
    blt @L_80056764
    cmpwi r6, 0x3
    blt @L_8005676C
    @L_80056764:
    li r3, -0x1
    b @L_80056778
    @L_8005676C:
    slwi r0, r6, 2
    addi r3, r1, 0x8
    lwzx r3, r3, r0
    @L_80056778:
    cmpwi r3, 0x0
    blt @L_80056788
    li r4, 0x1
    bl fn_80102254
    @L_80056788:
    lfs f1, lbl_8047BEC0(r2)
    lfs f0, lbl_8047BEC8(r2)
    stfs f1, lbl_8047A57C(r13)
    lwz r3, lbl_8047A584(r13)
    stfs f0, lbl_8047A578(r13)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
