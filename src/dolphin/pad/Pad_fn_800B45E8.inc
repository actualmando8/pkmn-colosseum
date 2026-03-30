    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    mr r31, r3
    addi r4, r1, 0xc
    lwz r3, 0x0(r3)
    bl fn_800B023C
    cmpwi r3, 0x0
    bge @L_800B4614
    b @L_800B4628
    @L_800B4614:
    li r0, -0x1
    stw r0, 0x0(r31)
    li r4, 0x0
    lwz r3, 0xc(r1)
    bl fn_800B02F4
    @L_800B4628:
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
