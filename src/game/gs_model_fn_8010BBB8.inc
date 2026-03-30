    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    cmplwi r30, 0x0
    bne @L_8010BBE0
    li r3, 0x0
    b @L_8010BCCC
@L_8010BBE0:
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8010BBF8
    li r3, 0x33d
    b @L_8010BCCC
@L_8010BBF8:
    mr r3, r30
    bl fn_8011E8DC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8010BC14
    li r3, 0x33d
    b @L_8010BCCC
@L_8010BC14:
    mr r3, r30
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r31, r3, 0, 16, 31
    cmplwi r31, 0xc9
    bne @L_8010BC84
    mr r3, r30
    bl fn_8011F5B0
    bl fn_80122334
    rlwinm r31, r3, 0, 24, 31
    mr r3, r30
    bl fn_80125390
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8010BC6C
    lis r3, lbl_8035B478@ha
    rlwinm r0, r31, 2, 0, 29
    addi r3, r3, lbl_8035B478@l
    lhzx r3, r3, r0
    b @L_8010BCCC
@L_8010BC6C:
    lis r3, lbl_8035B478@ha
    rlwinm r0, r31, 2, 0, 29
    addi r3, r3, lbl_8035B478@l
    add r3, r3, r0
    lhz r3, 0x2(r3)
    b @L_8010BCCC
@L_8010BC84:
    mr r3, r30
    bl fn_80125390
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8010BCB4
    mr r4, r31
    li r3, 0x0
    li r5, 0x5b
    li r6, 0x0
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
    b @L_8010BCCC
@L_8010BCB4:
    mr r4, r31
    li r3, 0x0
    li r5, 0x5b
    li r6, 0x1
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
@L_8010BCCC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
