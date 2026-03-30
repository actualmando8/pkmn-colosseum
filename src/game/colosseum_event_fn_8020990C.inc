    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r31, r4
    mr r30, r3
    mr r3, r31
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2a
    beq @L_80209940
    li r3, 0x0
    b @L_8020994C
@L_80209940:
    mr r3, r30
    mr r4, r31
    bl fn_8011B67C
@L_8020994C:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
