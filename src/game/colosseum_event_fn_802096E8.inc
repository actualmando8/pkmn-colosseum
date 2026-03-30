    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    li r3, 0x40
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2a
    beq @L_80209718
    li r3, 0x0
    b @L_80209724
@L_80209718:
    mr r3, r31
    li r4, 0x40
    bl fn_8011B67C
@L_80209724:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80209738
    li r3, 0x0
    b @L_802097B4
@L_80209738:
    li r3, 0x43
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2a
    beq @L_80209754
    li r3, 0x0
    b @L_80209760
@L_80209754:
    mr r3, r31
    li r4, 0x43
    bl fn_8011B67C
@L_80209760:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80209774
    li r3, 0x0
    b @L_802097B4
@L_80209774:
    li r3, 0x45
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2a
    beq @L_80209790
    li r3, 0x0
    b @L_8020979C
@L_80209790:
    mr r3, r31
    li r4, 0x45
    bl fn_8011B67C
@L_8020979C:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802097B0
    li r3, 0x0
    b @L_802097B4
@L_802097B0:
    li r3, 0x1
@L_802097B4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
