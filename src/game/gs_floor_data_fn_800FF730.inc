    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, 0x596
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    addi r3, r4, 0x9
    addi r4, r4, 0x8
    bl fn_8011288C
    lwz r3, lbl_8047ACC4(r13)
    lwz r0, lbl_8047ACA8(r13)
    cmplw r3, r0
    bge @L_800FF770
    li r0, 0x4
    stw r0, lbl_8047ACD8(r13)
    stw r31, lbl_80478B18(r13)
@L_800FF770:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

