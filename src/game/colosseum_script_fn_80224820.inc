    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    bl fn_80261B68
    li r3, 0x0
    bl fn_80261E7C
    bl fn_8026246C
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    bl fn_80205184
    mr r0, r3
    mr r3, r31
    mr r31, r0
    bl fn_80207BF4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x20
    bne @L_802248AC
    mr r4, r31
    li r3, 0x0
    li r5, 0xc
    li r6, 0x0
    bl fn_8011BEB4
    clrlslwi r0, r3, 24, 1
    b @L_802248C4
@L_802248AC:
    mr r4, r31
    li r3, 0x0
    li r5, 0xc
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 24, 31
@L_802248C4:
    mr r29, r0
    bl fn_8000817C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802248DC
    li r29, 0x63
@L_802248DC:
    la r31, lbl_80478D78(r13)
    lbz r0, 0x3(r31)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_8022491C
    mr r3, r30
    bl fn_802096E8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022491C
    lbz r0, 0x3(r31)
    li r3, 0x0
    li r4, 0x80
    rlwinm r0, r0, 0, 25, 31
    stb r0, 0x3(r31)
    bl fn_802249B8
    b @L_80224998
@L_8022491C:
    bl fn_800E0C54
    rlwinm r5, r3, 0, 16, 31
    li r4, 0x64
    divw r3, r5, r4
    rlwinm r0, r29, 0, 16, 31
    mullw r3, r3, r4
    subf r3, r3, r5
    cmpw r3, r0
    bgt @L_8022498C
    lbz r0, 0x3(r31)
    cmplwi r0, 0x0
    beq @L_8022498C
    mr r3, r30
    bl fn_802096E8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022498C
    rlwinm r0, r29, 0, 16, 31
    cmplwi r0, 0x64
    blt @L_8022497C
    li r3, 0x0
    li r4, 0x80
    bl fn_802249B8
    b @L_80224998
@L_8022497C:
    li r3, 0x0
    li r4, 0x0
    bl fn_802249B8
    b @L_80224998
@L_8022498C:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
@L_80224998:
    li r0, 0x0
    stb r0, 0x3(r31)
    stb r0, lbl_8047B625(r13)
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
