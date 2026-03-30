    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    li r3, 0x41
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2a
    beq @L_802098A0
    li r3, 0x0
    b @L_802098AC
@L_802098A0:
    mr r3, r31
    li r4, 0x41
    bl fn_8011B67C
@L_802098AC:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802098F4
    li r3, 0x42
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2a
    beq @L_802098D4
    li r3, 0x0
    b @L_802098E0
@L_802098D4:
    mr r3, r31
    li r4, 0x42
    bl fn_8011B67C
@L_802098E0:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802098F4
    li r3, 0x1
    b @L_802098F8
@L_802098F4:
    li r3, 0x0
@L_802098F8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
