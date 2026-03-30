    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021B898
    li r3, 0x0
    b @L_8021B8A4
@L_8021B898:
    mr r3, r31
    bl fn_802073C0
    li r3, 0x1
@L_8021B8A4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
