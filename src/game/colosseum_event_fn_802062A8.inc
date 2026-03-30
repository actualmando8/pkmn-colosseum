    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r6, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r5
    li r5, 0xf8
    bl fn_8012640C
    li r4, 0x0
    li r5, 0x29
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r3, r3, 0, 16, 31
    mr r4, r31
    bl fn_801F00D0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
