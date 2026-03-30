    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    bne @L_8020ECC0
    li r3, 0x0
    b @L_8020ECD0
@L_8020ECC0:
    bl fn_801FBF04
    extsh r0, r3
    rlwinm r0, r0, 1, 31, 31
    xori r3, r0, 0x1
@L_8020ECD0:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
