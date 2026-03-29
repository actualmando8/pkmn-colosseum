    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    li r3, 0x59
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lwz r0, lbl_8047A2E0(r13)
    cmpwi r0, 0x1
    bne @L_800141FC
    li r3, 0x4
    b @L_80014200
    @L_800141FC:
    li r3, 0x3
    @L_80014200:
    lwz r6, lbl_8047A2F4(r13)
    clrlwi r4, r31, 24
    clrlwi r5, r30, 16
    bl fn_8001BD3C
    cmpwi r31, 0x0
    bne @L_8001421C
    li r3, -0x1
    @L_8001421C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
