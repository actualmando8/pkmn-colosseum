    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0x10c
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80105FE0
    li r3, 0x10c
    li r4, 0x2
    li r5, 0x0
    bl fn_80102568
@L_80105FE0:
    li r0, 0x0
    stb r0, lbl_8047AD10(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

