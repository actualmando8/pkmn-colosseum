    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lhz r3, lbl_8047AE90(r13)
    bl fn_8011E778
    bl fn_8011E760
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
