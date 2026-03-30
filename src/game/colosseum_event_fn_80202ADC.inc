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
    cmplwi r0, 0x7c
    beq @L_80202B1C
    mr r3, r31
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    bne @L_80202B4C
@L_80202B1C:
    cmplwi r30, 0x0
    bne @L_80202B2C
    li r3, 0x0
    b @L_80202B40
@L_80202B2C:
    mr r3, r30
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80202B40:
    mr r4, r31
    bl fn_80121ADC
    b @L_80202B74
@L_80202B4C:
    mr r3, r31
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    beq @L_80202B68
    li r3, 0x0
    b @L_80202B74
@L_80202B68:
    mr r3, r30
    mr r4, r31
    bl fn_8011B67C
@L_80202B74:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
