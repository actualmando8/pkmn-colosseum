    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r4
    mr r31, r3
    mr r3, r30
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_8020260C
    mr r3, r30
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    beq @L_8020260C
    mr r3, r30
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    bne @L_802026A8
@L_8020260C:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r31, r3
    mr r3, r30
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_8020264C
    mr r3, r30
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    bne @L_8020267C
@L_8020264C:
    cmplwi r31, 0x0
    bne @L_8020265C
    li r3, 0x0
    b @L_80202670
@L_8020265C:
    mr r3, r31
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80202670:
    mr r4, r30
    bl fn_80121A6C
    b @L_802026D0
@L_8020267C:
    mr r3, r30
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    beq @L_80202698
    li r3, 0x0
    b @L_802026D0
@L_80202698:
    mr r3, r31
    mr r4, r30
    bl fn_8011B444
    b @L_802026D0
@L_802026A8:
    mr r3, r30
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xd8
    beq @L_802026C4
    li r3, 0x0
    b @L_802026D0
@L_802026C4:
    mr r3, r31
    mr r4, r30
    bl fn_8011B444
@L_802026D0:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
