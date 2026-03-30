    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_801023C4
    extsb r0, r31
    stb r0, 0x95(r3)
    b @L_801023CC
    @L_801023C4:
    li r3, -0x1
    b @L_801023D0
    @L_801023CC:
    li r3, 0x0
    @L_801023D0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
