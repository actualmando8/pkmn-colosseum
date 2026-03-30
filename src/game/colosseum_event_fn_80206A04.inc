    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    bne @L_80206A24
    li r3, 0x0
    b @L_80206AD8
@L_80206A24:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_80206A3C
    li r3, 0x0
    b @L_80206AD8
@L_80206A3C:
    mr r3, r31
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80206A60
    li r3, 0x0
    b @L_80206AD8
@L_80206A60:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80206A74
    li r3, 0x0
    b @L_80206AD8
@L_80206A74:
    cmplwi r31, 0x0
    bne @L_80206A84
    li r3, 0x0
    b @L_80206A98
@L_80206A84:
    mr r3, r31
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80206A98:
    cmplwi r3, 0x0
    bne @L_80206AA8
    li r3, 0x0
    b @L_80206AD8
@L_80206AA8:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80206ABC
    li r3, 0x0
    b @L_80206AD8
@L_80206ABC:
    mr r3, r31
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r3, 1, 31, 31
    xori r3, r0, 0x1
@L_80206AD8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
