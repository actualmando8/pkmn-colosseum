    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r4
    mr r31, r3
    mr r30, r5
    mr r3, r29
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_802024E4
    mr r3, r29
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    beq @L_802024E4
    mr r3, r29
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    bne @L_80202580
@L_802024E4:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r31, r3
    mr r3, r29
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_80202524
    mr r3, r29
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    bne @L_80202558
@L_80202524:
    cmplwi r31, 0x0
    bne @L_80202534
    li r3, 0x0
    b @L_80202548
@L_80202534:
    mr r3, r31
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80202548:
    mr r4, r29
    mr r5, r30
    bl fn_801219F4
    b @L_802025A4
@L_80202558:
    mr r3, r29
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    bne @L_802025A4
    mr r3, r31
    mr r4, r29
    mr r5, r30
    bl fn_8011B2C0
    b @L_802025A4
@L_80202580:
    mr r3, r29
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xd8
    bne @L_802025A4
    mr r3, r31
    mr r4, r29
    mr r5, r30
    bl fn_8011B2C0
@L_802025A4:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
