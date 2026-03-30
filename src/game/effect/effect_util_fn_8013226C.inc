    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lhz r3, lbl_8047AE7C(r13)
    bl fn_8011CA34
    bl fn_8011CA1C
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
