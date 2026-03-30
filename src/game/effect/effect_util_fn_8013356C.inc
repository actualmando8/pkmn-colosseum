    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmpwi r4, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    li r31, -0x1
    beq @L_8013358C
    b @L_80133590
    @L_8013358C:
    li r31, 0x1
    @L_80133590:
    cmpwi r31, 0x0
    blt @L_801335BC
    mr r3, r31
    bl fn_8012F11C
    clrlwi. r0, r3, 24
    beq @L_801335B4
    mr r3, r31
    bl fn_8012F150
    b @L_801335BC
    @L_801335B4:
    mr r3, r31
    bl fn_8012F1FC
    @L_801335BC:
    lwz r0, 0x14(r1)
    li r3, 0x0
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
