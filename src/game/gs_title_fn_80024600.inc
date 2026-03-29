    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    lwz r3, lbl_80478DDC(r13)
    lwz r3, 0x20(r3)
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80024638
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
@L_80024638:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
