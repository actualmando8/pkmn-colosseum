    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    bne @L_80134790
    li r3, 0x0
    li r4, 0x3
    bl fn_80129280
    @L_80134790:
    extsh. r0, r31
    blt @L_801347A4
    extsh r0, r31
    cmpwi r0, 0xeb
    blt @L_801347AC
    @L_801347A4:
    li r0, 0x0
    b @L_801347B8
    @L_801347AC:
    slwi r4, r0, 2
    addi r0, r4, 0x6dec
    add r0, r3, r0
    @L_801347B8:
    mr r3, r0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
