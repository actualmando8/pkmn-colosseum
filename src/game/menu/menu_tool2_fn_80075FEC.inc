    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_8011F5C8
    rlwinm r0, r3, 0, 16, 31
    cmpwi r0, 0x19a
    beq @L_80076020
    bge @L_8007603C
    cmpwi r0, 0x97
    beq @L_80076020
    b @L_8007603C
@L_80076020:
    mr r3, r31
    bl fn_8011E7A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8007603C
    li r3, 0x0
    b @L_80076040
@L_8007603C:
    li r3, 0x1
@L_80076040:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
