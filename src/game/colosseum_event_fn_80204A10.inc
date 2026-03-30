    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r4, r3
    li r3, 0x0
    stw r0, 0x14(r1)
    bl fn_801F4354
    cmplwi r3, 0x0
    bne @L_80204A38
    li r3, 0x0
    b @L_80204A4C
@L_80204A38:
    bl fn_801FB8F8
    rlwinm r0, r3, 0, 24, 31
    subfic r0, r0, 0x1
    cntlzw r0, r0
    rlwinm r3, r0, 27, 24, 31
@L_80204A4C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
