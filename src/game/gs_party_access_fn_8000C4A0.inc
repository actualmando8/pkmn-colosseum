    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0x1
    bl fn_800F7BC4
    rlwinm r0, r3, 0, 26, 26
    cmplwi r0, 0x0
    beq @L_8000C504
    addi r4, r1, 0x8
    li r3, 0x0
    bl fn_8001E3E0
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8000C4E0
    li r3, -0x1
    b @L_8000C508
    @L_8000C4E0:
    lwz r3, 0x8(r1)
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    bl fn_8001D7E4
    li r3, 0x1
    bl fn_801065B8
    li r3, 0x0
    b @L_8000C508
    @L_8000C504:
    li r3, 0x1
    @L_8000C508:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
