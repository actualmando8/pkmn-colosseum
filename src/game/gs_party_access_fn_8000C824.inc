    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    b @L_8000C8EC
    @L_8000C840:
    lwz r3, lbl_80478E20(r13)
    lwz r0, 0x0(r3)
    cmplw r31, r0
    bge @L_8000C8EC
    li r29, 0x0
    li r30, 0x0
    b @L_8000C898
    @L_8000C85C:
    cmpw r31, r29
    beq @L_8000C890
    lwz r3, lbl_80478E24(r13)
    lhzx r3, r3, r30
    bl fn_801666BC
    cmpwi r3, 0x4
    bge @L_8000C890
    cmpwi r3, 0x1
    bge @L_8000C884
    b @L_8000C890
    @L_8000C884:
    lwz r3, lbl_80478E24(r13)
    lhzx r3, r3, r30
    bl fn_80166B18
    @L_8000C890:
    addi r30, r30, 0x2
    addi r29, r29, 0x1
    @L_8000C898:
    lwz r3, lbl_80478E20(r13)
    lwz r0, 0x0(r3)
    cmplw r29, r0
    blt @L_8000C85C
    lwz r3, lbl_80478E24(r13)
    slwi r0, r31, 1
    lhzx r31, r3, r0
    mr r3, r31
    bl fn_801666BC
    cmpwi r3, 0x0
    beq @L_8000C8DC
    blt @L_8000C8DC
    cmpwi r3, 0x4
    bge @L_8000C8DC
    mr r3, r31
    bl fn_80166B18
    b @L_8000C8EC
    @L_8000C8DC:
    mr r3, r31
    li r4, 0x0
    li r5, 0x7f
    bl fn_801659FC
    @L_8000C8EC:
    li r3, 0x2
    li r4, 0x1
    bl fn_8010264C
    mr r31, r3
    cmpwi r31, -0x1
    bne @L_8000C840
    li r3, 0x2
    bl fn_80102510
    li r3, -0x1
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
