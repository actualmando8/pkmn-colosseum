    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x2
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    li r3, 0x0
    bl fn_80129280
    rlwinm r4, r31, 0, 16, 31
    bl fn_8012AC08
    mr r31, r3
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801D0AE0
    li r3, 0x0
    b @L_801D0AE8
@L_801D0AE0:
    mr r3, r31
    bl fn_8011F550
@L_801D0AE8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
