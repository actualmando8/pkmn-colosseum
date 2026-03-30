    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r4, r3
    li r3, 0x0
    stw r0, 0x14(r1)
    li r5, 0x9
    li r6, 0x0
    stw r31, 0xc(r1)
    bl fn_8011BEB4
    rlwinm r31, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x1
    bl fn_801F453C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80229BC0
    cmplwi r31, 0x98
    bne @L_80229BC0
    li r3, 0x1
    b @L_80229BC4
@L_80229BC0:
    li r3, 0x0
@L_80229BC4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
