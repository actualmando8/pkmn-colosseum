    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lis r4, lbl_80267DD8@ha
    addi r4, r4, lbl_80267DD8@l
    lwz r0, 0x0(r4)
    cmpw r3, r0
    bne @L_8006B010
    li r31, 0x0
    b @L_8006B05C
    @L_8006B010:
    addi r4, r4, 0x4
    lwz r0, 0x0(r4)
    cmpw r3, r0
    bne @L_8006B028
    li r31, 0x1
    b @L_8006B05C
    @L_8006B028:
    addi r4, r4, 0x4
    lwz r0, 0x0(r4)
    cmpw r3, r0
    bne @L_8006B040
    li r31, 0x2
    b @L_8006B05C
    @L_8006B040:
    addi r4, r4, 0x4
    lwz r0, 0x0(r4)
    cmpw r3, r0
    bne @L_8006B058
    li r31, 0x3
    b @L_8006B05C
    @L_8006B058:
    li r31, -0x1
    @L_8006B05C:
    cmpwi r31, 0x0
    bge @L_8006B06C
    li r3, 0x0
    b @L_8006B088
    @L_8006B06C:
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    mulli r4, r31, 0x1660
    mr r0, r3
    addi r3, r4, 0x24
    add r3, r0, r3
    @L_8006B088:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
