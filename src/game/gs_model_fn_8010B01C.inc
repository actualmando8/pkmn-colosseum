    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    cmplwi r28, 0x0
    bne @L_8010B054
    li r3, 0x0
    b @L_8010B140
@L_8010B054:
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8010B06C
    li r3, 0x33d
    b @L_8010B140
@L_8010B06C:
    mr r3, r28
    bl fn_8011E8DC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8010B088
    li r3, 0x33d
    b @L_8010B140
@L_8010B088:
    mr r3, r28
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r31, r3, 0, 16, 31
    cmplwi r31, 0xc9
    bne @L_8010B0F8
    mr r3, r28
    bl fn_8011F5B0
    bl fn_80122334
    rlwinm r31, r3, 0, 24, 31
    mr r3, r28
    bl fn_80125390
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8010B0E0
    lis r3, lbl_8035B478@ha
    rlwinm r0, r31, 2, 0, 29
    addi r3, r3, lbl_8035B478@l
    lhzx r3, r3, r0
    b @L_8010B140
@L_8010B0E0:
    lis r3, lbl_8035B478@ha
    rlwinm r0, r31, 2, 0, 29
    addi r3, r3, lbl_8035B478@l
    add r3, r3, r0
    lhz r3, 0x2(r3)
    b @L_8010B140
@L_8010B0F8:
    mr r3, r28
    bl fn_80125390
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8010B128
    mr r4, r31
    li r3, 0x0
    li r5, 0x5b
    li r6, 0x0
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
    b @L_8010B140
@L_8010B128:
    mr r4, r31
    li r3, 0x0
    li r5, 0x5b
    li r6, 0x1
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
@L_8010B140:
    mr r4, r29
    mr r5, r30
    bl fn_8010B16C
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
