    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, lbl_8047A4B8(r13)
    cmpwi r0, 0x0
    bge @L_80039F60
    bl fn_80102ED4
    @L_80039F60:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
