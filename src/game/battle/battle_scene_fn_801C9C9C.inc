    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    addi r3, r1, 0xc
    addi r4, r1, 0x8
    li r5, 0x1
    bl fn_8012EFB8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C9CCC
    li r3, 0x1
    bl fn_8012B5E4
@L_801C9CCC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
