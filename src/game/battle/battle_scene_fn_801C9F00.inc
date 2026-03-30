    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C9F34
    li r3, 0x0
    li r4, 0x18
    li r5, 0x1
    bl fn_8012A450
    li r3, 0x1
    bl fn_8012F1FC
    b @L_801C9F4C
@L_801C9F34:
    li r3, 0x0
    li r4, 0x18
    li r5, 0x0
    bl fn_8012A450
    li r3, 0x1
    bl fn_8012F150
@L_801C9F4C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
