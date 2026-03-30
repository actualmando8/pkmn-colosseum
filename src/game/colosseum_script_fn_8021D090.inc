    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x24(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r29, 0x14(r1)
    bl fn_801F54A4
    rlwinm r31, r3, 0, 16, 31
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r30, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r30
    mr r29, r0
    mr r4, r31
    li r5, 0x1
    bl fn_802656AC
    mr r0, r3
    li r3, 0x0
    mr r31, r0
    li r4, 0x6
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801F349C
    cmplwi r3, 0x0
    bne @L_8021D1EC
    mr r3, r30
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    rlwinm r7, r3, 0, 16, 31
    mr r3, r29
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x0
    bl fn_80011E68
    li r3, 0x8
    li r0, 0x0
    stb r3, lbl_8047B649(r13)
    li r29, 0x0
    stb r0, lbl_8047B648(r13)
    b @L_8021D1AC
@L_8021D16C:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x5d
    bl fn_801F54A4
    mr. r31, r3
    beq @L_8021D1A0
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021D1A0
    cmplw r30, r31
    beq @L_8021D1A0
    mr r29, r31
    b @L_8021D1BC
@L_8021D1A0:
    lbz r3, lbl_8047B648(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B648(r13)
@L_8021D1AC:
    lbz r6, lbl_8047B648(r13)
    lbz r0, lbl_8047B649(r13)
    cmplw r6, r0
    blt @L_8021D16C
@L_8021D1BC:
    cmplwi r29, 0x0
    beq @L_8021D1DC
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
@L_8021D1DC:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_8021D210
@L_8021D1EC:
    mr r7, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x42
    li r6, 0x0
    bl fn_801F4C14
    lis r3, lbl_803797BB@ha
    addi r0, r3, lbl_803797BB@l
    stw r0, lbl_8047B610(r13)
@L_8021D210:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
